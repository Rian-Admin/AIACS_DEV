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

print("실제 레이더 에뮬레이터와 통신하는 모드로 실행 중")

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
        
    def connect(self):
        """서버에 연결을 시도합니다."""
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
            
            # ID에 따라 특별히 처리할 파라미터
            if id_value == 3001:
                # 시간 동기화 명령의 경우
                if 'unix_sec' in params and 'unix_usec' in params:
                    command['header']['tv_sec'] = int(params['unix_sec'])
                    command['header']['tv_usec'] = int(params['unix_usec'])
                    print(f"시간 동기화 파라미터 적용: {params['unix_sec']}.{params['unix_usec']}")
            
            elif id_value == 3002:
                # 운용 제어 명령의 경우
                if 'OperationMode' in params:
                    command['OperationMode'] = int(params['OperationMode'])
                    print(f"운용 모드 설정: {command['OperationMode']} (1: 대기, 2: 운용)")
            
            elif id_value == 3014:
                # 운용 파라미터 설정 명령의 경우
                try:
                    # RDP(운용통제기) 파라미터 적용
                    if 'Rdp' in params:
                        rdp = params['Rdp']
                        for key, value in rdp.items():
                            if key in command['Rdp']:
                                command['Rdp'][key] = value
                                print(f"RDP {key} 설정: {value}")
                    
                    # 개별 RDP 필드 적용 (프론트엔드에서 플랫 구조로 전달된 경우)
                    for key in params:
                        if key.startswith('Rdp.'):
                            field = key.split('.')[-1]
                            if field in command['Rdp']:
                                command['Rdp'][field] = params[key]
                                print(f"RDP {field} 설정: {params[key]}")
                    
                    # 레이더 패널 개수 설정
                    if 'RadarPanelNum' in params:
                        panel_num = int(params['RadarPanelNum'])
                        command['RadarPanelNum'] = panel_num
                        # 패널 수에 맞게 Rsp 배열 조정
                        while len(command['Rsp']) < panel_num:
                            command['Rsp'].append(copy.deepcopy(command['Rsp'][0]))
                        while len(command['Rsp']) > panel_num:
                            command['Rsp'].pop()
                    
                    # 레이더 패널(RSP) 파라미터 설정
                    if 'Rsp' in params and isinstance(params['Rsp'], list):
                        for i, panel in enumerate(params['Rsp']):
                            if i < len(command['Rsp']):
                                for key, value in panel.items():
                                    if key in command['Rsp'][i]:
                                        command['Rsp'][i][key] = value
                                        print(f"RSP[{i}] {key} 설정: {value}")
                    
                    # 개별 RSP 필드 적용 (프론트엔드에서 플랫 구조로 전달된 경우)
                    for key in params:
                        if key.startswith('Rsp['):
                            # Rsp[0].Longitude 형태 파싱
                            parts = key.split('.')
                            idx_str = parts[0][4:-1]  # Rsp[0] -> 0
                            field = parts[1]
                            idx = int(idx_str)
                            
                            if idx < len(command['Rsp']) and field in command['Rsp'][idx]:
                                command['Rsp'][idx][field] = params[key]
                                print(f"RSP[{idx}] {field} 설정: {params[key]}")
                    
                    print(f"최종 운용 파라미터: {command}")
                except Exception as e:
                    print(f"운용 파라미터 적용 오류: {e}")
                    return False, f"운용 파라미터 적용 오류: {e}"
            
            elif id_value == 3012:
                # 성능 파라미터 설정 명령의 경우
                try:
                    # RDP 파라미터 적용
                    if 'Rdp' in params:
                        rdp = params['Rdp']
                        for key, value in rdp.items():
                            if key in command['Rdp']:
                                command['Rdp'][key] = value
                    
                    # 개별 RDP 필드 적용
                    for key in params:
                        if key.startswith('Rdp.'):
                            field = key.split('.')[-1]
                            if field in command['Rdp']:
                                command['Rdp'][field] = params[key]
                    
                    # 레이더 패널 개수 설정
                    if 'RadarPanelNum' in params:
                        panel_num = int(params['RadarPanelNum'])
                        command['RadarPanelNum'] = panel_num
                        # 패널 수에 맞게 Rsp 배열 조정
                        while len(command['Rsp']) < panel_num:
                            command['Rsp'].append(copy.deepcopy(command['Rsp'][0]))
                        while len(command['Rsp']) > panel_num:
                            command['Rsp'].pop()
                    
                    # 레이더 패널 파라미터 설정
                    if 'Rsp' in params and isinstance(params['Rsp'], list):
                        for i, panel in enumerate(params['Rsp']):
                            if i < len(command['Rsp']):
                                for key, value in panel.items():
                                    if key in command['Rsp'][i]:
                                        command['Rsp'][i][key] = value
                    
                    # 개별 RSP 필드 적용
                    for key in params:
                        if key.startswith('Rsp['):
                            parts = key.split('.')
                            idx_str = parts[0][4:-1]
                            field = parts[1]
                            idx = int(idx_str)
                            
                            if idx < len(command['Rsp']) and field in command['Rsp'][idx]:
                                command['Rsp'][idx][field] = params[key]
                    
                    print(f"최종 성능 파라미터: {command}")
                except Exception as e:
                    print(f"성능 파라미터 적용 오류: {e}")
                    return False, f"성능 파라미터 적용 오류: {e}"
            
            elif id_value == 3005:
                # 플롯트랙설정 명령의 경우
                for key in ['PlotSendFlag', 'PlotType', 'TrackSendFlag', 'TrackType']:
                    if key in params:
                        command[key] = int(params[key])
                print(f"플롯/트랙 설정: {command}")
            
            else:
                # 그 외 명령의 경우 일반적인 파라미터 적용
                for key, value in params.items():
                    # 헤더는 이미 처리되었으므로 건너뜀
                    if key != 'header':
                        command[key] = value
                print(f"일반 파라미터 적용: {params}")
        
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
            thread = threading.Thread(target=self.receive_data, daemon=True)
            thread.start()

    def receive_data(self):
        """서버로부터 데이터를 수신하여 동기화합니다."""
        reconnect_attempt = 0
        max_reconnect_attempts = 10  # 최대 재연결 시도 횟수 증가
        consecutive_errors = 0
        max_consecutive_errors = 3
        
        # 자동 재연결 비활성화 플래그
        auto_reconnect_enabled = False  # 자동 재연결 기능 비활성화

        # 메시지 버퍼 추가
        buffer = ""

        while self.running:
            try:
                # 소켓 연결 확인
                if not self.socket:
                    # 소켓이 없으면 잠시 대기
                    time.sleep(0.5)
                    
                    # 자동 재연결 비활성화: 소켓이 없어도 자동으로 재연결하지 않음
                    if auto_reconnect_enabled:
                        reconnect_attempt += 1
                        
                        if reconnect_attempt <= max_reconnect_attempts:
                            print(f"소켓 없음, 연결 시도 {reconnect_attempt}/{max_reconnect_attempts}")
                            success = self.reconnect()
                            if not success:
                                time.sleep(3)  # 재연결 실패 시 대기
                        else:
                            print(f"최대 재연결 시도 횟수 초과 ({max_reconnect_attempts}회), 다시 처음부터 시도합니다.")
                            reconnect_attempt = 0  # 재연결 시도 횟수 초기화
                            time.sleep(5)  # 잠시 대기 후 다시 시도
                    else:
                        print("자동 재연결이 비활성화됨 - 소켓이 연결되지 않았습니다")
                        time.sleep(5)  # 잠시 대기
                    continue  # 다음 루프로 이동
                
                try:
                    # 소켓이 유효한지 확인 (비차단 모드로 0 바이트 전송 시도)
                    self.socket.settimeout(0.01)
                    self.socket.send(b'')
                    
                    # 데이터 수신
                    self.socket.settimeout(1.0)  # 1초 타임아웃
                    data = self.socket.recv(16384)  # 버퍼 크기 증가 (8192 -> 16384)
                    
                    if not data:
                        # 데이터가 없으면 소켓이 닫혔을 가능성이 있음
                        print("데이터 수신 없음, 소켓이 닫혔을 가능성 있음")
                        consecutive_errors += 1
                        if consecutive_errors >= max_consecutive_errors:
                            raise socket.error("Connection closed")
                        continue  # 다음 루프로 이동
                    
                    # 디버깅용: 원시 데이터 출력
                    raw_data_str = data.decode('utf-8', errors='replace')
                    print(f"수신된 원시 데이터 (처음 100자): {raw_data_str[:100]}")
                    
                    buffer += raw_data_str
                    
                    # 여러 JSON 객체가 연결된 경우를 처리하기 위한 로직
                    while buffer:
                        try:
                            # JSON 객체 하나를 파싱
                            parsed_data = json.loads(buffer)
                            buffer = ""  # 버퍼 비우기
                            
                            # 데이터 동기화
                            with self.lock:
                                self.state = parsed_data
                            
                            # 메시지 ID 로깅
                            if isinstance(parsed_data, dict) and 'header' in parsed_data:
                                message_id = parsed_data['header'].get('id')
                                if message_id == 4001:
                                    serial_no = parsed_data['Rdp'].get('RdpSerialNo', 0)
                                    rdp_state = parsed_data['Rdp'].get('State', 0)
                                    print(f"4001 상태 메시지 수신: 상태={rdp_state}, 시리얼번호={serial_no}")
                                elif message_id == 4011:
                                    print(f"플롯 데이터(4011) 메시지 수신: {json.dumps(parsed_data, indent=2)[:100]}...")
                                    # 플롯 수 출력
                                    if 'nPlot' in parsed_data:
                                        print(f"플롯 수: {parsed_data.get('nPlot', 0)}")
                                elif message_id == 4012:
                                    print(f"플롯 데이터(4012) 메시지 수신: {json.dumps(parsed_data, indent=2)[:100]}...")
                                    # 플롯 수 출력
                                    if 'nPlot' in parsed_data:
                                        print(f"플롯 수: {parsed_data.get('nPlot', 0)}")
                                elif message_id == 4013:
                                    print(f"플롯 데이터(4013) 메시지 수신: {json.dumps(parsed_data, indent=2)[:100]}...")
                                    # 플롯 수 출력
                                    if 'nPlot' in parsed_data:
                                        print(f"플롯 수: {parsed_data.get('nPlot', 0)}")
                                elif message_id == 4016:
                                    print(f"트랙 데이터(4016) 메시지 수신: ID={parsed_data.get('trackId', 'N/A')}")
                                else:
                                    print(f"기타 메시지 수신: ID {message_id}")
                            
                            reconnect_attempt = 0  # 성공적인 수신 시 재연결 카운터 초기화
                            consecutive_errors = 0  # 오류 카운터 초기화
                            
                            # 긴 메시지 처리 후 잠시 쉬기
                            time.sleep(0.001)
                            
                            break  # 성공적으로 파싱했으므로 루프 종료
                            
                        except json.JSONDecodeError as e:
                            # 완전한 JSON 객체가 아닌 경우, 다음 중괄호 위치 찾기
                            next_json_start = buffer.find('{"header"', 1)
                            if next_json_start > 0:
                                # 첫 JSON 객체까지만 파싱 시도
                                first_json = buffer[:next_json_start]
                                try:
                                    parsed_data = json.loads(first_json)
                                    with self.lock:
                                        self.state = parsed_data
                                    
                                    # 메시지 ID 로깅
                                    if isinstance(parsed_data, dict) and 'header' in parsed_data:
                                        message_id = parsed_data['header'].get('id')
                                        if message_id == 4001:
                                            print("4001 상태 메시지 수신 (분할)")
                                        elif message_id == 4011:
                                            print(f"플롯 데이터(4011) 메시지 수신 (분할): {json.dumps(parsed_data, indent=2)[:100]}...")
                                        elif message_id == 4012:
                                            print(f"플롯 데이터(4012) 메시지 수신 (분할): {json.dumps(parsed_data, indent=2)[:100]}...")
                                        elif message_id == 4013:
                                            print(f"플롯 데이터(4013) 메시지 수신 (분할): {json.dumps(parsed_data, indent=2)[:100]}...")
                                        elif message_id == 4016:
                                            print(f"트랙 데이터(4016) 메시지 수신 (분할): ID={parsed_data.get('trackId', 'N/A')}")
                                        else:
                                            print(f"기타 메시지 수신 (분할): ID {message_id}")
                                    
                                    buffer = buffer[next_json_start:]  # 버퍼 업데이트
                                    consecutive_errors = 0  # 오류 카운터 초기화
                                except json.JSONDecodeError:
                                    # 첫 번째 부분도 유효한 JSON이 아니면 버퍼 초기화
                                    print(f"분할 JSON 파싱 실패, 버퍼 초기화")
                                    buffer = buffer[next_json_start:]
                                    consecutive_errors += 1
                            else:
                                # 더 많은 데이터를 기다려야 함
                                break
                    
                    # 버퍼가 너무 크면 일부 버림 (메모리 관리)
                    if len(buffer) > 32768:  # 32KB 이상이면
                        next_start = buffer.find('{"header"', 1)
                        if next_start > 0:
                            buffer = buffer[next_start:]  # 다음 메시지부터 유지
                        else:
                            buffer = buffer[-16384:]  # 마지막 16KB만 유지
                        print(f"버퍼 크기 조정: {len(buffer)} 바이트")
                    
                except socket.timeout:
                    # 타임아웃은 오류가 아님, 계속 진행
                    pass
                except socket.error as e:
                    print(f"데이터 수신 오류: {e}")
                    
                    # 자동 재연결 비활성화: 오류가 발생해도 자동으로 재연결하지 않음
                    if auto_reconnect_enabled:
                        reconnect_attempt += 1
                        consecutive_errors += 1
                        
                        # 재연결 시도 (최대 시도 횟수를 초과해도 계속 시도)
                        if reconnect_attempt <= max_reconnect_attempts:
                            print(f"자동 재연결 시도 {reconnect_attempt}/{max_reconnect_attempts}")
                            success = self.reconnect()
                            if not success:
                                time.sleep(3)  # 재연결 실패 시 대기
                        else:
                            print(f"최대 재연결 시도 횟수 초과 ({max_reconnect_attempts}회), 다시 처음부터 시도합니다.")
                            reconnect_attempt = 0  # 재연결 시도 횟수 초기화
                            time.sleep(5)  # 잠시 대기 후 다시 시도
                    else:
                        print("자동 재연결이 비활성화됨 - 소켓 오류 발생: " + str(e))
                        time.sleep(3)  # 오류 발생 시 잠시 대기
            except Exception as e:
                print(f"데이터 수신 스레드 오류: {e}")
                time.sleep(1)  # 일반적인 오류 발생 시 잠시 대기

    def reconnect(self):
        """서버에 재연결을 시도합니다."""
        print("서버 재연결 시도 중...")
        
        # 기존 연결 종료 (안전하게 처리)
        try:
            if self.socket:
                self.socket.close()
        except Exception as e:
            print(f"소켓 종료 오류 (무시됨): {e}")
        
        self.socket = None
        self.running = False
        
        # 잠시 대기 (연결 리소스 해제를 위해)
        time.sleep(1)
        
        # 새 연결 시도
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(5)  # 5초 타임아웃 설정
            
            print(f"재연결 시도: {self.ip}:{self.port}")
            self.socket.connect((self.ip, self.port))
            print(f"서버에 재연결 성공: {self.ip}:{self.port}")
            self.running = True
            
            # 데이터 수신 스레드 재시작
            self.start_receiving()
            return True
        except socket.timeout:
            print(f"재연결 타임아웃: {self.ip}:{self.port}")
            self.socket = None
            return False
        except ConnectionRefusedError:
            print(f"재연결 거부: {self.ip}:{self.port}")
            self.socket = None
            return False
        except socket.error as e:
            print(f"재연결 실패: {e}")
            self.socket = None
            return False
        except Exception as e:
            print(f"예상치 못한 재연결 오류: {e}")
            self.socket = None
            return False

    def get_state(self):
        """현재 저장된 상태 데이터를 반환합니다."""
        with self.lock:
            return self.state

    def _emulator_data_generator(self):
        """에뮬레이터 모드에서 데이터 생성"""
        # 이 메서드는 삭제됩니다
        pass
        
    def _create_radar_state(self):
        """에뮬레이터 모드에서 레이더 상태 메시지 생성"""
        # 이 메서드는 삭제됩니다
        pass
        
    def _create_command_response(self, command_id):
        """에뮬레이터 모드에서 명령에 대한 응답 생성"""
        # 이 메서드는 삭제됩니다
        pass

# 레이더 시스템 래퍼 클래스 (기존 유지)
class RFCoreWrapper:
    _instance = None
    _lock = threading.Lock()
    
    @classmethod
    def get_instance(cls):
        """싱글톤 패턴으로 인스턴스를 반환합니다."""
        if cls._instance is None:
            with cls._lock:
                if cls._instance is None:
                    cls._instance = cls()
        return cls._instance
    
    def __init__(self):
        self.connector = None
        self.protocol = RadarProtocol()
        self.connected = False
        self.last_state = None
        self.last_state_time = 0
        self.last_4001_state = None  # 4001 메시지 저장
        self.last_other_state = None  # 기타 메시지 저장
        self.last_plot_states = []    # 플롯 메시지 저장 (4011, 4012, 4013)
        self.last_track_states = []   # 트랙 메시지 저장 (4016)
        self.last_commands = {}  # 마지막으로 전송한 명령 저장
        
        # 상태 업데이트 스레드 시작
        self.status_thread = threading.Thread(target=self._status_update_loop, daemon=True)
        self.status_thread.start()
    
    def connect(self, ip, port=4102):
        """레이더 시스템에 연결합니다."""
        try:
            # 기존 연결 해제
            if self.connector:
                self.disconnect()
                
            # 새 연결 생성
            self.connector = RFCoreConnector(ip, port)
            success, message = self.connector.connect()
            
            if success:
                self.connected = True
                print(f"레이더 시스템 연결 성공: {ip}:{port}")
                # 연결 시 상태 초기화
                self.last_state = None
                self.last_state_time = 0
                self.last_4001_state = None
                self.last_other_state = None
                return True, message
            else:
                self.connected = False
                return False, message
        except Exception as e:
            self.connected = False
            print(f"레이더 시스템 연결 오류: {str(e)}")
            return False, f"연결 실패: {str(e)}"
    
    def disconnect(self):
        """레이더 시스템 연결을 종료합니다."""
        if self.connector:
            success, message = self.connector.disconnect()
            self.connector = None
            self.connected = False
            self.last_state = None
            self.last_4001_state = None
            self.last_other_state = None
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
                
            if not self.connected:
                return False, "레이더 시스템에 연결되어 있지 않습니다."
                
            # 파라미터 전달
            success, message = self.connector.send_command(command_id, params)
            
            if not success:
                # 전송 실패 - 연결 상태 확인 및 업데이트
                self.connected = False
                return False, f"명령 {command_id} 전송 실패: {message}"
            
            # 성공한 명령 저장
            if success and params:
                self.last_commands[command_id] = params
            
            return True, f"명령 {command_id} 전송 완료"
        except Exception as e:
            self.connected = False  # 오류 발생 시 연결 상태 초기화
            print(f"명령 전송 중 예외 발생: {str(e)}")
            return False, f"명령 전송 중 오류 발생: {str(e)}"
    
    def get_state(self):
        """현재 저장된 상태 데이터를 반환합니다."""
        if not self.connector or not self.connected:
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
            "connected": self.connected,
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
                    if self.connector and self.connected:
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
                    if not is_connected and self.connected:
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
                            self.connected = False
                    elif is_connected and not self.connected:
                        # 실제로는 연결되어 있지만 상태가 잘못된 경우
                        print("연결 상태 불일치: 실제로는 연결되어 있지만 상태는 끊어짐으로 표시되어 있음")
                        self.connected = True
                    elif is_connected and self.connected:
                        # 정상 연결 상태
                        reconnect_attempt = 0  # 재연결 시도 카운터 초기화
                    
                    # 연결 상태에 따라 연결 정보 업데이트
                    if self.connected and self.connector:
                        last_ip = self.connector.ip
                        last_port = self.connector.port
                
                # 데이터가 도착했는지 확인
                if self.connector and self.connected:
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
        if not self.connected:
            return {
                "running": False,
                "message": "에뮬레이터에 연결되어 있지 않습니다."
            }
        
        # 현재 구현에서는 간단하게 연결 상태만 확인
        # 실제 구현에서는 에뮬레이터의 시뮬레이션 상태를 확인하는 로직 필요
        return {
            "running": self.connected,
            "message": "에뮬레이터에 연결되어 있습니다."
        }
    
    def send_simulation_scenario(self, scenario_data):
        """시뮬레이션 시나리오 데이터를 에뮬레이터로 전송합니다.
        에뮬레이터는 T_Scenario.json 파일을 사용하여 시나리오를 로드합니다."""
        try:
            if not self.connector or not self.connected:
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
            if not self.connector or not self.connected:
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
        return self.connected and self.connector is not None

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