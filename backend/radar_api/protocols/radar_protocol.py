import json

class RadarProtocol:
    def __init__(self):
        self.commands = {}
        self._initialize_commands()

    def _initialize_commands(self):
        # 시간동기명령 (ID = 3001)
        # 통합체계와 레이더의 시간정보를 일치시키기 위해 사용하는 메시지
        self.commands[3001] = {
            "header": {
                "id": 3001,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초) - 1970년1월1일0시 이후 흐른 시간
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            }
        }
        
        # 상태정보 (ID = 4001)
        # 레이더 상태정보는 1초간격으로 레이더에서 접속된 장치로 전송
        self.commands[4001] = {
            "header": {
                "id": 4001,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "Rdp": {
                "State": 0,                 # 운용통제기 운용 상태, 1: Standby, 2: Operation
                "SerialNo": 0,              # 운용통제기 시리얼 번호
                "OpTime": 0,                # 출고 후 사용한 운용통제기 사용시간 (초)
                "RdpFault": 0,              # 운용통제기 고장 여부, 1:정상, 2:점검요망, 3:고장
                "RadarNum": 0,              # 레이더 번호
                "Scan": 0,                  # 스캔 번호
                "TrackNum": 0,              # 레이더가 추적하고 있는 트랙개수
                "HostTrackNum": 0,          # 통합체계로 전송하고 있는 트랙개수
                "PlotNum": 0,               # 레이더가 생성하고 있는 플롯의 개수
                "NtpServerConnect": 0       # NTP 서버 접속여부, 0:Not Connect, 1:Connect
            },
            "RadarPanelNum": 1,             # 동시에 운용중인 레이더 패널의 개수, 1~5
            "Rsp": [
                {
                    "Mode": 0,              # 레이더패널 모델
                    "SerialNo": 0,          # 레이더패널 시리얼 번호
                    "State": 0,             # 레이더 패널 상태, 0:Booting, 1:Standby, 13:Operation TxOn, 14:Operation TxOff
                    "OpTime": 0,            # 출고 후 사용한 레이더패널 사용시간
                    "Dwell": 0,             # Dwell 번호
                    "TxPanelFault": 0,      # 송신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "TxPanelTemp": 0,       # 송신패널 온도 정보 (도)
                    "RxPanelFault": 0,      # 수신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "RxPanelTemp": 0,       # 수신패널 온도 정보 (도)
                    "ProcessorFault": 0,    # 신호처리장치 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "ProcessorTemp": 0,     # 신호처리장치 온도 정보 (도)
                    "FanFault": 0           # 냉각팬 고장여부, 1:정상, 2:점검요망, 3:고장
                },
                {
                    "Mode": 0,              # 레이더패널 모델
                    "SerialNo": 0,          # 레이더패널 시리얼 번호
                    "State": 0,             # 레이더 패널 상태, 0:Booting, 1:Standby, 13:Operation TxOn, 14:Operation TxOff
                    "OpTime": 0,            # 출고 후 사용한 레이더패널 사용시간
                    "Dwell": 0,             # Dwell 번호
                    "TxPanelFault": 0,      # 송신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "TxPanelTemp": 0,       # 송신패널 온도 정보 (도)
                    "RxPanelFault": 0,      # 수신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "RxPanelTemp": 0,       # 수신패널 온도 정보 (도)
                    "ProcessorFault": 0,    # 신호처리장치 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "ProcessorTemp": 0,     # 신호처리장치 온도 정보 (도)
                    "FanFault": 0           # 냉각팬 고장여부, 1:정상, 2:점검요망, 3:고장
                },
                {
                    "Mode": 0,              # 레이더패널 모델
                    "SerialNo": 0,          # 레이더패널 시리얼 번호
                    "State": 0,             # 레이더 패널 상태, 0:Booting, 1:Standby, 13:Operation TxOn, 14:Operation TxOff
                    "OpTime": 0,            # 출고 후 사용한 레이더패널 사용시간
                    "Dwell": 0,             # Dwell 번호
                    "TxPanelFault": 0,      # 송신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "TxPanelTemp": 0,       # 송신패널 온도 정보 (도)
                    "RxPanelFault": 0,      # 수신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "RxPanelTemp": 0,       # 수신패널 온도 정보 (도)
                    "ProcessorFault": 0,    # 신호처리장치 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "ProcessorTemp": 0,     # 신호처리장치 온도 정보 (도)
                    "FanFault": 0           # 냉각팬 고장여부, 1:정상, 2:점검요망, 3:고장
                },
                {
                    "Mode": 0,              # 레이더패널 모델
                    "SerialNo": 0,          # 레이더패널 시리얼 번호
                    "State": 0,             # 레이더 패널 상태, 0:Booting, 1:Standby, 13:Operation TxOn, 14:Operation TxOff
                    "OpTime": 0,            # 출고 후 사용한 레이더패널 사용시간
                    "Dwell": 0,             # Dwell 번호
                    "TxPanelFault": 0,      # 송신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "TxPanelTemp": 0,       # 송신패널 온도 정보 (도)
                    "RxPanelFault": 0,      # 수신패널 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "RxPanelTemp": 0,       # 수신패널 온도 정보 (도)
                    "ProcessorFault": 0,    # 신호처리장치 고장 여부, 1:정상, 2:점검요망, 3:고장
                    "ProcessorTemp": 0,     # 신호처리장치 온도 정보 (도)
                    "FanFault": 0           # 냉각팬 고장여부, 1:정상, 2:점검요망, 3:고장
                }
            ]
        }
        
        # 운용제어 (ID = 3002)
        # 레이더 운용 시작(Operation) 및 중단(Standby) 제어
        # 사용자가 원하는 모드로 설정 필요 (1: Standby, 2: Operation)
        self.commands[3002] = {
            "header": {
                "id": 3002,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "OperationMode": 2              # 사용자 입력값으로 대체됨 - 1: Standby(대기), 2: Operation(운용)
        }
        
        # 운용 파라미터 요청 (ID = 3013)
        # 설정된 운용파라미터 정보를 요청하는 메시지
        self.commands[3013] = {
            "header": {
                "id": 3013,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            }
        }
        
        # 운용 파라미터 설정 요청 (ID = 3014)
        # 레이더 설치 위치, 레이더 헤딩방향, 안테나 Tilt, 플롯, 트랙의 좌표값 설정
        # 값들은 프론트엔드에서 사용자가 입력한 값이 적용됩니다
        self.commands[3014] = {
            "header": {
                "id": 3014,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "Rdp": {
                "Longitude": 0.0,           # 경도 (도) - 사용자 입력값으로 대체됨
                "Latitude": 0.0,            # 위도 (도) - 사용자 입력값으로 대체됨
                "Altitude": 0.0,            # 고도 (미터) - 해수면 기준으로 레이더가 설치된 고도, 탐지된 표적 데이터는 해수면 기준 고도로 제공됨
                "AzimuthOffset": 0.0,       # 방위각 오프셋 (도) - 레이더와 다른 센서 간 오차 보정용, 양의 각도는 시계방향, 음의 각도는 시계반대방향으로 보정
                "NtpServer": [0, 0, 0, 0]   # NTP 서버 IP 주소 - 값이 있으면 주기적으로 NTP 서버와 시간 동기화, 0으로 설정 시 NTP 서버와 시각동기화 중단
            },
            "RadarPanelNum": 1,             # 동시에 운용중인 레이더 패널의 개수(1~5), 시스템 설정과 불일치 시 설정 변경이 적용되지 않음
            "Rsp": [
                {
                    "Longitude": 0.0,       # 경도 (도) - 사용자 입력값으로 대체됨
                    "Latitude": 0.0,        # 위도 (도) - 사용자 입력값으로 대체됨
                    "Altitude": 0.0,        # 고도 (미터) - 해수면 기준 레이더 패널 설치 고도
                    "Tilt": 0.0,            # 레이더 패널의 설치 기울기 (Back +, Front -) (도) - 사용자 입력값으로 대체됨
                    "Slant": 0.0,           # 레이더 패널의 좌우 기울기 (Left -, Right +) (도) - 사용자 입력값으로 대체됨
                    "Heading": 0.0          # 안테나 방향, 진북을 기준으로 시계방향이 + (도) - 사용자 입력값으로 대체됨
                }
            ]
        }
        
        # 운용 파라미터 정보 (ID = 4003)
        # 설정된 레이더 운용 파라미터 설정을 전송
        self.commands[4003] = {
            "header": {
                "id": 4003,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 레이더에서 수신 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 레이더에서 수신 시 현재시간으로 업데이트됨
            },
            "Rdp": {
                "Longitude": 0.0,           # 경도 (도) - 실제 설정값으로 대체됨
                "Latitude": 0.0,            # 위도 (도) - 실제 설정값으로 대체됨
                "Altitude": 0.0,            # 고도 (미터) - 해수면 기준으로 레이더가 설치된 고도, 탐지된 표적 데이터는 해수면 기준 고도로 제공됨
                "AzimuthOffset": 0.0,       # 방위각 오프셋 (도) - 레이더와 다른 센서 간 오차 보정용, 양의 각도는 시계방향, 음의 각도는 시계반대방향으로 보정
                "NtpServer": [0, 0, 0, 0]   # NTP 서버 IP 주소 - 값이 있으면 주기적으로 NTP 서버와 시간 동기화, 0으로 설정 시 NTP 서버와 시각동기화 중단
            },
            "RadarPanelNum": 1,             # 동시에 운용중인 레이더 패널의 개수(1~5), 시스템 설정과 불일치 시 설정 변경이 적용되지 않음
            "Rsp": [
                {
                    "Longitude": 0.0,       # 경도 (도) - 실제 설정값으로 대체됨
                    "Latitude": 0.0,        # 위도 (도) - 실제 설정값으로 대체됨
                    "Altitude": 0.0,        # 고도 (미터) - 해수면 기준 레이더 패널 설치 고도
                    "Tilt": 0.0,            # 레이더 패널의 설치 기울기 (Back +, Front -) (도) - 실제 설정값으로 대체됨
                    "Slant": 0.0,           # 레이더 패널의 좌우 기울기 (Left -, Right +) (도) - 실제 설정값으로 대체됨
                    "Heading": 0.0          # 안테나 방향, 진북을 기준으로 시계방향이 + (도) - 실제 설정값으로 대체됨
                }
            ]
        }
        
        # 성능 파라미터 요청 (ID = 3011)
        # 설정된 성능파라미터 정보를 요청하는 메시지
        self.commands[3011] = {
            "header": {
                "id": 3011,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            }
        }
        
        # 성능 파라미터 설정 요청 (ID = 3012)
        # 탐지확률을 높이거나 클러터가 많은 지역에서 허위 표적을 억제하기 위한 파라미터 설정
        # 값들은 프론트엔드에서 사용자가 입력한 값이 적용됩니다
        # 레이더 파라미터는 표적탐지에 적용되는 것(Rsp내)과 표적 추적에 적용되는 파라미터(Rdp내)로 구분됩니다
        self.commands[3012] = {
            "header": {
                "id": 3012,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "Rdp": {
                "LowAltitude": 0,           # 지상에 붙어있는 표적 제어용(m) - 이 높이보다 낮은 트랙은 제거됨, 작게 설정할수록 더 많은 표적 추적 가능
                "MaxVelocity": 0            # 추적할 수 있는 최대 속도(m/sec) - 이보다 빠른 표적은 추적하지 않음, 크게 설정할수록 허위 추적 가능성 증가
            },
            "RadarPanelNum": 1,             # 동시에 운용중인 레이더 패널의 개수(1~5) - Rsp 구조체는 이 수만큼 반복됨, 최대 5개까지 가능
            "Rsp": [
                {
                    "CFAR_Threshold": 0,    # 오경보(False Alarm) 임계값(dB) - 기본값 -60dB(Pfa 10e-6), 낮추면 오경보 증가하나 표적 탐지 향상, -60~-50 권장
                    "MinRange": 0.0,        # 최소 탐지 거리(m) - 설정값보다 가까운 표적 제거, 근거리에 클러터가 많은 경우 사용 권장
                    "MaxRange": 0.0,        # 최대 탐지 거리(m) - 설정값보다 먼 표적 제거
                    "MinVelocity": 0.0,     # 최소 방사 속도(m/sec) - 낮은 도플러를 가진 표적 제거, 해안가 설치 시 해수면 클러터 제거에 유용
                    "TxAtten": 0,           # 송신 신호 감쇠(dB) - 송신신호 크기 감쇠, 일반적으로 0 사용
                    "RxAtten": 0            # 수신 신호 감쇠(dB) - 수신신호 크기 감쇠, 일반적으로 0 사용, RF신호 포화 방지용
                }
            ]
        }
        
        # 성능 파라미터 설정 정보 (ID = 4005)
        # 설정된 레이더 성능 파라미터 정보 전송
        # 레이더 파라미터는 표적탐지에 적용되는 것(Rsp내)과 표적 추적에 적용되는 파라미터(Rdp내)로 구분됩니다
        self.commands[4005] = {
            "header": {
                "id": 4005,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 레이더에서 수신 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 레이더에서 수신 시 현재시간으로 업데이트됨
            },
            "Rdp": {
                "LowAltitude": 0,           # 지상에 붙어있는 표적 제어용(m) - 이 높이보다 낮은 트랙은 제거됨, 작게 설정할수록 더 많은 표적 추적 가능
                "MaxVelocity": 0            # 추적할 수 있는 최대 속도(m/sec) - 이보다 빠른 표적은 추적하지 않음, 크게 설정할수록 허위 추적 가능성 증가
            },
            "RadarPanelNum": 1,             # 동시에 운용중인 레이더 패널의 개수(1~5) - Rsp 구조체는 이 수만큼 반복됨, 최대 5개까지 가능
            "Rsp": [
                {
                    "CFAR_Threshold": 0,    # 오경보(False Alarm) 임계값(dB) - 기본값 -60dB(Pfa 10e-6), 낮추면 오경보 증가하나 표적 탐지 향상, -60~-50 권장
                    "MinRange": 0.0,        # 최소 탐지 거리(m) - 설정값보다 가까운 표적 제거, 근거리에 클러터가 많은 경우 사용 권장
                    "MaxRange": 0.0,        # 최대 탐지 거리(m) - 설정값보다 먼 표적 제거
                    "MinVelocity": 0.0,     # 최소 방사 속도(m/sec) - 낮은 도플러를 가진 표적 제거, 해안가 설치 시 해수면 클러터 제거에 유용
                    "TxAtten": 0,           # 송신 신호 감쇠(dB) - 송신신호 크기 감쇠, 일반적으로 0 사용
                    "RxAtten": 0            # 수신 신호 감쇠(dB) - 수신신호 크기 감쇠, 일반적으로 0 사용, RF신호 포화 방지용
                }
            ]
        }
        
        # 빔스케줄정보 요청 (ID = 3031)
        # 설정된 빔 스케줄 정보를 요청하기 위해 통합체계에서 레이더로 전송하는 메시지
        # 레이더는 이 요청에 대해 빔스케줄 정보(ID=4031)를 회신
        self.commands[3031] = {
            "header": {
                "id": 3031,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "RadarPanelId": 1               # 정보를 요청하고자 하는 레이더 패널 번호(1~5)
        }
        
        # 빔스케줄정보 변경요청 (ID = 3032)
        # 스케줄 모드 제어를 사용하여 빔의 스케줄 범위를 변경
        # 빔 스케줄은 Default, Add, Remove 영역으로 나누어짐
        # 방위각 0도는 레이더 패널의 Boresight를 의미
        # 레이더는 이 요청을 처리한 후 빔스케줄 정보(ID=4031)를 회신
        self.commands[3032] = {
            "header": {
                "id": 3032,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "RadarPanelId": 1,              # 제어하고자 하는 레이더 패널 번호(1~5)
            "Default": {
                "AzStart": 0,               # 방위각 시작 지점 - 레이더 빔 중심점 기준
                "AzStop": 35,               # 방위각 종료 지점 - 최대 조향 각도 범위
                "AzStep": 0,                # 방위각 조향 간격 - 방위각 방향으로 빔을 조향하는 간격
                "AzDisplace": 35,           # 방위각 이동량 - Scan에 따라 빔 조향각을 변경하는 기능
                "ElStart": 0,               # 고각 시작 지점 - 레이더 빔 중심점 기준
                "ElStop": 35,               # 고각 종료 지점 - 최대 조향 각도 범위
                "ElStep": 0,                # 고각 조향 간격 - 고각 방향으로 빔을 조향하는 간격
                "ElWide": 35,               # 고각 빔폭 확장 - 고각 빔폭을 넓히는 기능(0:Narrow, 1:Wide)
                "ElDisplace": 35            # 고각 이동량 - Scan에 따라 빔 조향각을 변경하는 기능
            },
            "nAdd": 3,                      # 추가되는 빔 스케줄링 영역의 수 (문서상 고정값 3 권장)
            "Add": [
                {
                    "Valid": 1,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 35,           # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 35,           # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 35            # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                },
                {
                    "Valid": 1,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0,            # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 0             # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                },
                {
                    "Valid": 1,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0,            # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 0             # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                }
            ],
            "nRemove": 3,                   # 삭제되는 빔 스케줄링 영역의 수 (문서상 고정값 3 권장)
            "Remove": [
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 35,           # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 35            # 고각 종료 지점
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0             # 고각 종료 지점
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0             # 고각 종료 지점
                }
            ]
        }
        
        # 빔스케줄정보 (ID = 4031)
        # 레이더에 설정된 스케줄 정보를 통합체계로 전송
        # 빔 조향 순서는 좌측하단에서 출발하여 먼저 Elevation을 탐색한 후 우측으로 조향각을 옮겨가며 빔 스캐닝 진행
        # 메시지 사이즈는 추가/삭제되는 스케줄링 영역의 개수에 따라 달라짐
        self.commands[4031] = {
            "header": {
                "id": 4031,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 레이더에서 수신 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 레이더에서 수신 시 현재시간으로 업데이트됨
            },
            "RadarPanelId": 1,              # 정보를 제공하는 레이더 패널 번호(1~5)
            "Default": {
                "AzStart": 0,               # 방위각 시작 지점 - 레이더 빔 중심점 기준
                "AzStop": 35,               # 방위각 종료 지점 - 최대 조향 각도 범위
                "AzStep": 0,                # 방위각 조향 간격 - 방위각 방향으로 빔을 조향하는 간격
                "AzDisplace": 35,           # 방위각 이동량 - Scan에 따라 빔 조향각을 변경하는 기능
                "ElStart": 0,               # 고각 시작 지점 - 레이더 빔 중심점 기준
                "ElStop": 35,               # 고각 종료 지점 - 최대 조향 각도 범위
                "ElStep": 0,                # 고각 조향 간격 - 고각 방향으로 빔을 조향하는 간격
                "ElWide": 35,               # 고각 빔폭 확장 - 고각 빔폭을 넓히는 기능(0:Narrow, 1:Wide)
                "ElDisplace": 35            # 고각 이동량 - Scan에 따라 빔 조향각을 변경하는 기능
            },
            "nAdd": 3,                      # 추가되는 빔 스케줄링 영역의 수, 고정값 3 권장
            "Add": [
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 35,           # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 35,           # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 35            # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0,            # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 0             # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "AzStep": 0,            # 방위각 조향 간격
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0,            # 고각 종료 지점
                    "ElStep": 0,            # 고각 조향 간격
                    "ElWide": 0             # 고각 빔폭 확장 (0:Narrow, 1:Wide)
                }
            ],
            "nRemove": 3,                   # 삭제되는 빔 스케줄링 영역의 수, 고정값 3 권장
            "Remove": [
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 35,           # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 35            # 고각 종료 지점
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0             # 고각 종료 지점
                },
                {
                    "Valid": 0,             # 1:Valid, 0:Invalid - 해당 항목의 유효성
                    "AzStart": 0,           # 방위각 시작 지점
                    "AzStop": 0,            # 방위각 종료 지점
                    "ElStart": 0,           # 고각 시작 지점
                    "ElStop": 0             # 고각 종료 지점
                }
            ]
        }
        
        # 빔그리드정보요청 (ID = 3033)
        self.commands[3033] = {
            "header": {
                "id": 3033,
                "tv_sec": 1457804,
                "tv_usec": 0
            },
            "RadarPanelId": 1
        }
        
        # 빔그리드정보 (ID = 4033)
        self.commands[4033] = {
            "header": {
                "id": 4033,
                "tv_sec": 1457804,
                "tv_usec": 0
            },
            "RadarPanelId": 1,
            "nGrid": 1,
            "Grid": [
                {
                    "Az": -40,
                    "El": 5,
                    "AzBw": 5,
                    "ElBw": 10
                },
                {
                    "Az": -40,
                    "El": 14,
                    "AzBw": 5,
                    "ElBw": 10
                },
                {
                    "Az": -35,
                    "El": 5,
                    "AzBw": 5,
                    "ElBw": 10
                },
                {
                    "Az": -40,
                    "El": 14,
                    "AzBw": 5,
                    "ElBw": 10
                }
            ]
        }
        
        # 빔 방사 금지영역 설정 (ID = 3019)
        # 빔 방사 금지영역을 방위각 및 고각 섹터로 설정
        # 이 메시지 적용 시 기존 빔 스케줄링의 Remove 정보는 삭제되고 이 설정에 맞게 재설정됨
        # 빔 방사 금지영역이 북쪽 방향을 포함하는 경우 북쪽 방향 기준으로 2개 영역으로 나누어 설정해야 함
        # 레이더는 이 메시지를 받으면 빔 방사금지영역 설정회신(ID=4019)을 통합시스템으로 전송
        self.commands[3019] = {
            "header": {
                "id": 3019,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            },
            "BeamProhibitAreaNum": 0,       # 빔 방사 금지영역 개수 (0~3), 0이면 금지영역 없음
            "Area": [
                {
                    "StartAzAngle": 0.0,    # 시작 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
                    "EndAzAngle": 0.0,      # 종료 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
                    "StartElAngle": 0.0,    # 시작 고각 (도) - 수평이 0, 위쪽 방향으로 0~90
                    "EndElAngle": 0.0       # 종료 고각 (도) - 수평이 0, 위쪽 방향으로 0~90
                }
            ]
        }
        
        # 빔 방사 금지영역 설정정보 요청 (ID = 3022)
        # 현재 설정된 빔 방사 금지영역 정보를 요청
        self.commands[3022] = {
            "header": {
                "id": 3022,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 전송 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 전송 시 현재시간으로 업데이트됨
            }
        }
        
        # 빔 방사 금지영역 설정 회신 (ID = 4019)
        # 빔 방사 금지영역 설정 요청(ID=3019)에 대한 응답 또는 설정정보 요청(ID=3022)에 대한 응답
        self.commands[4019] = {
            "header": {
                "id": 4019,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초) - 레이더에서 수신 시 현재시간으로 업데이트됨
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초) - 레이더에서 수신 시 현재시간으로 업데이트됨
            },
            "BeamProhibitAreaNum": 0,       # 빔 방사 금지영역 개수 (0~3), 0이면 금지영역 없음
            "Area": [
                {
                    "StartAzAngle": 0.0,    # 시작 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
                    "EndAzAngle": 0.0,      # 종료 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
                    "StartElAngle": 0.0,    # 시작 고각 (도) - 수평이 0, 위쪽 방향으로 0~90
                    "EndElAngle": 0.0       # 종료 고각 (도) - 수평이 0, 위쪽 방향으로 0~90
                }
            ]
        }
        
        # 플롯정보 (Polar) (ID = 4013)
        # 탐지된 레이더 플롯 정보를 극좌표계(Polar)로 전송. 레이더 빔 스캐닝이 끝나면 플롯 데이터를 전송 (최대 256개)
        self.commands[4013] = {
            "header": {
                "id": 4013,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "nPlot": 0,                     # 플롯 데이터 개수 (최대 256개)
            "PlotType": 3,                  # 플롯 데이터 형태 (3: Polar 좌표계)
            "Plot": [
                {
                    "plotId": 0,            # 플롯 ID (고유 식별자)
                    "tv_sec": 0,            # 플롯 탐지 시간 (Unix Time 초)
                    "tv_usec": 0,           # 플롯 탐지 시간 (Unix Time 마이크로초)
                    "Range": 0.0,           # 거리 (미터) - 레이더로부터 표적까지의 직선거리
                    "Azimuth": 0.0,         # 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
                    "Elevation": 0.0,       # 고도각 (도) - 수평이 0, 위쪽 방향으로 0~90
                    "RCS": 0.0              # 레이더 단면적 (dBsm) - 표적의 크기를 나타내는 값
                }
            ]
        }
        
        # 트랙정보 (Polar) (ID = 4016)
        # 탐지된 레이더 트랙 데이터를 극좌표계로 전송. 한 메시지에 1개의 트랙정보가 포함되며, 여러 트랙이 있는 경우 메시지가 반복 전송됨
        self.commands[4016] = {
            "header": {
                "id": 4016,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "trackId": 0,                   # 트랙 ID (고유 식별자)
            "type": 0,                      # 트랙 타입 (1: 항공기, 2: 무인기/드론, 3: 조류, 4: 기타)
            "tv_sec": 0,                    # 트랙 생성/갱신 시간 (Unix Time 초)
            "tv_usec": 0,                   # 트랙 생성/갱신 시간 (Unix Time 마이크로초)
            "Class": 0,                     # 트랙 클래스 분류 결과 (0: 미분류, 1-N: 표적 클래스)
            "Reliability": 0,               # 트랙 신뢰도 (0-100%), 값이 높을수록 신뢰도 높음
            "Range": 0.0,                   # 거리 (미터) - 레이더로부터 표적까지의 직선거리
            "Azimuth": 0.0,                 # 방위각 (도) - 북쪽이 0, 시계방향으로 0~360
            "Elevation": 0.0,               # 고도각 (도) - 수평이 0, 위쪽 방향으로 0~90
            "RCS": 0.0,                     # 레이더 단면적 (dBsm) - 표적의 크기를 나타내는 값
            "vEast": 0.0,                   # 동쪽 방향 속도 (m/s)
            "vNorth": 0.0,                  # 북쪽 방향 속도 (m/s)
            "vUp": 0.0,                     # 상향 방향 속도 (m/s)
            "nTrace": 0,                    # 트레이스(과거 위치) 개수 (최대 10개)
            "Trace": [                      # 트랙의 과거 위치 정보 배열
                {
                    "Range": 0.0,           # 과거 위치 거리 (미터)
                    "Azimuth": 0.0,         # 과거 위치 방위각 (도)
                    "Elevation": 0.0        # 과거 위치 고도각 (도)
                }
            ]
        }
        
        # 트랙종료 정보 (ID = 4017)
        # 더 이상 탐지되지 않아 추적을 종료한 트랙 정보를 제공
        # 이 메시지 이후 해당 trackId를 가진 트랙 정보는 더 이상 제공되지 않음
        self.commands[4017] = {
            "header": {
                "id": 4017,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "nTrack": 0,                    # 종료된 트랙 개수
            "track": [                      # 종료된 트랙 ID 배열
                {
                    "trackId": 0            # 종료된 트랙 ID
                }
            ]
        }
        
        # 트랙종료 요청 (ID = 3044)
        # 특정 트랙을 더 이상 추적하지 않도록 요청
        self.commands[3044] = {
            "header": {
                "id": 3044,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "trackId": 0,                   # 종료할 트랙 ID
            "Class": 0                      # 트랙 클래스 (0으로 설정)
        }
        
        # 수동트랙식별적용요청 (ID = 3041)
        # 통합체계에서 트랙을 식별하여 레이더에 알려주는 메시지
        # 이 메시지 이후 해당 트랙의 식별값은 사용자가 지정한 값으로 고정됨
        self.commands[3041] = {
            "header": {
                "id": 3041,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "trackId": 0,                   # 식별할 트랙 ID
            "Class": 0                      # 설정할 트랙 클래스 값
        }
        
        # 고속갱신설정요청 (ID = 3042)
        # 특정 트랙의 갱신 주기를 높이기 위한 요청
        self.commands[3042] = {
            "header": {
                "id": 3042,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "trackId": 0                    # 고속갱신할 트랙 ID
        }
        
        # 고속갱신해제요청 (ID = 3043)
        # 고속갱신 설정된 트랙을 해제하기 위한 요청
        self.commands[3043] = {
            "header": {
                "id": 3043,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "trackId": 0                    # 고속갱신 해제할 트랙 ID
        }
        
        # 실시간 빔조향 정보 요청 (ID = 3045)
        # 레이더가 실시간으로 조향하는 빔 정보를 요청
        self.commands[3045] = {
            "header": {
                "id": 3045,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "SendFlag": 0                   # 빔조향 정보 전송 여부 (0: 전송 안함, 1: 전송)
        }
        
        # 실시간 빔조향 정보 (ID = 4045)
        # 레이더가 실시간으로 조향하는 빔 정보를 제공
        self.commands[4045] = {
            "header": {
                "id": 4045,                 # 메시지 ID
                "tv_sec": 0,                # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "RadarPanelId": 0,              # 레이더 패널 ID
            "Heading": 0.0,                 # 레이더 헤딩 각도 (도)
            "Az": 0.0,                      # 빔 방위각 (도)
            "El": 0.0,                      # 빔 고도각 (도)
            "AzBw": 0.0,                    # 빔 방위각 빔폭 (도)
            "ElBw": 0.0                     # 빔 고도각 빔폭 (도)
        }
        
        # 플롯트랙설정요청 (ID = 3005)
        self.commands[3005] = {
            "header": {
                "id": 3005,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "PlotSendFlag": 1,              # 플롯 데이터 전송여부, 0: 전송안함, 1: 전송
            "PlotType": 3,                  # 플롯데이터 형태, 1: LLA, 2: ENU, 3: Polar
            "TrackSendFlag": 1,             # 트랙 데이터 전송여부, 0: 전송안함, 1: 전송
            "TrackType": 3                  # 트랙데이터 형태, 1: LLA, 2: ENU, 3: Polar
        }
        
        # 설정 초기화 요청 (ID = 3999)
        # 사용자가 설정가능한 모든 설정들을 초기화
        self.commands[3999] = {
            "header": {
                "id": 3999,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            }
        }
        
        # 영역설정요청 (ID = 3035)
        # 설정된 영역 정보를 변경하는 요청
        # 드론탐지레이더는 영역설정변경요청을 처리한 후, 영역설정정보(ID=4034)를 회신
        self.commands[3035] = {
            "header": {
                "id": 3035,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "nArea": 2,                     # 영역 개수
            "Area": [
                {
                    "Type": 1,              # 영역 타입
                    "Valid": 1,             # 영역 적용 여부 (0: 미적용, 1: 적용)
                    "Point": [
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        }
                    ],
                    "HeightBottom": 0,      # 최소 고도 (m)
                    "HeightTop": 0          # 최대 고도 (m)
                },
                {
                    "Type": 1,              # 영역 타입
                    "Valid": 1,             # 영역 적용 여부 (0: 미적용, 1: 적용)
                    "Point": [
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        }
                    ],
                    "HeightBottom": 0,      # 최소 고도 (m)
                    "HeightTop": 0          # 최대 고도 (m)
                }
            ]
        }
        
        # 영역정보요청 (ID = 3034)
        # 설정되어 있는 레이더 영역 정보를 요청하는 메시지
        # 레이더는 이 요청에 대해 영역설정정보(ID=4034)를 회신
        self.commands[3034] = {
            "header": {
                "id": 3034,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            }
        }
        
        # 영역설정정보 (ID = 4034)
        # 설정된 영역 정보를 전송
        # 영역설정요청(ID=3034) 또는 영역설정변경요청(ID=3035)에 대한 응답
        self.commands[4034] = {
            "header": {
                "id": 4034,                 # 메시지 ID
                "tv_sec": 1457804,          # 메시지가 생성된 Unix Time (초)
                "tv_usec": 0                # 메시지가 생성된 Unix Time (마이크로초)
            },
            "nArea": 2,                     # 영역 개수
            "Area": [
                {
                    "Type": 1,              # 영역 타입
                    "Valid": 1,             # 영역 적용 여부 (0: 미적용, 1: 적용)
                    "Point": [
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        }
                    ],
                    "HeightBottom": 0,      # 최소 고도 (m)
                    "HeightTop": 0          # 최대 고도 (m)
                },
                {
                    "Type": 1,              # 영역 타입
                    "Valid": 1,             # 영역 적용 여부 (0: 미적용, 1: 적용)
                    "Point": [
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        },
                        {
                            "Longitude": 0, # 경도 (도)
                            "Latitude": 0   # 위도 (도)
                        }
                    ],
                    "HeightBottom": 0,      # 최소 고도 (m)
                    "HeightTop": 0          # 최대 고도 (m)
                }
            ]
        }

    def get_command(self, command_id):
        """주어진 명령어 ID에 해당하는 명령어를 반환합니다."""
        if command_id in self.commands:
            return self.commands[command_id]
        return None
        
    def get_command_json(self, command_id):
        """주어진 명령어 ID에 해당하는 명령어를 JSON 문자열로 반환합니다."""
        command = self.get_command(command_id)
        if command:
            return json.dumps(command, indent=2, ensure_ascii=False)
        return None
        
    def list_commands(self):
        """사용 가능한 모든 명령어 ID 목록을 반환합니다."""
        return sorted(list(self.commands.keys()))

# 사용 예시
if __name__ == "__main__":
    radar = RadarProtocol()
    print("사용 가능한 명령어 ID:", radar.list_commands())
    
    # 특정 명령어 조회
    command_id = 3001
    command_json = radar.get_command_json(command_id)
    if command_json:
        print(f"명령어 ID {command_id}의 JSON:")
        print(command_json) 