import time
import threading
import math
import serial
import os

class PanTiltController:
    """팬틸트 하드웨어 제어를 위한 클래스"""
    
    # 싱글톤 인스턴스
    _instance = None
    _lock = None
    
    @classmethod
    def get_instance(cls):
        """싱글톤 인스턴스 반환"""
        if cls._instance is None:
            if cls._lock is None:
                cls._lock = threading.Lock()
                
            with cls._lock:
                if cls._instance is None:
                    cls._instance = PanTiltController()
        return cls._instance
    
    def __init__(self):
        """팬틸트 컨트롤러 초기화"""
        if PanTiltController._instance is not None:
            return
            
        # 설정
        self.port = None  # 시리얼 포트 (예: 'COM3', '/dev/ttyUSB0')
        self.baudrate = 9600  # 기본 baudrate
        self.serial_conn = None
        self.connected = False
        
        # 현재 위치 및 한계
        self.current_pan = 0  # 현재 팬 위치 (도)
        self.current_tilt = 0  # 현재 틸트 위치 (도)
        self.max_pan = 60  # 최대 팬 각도 (각 방향)
        self.max_tilt = 45  # 최대 틸트 각도
        
        # 제어 스레드 및 락
        self.lock = threading.Lock()
        self.running = True
        
        # 모션 콜백
        self.motion_callbacks = []
        
        # 연결 시도
        self._try_connect()
    
    def _try_connect(self):
        """하드웨어에 연결 시도"""
        try:
            # 포트 자동 감지 (필요한 경우)
            if self.port is None:
                # 여기에 포트 자동 감지 로직 추가
                self.port = self._auto_detect_port()
            
            if self.port:
                # 시리얼 연결
                self.serial_conn = serial.Serial(
                    port=self.port,
                    baudrate=self.baudrate,
                    timeout=1
                )
                
                # 연결 확인
                if self.serial_conn.is_open:
                    self.connected = True
                    print(f"[PanTilt] 연결 성공: {self.port}")
                    
                    # 초기화 명령 전송
                    self._send_command("INIT")
                    time.sleep(1)  # 초기화 대기
                    
                    # 현재 위치 요청
                    self._request_position()
                else:
                    print(f"[PanTilt] 연결 실패: {self.port}")
            else:
                print("[PanTilt] 사용 가능한 포트를 찾을 수 없음")
                
        except Exception as e:
            import traceback
            print(f"[PanTilt] 연결 오류: {e}")
            print(traceback.format_exc())
            self.connected = False
    
    def _auto_detect_port(self):
        """사용 가능한 시리얼 포트 자동 감지"""
        try:
            import serial.tools.list_ports
            ports = list(serial.tools.list_ports.comports())
            
            if not ports:
                return None
                
            # 첫 번째 사용 가능한 포트 사용 (실제 구현에서는 더 정교한 감지 필요)
            return ports[0].device
        except Exception as e:
            print(f"[PanTilt] 포트 자동 감지 오류: {e}")
            return None
    
    def _send_command(self, cmd):
        """팬틸트에 명령 전송"""
        if not self.connected or self.serial_conn is None:
            return False
            
        try:
            # 명령 형식: 'PAN:45' 또는 'TILT:30' 또는 'INIT'
            cmd_str = f"{cmd}\n"
            self.serial_conn.write(cmd_str.encode())
            self.serial_conn.flush()
            
            # 응답 대기 (선택적)
            response = self.serial_conn.readline().decode().strip()
            print(f"[PanTilt] 명령: {cmd}, 응답: {response}")
            
            return True
        except Exception as e:
            print(f"[PanTilt] 명령 전송 오류: {e}")
            self.connected = False
            return False
    
    def _request_position(self):
        """현재 위치 요청"""
        if not self.connected:
            return
            
        try:
            self._send_command("GET_POS")
            # 응답 처리 로직은 실제 하드웨어에 맞게 구현 필요
        except Exception as e:
            print(f"[PanTilt] 위치 요청 오류: {e}")
    
    def set_port(self, port, baudrate=9600):
        """시리얼 포트 설정"""
        with self.lock:
            # 기존 연결 종료
            if self.connected and self.serial_conn:
                try:
                    self.serial_conn.close()
                except Exception:
                    pass
            
            self.port = port
            self.baudrate = baudrate
            self.connected = False
            
            # 새 연결 시도
            self._try_connect()
            return self.connected
    
    def move_to(self, pan_angle=None, tilt_angle=None):
        """지정된 각도로 이동"""
        with self.lock:
            if not self.connected:
                return False
                
            success = True
            
            # 팬 이동
            if pan_angle is not None:
                # 한계 내로 조정
                pan_angle = max(-self.max_pan, min(self.max_pan, pan_angle))
                
                # 명령 전송
                cmd = f"PAN:{pan_angle:.1f}"
                if self._send_command(cmd):
                    self.current_pan = pan_angle
                    # 콜백 호출
                    self._notify_motion_callbacks()
                else:
                    success = False
            
            # 틸트 이동
            if tilt_angle is not None:
                # 한계 내로 조정
                tilt_angle = max(-self.max_tilt, min(self.max_tilt, tilt_angle))
                
                # 명령 전송
                cmd = f"TILT:{tilt_angle:.1f}"
                if self._send_command(cmd):
                    self.current_tilt = tilt_angle
                    # 콜백 호출
                    self._notify_motion_callbacks()
                else:
                    success = False
            
            return success
    
    def get_position(self):
        """현재 팬틸트 위치 반환"""
        with self.lock:
            return {
                "pan": self.current_pan,
                "tilt": self.current_tilt,
                "connected": self.connected
            }
    
    def move_relative(self, pan_delta=0, tilt_delta=0):
        """현재 위치에서 상대적으로 이동"""
        with self.lock:
            new_pan = self.current_pan + pan_delta
            new_tilt = self.current_tilt + tilt_delta
            return self.move_to(new_pan, new_tilt)
    
    def center(self):
        """중앙(0도) 위치로 이동"""
        return self.move_to(0, 0)
    
    def add_motion_callback(self, callback):
        """모션 이벤트 콜백 추가"""
        with self.lock:
            if callback not in self.motion_callbacks:
                self.motion_callbacks.append(callback)
    
    def remove_motion_callback(self, callback):
        """모션 이벤트 콜백 제거"""
        with self.lock:
            if callback in self.motion_callbacks:
                self.motion_callbacks.remove(callback)
    
    def _notify_motion_callbacks(self):
        """모션 콜백 호출"""
        position = self.get_position()
        for callback in self.motion_callbacks:
            try:
                callback(position)
            except Exception as e:
                print(f"[PanTilt] 콜백 오류: {e}")
    
    def stop(self):
        """컨트롤러 정지 및 자원 정리"""
        with self.lock:
            self.running = False
            
            # 시리얼 연결 종료
            if self.serial_conn and self.connected:
                try:
                    self.serial_conn.close()
                except Exception:
                    pass
                
            self.connected = False
            print("[PanTilt] 컨트롤러 정지됨") 