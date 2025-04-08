# C:\Users\AAA\Documents\GitHub\AIACS\AIACS_WEB\backend\camera\hardware\bird_controller.py
import socket
import time
import threading
import struct
import logging
import sys
import binascii

# 로그 활성화
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)  # INFO 레벨 이상의 로그를 출력
# 로그 핸들러 추가
handler = logging.StreamHandler(sys.stdout)
handler.setFormatter(logging.Formatter('[%(asctime)s] [%(name)s] [%(levelname)s] %(message)s'))
logger.addHandler(handler)
logger.propagate = False  # 중복 로그 방지

# 명령 ID 정의
CMD_PLAY = 0x01
CMD_SET_VOLUME = 0x02

# 장치 ID 정의
DEV_COMMON = 0x00

# ETX 값
ETX_VALUE = 0x5555

class BirdController:
    """조류퇴치기 하드웨어 제어 TCP 클라이언트 클래스"""
    
    _instance = None
    _lock = threading.Lock()
    
    @classmethod
    def get_instance(cls, server_address='192.168.219.15', server_port=9090):
        """싱글톤 인스턴스 반환"""
        with cls._lock:
            if cls._instance is None:
                cls._instance = cls(server_address, server_port)
            return cls._instance
    
    def __init__(self, server_address='192.168.219.15', server_port=9090):
        self.server_address = server_address
        self.server_port = server_port
        
        # UID 설정 - "ADMCST" (4A 44 37 35 33 37)
        self.uid = binascii.unhexlify("4A4437353337")
        
        # 소켓 연결 관련
        self.socket = None
        self.connected = False
        self.reconnect_thread = None
        self.reconnect_interval = 5  # 재연결 시도 간격(초)
        self.connection_lock = threading.Lock()
        
        # 상태 정보
        self.current_volume = 50  # 기본값 50%
        self.is_playing = False
        self.current_sound_index = 0
        self.is_repeat_mode = False
        
        # 사운드 파일 목록
        self.sound_files = [
            "Sound 0", 
            "Sound 1", 
        ]
        
        # 자동 연결 시도
        self.start_reconnect_thread()
    
    def start_reconnect_thread(self):
        """서버 재연결 스레드 시작"""
        if self.reconnect_thread is None or not self.reconnect_thread.is_alive():
            self.reconnect_thread = threading.Thread(target=self._reconnect_loop, daemon=True)
            self.reconnect_thread.start()
    
    def _reconnect_loop(self):
        """서버 재연결 시도 루프"""
        while True:
            if not self.connected:
                self.connect()
            time.sleep(self.reconnect_interval)
    
    def connect(self):
        """스피커 서버에 연결"""
        with self.connection_lock:
            if self.connected:
                return True
            
            try:
                if self.socket:
                    try:
                        self.socket.close()
                    except:
                        pass
                
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.settimeout(3.0)  # 연결 타임아웃 3초
                
                logger.info(f"스피커 서버({self.server_address}:{self.server_port})에 연결 시도...")
                self.socket.connect((self.server_address, self.server_port))
                
                self.connected = True
                logger.info(f"스피커 서버에 연결되었습니다")
                return True
            
            except (socket.error, ConnectionRefusedError) as e:
                logger.warning(f"스피커 서버 연결 실패: {e}")
                self.connected = False
                return False
            except Exception as e:
                logger.error(f"연결 중 예외 발생: {e}")
                self.connected = False
                return False
    
    def disconnect(self):
        """서버 연결 종료"""
        with self.connection_lock:
            if self.socket:
                try:
                    self.socket.close()
                except:
                    pass
                self.socket = None
            
            self.connected = False
            logger.info("스피커 서버 연결이 종료되었습니다")
    
    def is_connected(self):
        """연결 상태 확인"""
        return self.connected
    
    def _create_packet(self, dev_id, cmd_id, data=b''):
        """프로토콜에 맞는 패킷 생성
        형식: UID(6B) + Length(2B) + DevID(1B) + CmdID(1B) + Data(nB) + ETX(2B)
        """
        # Payload = DevID + CmdID + Data
        payload = bytes([dev_id, cmd_id]) + data
        payload_length = len(payload)
        
        # 패킷 생성
        packet = (
            self.uid +                                     # UID (6 bytes)
            struct.pack('>H', payload_length) +            # Length (2 bytes, big endian)
            payload +                                      # Payload (DevID + CmdID + Data)
            struct.pack('>H', ETX_VALUE)                   # ETX (2 bytes, big endian)
        )
        
        return packet
    
    def _send_command(self, dev_id, cmd_id, data=b''):
        """명령 전송"""
        if not self.connect():
            logger.error("명령 전송 실패: 서버에 연결되어 있지 않습니다")
            return False
        
        packet = self._create_packet(dev_id, cmd_id, data)
        
        try:
            with self.connection_lock:
                if not self.connected:
                    return False
                
                self.socket.sendall(packet)
                logger.debug(f"명령 전송: DevID={dev_id:02X}, CmdID={cmd_id:02X}, Data={data.hex()}")
                
                # 응답 수신은 필요에 따라 구현
                
            return True
        
        except socket.error as e:
            logger.error(f"명령 전송 중 소켓 오류: {e}")
            self.connected = False
            return False
        except Exception as e:
            logger.error(f"명령 전송 중 예외 발생: {e}")
            return False
    
    def play_sound(self, sound_index, repeat=False):
        """특정 음원 재생
        sound_index: 0-9 사이의 음원 인덱스
        repeat: True(반복재생) 또는 False(한번 재생)
        """
        if not (0 <= sound_index <= 9):
            logger.warning(f"잘못된 사운드 인덱스: {sound_index}")
            return False
        
        # 재생 명령 데이터 구성: 사운드 인덱스 + 재생 모드
        repeat_mode = 0x01 if repeat else 0x00
        data = bytes([sound_index, repeat_mode])
        
        # 상태 업데이트
        if self._send_command(DEV_COMMON, CMD_PLAY, data):
            self.is_playing = True
            self.current_sound_index = sound_index
            self.is_repeat_mode = repeat
            logger.info(f"음원 재생: 인덱스={sound_index}, 반복={repeat}")
            return True
        return False
    
    def stop_sound(self):
        """음원 재생 중지"""
        # 정지 명령 데이터: 0xFF + 0x00
        data = bytes([0xFF, 0x00])
        
        # 상태 업데이트
        if self._send_command(DEV_COMMON, CMD_PLAY, data):
            self.is_playing = False
            self.is_repeat_mode = False
            logger.info("음원 재생 정지")
            return True
        return False
    
    def set_volume(self, volume):
        """볼륨 설정 (0-100%)"""
        if not (0 <= volume <= 100):
            logger.warning(f"잘못된 볼륨값: {volume}")
            return False
        
        # 볼륨 명령 데이터
        data = bytes([volume])
        
        # 상태 업데이트
        if self._send_command(DEV_COMMON, CMD_SET_VOLUME, data):
            self.current_volume = volume
            logger.info(f"볼륨 설정: {volume}%")
            return True
        return False
    
    def get_controller_status(self):
        """퇴치기 상태 확인"""
        return {
            'connected': self.connected,
            'server_address': f"{self.server_address}:{self.server_port}",
            'current_volume': self.current_volume,
            'is_playing': self.is_playing,
            'current_sound': self.sound_files[self.current_sound_index] if self.is_playing else None,
            'current_sound_index': self.current_sound_index if self.is_playing else None,
            'is_repeat_mode': self.is_repeat_mode
        }
        
    def process_detection(self, box, frame_width, frame_height):
        """객체 감지 결과를 처리하여 조류퇴치기 제어
        box: [x1, y1, x2, y2] 좌표 리스트
        """
        if not self.connected:
            logger.warning("조류퇴치기가 연결되지 않았습니다. 감지 결과 처리를 건너뜁니다.")
            return
            
        try:
            # 스피커 발사 비활성화 (주석 처리)
            # if not self.is_playing:
            #     logger.info("객체 감지됨: 조류퇴치 사운드 재생")
            #     self.play_sound(0, repeat=False)
            
            # 로그만 남기고 실제 스피커는 작동하지 않음
            logger.info("객체 감지됨: 스피커 발사 비활성화 상태")
        except Exception as e:
            logger.error(f"조류퇴치기 제어 오류: {e}")
            
    def reset_detection(self):
        """감지 상태 초기화 - 필요시 음원 재생 중지"""
        # 아무것도 감지되지 않을 때의 처리 로직
        # 현재는 별도 조치를 취하지 않음 (재생 중인 사운드는 유지)
        pass