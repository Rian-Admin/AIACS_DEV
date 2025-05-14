"""
PTZ 카메라 조류 추적 컨트롤러
ONVIF 프로토콜을 사용하여 PTZ 기능이 있는 카메라로 조류를 추적하는 모듈입니다.
"""

import time
import logging
import threading
import os
import json
from typing import Dict, Optional, Any, Tuple, List
from threading import Timer, Lock
from onvif import ONVIFCamera
from collections import deque
import numpy as np

# 로깅 설정
logger = logging.getLogger(__name__)

# 프리셋 저장 경로 설정
PRESET_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'data', 'presets')
os.makedirs(PRESET_DIR, exist_ok=True)

class PTZTrackerController:
    """조류 추적용 PTZ 제어 클래스"""
    
    # 싱글톤 인스턴스
    _instance = None 
    _instance_lock = threading.Lock()
    
    # 추적 관련 상수 정의
    CENTER_ZONE_SIZE = 0.1  # 화면 중앙 영역 크기 (전체 화면의 비율)
    MAX_PAN_VALUE = 0.33   # 최대 팬 각도 (좌우, 정규화된 값, 약 60도에 해당)
    NO_OBJECT_TIMEOUT = 5  # 객체 없을 때 홈 위치로 복귀 시간 (초)
    AUTO_SCAN_TIMEOUT = 15 # 자동 스캔 모드 전환 시간 (초)
    
    @classmethod
    def get_instance(cls):
        """싱글톤 패턴 구현"""
        with cls._instance_lock:
            if cls._instance is None:
                cls._instance = cls()
        return cls._instance
    
    def __init__(self):
        """초기화"""
        if PTZTrackerController._instance is not None:
            raise RuntimeError("싱글톤 객체에 직접 접근하지 마세요. get_instance()를 사용하세요.")
        
        # 카메라별 PTZ 상태 관리
        self.ptz_sessions = {}        # 카메라 ID별 PTZ 세션 저장
        self.move_locks = {}          # 카메라 ID별 잠금 객체
        self.is_moving = {}           # 카메라 ID별 이동 상태
        self.timeout_timers = {}      # 카메라 ID별 타임아웃 타이머
        self.tracking_status = {}     # 카메라 ID별 추적 상태
        self.preset_positions = {}    # 카메라 ID별 프리셋 위치
        self.is_scanning = {}         # 카메라 ID별 스캔 진행 상태
        
        # 추적 알고리즘 관련 데이터 구조
        self.bbox_centers = {}        # 카메라 ID별 바운딩 박스 중앙값 저장 (최근 초당 값)
        self.last_detection_time = {} # 카메라 ID별 마지막 객체 감지 시간
        self.return_home_timers = {}  # 카메라 ID별 홈 위치 복귀 타이머
        self.auto_scan_timers = {}    # 카메라 ID별 자동 스캔 모드 타이머
        self.camera_angles = {}       # 카메라 ID별 현재 각도 저장
        self.scan_config = {}         # 카메라 ID별 스캔 설정
        self.scan_timers = {}         # 카메라 ID별 스캔 관련 타이머 저장
        
        # 저장된 프리셋 불러오기
        self._load_all_presets()
        
        logger.info("PTZ 추적 컨트롤러가 초기화되었습니다.")
    
    def _get_preset_file_path(self, camera_id):
        """카메라 ID별 프리셋 저장 파일 경로 반환"""
        return os.path.join(PRESET_DIR, f'presets_camera_{camera_id}.json')
    
    def _save_presets(self, camera_id):
        """프리셋 설정을 파일로 저장"""
        try:
            camera_id = str(camera_id)
            if camera_id in self.preset_positions:
                file_path = self._get_preset_file_path(camera_id)
                with open(file_path, 'w') as f:
                    json.dump(self.preset_positions[camera_id], f, indent=2)
                logger.info(f"카메라 {camera_id}의 프리셋이 성공적으로 저장되었습니다.")
                return True
            return False
        except Exception as e:
            logger.error(f"카메라 {camera_id} 프리셋 저장 중 오류: {str(e)}")
            return False
    
    def _load_presets(self, camera_id):
        """프리셋 설정을 파일에서 불러오기"""
        try:
            camera_id = str(camera_id)
            file_path = self._get_preset_file_path(camera_id)
            if os.path.exists(file_path):
                with open(file_path, 'r') as f:
                    self.preset_positions[camera_id] = json.load(f)
                logger.info(f"카메라 {camera_id}의 프리셋을 불러왔습니다.")
                return True
            else:
                self.preset_positions[camera_id] = {}
                return False
        except Exception as e:
            logger.error(f"카메라 {camera_id} 프리셋 불러오기 중 오류: {str(e)}")
            self.preset_positions[camera_id] = {}
            return False
    
    def _load_all_presets(self):
        """모든 프리셋 파일 불러오기"""
        try:
            # 프리셋 디렉토리 탐색
            if os.path.exists(PRESET_DIR):
                for filename in os.listdir(PRESET_DIR):
                    if filename.startswith('presets_camera_') and filename.endswith('.json'):
                        # 파일명에서 카메라 ID 추출
                        camera_id = filename.replace('presets_camera_', '').replace('.json', '')
                        self._load_presets(camera_id)
            logger.info("모든 카메라 프리셋을 불러왔습니다.")
        except Exception as e:
            logger.error(f"프리셋 불러오기 중 오류: {str(e)}")
    
    def setup_onvif_ptz(self, camera_id, rtsp_url, username='admin', password='admin123', port=80):
        """특정 카메라의 ONVIF PTZ 설정

        Args:
            camera_id: 카메라 ID
            rtsp_url: 카메라 RTSP URL
            username: ONVIF 인증 사용자명
            password: ONVIF 인증 비밀번호
            port: ONVIF 포트

        Returns:
            bool: 설정 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환 (딕셔너리 키로 사용)
            camera_id = str(camera_id)
            
            # 이미 설정된 경우 재사용
            if camera_id in self.ptz_sessions:
                logger.info(f"카메라 {camera_id}의 PTZ 세션이 이미 설정되어 있습니다.")
                return True
            
            # IP 추출
            ip = None
            try:
                if '@' in rtsp_url:
                    ip = rtsp_url.split('@')[1].split(':')[0]
                elif rtsp_url.startswith('rtsp://'):
                    ip = rtsp_url.split('rtsp://')[1].split(':')[0]
                else:
                    logger.error(f"카메라 {camera_id}의 URL에서 IP를 추출할 수 없습니다: {rtsp_url}")
                    return False
            except Exception as e:
                logger.error(f"카메라 {camera_id}의 URL에서 IP 추출 중 오류: {str(e)}")
                return False
            
            # 포트 80으로 시도
            try:
                logger.info(f"카메라 {camera_id}에 ONVIF 연결 시도 (IP: {ip}, 포트: {port})")
                mycam = ONVIFCamera(ip, port, username, password)
                logger.info(f"카메라 {camera_id}에 포트 {port}로 연결 성공")
            except Exception as e:
                logger.warning(f"카메라 {camera_id} 포트 {port}로 연결 실패, 포트 8000 시도: {str(e)}")
                try:
                    # 포트 8000으로 시도
                    mycam = ONVIFCamera(ip, 8000, username, password)
                    logger.info(f"카메라 {camera_id}에 포트 8000으로 연결 성공")
                except Exception as e:
                    logger.error(f"카메라 {camera_id} ONVIF 연결 실패 (포트 {port}, 8000): {str(e)}")
                    return False
            
            # PTZ 및 미디어 서비스 생성
            ptz = mycam.create_ptz_service()
            media = mycam.create_media_service()
            media_profile = media.GetProfiles()[0]
            
            # PTZ 설정 옵션
            ptz_configuration_options = ptz.GetConfigurationOptions({
                'ConfigurationToken': media_profile.PTZConfiguration.token
            })
            
            # 세션 정보 저장
            self.ptz_sessions[camera_id] = {
                'ptz': ptz,
                'media': media,
                'profile': media_profile,
                'config_options': ptz_configuration_options,
                'camera_ip': ip,
                'username': username,
                'password': password,
                'port': port
            }
            
            # 잠금 객체 및 상태 초기화
            self.move_locks[camera_id] = Lock()
            self.is_moving[camera_id] = False
            self.tracking_status[camera_id] = False
            
            # 추적 관련 상태 초기화
            self.bbox_centers[camera_id] = deque(maxlen=10)  # 최근 10개 중앙값 저장
            self.last_detection_time[camera_id] = time.time()
            self.camera_angles[camera_id] = 0.0  # 현재 팬 각도
            
            # 이 카메라의 프리셋 불러오기
            if camera_id not in self.preset_positions:
                self._load_presets(camera_id)
            
            logger.info(f"카메라 {camera_id}의 ONVIF PTZ 설정 완료")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} ONVIF PTZ 설정 오류: {str(e)}")
            return False
    
    def control_ptz(self, camera_id, direction, is_continuous=True, speed=0.7):
        """카메라 PTZ 제어

        Args:
            camera_id: 카메라 ID
            direction: 이동 방향 (LEFT, RIGHT, UP, DOWN, ZOOM_IN, ZOOM_OUT)
            is_continuous: 연속 이동 여부 (True: 이동 시작, False: 정지)
            speed: 이동 속도 (0.0~1.0)

        Returns:
            bool: 제어 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다. 먼저 setup_onvif_ptz를 호출하세요.")
                return False

            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # PTZ 이동 잠금 획득
            with self.move_locks[camera_id]:
                # 추적 중인 경우 수동 제어는 불가
                if self.tracking_status[camera_id]:
                    logger.warning(f"카메라 {camera_id}가 추적 모드 중입니다. 먼저 추적을 중지하세요.")
                    return False
                
                # 이동 중지 요청인 경우
                if not is_continuous:
                    self.is_moving[camera_id] = False
                    return self._stop_ptz(camera_id)
                
                # 이미 이동 중이면 무시
                if self.is_moving[camera_id]:
                    logger.debug(f"카메라 {camera_id}가 이미 이동 중입니다.")
                    return True
                
                # PTZ 이동 요청 생성
                request = ptz.create_type('ContinuousMove')
                request.ProfileToken = profile.token
                
                # PTZ 속도 요청 생성
                velocity = {
                    'PanTilt': {'x': 0, 'y': 0},
                    'Zoom': {'x': 0}
                }
                
                # 방향에 따른 속도 설정
                if direction == "LEFT":
                    velocity['PanTilt']['x'] = -speed
                elif direction == "RIGHT":
                    velocity['PanTilt']['x'] = speed
                elif direction == "UP":
                    velocity['PanTilt']['y'] = speed
                elif direction == "DOWN":
                    velocity['PanTilt']['y'] = -speed
                elif direction == "ZOOM_IN":
                    velocity['Zoom']['x'] = speed
                elif direction == "ZOOM_OUT":
                    velocity['Zoom']['x'] = -speed
                
                # 속도값 설정
                request.Velocity = velocity
                
                # PTZ 이동 실행
                ptz.ContinuousMove(request)
                logger.info(f"카메라 {camera_id} PTZ {direction} 제어 명령 전송됨")
                self.is_moving[camera_id] = True
                
                # 안전을 위한 타임아웃 설정 (30초 후 자동 정지)
                self._set_safety_timeout(camera_id, 30)
                
                return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} PTZ 제어 오류: {str(e)}")
            return False
    
    def _stop_ptz(self, camera_id):
        """카메라 PTZ 정지

        Args:
            camera_id: 카메라 ID

        Returns:
            bool: 정지 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            logger.info(f"카메라 {camera_id} PTZ 정지 명령 시도")
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 정지 타이머 취소
            self._cancel_safety_timeout(camera_id)
            
            # 정지 요청 생성
            request = ptz.create_type('Stop')
            request.ProfileToken = profile.token
            request.PanTilt = True
            request.Zoom = True
            
            try:
                # 정지 명령 실행
                logger.info(f"카메라 {camera_id} Stop 명령 전송")
                ptz.Stop(request)
                logger.info(f"카메라 {camera_id} Stop 명령 전송 성공")
                
                self.is_moving[camera_id] = False
                return True
            except Exception as inner_e:
                logger.error(f"카메라 {camera_id} Stop 명령 전송 실패: {str(inner_e)}")
                import traceback
                logger.error(f"정지 명령 실패 스택 추적: {traceback.format_exc()}")
                # 에러가 발생해도 이동 상태는 종료로 표시
                self.is_moving[camera_id] = False
                return False
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} PTZ 정지 오류: {str(e)}")
            import traceback
            logger.error(f"PTZ 정지 스택 추적: {traceback.format_exc()}")
            return False
    
    def start_tracking_and_scan(self, camera_id, horizontal_steps=3, vertical_steps=2, detection_data=None):
        """추적 및 스캔 통합 시작
        
        Args:
            camera_id: 카메라 ID
            horizontal_steps: 수평 스캔 단계 수
            vertical_steps: 수직 스캔 단계 수
            detection_data: 초기 감지 데이터 (선택)
            
        Returns:
            bool: 시작 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다. 먼저 setup_onvif_ptz를 호출하세요.")
                return False
            
            # 스캔 설정 저장
            self.set_scan_config(camera_id, horizontal_steps, vertical_steps)
            
            # 추적 상태 업데이트
            self.tracking_status[camera_id] = True
            
            # 추적 관련 데이터 초기화
            self.bbox_centers[camera_id] = deque(maxlen=10)
            self.last_detection_time[camera_id] = time.time()
            
            # 현재 위치 정보 업데이트
            position = self.get_camera_position(camera_id)
            if position and 'pan' in position:
                self.camera_angles[camera_id] = position['pan']
            else:
                self.camera_angles[camera_id] = 0.0
            
            # 감지 데이터가 있으면 즉시 업데이트
            if detection_data:
                self.update_detection_data(camera_id, detection_data)
            
            # 현재 스캔 모드를 즉시 시작 (중요: 이 부분이 이전에 빠져있었음)
            # 객체가 감지되지 않으면 바로 자동 스캔 시작
            logger.info(f"카메라 {camera_id} 자동 스캔 모드 즉시 시작")
            
            # 간단한 스캔 패턴: 중앙 → 왼쪽 → 중앙 → 오른쪽 → 중앙
            scan_positions = [0.0, -0.2, 0.0, 0.2, 0.0]
            wait_times = [5.0, 3.0, 5.0, 3.0, 5.0]
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if current_position:
                logger.info(f"카메라 {camera_id} 현재 위치: {current_position}")
                
                # 처음에 중앙(0)으로 이동
                try:
                    # 위치 이동 (Pan만 변경)
                    logger.info(f"카메라 {camera_id} 초기 위치(0)로 이동")
                    result = self._absolute_move_to_position(
                        camera_id, 
                        0.0,  # Pan = 0
                        current_position.get('tilt', 0.0),  # 현재 Tilt 유지
                        current_position.get('zoom', 0.0)   # 현재 Zoom 유지
                    )
                    
                    if result:
                        logger.info(f"카메라 {camera_id} 초기 위치(0) 이동 성공")
                        
                        # 모든 기존 스캔 타이머 취소
                        self._cancel_all_scan_timers(camera_id)
                        
                        # 스캔 중 플래그 초기화
                        self.is_scanning[camera_id] = False
                        
                        # 10초 후에 스캔 시작
                        timer = Timer(10.0, self._start_scan_sequence, args=[camera_id])
                        timer.daemon = True
                        timer.start()
                        
                        # 타이머 저장
                        if camera_id not in self.scan_timers:
                            self.scan_timers[camera_id] = {}
                        self.scan_timers[camera_id]['_start_scan_sequence'] = timer
                    else:
                        logger.error(f"카메라 {camera_id} 초기 위치(0) 이동 실패")
                except Exception as move_error:
                    logger.error(f"카메라 {camera_id} 초기 이동 오류: {str(move_error)}")
            
            logger.info(f"카메라 {camera_id} 추적 및 스캔 통합 시작: 수평 {horizontal_steps}단계, 수직 {vertical_steps}단계")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 추적 및 스캔 통합 시작 오류: {str(e)}")
            return False
    
    def start_tracking(self, camera_id, detection_data=None):
        """조류 추적 시작

        Args:
            camera_id: 카메라 ID
            detection_data: 조류 감지 데이터 (bounding box, 좌표 등)

        Returns:
            bool: 추적 시작 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다. 먼저 setup_onvif_ptz를 호출하세요.")
                return False
            
            # 추적 상태 업데이트
            self.tracking_status[camera_id] = True
            
            # 추적 관련 데이터 초기화
            self.bbox_centers[camera_id] = deque(maxlen=10)
            self.last_detection_time[camera_id] = time.time()
            
            # 현재 위치 정보 업데이트
            position = self.get_camera_position(camera_id)
            if position and 'pan' in position:
                self.camera_angles[camera_id] = position['pan']
            else:
                self.camera_angles[camera_id] = 0.0
            
            # 추적 타이머 취소
            self._cancel_return_home_timer(camera_id)
            self._cancel_auto_scan_timer(camera_id)
            self._cancel_all_scan_timers(camera_id)
            
            # 스캔 상태 초기화
            self.is_scanning[camera_id] = False
            
            # 감지 데이터가 있으면 즉시 업데이트
            if detection_data:
                self.update_detection_data(camera_id, detection_data)
            
            logger.info(f"카메라 {camera_id} 조류 추적 시작됨")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 조류 추적 시작 오류: {str(e)}")
            return False
    
    def stop_tracking(self, camera_id):
        """조류 추적 중지

        Args:
            camera_id: 카메라 ID

        Returns:
            bool: 추적 중지 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 이동 중이면 정지
            if self.is_moving[camera_id]:
                self._stop_ptz(camera_id)
            
            # 타이머 취소
            self._cancel_return_home_timer(camera_id)
            self._cancel_auto_scan_timer(camera_id)
            self._cancel_all_scan_timers(camera_id)
            
            # 스캔 상태 초기화
            if camera_id in self.is_scanning:
                del self.is_scanning[camera_id]
            
            # 추적 상태 업데이트
            self.tracking_status[camera_id] = False
            
            logger.info(f"카메라 {camera_id} 조류 추적 중지됨")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 조류 추적 중지 오류: {str(e)}")
            return False
    
    def set_preset(self, camera_id, preset_name):
        """현재 카메라 위치를 프리셋으로 저장

        Args:
            camera_id: 카메라 ID
            preset_name: 프리셋 이름

        Returns:
            bool: 프리셋 저장 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 현재 위치 가져오기
            position = self.get_camera_position(camera_id)
            if not position:
                logger.error(f"카메라 {camera_id} 현재 위치를 가져올 수 없습니다.")
                return False
            
            # 프리셋 저장
            self.preset_positions.setdefault(camera_id, {})
            self.preset_positions[camera_id][preset_name] = position
            
            # 파일로 저장
            self._save_presets(camera_id)
            
            logger.info(f"카메라 {camera_id} 프리셋 '{preset_name}' 저장됨: {position}")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 프리셋 저장 오류: {str(e)}")
            return False
    
    def delete_preset(self, camera_id, preset_name):
        """저장된 프리셋 삭제

        Args:
            camera_id: 카메라 ID
            preset_name: 프리셋 이름

        Returns:
            bool: 삭제 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 프리셋 존재 여부 확인
            if camera_id not in self.preset_positions or preset_name not in self.preset_positions[camera_id]:
                logger.warning(f"카메라 {camera_id}의 프리셋 '{preset_name}'이 존재하지 않습니다.")
                return False
            
            # 프리셋 삭제
            del self.preset_positions[camera_id][preset_name]
            
            # 파일로 저장
            self._save_presets(camera_id)
            
            logger.info(f"카메라 {camera_id} 프리셋 '{preset_name}' 삭제됨")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 프리셋 삭제 오류: {str(e)}")
            return False
    
    def goto_preset(self, camera_id, preset_name):
        """저장된 프리셋 위치로 이동

        Args:
            camera_id: 카메라 ID
            preset_name: 프리셋 이름

        Returns:
            bool: 이동 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 프리셋 존재 여부 확인
            if camera_id not in self.preset_positions or preset_name not in self.preset_positions[camera_id]:
                logger.error(f"카메라 {camera_id}의 프리셋 '{preset_name}'이 존재하지 않습니다.")
                return False
            
            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 프리셋 위치 가져오기
            preset_pos = self.preset_positions[camera_id][preset_name]
            
            # 절대 이동 요청 생성
            request = ptz.create_type('AbsoluteMove')
            request.ProfileToken = profile.token
            
            # 위치 설정
            request.Position = {
                'PanTilt': {'x': preset_pos['pan'], 'y': preset_pos['tilt']},
                'Zoom': {'x': preset_pos['zoom']}
            }
            
            # 이동 실행
            ptz.AbsoluteMove(request)
            
            logger.info(f"카메라 {camera_id} 프리셋 '{preset_name}'으로 이동")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 프리셋 이동 오류: {str(e)}")
            return False
    
    def scan_area(self, camera_id, horizontal_steps=3, vertical_steps=2):
        """지정된 영역을 스캔하여 조류를 탐지

        Args:
            camera_id: 카메라 ID
            horizontal_steps: 수평 스캔 단계 수
            vertical_steps: 수직 스캔 단계 수

        Returns:
            bool: 스캔 시작 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 스캔 영역 설정 (현재는 로그만 출력)
            logger.info(f"카메라 {camera_id} 영역 스캔 시작 (가로 {horizontal_steps}단계, 세로 {vertical_steps}단계)")
            
            # 추후 실제 스캔 로직 구현 예정
            
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 영역 스캔 오류: {str(e)}")
            return False
    
    def track_to_coordinates(self, camera_id, coordinates):
        """특정 좌표로 카메라 이동 (좌표 기반 추적)

        Args:
            camera_id: 카메라 ID
            coordinates: 이동할 좌표 (pan, tilt, zoom)

        Returns:
            bool: 이동 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return False
            
            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 절대 이동 요청 생성
            request = ptz.create_type('AbsoluteMove')
            request.ProfileToken = profile.token
            
            # 위치 설정
            request.Position = {
                'PanTilt': {'x': coordinates['pan'], 'y': coordinates['tilt']},
                'Zoom': {'x': coordinates['zoom']}
            }
            
            # 이동 실행
            ptz.AbsoluteMove(request)
            
            logger.info(f"카메라 {camera_id} 좌표 {coordinates}로 이동")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 좌표 이동 오류: {str(e)}")
            return False
    
    def _set_safety_timeout(self, camera_id, timeout_seconds=30):
        """안전을 위한 자동 정지 타이머 설정

        Args:
            camera_id: 카메라 ID
            timeout_seconds: 자동 정지 시간(초)
        """
        # 이전 타이머 취소
        self._cancel_safety_timeout(camera_id)
        
        # 새 타이머 설정
        timer = Timer(timeout_seconds, self._auto_stop_ptz, args=[camera_id])
        timer.daemon = True
        timer.start()
        self.timeout_timers[camera_id] = timer
        logger.debug(f"카메라 {camera_id} 안전 타이머 설정: {timeout_seconds}초")
    
    def _cancel_safety_timeout(self, camera_id):
        """안전 타이머 취소

        Args:
            camera_id: 카메라 ID
        """
        if camera_id in self.timeout_timers:
            timer = self.timeout_timers[camera_id]
            timer.cancel()
            del self.timeout_timers[camera_id]
            logger.debug(f"카메라 {camera_id} 안전 타이머 취소")
    
    def _auto_stop_ptz(self, camera_id):
        """타임아웃에 의한 자동 정지 처리

        Args:
            camera_id: 카메라 ID
        """
        logger.warning(f"카메라 {camera_id} 안전 타임아웃으로 PTZ 자동 정지")
        self._stop_ptz(camera_id)
    
    def release_camera(self, camera_id):
        """카메라 PTZ 세션 해제

        Args:
            camera_id: 카메라 ID

        Returns:
            bool: 해제 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 이동 중이면 정지
            if camera_id in self.is_moving and self.is_moving[camera_id]:
                self._stop_ptz(camera_id)
            
            # 타이머 취소
            self._cancel_safety_timeout(camera_id)
            self._cancel_return_home_timer(camera_id)
            self._cancel_auto_scan_timer(camera_id)
            self._cancel_all_scan_timers(camera_id)
            
            # 스캔 상태 초기화
            if camera_id in self.is_scanning:
                del self.is_scanning[camera_id]
            
            # 세션 정리
            if camera_id in self.ptz_sessions:
                del self.ptz_sessions[camera_id]
            
            # 상태 정보 정리
            if camera_id in self.move_locks:
                del self.move_locks[camera_id]
            
            if camera_id in self.is_moving:
                del self.is_moving[camera_id]
            
            if camera_id in self.tracking_status:
                del self.tracking_status[camera_id]
            
            if camera_id in self.bbox_centers:
                del self.bbox_centers[camera_id]
            
            if camera_id in self.last_detection_time:
                del self.last_detection_time[camera_id]
            
            if camera_id in self.camera_angles:
                del self.camera_angles[camera_id]
            
            if camera_id in self.scan_timers:
                del self.scan_timers[camera_id]
            
            logger.info(f"카메라 {camera_id} PTZ 세션 해제됨")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} PTZ 세션 해제 오류: {str(e)}")
            return False
    
    def get_camera_position(self, camera_id):
        """현재 카메라 위치 정보 가져오기

        Args:
            camera_id: 카메라 ID

        Returns:
            dict: 위치 정보 (pan, tilt, zoom) 또는 None
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다.")
                return None
            
            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 현재 위치 가져오기
            status = ptz.GetStatus({'ProfileToken': profile.token})
            
            # 결과 변환
            position = {}
            
            if hasattr(status, 'Position') and hasattr(status.Position, 'PanTilt'):
                position['pan'] = status.Position.PanTilt.x
                position['tilt'] = status.Position.PanTilt.y
            
            if hasattr(status, 'Position') and hasattr(status.Position, 'Zoom'):
                position['zoom'] = status.Position.Zoom.x
            
            logger.info(f"카메라 {camera_id} 현재 위치: {position}")
            return position
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 위치 정보 가져오기 오류: {str(e)}")
            return None
    
    def is_ptz_moving(self, camera_id):
        """카메라 이동 상태 확인

        Args:
            camera_id: 카메라 ID

        Returns:
            bool: 이동 중 여부
        """
        # 카메라 ID를 문자열로 변환
        camera_id = str(camera_id)
        
        return camera_id in self.is_moving and self.is_moving[camera_id]
    
    def is_tracking_active(self, camera_id):
        """카메라 추적 상태 확인

        Args:
            camera_id: 카메라 ID

        Returns:
            bool: 추적 중 여부
        """
        # 카메라 ID를 문자열로 변환
        camera_id = str(camera_id)
        
        return camera_id in self.tracking_status and self.tracking_status[camera_id]
    
    def update_detection_data(self, camera_id, detection_boxes):
        """
        감지된 객체의 바운딩 박스 정보 업데이트 (추적 알고리즘용)
        
        Args:
            camera_id (str): 카메라 ID
            detection_boxes (list): 감지된 바운딩 박스 목록 [x1, y1, x2, y2] 형식
            
        Returns:
            bool: 업데이트 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.warning(f"카메라 {camera_id}의 PTZ 세션이 없습니다. 먼저 setup_onvif_ptz를 호출하세요.")
                return False
            
            # 추적 모드가 아니면 데이터 무시
            if not self.tracking_status.get(camera_id, False):
                return False
            
            # 감지된 객체가 없는 경우
            if not detection_boxes or len(detection_boxes) == 0:
                # 객체 없는 시간 처리
                self._handle_no_object_detected(camera_id)
                return True
            
            # 객체가 감지된 경우 타이머 초기화
            self._cancel_return_home_timer(camera_id)
            self._cancel_auto_scan_timer(camera_id)
            
            # 바운딩 박스 중앙값 계산
            centers = []
            for box in detection_boxes:
                # 바운딩 박스 형식: [x1, y1, x2, y2] - 좌상단과 우하단 좌표
                x1, y1, x2, y2 = box
                center_x = (x1 + x2) / 2.0
                centers.append(center_x)
            
            # 평균 중앙값 계산
            if centers:
                avg_center = sum(centers) / len(centers)
                # 값을 정규화 (0.0~1.0 범위로 변환, 0.5가 화면 중앙)
                normalized_center = avg_center
                
                # 최근 중앙값 목록에 추가
                self.bbox_centers[camera_id].append(normalized_center)
                
                # 마지막 감지 시간 업데이트
                self.last_detection_time[camera_id] = time.time()
                
                # 방향 결정 및 카메라 이동
                self._determine_direction_and_move(camera_id)
            
            return True
        
        except Exception as e:
            logger.error(f"카메라 {camera_id} 감지 데이터 업데이트 오류: {str(e)}")
            return False
    
    def _determine_direction_and_move(self, camera_id):
        """
        바운딩 박스 중앙값을 기반으로 카메라 이동 방향 결정
        
        Args:
            camera_id (str): 카메라 ID
        """
        try:
            # 최근 중앙값이 없으면 처리 불가
            if not self.bbox_centers[camera_id]:
                return
            
            # 최근 중앙값의 평균 계산
            avg_center = sum(self.bbox_centers[camera_id]) / len(self.bbox_centers[camera_id])
            
            # 화면 중앙을 0.5로 가정 (정규화된 좌표)
            center_threshold = self.CENTER_ZONE_SIZE / 2.0
            
            # 현재 카메라 각도 가져오기
            current_angle = self.camera_angles.get(camera_id, 0.0)
            
            # 이동 방향 결정
            if avg_center < 0.5 - center_threshold:
                # 객체가 왼쪽에 있음 - 카메라를 왼쪽으로 이동
                if current_angle <= -self.MAX_PAN_VALUE:
                    # 최대 좌측 각도 도달, 이동 제한
                    logger.info(f"카메라 {camera_id} 좌측 최대 각도 도달 (현재 각도: {current_angle})")
                    self._stop_ptz(camera_id)
                else:
                    # 값이 0.5에서 얼마나 떨어져 있는지에 따라 이동 속도 조절
                    deviation = abs(0.5 - avg_center)
                    speed = min(0.9, max(0.2, deviation * 2.0))  # 0.2~0.9 범위 속도
                    
                    # 왼쪽으로 이동
                    self._move_camera_with_tracking(camera_id, "LEFT", speed)
                    
                    # 각도 업데이트 (왼쪽은 음수 방향)
                    # 정규화된 값으로 각도 업데이트 (0.02 정도씩 변화)
                    self.camera_angles[camera_id] = max(-self.MAX_PAN_VALUE, current_angle - 0.02)
                    
            elif avg_center > 0.5 + center_threshold:
                # 객체가 오른쪽에 있음 - 카메라를 오른쪽으로 이동
                if current_angle >= self.MAX_PAN_VALUE:
                    # 최대 우측 각도 도달, 이동 제한
                    logger.info(f"카메라 {camera_id} 우측 최대 각도 도달 (현재 각도: {current_angle})")
                    self._stop_ptz(camera_id)
                else:
                    # 값이 0.5에서 얼마나 떨어져 있는지에 따라 이동 속도 조절
                    deviation = abs(0.5 - avg_center)
                    speed = min(0.9, max(0.2, deviation * 2.0))  # 0.2~0.9 범위 속도
                    
                    # 오른쪽으로 이동
                    self._move_camera_with_tracking(camera_id, "RIGHT", speed)
                    
                    # 각도 업데이트 (오른쪽은 양수 방향)
                    # 정규화된 값으로 각도 업데이트 (0.02 정도씩 변화)
                    self.camera_angles[camera_id] = min(self.MAX_PAN_VALUE, current_angle + 0.02)
                    
            else:
                # 객체가 중앙에 있음 - 현재 위치 유지
                logger.debug(f"카메라 {camera_id} 객체가 중앙에 있음, 현재 위치 유지")
                self._stop_ptz(camera_id)
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 방향 결정 오류: {str(e)}")
    
    def _move_camera_with_tracking(self, camera_id, direction, speed):
        """
        추적 모드에서 카메라 이동 (기존 control_ptz와 유사하나 추적 모드 검사 없음)
        
        Args:
            camera_id (str): 카메라 ID
            direction (str): 이동 방향 (LEFT, RIGHT)
            speed (float): 이동 속도 (0.0~1.0)
        """
        try:
            logger.info(f"카메라 {camera_id} 추적 모드 이동 시작 - 방향: {direction}, 속도: {speed}")
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.error(f"카메라 {camera_id} PTZ 세션 없음, 이동 불가")
                return False

            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 이미 이동 중이면 명령 무시 (방향이 바뀔 때만 새로 명령)
            if self.is_moving[camera_id]:
                logger.info(f"카메라 {camera_id} 이미 이동 중, 명령 무시")
                return True
            
            # 현재 위치 가져오기 - Tilt와 Zoom 값을 유지하기 위해
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 현재 위치를 가져올 수 없음")
                current_position = {'pan': 0.0, 'tilt': 0.0, 'zoom': 0.0}
            
            logger.info(f"카메라 {camera_id} 현재 위치: {current_position}")
            
            # PTZ 이동 요청 생성
            request = ptz.create_type('ContinuousMove')
            request.ProfileToken = profile.token
            
            # PTZ 속도 요청 생성 - Pan만 변경하고 Tilt와 Zoom은 0으로 설정
            velocity = {
                'PanTilt': {'x': 0, 'y': 0},
                'Zoom': {'x': 0}
            }
            
            # 방향에 따른 속도 설정 (Pan만 변경)
            if direction == "LEFT":
                velocity['PanTilt']['x'] = -speed
                logger.info(f"카메라 {camera_id} LEFT 방향 속도 설정: x = {-speed}, y = 0")
            elif direction == "RIGHT":
                velocity['PanTilt']['x'] = speed
                logger.info(f"카메라 {camera_id} RIGHT 방향 속도 설정: x = {speed}, y = 0")
            else:
                logger.error(f"카메라 {camera_id} 지원하지 않는 방향: {direction}")
                return False
            
            # 속도값 설정
            request.Velocity = velocity
            
            try:
                # PTZ 이동 실행
                logger.info(f"카메라 {camera_id} ContinuousMove 명령 전송 시도")
                ptz.ContinuousMove(request)
                logger.info(f"카메라 {camera_id} ContinuousMove 명령 전송 성공")
                self.is_moving[camera_id] = True
                
                # 안전을 위한 타임아웃 설정 (10초 후 자동 정지 - 더 짧게 설정)
                self._set_safety_timeout(camera_id, 10)
                
                return True
            except Exception as inner_e:
                logger.error(f"카메라 {camera_id} ContinuousMove 명령 전송 실패: {str(inner_e)}")
                import traceback
                logger.error(f"이동 명령 실패 스택 추적: {traceback.format_exc()}")
                return False
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 추적 모드 이동 오류: {str(e)}")
            import traceback
            logger.error(f"추적 모드 이동 스택 추적: {traceback.format_exc()}")
            return False
    
    def _handle_no_object_detected(self, camera_id):
        """
        객체가 감지되지 않을 때 처리 로직
        
        Args:
            camera_id (str): 카메라 ID
        """
        # 마지막 객체 감지 시간 확인
        current_time = time.time()
        last_detection = self.last_detection_time.get(camera_id, 0)
        time_since_last_detection = current_time - last_detection
        
        # 타이머가 없고, 일정 시간 이상 객체가 감지되지 않은 경우 홈 위치 복귀 타이머 설정
        if (camera_id not in self.return_home_timers or self.return_home_timers[camera_id] is None) and \
           time_since_last_detection >= self.NO_OBJECT_TIMEOUT:
            logger.info(f"카메라 {camera_id} {self.NO_OBJECT_TIMEOUT}초 동안 객체 미감지, 홈 위치 복귀 예정")
            self._set_return_home_timer(camera_id)
    
    def _set_return_home_timer(self, camera_id):
        """
        홈 위치(0도)로 복귀하는 타이머 설정
        
        Args:
            camera_id (str): 카메라 ID
        """
        # 이전 타이머 취소
        self._cancel_return_home_timer(camera_id)
        
        # 새 타이머 설정
        timer = Timer(1.0, self._return_to_home_position, args=[camera_id])
        timer.daemon = True
        timer.start()
        self.return_home_timers[camera_id] = timer
        logger.debug(f"카메라 {camera_id} 홈 위치 복귀 타이머 설정")
    
    def _cancel_return_home_timer(self, camera_id):
        """
        홈 위치 복귀 타이머 취소
        
        Args:
            camera_id (str): 카메라 ID
        """
        if camera_id in self.return_home_timers and self.return_home_timers[camera_id] is not None:
            self.return_home_timers[camera_id].cancel()
            self.return_home_timers[camera_id] = None
            logger.debug(f"카메라 {camera_id} 홈 위치 복귀 타이머 취소")
    
    def _return_to_home_position(self, camera_id):
        """
        카메라를 홈 위치(0도)로 복귀시키는 함수
        
        Args:
            camera_id (str): 카메라 ID
        """
        try:
            logger.info(f"카메라 {camera_id} 홈 위치(0)로 복귀 시작")
            
            # 현재 각도 확인
            current_angle = self.camera_angles.get(camera_id, 0.0)
            
            # 이미 0도 근처이면 무시
            if abs(current_angle) < 0.02:
                logger.info(f"카메라 {camera_id} 이미 홈 위치 근처 (현재 각도: {current_angle})")
                self._set_auto_scan_timer(camera_id)
                return
            
            # 홈 위치로 복귀하기 위한 방향 결정
            direction = "RIGHT" if current_angle < 0 else "LEFT"
            
            # 이동 속도 설정 (각도에 비례)
            speed = min(0.8, max(0.3, abs(current_angle) / 0.33))
            
            # 카메라 이동
            self._move_camera_with_tracking(camera_id, direction, speed)
            
            # 현재 각도 업데이트
            if direction == "RIGHT":
                self.camera_angles[camera_id] = min(0.0, current_angle + 0.05)
            else:
                self.camera_angles[camera_id] = max(0.0, current_angle - 0.05)
            
            # 홈 위치에 도달했는지 확인
            if abs(self.camera_angles[camera_id]) < 0.02:
                # 홈 위치 도달
                self._stop_ptz(camera_id)
                self.camera_angles[camera_id] = 0.0
                logger.info(f"카메라 {camera_id} 홈 위치(0) 도달")
                
                # 자동 스캔 타이머 설정
                self._set_auto_scan_timer(camera_id)
            else:
                # 홈 위치 도달할 때까지 타이머 재설정
                self._set_return_home_timer(camera_id)
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 홈 위치 복귀 오류: {str(e)}")
    
    def _set_auto_scan_timer(self, camera_id):
        """
        자동 팬 스캔 모드 타이머 설정
        
        Args:
            camera_id (str): 카메라 ID
        """
        # 이전 타이머 취소
        self._cancel_auto_scan_timer(camera_id)
        
        # 새 타이머 설정
        timer = Timer(self.AUTO_SCAN_TIMEOUT, self._start_scan_sequence, args=[camera_id])
        timer.daemon = True
        timer.start()
        self.auto_scan_timers[camera_id] = timer
        logger.debug(f"카메라 {camera_id} 자동 스캔 모드 전환 타이머 설정 ({self.AUTO_SCAN_TIMEOUT}초)")
    
    def _cancel_auto_scan_timer(self, camera_id):
        """
        자동 팬 스캔 모드 타이머 취소
        
        Args:
            camera_id (str): 카메라 ID
        """
        if camera_id in self.auto_scan_timers and self.auto_scan_timers[camera_id] is not None:
            self.auto_scan_timers[camera_id].cancel()
            self.auto_scan_timers[camera_id] = None
            logger.debug(f"카메라 {camera_id} 자동 스캔 모드 타이머 취소")
    
    def _start_scan_sequence(self, camera_id):
        """
        스캔 시퀀스 시작 - 수동으로 스캔 함수 호출
        """
        try:
            # 이미 스캔 중이면 중복 실행 방지
            if self.is_scanning.get(camera_id, False):
                logger.warning(f"카메라 {camera_id} 이미 스캔 중, 중복 스캔 요청 무시")
                return
            
            # 스캔 상태 플래그 설정
            self.is_scanning[camera_id] = True
            
            logger.info(f"카메라 {camera_id} 스캔 시퀀스 시작")
            
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 왼쪽으로 이동
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 왼쪽(-0.2)으로 이동
            logger.info(f"카메라 {camera_id} 왼쪽(-0.2)으로 이동")
            left_result = self._absolute_move_to_position(
                camera_id, 
                -0.2,  # Pan = -0.2
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not left_result:
                logger.error(f"카메라 {camera_id} 왼쪽 이동 실패")
                self.is_scanning[camera_id] = False
                return
            
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_move_to_center_from_left')
            
            # 다음 이동 계획 (10초 후 중앙으로)
            timer = Timer(10.0, self._move_to_center_from_left, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            if camera_id not in self.scan_timers:
                self.scan_timers[camera_id] = {}
            self.scan_timers[camera_id]['_move_to_center_from_left'] = timer
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 스캔 시퀀스 시작 오류: {str(e)}")
            import traceback
            logger.error(f"스택 추적: {traceback.format_exc()}")
            # 오류 발생 시 스캔 상태 초기화
            self.is_scanning[camera_id] = False
    
    def _move_to_center_from_left(self, camera_id):
        """왼쪽에서 중앙으로 이동"""
        try:
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 중앙(0)으로 이동
            logger.info(f"카메라 {camera_id} 중앙(0)으로 이동")
            center_result = self._absolute_move_to_position(
                camera_id, 
                0.0, 
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not center_result:
                logger.error(f"카메라 {camera_id} 중앙 이동 실패")
                self.is_scanning[camera_id] = False
                return
        
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_move_to_right')
            
            # 다음 이동 계획 (10초 후 오른쪽으로)
            timer = Timer(10.0, self._move_to_right, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            self.scan_timers[camera_id]['_move_to_right'] = timer
        
        except Exception as e:
            logger.error(f"카메라 {camera_id} 중앙 이동 오류: {str(e)}")
            self.is_scanning[camera_id] = False

    def _move_to_right(self, camera_id):
        """중앙에서 오른쪽으로 이동"""
        try:
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 오른쪽(0.2)으로 이동
            logger.info(f"카메라 {camera_id} 오른쪽(0.2)으로 이동")
            right_result = self._absolute_move_to_position(
                camera_id, 
                0.2, 
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not right_result:
                logger.error(f"카메라 {camera_id} 오른쪽 이동 실패")
                self.is_scanning[camera_id] = False
                return
            
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_move_to_center_from_right')
            
            # 다음 이동 계획 (10초 후 중앙으로)
            timer = Timer(10.0, self._move_to_center_from_right, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            self.scan_timers[camera_id]['_move_to_center_from_right'] = timer
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 오른쪽 이동 오류: {str(e)}")
            self.is_scanning[camera_id] = False

    def _move_to_center_from_right(self, camera_id):
        """오른쪽에서 중앙으로 이동 후 스캔 다시 시작"""
        try:
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 중앙(0)으로 이동
            logger.info(f"카메라 {camera_id} 중앙(0)으로 이동")
            center_result = self._absolute_move_to_position(
                camera_id, 
                0.0, 
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not center_result:
                logger.error(f"카메라 {camera_id} 중앙 이동 실패")
                self.is_scanning[camera_id] = False
                return
            
            # 스캔 완료 후 상태 초기화
            self.is_scanning[camera_id] = False
            
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_start_scan_sequence')
            
            # 10초 후 다시 스캔 시작 (스캔 루프)
            timer = Timer(10.0, self._start_scan_sequence, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            self.scan_timers[camera_id]['_start_scan_sequence'] = timer
        
        except Exception as e:
            logger.error(f"카메라 {camera_id} 중앙 이동 오류: {str(e)}")
            self.is_scanning[camera_id] = False
    
    def _cancel_scan_timer(self, camera_id, timer_name):
        """
        특정 스캔 타이머 취소
        
        Args:
            camera_id (str): 카메라 ID
            timer_name (str): 타이머 이름
        """
        if camera_id in self.scan_timers and timer_name in self.scan_timers[camera_id]:
            if self.scan_timers[camera_id][timer_name]:
                self.scan_timers[camera_id][timer_name].cancel()
                self.scan_timers[camera_id][timer_name] = None
                logger.debug(f"카메라 {camera_id} {timer_name} 타이머 취소")
    
    def _cancel_all_scan_timers(self, camera_id):
        """
        카메라 ID에 대한 모든 스캔 관련 타이머 취소
        
        Args:
            camera_id (str): 카메라 ID
        """
        if camera_id in self.scan_timers:
            for timer_name, timer in self.scan_timers[camera_id].items():
                if timer:
                    timer.cancel()
            
            # 타이머 딕셔너리 초기화
            self.scan_timers[camera_id] = {}
            logger.debug(f"카메라 {camera_id} 모든 스캔 타이머 취소")
    
    def set_scan_config(self, camera_id, horizontal_steps=3, vertical_steps=2):
        """스캔 설정 저장
        
        Args:
            camera_id (str): 카메라 ID
            horizontal_steps (int): 수평 스캔 단계 수
            vertical_steps (int): 수직 스캔 단계 수
            
        Returns:
            bool: 설정 성공 여부
        """
        try:
            # 카메라 ID를 문자열로 변환
            camera_id = str(camera_id)
            
            # 스캔 설정 저장
            self.scan_config[camera_id] = {
                'horizontal_steps': max(1, min(5, horizontal_steps)),  # 1~5 범위로 제한
                'vertical_steps': max(1, min(3, vertical_steps))       # 1~3 범위로 제한
            }
            
            logger.info(f"카메라 {camera_id} 스캔 설정 업데이트: 수평 {horizontal_steps}단계, 수직 {vertical_steps}단계")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 스캔 설정 저장 오류: {str(e)}")
            return False
    
    def get_scan_config(self, camera_id):
        """스캔 설정 조회
        
        Args:
            camera_id (str): 카메라 ID
            
        Returns:
            dict: 스캔 설정 정보
        """
        camera_id = str(camera_id)
        if camera_id in self.scan_config:
            return self.scan_config[camera_id]
        else:
            # 기본 설정 반환
            return {
                'horizontal_steps': 3,
                'vertical_steps': 2
            }
    
    def _absolute_move_to_position(self, camera_id, pan, tilt, zoom):
        """
        카메라를 지정된 PTZ 값으로 이동
        
        Args:
            camera_id (str): 카메라 ID
            pan (float): Pan 값
            tilt (float): Tilt 값
            zoom (float): Zoom 값
        
        Returns:
            bool: 이동 성공 여부
        """
        try:
            logger.info(f"카메라 {camera_id} 절대 위치 이동: Pan={pan}, Tilt={tilt}, Zoom={zoom}")
            
            # 세션 존재 여부 확인
            if camera_id not in self.ptz_sessions:
                logger.error(f"카메라 {camera_id} PTZ 세션 없음, 이동 불가")
                return False
            
            # 설정 정보 가져오기
            session = self.ptz_sessions[camera_id]
            ptz = session['ptz']
            profile = session['profile']
            
            # 절대 이동 요청 생성
            request = ptz.create_type('AbsoluteMove')
            request.ProfileToken = profile.token
            
            # PTZ 위치 설정
            request.Position = {
                'PanTilt': {'x': pan, 'y': tilt},
                'Zoom': {'x': zoom}
            }
            
            # 이동 실행
            logger.info(f"카메라 {camera_id} AbsoluteMove 명령 전송 시도")
            ptz.AbsoluteMove(request)
            logger.info(f"카메라 {camera_id} AbsoluteMove 명령 전송 성공")
            
            # 내부 상태 업데이트
            self.camera_angles[camera_id] = pan
            
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 절대 위치 이동 오류: {str(e)}")
            import traceback
            logger.error(f"스택 추적: {traceback.format_exc()}")
            return False    
    def _move_to_right(self, camera_id):
        """중앙에서 오른쪽으로 이동"""
        try:
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 오른쪽(0.2)으로 이동
            logger.info(f"카메라 {camera_id} 오른쪽(0.2)으로 이동")
            right_result = self._absolute_move_to_position(
                camera_id, 
                0.2, 
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not right_result:
                logger.error(f"카메라 {camera_id} 오른쪽 이동 실패")
                self.is_scanning[camera_id] = False
                return
            
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_move_to_center_from_right')
            
            # 다음 이동 계획 (10초 후 중앙으로)
            timer = Timer(10.0, self._move_to_center_from_right, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            self.scan_timers[camera_id]['_move_to_center_from_right'] = timer
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 오른쪽 이동 오류: {str(e)}")
            self.is_scanning[camera_id] = False
    def _move_to_center_from_right(self, camera_id):
        """오른쪽에서 중앙으로 이동 후 스캔 다시 시작"""
        try:
            # 객체가 감지되었는지 확인
            time_since_detection = time.time() - self.last_detection_time.get(camera_id, 0)
            if time_since_detection < self.NO_OBJECT_TIMEOUT:
                logger.info(f"카메라 {camera_id} 객체가 감지됨, 스캔 취소")
                self.is_scanning[camera_id] = False
                return
            
            # 현재 위치 가져오기
            current_position = self.get_camera_position(camera_id)
            if not current_position:
                logger.error(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                self.is_scanning[camera_id] = False
                return
            
            # 중앙(0)으로 이동
            logger.info(f"카메라 {camera_id} 중앙(0)으로 이동")
            center_result = self._absolute_move_to_position(
                camera_id, 
                0.0, 
                current_position.get('tilt', 0.0), 
                current_position.get('zoom', 0.0)
            )
            
            if not center_result:
                logger.error(f"카메라 {camera_id} 중앙 이동 실패")
                self.is_scanning[camera_id] = False
                return
            
            # 스캔 완료 후 상태 초기화
            self.is_scanning[camera_id] = False
            
            # 기존 타이머 취소 후 새 타이머 생성
            self._cancel_scan_timer(camera_id, '_start_scan_sequence')
            
            # 10초 후 다시 스캔 시작 (스캔 루프)
            timer = Timer(10.0, self._start_scan_sequence, args=[camera_id])
            timer.daemon = True
            timer.start()
            
            # 타이머 저장
            self.scan_timers[camera_id]['_start_scan_sequence'] = timer
        
        except Exception as e:
            logger.error(f"카메라 {camera_id} 중앙 이동 오류: {str(e)}")
            self.is_scanning[camera_id] = False 

