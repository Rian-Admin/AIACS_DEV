import os
import cv2
import time
import logging
from typing import Dict, Optional, Union, Any
from django.core.cache import cache
from django.conf import settings

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

class CameraManager:

    def __init__(self):
        # RTSP 전송 프로토콜 설정 - 10fps와 TCP로 최적화
        os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;tcp|timeout;15000000|stimeout;15000000|max_delay;500000|framerate;10"
        
        # 카메라 URL 캐시
        self._url_cache = {}
        self._cache_timeout = 1  # 캐시 만료 시간 (초)
        self._last_db_check = 0
        self._db_refresh_interval = 5  # DB 체크 간격 (초) - 1초에서 5초로 늘림
        
        # Django의 Camera 모델에서 카메라 정보 초기 로드
        self._load_cameras_from_db()
        
        self._last_access = {}
        self._optimized_urls = {}
        
        # 디버깅용 로깅 추가
        logger.info(f"카메라 매니저 초기화 완료, 캐시 상태: {self._url_cache}")
        
        # 성능 및 오류 관련 설정
        self.connection_timeout = 15  # 연결 타임아웃 시간 (초)
        self.reconnect_attempts = 5   # 재연결 시도 횟수
    
    def _load_cameras_from_db(self):
        """데이터베이스에서 카메라 정보 로드"""
        try:
            # Django 모델에서 카메라 정보 가져오기
            from ..models import Camera
            
            # 모든 카메라 가져오기 (상태와 관계없이)
            cameras = Camera.objects.all()
            
            # 이전 캐시 비우기
            self._url_cache.clear()
            
            # URL 캐시 업데이트
            loaded_count = 0
            for camera in cameras:
                try:
                    camera_id = camera.camera_id
                    rtsp_address = camera.rtsp_address
                    self._url_cache[camera_id] = rtsp_address
                    logger.info(f"카메라 {camera_id} 캐싱: {rtsp_address}")
                    loaded_count += 1
                except UnicodeDecodeError as ude:
                    logger.error(f"카메라 객체 처리 중 인코딩 오류: {ude}")
                    continue
                except Exception as ex:
                    logger.error(f"카메라 객체 처리 중 오류: {ex}")
                    continue
            
            # 마지막 DB 체크 시간 업데이트
            self._last_db_check = time.time()
            
            logger.info(f"데이터베이스에서 {loaded_count}/{len(cameras)} 개의 카메라 정보를 로드했습니다.")
            
        except Exception as e:
            logger.error(f"카메라 정보 로드 오류: {e}")
            import traceback
            logger.error(f"상세 오류 정보: {traceback.format_exc()}")
            
            # 데이터베이스 연결 실패시 빈 캐시 유지 (하드코딩된 URL 제거)
            self._url_cache = {}
            logger.warning("데이터베이스 연결 실패. 카메라 정보가 로드되지 않았습니다.")
    
    def _refresh_camera_cache_if_needed(self):
        """캐시 갱신이 필요한 경우 DB에서 카메라 정보 다시 로드"""
        current_time = time.time()
        
        # 마지막 DB 체크 이후 지정된 시간이 지났으면 다시 로드
        if current_time - self._last_db_check > self._db_refresh_interval:
            logger.info("캐시 만료로 카메라 정보 리로드")
            self._load_cameras_from_db()
            
    def force_refresh(self):
        """카메라 URL 캐시를 강제로 갱신합니다."""
        logger.info("카메라 URL 캐시를 강제로 갱신합니다.")
        self._url_cache.clear()  # 캐시 완전히 비우기
        self._last_db_check = 0  # 다음 호출 시 즉시 DB에서 다시 로드하도록 설정
        self._load_cameras_from_db()  # 즉시 DB에서 다시 로드
        return True
            
    def get_camera_url(self, camera_number: Union[int, str]) -> Optional[str]:
        """카메라 URL을 가져옵니다."""
        # DB 캐시 갱신 필요 여부 확인
        self._refresh_camera_cache_if_needed()
        
        # 문자열로 들어온 경우 int로 변환 시도
        try:
            # 항상 문자열 키로 저장하기 위해 str로 변환
            camera_key = str(camera_number)
        except (ValueError, TypeError):
            camera_key = str(camera_number)  # 변환 실패시 문자열로 저장
        
        logger.info(f"카메라 URL 요청: {camera_key}")
            
        if camera_key not in self._url_cache:
            # 캐시에 없는 경우 DB에서 특정 카메라만 다시 로드 시도
            try:
                from ..models import Camera
                # 모든 상태의 카메라를 확인
                camera = Camera.objects.filter(camera_id=camera_key).first()
                
                if camera:
                    self._url_cache[camera_key] = camera.rtsp_address
                    logger.info(f"DB에서 직접 조회: 카메라 {camera_key} = {camera.rtsp_address}")
                else:
                    # 로그 추가
                    logger.warning(f"카메라 {camera_key} URL을 찾을 수 없습니다.")
                    return None
                    
            except Exception as e:
                logger.error(f"카메라 {camera_key} 정보 조회 오류: {e}")
                import traceback
                logger.error(traceback.format_exc())
                return None
        else:
            # 캐시에 있더라도 최신 정보 확인 (URL이 변경되었을 가능성이 있음)
            try:
                from ..models import Camera
                camera = Camera.objects.filter(camera_id=camera_key).first()
                
                if camera and camera.rtsp_address != self._url_cache[camera_key]:
                    # DB의 URL과 캐시된 URL이 다른 경우 업데이트
                    logger.info(f"카메라 {camera_key}의 URL이 변경되었습니다: {self._url_cache[camera_key]} -> {camera.rtsp_address}")
                    self._url_cache[camera_key] = camera.rtsp_address
            except Exception as e:
                # 오류 발생 시 무시하고 기존 캐시 사용
                logger.warning(f"카메라 {camera_key} 최신 정보 확인 중 오류: {e}")
            
        # 마지막 접근 시간 업데이트
        self._last_access[camera_key] = time.time()
        
        # 캐시된 URL 반환하면서 로깅
        url = self._url_cache.get(camera_key)
        logger.info(f"반환된 카메라 URL: {camera_key} = {url}")
        return url
    
    def get_stream_options(self) -> Dict[str, str]:
        """10FPS로 최적화된 RTSP 스트림 옵션"""
        return {
            'rtsp_transport': 'tcp',       # TCP 사용 (더 안정적)
            'buffer_size': '1048576',      # 버퍼 크기 1MB로 축소
            'fflags': 'nobuffer',          # 버퍼링 사용하지 않음 (실시간성 확보)
            'flags': 'low_delay',          # 낮은 지연 설정
            'max_delay': '500000',         # 최대 지연 500ms로 제한
            'framerate': '10'              # 10fps로 고정
        }
    
    def create_stream_url(self, rtsp_url: str) -> str:
        """추가 옵션을 제거한 기본 URL 반환"""
        # 파라미터를 보존하여 전체 URL 그대로 사용
        return rtsp_url
    
    def get_optimized_capture(self, camera_number: Union[int, str]) -> Optional[cv2.VideoCapture]:
        """최적화된 OpenCV 캡처 객체 생성 - 10fps 고정"""
        # 카메라 URL 가져오기
        rtsp_url = self.get_camera_url(camera_number)
        if not rtsp_url:
            return None
            
        # 향상된 VideoCapture 생성
        try:
            # FFMPEG 옵션 문자열 설정 - 10fps 포함
            options = self.get_stream_options()
            options_str = ' '.join([f"{k};{v}" for k, v in options.items()])
            os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = options_str
            
            # 로그 추가
            logger.info(f"카메라 {camera_number} 연결 시도: {rtsp_url}")
            logger.debug(f"FFMPEG 옵션: {options_str}")
            
            # 외부 경로인지 확인 (절대 경로 또는 다른 드라이브 경로일 경우)
            is_external_path = (
                os.path.isabs(rtsp_url) and os.path.exists(rtsp_url) and 
                not rtsp_url.startswith(os.path.join(settings.BASE_DIR, 'video_uploads'))
            )
            
            # 캡처 객체 생성
            if is_external_path:
                logger.info(f"외부 비디오 파일 사용: {rtsp_url}")
                # 외부 파일 경로인 경우 직접 파일 경로 사용
                cap = cv2.VideoCapture(rtsp_url)
            else:
                # 기본 URL로 변환
                rtsp_url = self.create_stream_url(rtsp_url)
                # RTSP 주소나 내부 업로드 파일인 경우
                cap = cv2.VideoCapture(rtsp_url, cv2.CAP_FFMPEG)
            
            # 연결 확인
            if cap.isOpened():
                # 성능 최적화 설정
                cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)     # 버퍼 크기 최소화 (실시간성 확보)
                # FPS 설정: 10fps로 고정
                cap.set(cv2.CAP_PROP_FPS, 10)
                
                # 정상적으로 프레임을 읽을 수 있는지 확인
                ret, test_frame = cap.read()
                if not ret or test_frame is None:
                    logger.warning(f"카메라 {camera_number} 연결됐지만 프레임을 읽을 수 없음")
                    cap.release()
                    return None
                
                logger.info(f"카메라 {camera_number} 연결 성공")
                return cap
            else:
                logger.warning(f"카메라 {camera_number} 연결 실패")
                return None
                
        except Exception as e:
            logger.error(f"카메라 {camera_number} 초기화 중 오류: {e}")
            return None