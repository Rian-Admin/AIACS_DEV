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
            ptz.Stop(request)
            
            logger.info(f"카메라 {camera_id} PTZ 정지")
            self.is_moving[camera_id] = False
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} PTZ 정지 오류: {str(e)}")
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
            
            # 추적 로직 구현 (현재는 로그만 출력)
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