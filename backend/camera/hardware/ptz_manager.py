"""
PTZ 카메라 제어 매니저
ONVIF 프로토콜을 사용하여 PTZ 기능이 있는 카메라의 팬(Pan), 틸트(Tilt), 줌(Zoom) 기능을 제어하는 모듈입니다.
"""

import time
import logging
import threading
import traceback  # 추가: 자세한 오류 추적을 위해
from typing import Dict, Optional, Any
from threading import Timer, Lock
from onvif import ONVIFCamera

# 로깅 설정
logger = logging.getLogger(__name__)

class PTZManager:
    """카메라별 PTZ 제어 관리 클래스"""
    
    # 싱글톤 인스턴스
    _instance = None 
    _instance_lock = threading.Lock()
    
    # 카메라 제조사별 패턴
    CAMERA_VENDORS = {
        '1': 'UNKNOWN',  # 기본값
        '2': 'HIKVISION'
    }
    
    @classmethod
    def get_instance(cls):
        """싱글톤 패턴 구현"""
        with cls._instance_lock:
            if cls._instance is None:
                cls._instance = cls()
        return cls._instance
    
    def __init__(self):
        """초기화"""
        if PTZManager._instance is not None:
            raise RuntimeError("싱글톤 객체에 직접 접근하지 마세요. get_instance()를 사용하세요.")
        
        # 카메라별 PTZ 상태 관리
        self.ptz_sessions = {}  # 카메라 ID별 PTZ 세션 저장
        self.move_locks = {}    # 카메라 ID별 잠금 객체
        self.is_moving = {}     # 카메라 ID별 이동 상태
        self.timeout_timers = {} # 카메라 ID별 타임아웃 타이머
        self.camera_vendor = {} # 카메라 ID별 제조사 정보
        
        logger.info("PTZ 매니저가 초기화되었습니다.")
    
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
            
            # 카메라 장치 정보 가져오기 시도
            try:
                devicemgmt = mycam.create_devicemgmt_service()
                device_info = devicemgmt.GetDeviceInformation()
                logger.info(f"카메라 {camera_id} 장치 정보: 제조사={device_info.Manufacturer}, 모델={device_info.Model}")
                
                # 제조사 정보 저장
                vendor = device_info.Manufacturer.upper() if hasattr(device_info, 'Manufacturer') else 'UNKNOWN'
                if 'HIK' in vendor:
                    self.camera_vendor[camera_id] = 'HIKVISION'
                elif 'DAHUA' in vendor:
                    self.camera_vendor[camera_id] = 'DAHUA'
                else:
                    self.camera_vendor[camera_id] = vendor
                    
            except Exception as e:
                logger.warning(f"카메라 {camera_id} 장치 정보 가져오기 실패: {str(e)}")
                # 기본값 설정 (특정 카메라 ID별 제조사 정보 활용)
                self.camera_vendor[camera_id] = self.CAMERA_VENDORS.get(camera_id, 'UNKNOWN')
            
            # PTZ 설정 옵션
            try:
                ptz_configuration_options = ptz.GetConfigurationOptions({
                    'ConfigurationToken': media_profile.PTZConfiguration.token
                })
            except Exception as e:
                logger.warning(f"카메라 {camera_id} PTZ 설정 옵션 가져오기 실패: {str(e)}")
                ptz_configuration_options = None
            
            # 세션 정보 저장
            self.ptz_sessions[camera_id] = {
                'ptz': ptz,
                'media': media,
                'profile': media_profile,
                'config_options': ptz_configuration_options,
                'camera_ip': ip,
                'username': username,
                'password': password,
                'port': port,
                'vendor': self.camera_vendor.get(camera_id, 'UNKNOWN')
            }
            
            # 잠금 객체 및 상태 초기화
            self.move_locks[camera_id] = Lock()
            self.is_moving[camera_id] = False
            
            logger.info(f"카메라 {camera_id}의 ONVIF PTZ 설정 완료 (제조사: {self.camera_vendor.get(camera_id, 'UNKNOWN')})")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} ONVIF PTZ 설정 오류: {str(e)}")
            logger.error(traceback.format_exc())  # 추가: 오류 추적
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
            
            logger.info(f"카메라 {camera_id} PTZ 세션 해제됨")
            return True
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} PTZ 세션 해제 오류: {str(e)}")
            return False
    
    def get_camera_position(self, camera_id):
        """현재 카메라 위치 정보 가져오기 - 다양한 제조사 지원

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
            vendor = session['vendor']
            
            logger.info(f"카메라 {camera_id} 위치 정보 조회 시도 (제조사: {vendor})")
            
            # 제조사별 다른 방식 적용
            position = None
            
            # 첫 번째 방식: 표준 GetStatus 시도 (Position.PanTilt 체크)
            try:
                # 현재 위치 가져오기
                logger.debug(f"카메라 {camera_id} GetStatus 요청 시도 - 방법 1")
                status = ptz.GetStatus({'ProfileToken': profile.token})
                
                # 결과 변환
                if hasattr(status, 'Position'):
                    position = {}
                    
                    if hasattr(status.Position, 'PanTilt'):
                        position['pan'] = status.Position.PanTilt.x
                        position['tilt'] = status.Position.PanTilt.y
                        logger.debug(f"카메라 {camera_id} PanTilt 정보 획득: {position['pan']}, {position['tilt']}")
                    else:
                        logger.warning(f"카메라 {camera_id} PanTilt 속성 없음")
                    
                    if hasattr(status.Position, 'Zoom'):
                        position['zoom'] = status.Position.Zoom.x
                        logger.debug(f"카메라 {camera_id} Zoom 정보 획득: {position['zoom']}")
                    else:
                        logger.warning(f"카메라 {camera_id} Zoom 속성 없음")
                        position['zoom'] = 0.0  # 기본값
                else:
                    logger.warning(f"카메라 {camera_id} Position 속성 없음")
            except Exception as e:
                logger.warning(f"카메라 {camera_id} GetStatus 방법 1 실패: {str(e)}")
            
            # 여전히 위치 정보가 없으면 두 번째 방식 시도 (AbsolutePosition 사용)
            if not position or len(position) == 0:
                try:
                    logger.debug(f"카메라 {camera_id} GetPosition 요청 시도 - 방법 2 (AbsolutePosition)")
                    # 일부 카메라는 다른 메서드 사용
                    request = ptz.create_type('GetPresets')
                    request.ProfileToken = profile.token
                    presets = ptz.GetPresets(request)
                    
                    # 프리셋이 있으면 현재 위치를 대략적으로 추정
                    if presets and len(presets) > 0:
                        # 첫 번째 프리셋의 위치 정보 사용 (임시)
                        for preset in presets:
                            if hasattr(preset, 'PTZPosition'):
                                position = {}
                                if hasattr(preset.PTZPosition, 'PanTilt'):
                                    position['pan'] = preset.PTZPosition.PanTilt.x
                                    position['tilt'] = preset.PTZPosition.PanTilt.y
                                if hasattr(preset.PTZPosition, 'Zoom'):
                                    position['zoom'] = preset.PTZPosition.Zoom.x
                                logger.debug(f"카메라 {camera_id} 프리셋에서 위치 정보 추정: {position}")
                                break
                except Exception as e:
                    logger.warning(f"카메라 {camera_id} GetPosition 방법 2 실패: {str(e)}")
            
            # 세 번째 방식: 카메라 제조사별 특별 처리
            if not position or len(position) == 0:
                # 기본 위치 정보 제공 (실제 값은 아니지만, UI 표시 목적으로)
                if camera_id == '1':  # 1번 카메라에 특화된 임시 해결책
                    logger.info(f"카메라 {camera_id}에 대한 임시 위치 정보 제공")
                    position = {
                        'pan': 0.0,    # 기본값
                        'tilt': 0.0,   # 기본값
                        'zoom': 0.0    # 기본값
                    }
            
            if position and len(position) > 0:
                # 카메라가 정확한 위치 정보를 제공하지 않으면 pan만이라도 표시
                if 'pan' not in position:
                    position['pan'] = 0.0
                if 'tilt' not in position:
                    position['tilt'] = 0.0
                if 'zoom' not in position:
                    position['zoom'] = 0.0
                    
                logger.info(f"카메라 {camera_id} 현재 위치: {position}")
                return position
            else:
                logger.warning(f"카메라 {camera_id} 위치 정보를 가져올 수 없음")
                return None
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 위치 정보 가져오기 오류: {str(e)}")
            logger.error(traceback.format_exc())  # 추가: 오류 추적
            
            # 오류 발생 시 기본값 제공 (UI 표시 용도)
            if camera_id in ['1', '2']:  # 특정 카메라 ID에 대한 임시 해결책
                return {
                    'pan': 0.0,
                    'tilt': 0.0,
                    'zoom': 0.0
                }
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