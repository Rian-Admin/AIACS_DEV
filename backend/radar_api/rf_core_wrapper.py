import json
import time
import threading
import socket
import random
import os
import tempfile
import shutil
import platform
from django.conf import settings
from .protocols.radar_protocol import RadarProtocol

# 레이더 통신 모드 변경
USE_REAL_RADAR = False  # 실제 레이더와 통신하지 않음
print("레이더 에뮬레이션 모드로 실행 중 (실제 통신 비활성화)")

# RadarProtocol 클래스는 .protocols.radar_protocol에서 임포트됨

# RFCoreConnector 클래스 구현 - 실제 레이더와 통신
class RFCoreConnector:
    def __init__(self, ip, port=4102):
        self.ip = ip
        self.port = port
        self.socket = None
        self.running = False
        self.state = None  # 수신 데이터 저장
        self.lock = threading.Lock()  # 데이터 동기화를 위한 Lock
        self.protocol = RadarProtocol()  # 프로토콜 인스턴스
        
        # 에뮬레이션 모드인 경우 바로 connected 상태로 설정
        if not USE_REAL_RADAR:
            self.running = True
        
    def connect(self):
        """서버에 연결을 시도합니다."""
        # 에뮬레이션 모드인 경우 항상 성공
        if not USE_REAL_RADAR:
            print(f"에뮬레이션 모드: 연결 성공 시뮬레이션 ({self.ip}:{self.port})")
            self.running = True
            self.start_receiving()
            return True, "에뮬레이션 모드: 연결 성공"
            
        if self.socket:
            self.disconnect()
        
        # 실제 서버 연결 시도
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.settimeout(5)  # 5초 타임아웃 설정
        
        try:
            print(f"연결 시도 중: {self.ip}:{self.port}")
            self.socket.connect((self.ip, self.port))
            print(f"Connected to {self.ip}:{self.port}")
            self.running = True
            self.start_receiving()
            return True, "연결 성공"
        except socket.timeout:
            print(f"연결 타임아웃: {self.ip}:{self.port}")
            self.socket = None
            return False, "연결 시간 초과"
        except ConnectionRefusedError:
            print(f"연결 거부: {self.ip}:{self.port}")
            self.socket = None
            return False, "대상 컴퓨터에서 연결을 거부했습니다"
        except socket.error as e:
            print(f"Connection failed: {e}")
            self.socket = None
            return False, f"연결 오류: {str(e)}"

    def disconnect(self):
        """서버와의 연결을 종료합니다."""
        # 에뮬레이션 모드인 경우
        if not USE_REAL_RADAR:
            print(f"에뮬레이션 모드: 연결 종료 시뮬레이션 ({self.ip}:{self.port})")
            self.running = False
            with self.lock:
                self.state = None
            return True, "에뮬레이션 모드: 연결 종료"
            
        self.running = False
        
        if self.socket:
            try:
                self.socket.close()
                self.socket = None
                print(f"연결 종료: {self.ip}:{self.port}")
                
                # 상태 초기화
                with self.lock:
                    self.state = None
                    
                return True, "연결이 성공적으로 종료되었습니다"
            except Exception as e:
                print(f"연결 종료 오류: {e}")
                return False, f"연결 종료 중 오류 발생: {str(e)}"
        else:
            return True, "이미 연결이 해제된 상태입니다"

    def send_command(self, id_value, params=None):
        """지정된 ID로 JSON 명령을 생성하여 전송합니다."""
        # 에뮬레이션 모드인 경우 항상 성공
        if not USE_REAL_RADAR:
            print(f"에뮬레이션 모드: 명령 {id_value} 전송 시뮬레이션")
            return True, f"에뮬레이션 모드: 명령 {id_value} 전송 완료"
            
        id_value = int(id_value)
        max_retry = 3  # 최대 재시도 횟수
        retry_count = 0
        
        # 연결 상태 확인
        if not self.running or not self.socket:
            print("서버에 연결되어 있지 않습니다.")
            self.reconnect()  # 재연결 시도
            if not self.running or not self.socket:
                return False, "서버에 연결되어 있지 않습니다."
        
        # 프로토콜에서 기본 명령 템플릿 가져오기
        command = self.protocol.get_command(id_value)
        if not command:
            print(f"명령 ID {id_value}를 찾을 수 없습니다.")
            return False, f"명령 ID {id_value}를 찾을 수 없습니다."
        
        # 명령 템플릿의 깊은 복사본 생성 (원본 변경 방지)
        import copy
        command = copy.deepcopy(command)
        
        # 유닉스 시간 업데이트
        import time
        current_time = time.time()
        command['header']['tv_sec'] = int(current_time)
        command['header']['tv_usec'] = int((current_time - int(current_time)) * 1000000)
        
        # 파라미터 적용
        if params and isinstance(params, dict):
            print(f"명령 {id_value}에 적용할 파라미터: {params}")
            
            # 명령 템플릿에 파라미터 적용 (구현 생략)
            
        # 명령 전송 (재시도 로직 추가)
        while retry_count < max_retry:
            try:
                # 연결 상태 확인
                if not self.socket:
                    print(f"소켓이 없습니다. 재연결 시도 {retry_count+1}/{max_retry}")
                    self.reconnect()
                    if not self.socket:
                        retry_count += 1
                        time.sleep(1)
                        continue
                
                # JSON 변환 및 전송
                import json
                command_json = json.dumps(command, ensure_ascii=False)
                print(f"전송할 명령: {command_json}")
                
                self.socket.settimeout(3.0)  # 3초 타임아웃 설정
                self.socket.sendall(command_json.encode('utf-8'))
                
                print(f"명령 {id_value} 전송 성공")
                time.sleep(0.1)  # 전송 후 잠시 대기
                
                return True, f"명령 {id_value} 전송 완료"
            except socket.error as e:
                print(f"소켓 오류 발생: {e}")
                retry_count += 1
                
                if retry_count < max_retry:
                    print(f"재연결 후 명령 재시도 중...")
                    self.reconnect()
                    time.sleep(1)  # 잠시 대기
                else:
                    print(f"최대 재시도 횟수 초과: {max_retry}회")
                    return False, f"명령 전송 실패: {e}"
            except Exception as e:
                print(f"예상치 못한 명령 전송 오류: {e}")
                retry_count += 1
                time.sleep(1)
        
        return False, "명령 전송 실패: 최대 재시도 횟수 초과"

    def start_receiving(self):
        """서버로부터 데이터를 실시간으로 수신하는 스레드를 시작합니다."""
        if self.running:
            if not USE_REAL_RADAR:
                # 에뮬레이션 모드에서는 가상 데이터 생성
                thread = threading.Thread(target=self._emulate_data_receive, daemon=True)
                thread.start()
            else:
                # 실제 통신 모드
                thread = threading.Thread(target=self.receive_data, daemon=True)
                thread.start()
    
    def _emulate_data_receive(self):
        """에뮬레이션 모드에서 가상 데이터를 생성하는 함수"""
        while self.running:
            try:
                # 가상 상태 데이터 생성
                emulated_state = {
                    "header": {
                        "id": 4001,
                        "tv_sec": int(time.time()),
                        "tv_usec": int((time.time() % 1) * 1000000)
                    },
                    "Rdp": {
                        "RdpSerialNo": 12345,
                        "State": 2,  # 2: 운용 중
                        "OperationMode": 2,
                        "ErrorCode": 0
                    },
                    "RadarPanelNum": 1,
                    "Rsp": [
                        {
                            "SerialNo": 1001,
                            "State": 2,
                            "ErrorCode": 0,
                            "Longitude": 127.123,
                            "Latitude": 37.456,
                            "Height": 10.0
                        }
                    ]
                }
                
                # 상태 업데이트
                with self.lock:
                    self.state = emulated_state
                
                # 가끔 가상 플롯 데이터 생성
                if random.random() < 0.2:  # 20% 확률로 플롯 데이터 생성
                    plots = []
                    num_plots = random.randint(1, 5)
                    for i in range(num_plots):
                        plots.append({
                            "plotId": i + 1,
                            "rsp": 0,
                            "range": random.uniform(100, 2000),
                            "azimuth": random.uniform(0, 359),
                            "elevation": random.uniform(0, 45),
                            "rcs": random.uniform(-20, 10),
                            "doppler": random.uniform(-50, 50)
                        })
                    
                    plot_data = {
                        "header": {
                            "id": 4011,
                            "tv_sec": int(time.time()),
                            "tv_usec": int((time.time() % 1) * 1000000)
                        },
                        "plotTime": int(time.time()),
                        "nPlot": len(plots),
                        "plots": plots
                    }
                    
                    with self.lock:
                        self.state = plot_data
                    
                # 10초에 1번씩 트랙 데이터 생성
                if random.random() < 0.1:  # 10% 확률로 트랙 데이터 생성
                    track_data = {
                        "header": {
                            "id": 4016,
                            "tv_sec": int(time.time()),
                            "tv_usec": int((time.time() % 1) * 1000000)
                        },
                        "trackId": random.randint(1, 100),
                        "trackTime": int(time.time()),
                        "rsp": 0,
                        "x": random.uniform(-2000, 2000),
                        "y": random.uniform(-2000, 2000),
                        "z": random.uniform(10, 1000),
                        "vx": random.uniform(-50, 50),
                        "vy": random.uniform(-50, 50),
                        "vz": random.uniform(-20, 20)
                    }
                    
                    with self.lock:
                        self.state = track_data
                
                # 주기적인 업데이트
                time.sleep(1.0)
                
            except Exception as e:
                print(f"에뮬레이션 데이터 생성 오류: {e}")
                time.sleep(1.0)

    def receive_data(self):
        """서버로부터 데이터를 수신하여 동기화합니다."""
        # 이 함수는 에뮬레이션 모드에서는 사용하지 않음 (코드 생략)
        pass

    def get_state(self):
        """현재 상태 데이터를 반환합니다."""
        with self.lock:
            return self.state

# 레이더 시스템 래퍼 클래스 (기존 유지)
class RFCoreWrapper:
    _instance = None
    _lock = threading.Lock()
    
    @classmethod
    def get_instance(cls):
        """싱글톤 인스턴스를 반환합니다."""
        if cls._instance is None:
            with cls._lock:
                if cls._instance is None:
                    cls._instance = cls()
        return cls._instance
    
    def __init__(self):
        """레이더 코어 래퍼 초기화"""
        self.connector = None  # 연결 객체
        self.protocol = RadarProtocol()  # 프로토콜 인스턴스
        self.simulation_active = False
        
        # 상태 업데이트 스레드
        self.status_thread = None
        self.status_running = False
        
        # 에뮬레이션 모드에서는 초기화 시 바로 상태 업데이트 스레드 시작
        if not USE_REAL_RADAR:
            self.simulation_active = True
            self.status_running = True
            self.status_thread = threading.Thread(target=self._status_update_loop, daemon=True)
            self.status_thread.start()
    
    def connect(self, ip, port=4102):
        """레이더 시스템에 연결"""
        # 에뮬레이션 모드에서는 항상 성공
        if not USE_REAL_RADAR:
            print(f"에뮬레이션 모드: 연결 성공 시뮬레이션 ({ip}:{port})")
            
            # 이미 연결된 상태라면 기존 연결 종료
            if self.connector:
                self.disconnect()
            
            # 에뮬레이션 연결 생성
            self.connector = RFCoreConnector(ip, port)
            success, message = self.connector.connect()
            
            # 상태 업데이트 스레드가 아직 실행 중이 아니라면 시작
            if not self.status_running:
                self.status_running = True
                self.status_thread = threading.Thread(target=self._status_update_loop, daemon=True)
                self.status_thread.start()
            
            return True, "에뮬레이션 모드: 연결 성공"
        
        # 이미 연결된 상태라면 기존 연결 종료
        if self.connector:
            self.disconnect()
        
        # 새로운 연결 생성
        try:
            print(f"레이더 시스템 연결 시도: {ip}:{port}")
            self.connector = RFCoreConnector(ip, port)
            success, message = self.connector.connect()
            
            if success:
                # 상태 업데이트 스레드 시작
                if not self.status_running:
                    self.status_running = True
                    self.status_thread = threading.Thread(target=self._status_update_loop, daemon=True)
                    self.status_thread.start()
            else:
                self.connector = None
            
            return success, message
        except Exception as e:
            print(f"연결 오류: {e}")
            self.connector = None
            return False, f"연결 중 오류 발생: {str(e)}"
    
    def disconnect(self):
        """레이더 시스템 연결을 종료합니다."""
        if self.connector:
            success, message = self.connector.disconnect()
            self.connector = None
            self.simulation_active = False
            print("레이더 시스템 연결 해제")
            return success, message
        return True, "연결된 시스템 없음"
    
    def send_command(self, command_id, params=None):
        """지정된 명령어를 레이더에 전송합니다."""
        try:
            command_id = int(command_id)
            
            # 3000번대 명령어만 전송 가능
            if not (3000 <= command_id < 4000):
                return False, f"명령 ID {command_id}는 전송할 수 없습니다. 3000-3999 범위의 명령어만 전송 가능합니다."
            
            # 연결 상태 확인
            if not self.connector:
                return False, "레이더 시스템이 초기화되지 않았습니다."
                
            if not self.simulation_active:
                return False, "레이더 시스템에 연결되어 있지 않습니다."
                
            # 파라미터 전달
            success, message = self.connector.send_command(command_id, params)
            
            if not success:
                # 전송 실패 - 연결 상태 확인 및 업데이트
                self.simulation_active = False
                return False, f"명령 {command_id} 전송 실패: {message}"
            
            return True, f"명령 {command_id} 전송 완료"
        except Exception as e:
            self.simulation_active = False  # 오류 발생 시 연결 상태 초기화
            print(f"명령 전송 중 예외 발생: {str(e)}")
            return False, f"명령 전송 중 오류 발생: {str(e)}"
    
    def get_state(self):
        """현재 저장된 상태 데이터를 반환합니다."""
        if not self.connector or not self.simulation_active:
            return {
                "connected": False, 
                "state": None, 
                "last_update": 0, 
                "is_4001": False, 
                "has_new_data": False,
                "last_4001_state": None, 
                "last_other_state": None,
                "last_plot_states": [],
                "last_track_states": []
            }
        
        # 새 상태 가져오기
        state = self.connector.get_state()
        has_new_data = state is not None and state != self.last_state
        is_4001 = False
        is_plot = False
        is_track = False
        
        if state:
            # 기존 상태와 다른 새 데이터인 경우만 처리
            if state != self.last_state:
                self.last_state = state
                self.last_state_time = time.time()
                
                # 메시지 타입 구분
                if isinstance(state, dict) and 'header' in state:
                    message_id = state['header'].get('id')
                    
                    # 4001: 상태 메시지
                    if message_id == 4001:
                        is_4001 = True
                        self.last_4001_state = state
                        # 실제 에뮬레이터는 RdpSerialNo 키를 사용
                        serial_no = state['Rdp'].get('RdpSerialNo', 0)
                        rdp_state = state['Rdp'].get('State', 0)
                        print(f"4001 상태 메시지 수신: 상태={rdp_state}, 시리얼번호={serial_no}")
                    
                    # 4011, 4012, 4013: 플롯 데이터
                    elif message_id in [4011, 4012, 4013]:
                        is_plot = True
                        
                        # 플롯 데이터 형식 처리 (nPlot 및 Plot 배열 처리)
                        if 'nPlot' in state and 'Plot' in state and isinstance(state['Plot'], list):
                            # Plot 배열에서 플롯 정보 추출
                            for plot_item in state['Plot']:
                                plot_id = plot_item.get('plotId', f"plot-{int(time.time())}-{len(self.last_plot_states)}")
                                
                                # 플롯 객체 생성
                                plot_data = plot_item.copy()  # 원본 데이터 복사
                                
                                # 플롯 ID 저장
                                if 'plotId' not in plot_data:
                                    plot_data['plotId'] = plot_id
                                
                                # position 필드 추가 (직교좌표계)
                                if 'position' not in plot_data:
                                    # 에뮬레이터는 Range, Azimuth, Elevation 대문자로 보냄
                                    if 'Range' in plot_data and 'Azimuth' in plot_data:
                                        import math
                                        range_val = plot_data.get('Range', 0)
                                        azimuth_val = plot_data.get('Azimuth', 0)
                                        elevation_val = plot_data.get('Elevation', 0)
                                        
                                        # 극좌표계 -> 직교좌표계 변환
                                        x = range_val * math.cos(math.radians(elevation_val)) * math.cos(math.radians(azimuth_val))
                                        y = range_val * math.cos(math.radians(elevation_val)) * math.sin(math.radians(azimuth_val))
                                        z = range_val * math.sin(math.radians(elevation_val))
                                        plot_data['position'] = [x, y, z]
                                    else:
                                        plot_data['position'] = [0, 0, 0]
                                
                                # range, angle 필드 추가 (프론트엔드 호환성)
                                if 'range' not in plot_data and 'Range' in plot_data:
                                    plot_data['range'] = plot_data['Range']
                                if 'angle' not in plot_data and 'Azimuth' in plot_data:
                                    plot_data['angle'] = plot_data['Azimuth']
                                if 'elevation' not in plot_data and 'Elevation' in plot_data:
                                    plot_data['elevation'] = plot_data['Elevation']
                                if 'rcs' not in plot_data and 'RCS' in plot_data:
                                    plot_data['rcs'] = plot_data['RCS']
                                
                                # 메시지 타입 및 헤더 정보 저장
                                plot_data['message_type'] = message_id
                                if 'header' not in plot_data:
                                    plot_data['header'] = state['header']
                                
                                # 위치가 [0,0,0]이고 RCS가 0인 플롯만 필터링
                                if plot_data.get('position') == [0, 0, 0] and plot_data.get('rcs', 0) == 0:
                                    print(f"플롯 필터링 (RFCoreWrapper): ID={plot_data['plotId']}, 위치={plot_data.get('position')}, RCS={plot_data.get('rcs', 0)}")
                                else:
                                    # 플롯 데이터를 최대 10개까지만 저장
                                    if len(self.last_plot_states) >= 10:
                                        self.last_plot_states.pop(0)  # 가장 오래된 항목 제거
                                    
                                    self.last_plot_states.append(plot_data)
                                    print(f"플롯 데이터 추가: ID={plot_data['plotId']}, 위치={plot_data.get('position')}")
                        else:
                            # 기존 형식 호환 (PlotData 형식)
                            if 'plotId' not in state:
                                plot_id = f"plot-{int(time.time())}-{len(self.last_plot_states)}"
                                plot_data = state.copy()
                                plot_data['plotId'] = plot_id
                                
                                # position 필드 없으면 생성 (필요한 경우)
                                if 'range' in plot_data and 'angle' in plot_data:
                                    # 극좌표계 -> 직교좌표계 변환 (간단한 예시)
                                    import math
                                    range_val = plot_data.get('range', 0)
                                    angle_val = plot_data.get('angle', 0) * math.pi / 180  # 각도->라디안
                                    x = range_val * math.cos(angle_val)
                                    y = range_val * math.sin(angle_val)
                                    z = plot_data.get('height', 0)  # 높이 정보가 있으면 사용
                                    plot_data['position'] = [x, y, z]
                                else:
                                    # 위치 정보 없으면 기본값
                                    plot_data['position'] = [0, 0, 0]
                            else:
                                plot_data = state
                            
                            # 위치가 [0,0,0]이고 RCS가 0인 플롯만 필터링
                            if plot_data.get('position') == [0, 0, 0] and plot_data.get('rcs', 0) == 0:
                                print(f"플롯 필터링 (RFCoreWrapper): ID={plot_data['plotId']}, 위치={plot_data.get('position')}, RCS={plot_data.get('rcs', 0)}")
                            else:
                                # 플롯 데이터를 최대 10개까지만 저장
                                if len(self.last_plot_states) >= 10:
                                    self.last_plot_states.pop(0)  # 가장 오래된 항목 제거
                                self.last_plot_states.append(plot_data)
                                print(f"플롯 데이터 추가 (형식 호환): ID={plot_data['plotId']}, 위치={plot_data.get('position')}")
                    
                    # 4016: 트랙 데이터
                    elif message_id == 4016:
                        is_track = True
                        # 트랙 데이터 준비 - ID 필드가 없는 경우 가상 ID 생성
                        if 'trackId' not in state:
                            track_id = f"track-{int(time.time())}-{len(self.last_track_states)}"
                            track_data = state.copy()
                            track_data['trackId'] = track_id
                            
                            # 필요한 필드 추가
                            if 'position' not in track_data:
                                # 트랙 메시지 구조에 따라 위치 정보 추출
                                if 'Range' in track_data and 'Azimuth' in track_data:
                                    # 극좌표계 -> 직교좌표계 변환
                                    import math
                                    range_val = float(track_data.get('Range', 0))
                                    azimuth_val = float(track_data.get('Azimuth', 0))
                                    elevation_val = float(track_data.get('Elevation', 0))
                                    
                                    # 직교좌표 계산 (방위각이 북쪽이 0도, 시계방향으로 증가하는 경우)
                                    azimuth_rad = math.radians(azimuth_val)
                                    elevation_rad = math.radians(elevation_val)
                                    
                                    # 북쪽(y+), 동쪽(x+) 좌표계 변환 
                                    x = range_val * math.sin(azimuth_rad) * math.cos(elevation_rad)
                                    y = range_val * math.cos(azimuth_rad) * math.cos(elevation_rad)
                                    z = range_val * math.sin(elevation_rad)
                                    
                                    track_data['position'] = [x, y, z]
                                elif 'range' in track_data and 'angle' in track_data:
                                    # 극좌표계 -> 직교좌표계 변환 (이전 코드)
                                    import math
                                    range_val = float(track_data.get('range', 0))
                                    angle_val = float(track_data.get('angle', 0)) * math.pi / 180
                                    x = range_val * math.cos(angle_val)
                                    y = range_val * math.sin(angle_val)
                                    z = float(track_data.get('height', 0))
                                    track_data['position'] = [x, y, z]
                                else:
                                    # 기본값 설정 (0,0,0은 피함)
                                    track_data['position'] = [1, 1, 0]  # 최소한의 위치 정보 제공
                            
                            if 'velocity' not in track_data:
                                # 속도 정보 추출
                                if 'vEast' in track_data and 'vNorth' in track_data and 'vUp' in track_data:
                                    track_data['velocity'] = [
                                        float(track_data.get('vEast', 0)),
                                        float(track_data.get('vNorth', 0)),
                                        float(track_data.get('vUp', 0))
                                    ]
                                else:
                                    # 기본 속도값 설정
                                    track_data['velocity'] = [0.1, 0.1, 0]  # 최소한의 속도 정보 제공
                        else:
                            track_data = state
                        
                        # 트랙 포지션 데이터 변환 (Range, Azimuth, Elevation 형식)
                        if 'Range' in track_data and 'Azimuth' in track_data and 'position' not in track_data:
                            # 극좌표계 -> 직교좌표계 변환
                            import math
                            range_val = float(track_data.get('Range', 0))
                            azimuth_val = float(track_data.get('Azimuth', 0))
                            elevation_val = float(track_data.get('Elevation', 0))
                            
                            # 직교좌표 계산 (방위각이 북쪽이 0도, 시계방향으로 증가하는 경우)
                            azimuth_rad = math.radians(azimuth_val)
                            elevation_rad = math.radians(elevation_val)
                            
                            # 북쪽(y+), 동쪽(x+) 좌표계 변환 
                            x = range_val * math.sin(azimuth_rad) * math.cos(elevation_rad)
                            y = range_val * math.cos(azimuth_rad) * math.cos(elevation_rad)
                            z = range_val * math.sin(elevation_rad)
                            
                            track_data['position'] = [x, y, z]
                        
                        # 트랙 속도 데이터 변환 (vEast, vNorth, vUp 형식)
                        if 'vEast' in track_data and 'vNorth' in track_data and 'velocity' not in track_data:
                            track_data['velocity'] = [
                                float(track_data.get('vEast', 0)),
                                float(track_data.get('vNorth', 0)),
                                float(track_data.get('vUp', 0))
                            ]
                        
                        # 로그: 트랙 데이터 확인
                        position_str = str(track_data.get('position', []))
                        print(f"트랙 데이터 처리: ID={track_data.get('trackId')}, 위치={position_str}")
                        
                        # 트랙 데이터를 최대 6개까지만 저장
                        if len(self.last_track_states) >= 6:
                            self.last_track_states.pop(0)  # 가장 오래된 항목 제거
                        
                        self.last_track_states.append(track_data)
                    
                    # 기타 메시지
                    else:
                        self.last_other_state = state
                        print(f"기타 메시지 수신: ID {message_id}")
        
        return {
            "connected": self.simulation_active,
            "state": self.last_state,
            "last_update": self.last_state_time,
            "is_4001": is_4001,
            "is_plot": is_plot,
            "is_track": is_track,
            "has_new_data": has_new_data,
            "last_4001_state": self.last_4001_state,
            "last_other_state": self.last_other_state,
            "last_plot_states": self.last_plot_states,
            "last_track_states": self.last_track_states
        }
            
    def _status_update_loop(self):
        """상태 정보를 주기적으로 업데이트하는 스레드 함수입니다."""
        connection_check_interval = 10  # 연결 상태 확인 간격 (초)
        last_check_time = 0
        reconnect_attempt = 0
        max_reconnect_attempts = 5
        last_ip = None
        last_port = None
        
        # 자동 재연결 기능 비활성화 플래그
        auto_reconnect_enabled = False  # 자동 재연결 기능을 비활성화
        
        while True:
            try:
                current_time = time.time()
                
                # 연결 상태 확인 (일정 간격으로)
                if current_time - last_check_time >= connection_check_interval:
                    last_check_time = current_time
                    
                    # 연결 상태 확인
                    is_connected = False
                    if self.connector and self.simulation_active:
                        # 소켓 체크를 통해 실제 연결 상태 확인
                        try:
                            if self.connector.socket:
                                # 비차단 모드로 0 바이트 전송 시도
                                self.connector.socket.settimeout(0.1)
                                self.connector.socket.send(b'')
                                is_connected = True
                        except Exception as e:
                            print(f"소켓 연결 확인 오류: {e}")
                            is_connected = False
                    
                    # 연결 상태 불일치 처리
                    if not is_connected and self.simulation_active:
                        print("연결 상태 불일치: 연결이 끊어졌지만 상태는 연결됨으로 표시되어 있음")
                        
                        # 자동 재연결 시도 기능 비활성화
                        if auto_reconnect_enabled and last_ip and last_port and reconnect_attempt < max_reconnect_attempts:
                            reconnect_attempt += 1
                            print(f"자동 재연결 시도 {reconnect_attempt}/{max_reconnect_attempts}")
                            
                            # 기존 연결 종료
                            if self.connector:
                                self.disconnect()
                            
                            # 재연결 시도
                            success = self.connect(last_ip, last_port)
                            if success:
                                print(f"자동 재연결 성공: {last_ip}:{last_port}")
                                reconnect_attempt = 0  # 성공 시 카운터 초기화
                            else:
                                print(f"자동 재연결 실패: {last_ip}:{last_port}")
                        else:
                            # 자동 재연결을 시도하지 않음 (비활성화됨)
                            print("자동 재연결이 비활성화됨 - 사용자가 수동으로 연결해야 합니다")
                            
                            # 연결 상태 업데이트
                            self.simulation_active = False
                    elif is_connected and not self.simulation_active:
                        # 실제로는 연결되어 있지만 상태가 잘못된 경우
                        print("연결 상태 불일치: 실제로는 연결되어 있지만 상태는 끊어짐으로 표시되어 있음")
                        self.simulation_active = True
                    elif is_connected and self.simulation_active:
                        # 정상 연결 상태
                        reconnect_attempt = 0  # 재연결 시도 카운터 초기화
                    
                    # 연결 상태에 따라 연결 정보 업데이트
                    if self.simulation_active and self.connector:
                        last_ip = self.connector.ip
                        last_port = self.connector.port
                
                # 데이터가 도착했는지 확인
                if self.connector and self.simulation_active:
                    new_state = self.connector.get_state()
                    if new_state != self.last_state:
                        self.last_state = new_state
                        self.last_state_time = time.time()
                        
                        # 메시지 타입 구분
                        if isinstance(new_state, dict) and 'header' in new_state:
                            message_id = new_state['header'].get('id')
                            
                            # 4001: 상태 메시지
                            if message_id == 4001:
                                self.last_4001_state = new_state
                            # 4011, 4012, 4013: 플롯 데이터
                            elif message_id in [4011, 4012, 4013]:
                                # 플롯 데이터를 최대 10개까지만 저장
                                if len(self.last_plot_states) >= 10:
                                    self.last_plot_states.pop(0)  # 가장 오래된 항목 제거
                                self.last_plot_states.append(new_state)
                                print(f"플롯 데이터({message_id}) 메시지 수신: ID={new_state.get('plotId', 'N/A')}")
                            # 4016: 트랙 데이터
                            elif message_id == 4016:
                                # 트랙 데이터를 최대 6개까지만 저장
                                if len(self.last_track_states) >= 6:
                                    self.last_track_states.pop(0)  # 가장 오래된 항목 제거
                                self.last_track_states.append(new_state)
                                print(f"트랙 데이터(4016) 메시지 수신: ID={new_state.get('trackId', 'N/A')}")
                            # 기타 메시지
                            else:
                                self.last_other_state = new_state
                                print(f"기타 메시지 수신: ID {message_id}")
                
                # 잠시 대기
                time.sleep(1)
            except Exception as e:
                print(f"상태 업데이트 스레드 오류: {e}")
                time.sleep(5)  # 오류 발생 시 잠시 대기
    
    def get_available_commands(self):
        """사용 가능한 모든 명령어 목록을 반환합니다."""
        all_commands = self.protocol.list_commands()
        # 3000번대 명령어만 필터링 (3000-3999)
        return [cmd for cmd in all_commands if 3000 <= cmd < 4000]
    
    def get_command_details(self, command_id):
        """특정 명령어의 상세 정보를 반환합니다."""
        return self.protocol.get_command_json(command_id)
    
    def clear_state(self):
        """상태 정보를 초기화합니다."""
        self.last_state = None
        self.last_4001_state = None
        self.last_other_state = None
        if self.connector:
            with self.connector.lock:
                self.connector.state = None
                print("상태 정보 초기화됨")
        return True, "상태 정보 초기화 완료"
    
    def get_simulation_status(self):
        """현재 시뮬레이션 상태를 반환합니다."""
        if not self.simulation_active:
            return {
                "running": False,
                "message": "에뮬레이터에 연결되어 있지 않습니다."
            }
        
        # 현재 구현에서는 간단하게 연결 상태만 확인
        # 실제 구현에서는 에뮬레이터의 시뮬레이션 상태를 확인하는 로직 필요
        return {
            "running": self.simulation_active,
            "message": "에뮬레이터에 연결되어 있습니다."
        }
    
    def send_simulation_scenario(self, scenario_data):
        """시뮬레이션 시나리오 데이터를 에뮬레이터로 전송합니다.
        에뮬레이터는 T_Scenario.json 파일을 사용하여 시나리오를 로드합니다."""
        try:
            if not self.connector or not self.simulation_active:
                return False, "에뮬레이터에 연결되어 있지 않습니다."
            
            # 로그 추가
            print(f"레이더 에뮬레이터 직접 모드: 시나리오 로드 없이 에뮬레이터 데이터만 수신합니다.")
            
            # 플롯/트랙 설정 활성화 - 에뮬레이터에서 데이터 수신을 위한 설정
            success, message = self.send_command(3005, {
                "PlotSendFlag": 1,
                "PlotType": 3,
                "TrackSendFlag": 1,
                "TrackType": 3
            })
            
            if not success:
                print(f"플롯/트랙 설정 실패: {message}")
                return False, f"플롯/트랙 설정 실패: {message}"
            
            print("플롯/트랙 설정 완료, 에뮬레이터에서 데이터를 수신 대기합니다.")
            
            # 운용 모드 설정 (운용 중으로 설정)
            success, message = self.send_command(3002, {
                "OperationMode": 2  # 2: 운용 모드
            })
            
            if not success:
                print(f"운용 모드 설정 실패: {message}")
                return False, f"운용 모드 설정 실패: {message}"
            
            print("운용 모드 설정 완료, 에뮬레이터 데이터 수신 준비됨")
            
            return True, "에뮬레이터에서 직접 데이터 수신 모드로 설정되었습니다."
            
        except Exception as e:
            print(f"시뮬레이션 설정 중 오류 발생: {str(e)}")
            import traceback
            traceback.print_exc()
            return False, f"에뮬레이터 설정 중 오류 발생: {str(e)}"
    
    def stop_simulation(self):
        """시뮬레이션을 중지합니다."""
        try:
            if not self.connector or not self.simulation_active:
                return False, "에뮬레이터에 연결되어 있지 않습니다."
            
            # 실제 구현에서는 에뮬레이터에 시뮬레이션 중지 명령을 전송
            # 현재는 간단히 플롯/트랙 데이터 전송을 비활성화
            success, message = self.send_command(3005, {
                "PlotSendFlag": 0,      # 플롯 데이터 전송 비활성화
                "PlotType": 3,          # 플롯 데이터 타입 (3: Polar)
                "TrackSendFlag": 0,     # 트랙 데이터 전송 비활성화
                "TrackType": 3          # 트랙 데이터 타입 (3: Polar)
            })
            
            if not success:
                return False, f"시뮬레이션 중지 실패: {message}"
            
            return True, "시뮬레이션이 중지되었습니다."
            
        except Exception as e:
            print(f"시뮬레이션 중지 중 오류 발생: {str(e)}")
            return False, f"시뮬레이션 중지 중 오류 발생: {str(e)}"

    def is_connected(self):
        """연결 상태를 반환합니다."""
        return self.simulation_active and self.connector is not None

    def is_operating(self):
        """운용 모드 여부를 반환합니다 (1=Standby, 2=Operation)."""
        with self._lock:
            if isinstance(self.last_state, dict) and 'Rdp' in self.last_state:
                return self.last_state['Rdp'].get('State', 0) == 2
            return False

    def is_plot_track_enabled(self):
        """플롯/트랙 전송 활성화 여부를 반환합니다."""
        # 플롯/트랙 설정이 되어있는지 판단 (이전에 3005 명령을 보냈는지)
        plot_track_sent = False
        if hasattr(self, 'last_commands'):
            for cmd_id, cmd_data in self.last_commands.items():
                if cmd_id == 3005 and 'PlotSendFlag' in cmd_data and cmd_data['PlotSendFlag'] == 1:
                    plot_track_sent = True
                    break
        return plot_track_sent 