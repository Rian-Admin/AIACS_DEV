import os
import cv2
import time
import logging
import threading
from typing import Dict, Optional, Union, Any
from django.core.cache import cache
from django.conf import settings

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

class CameraManager:
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
        # 이미 인스턴스가 있으면 초기화 중단
        if hasattr(CameraManager, '_instance') and CameraManager._instance is not None:
            return
            
        # RTSP 전송 프로토콜 설정 - 최소 옵션만 사용
        os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"
        
        # 카메라 URL 캐시
        self._url_cache = {}
        self._cache_timeout = 1  # 캐시 만료 시간 (초) - 1초로 단축
        self._last_db_check = 0
        self._db_refresh_interval = 5  # DB 체크 간격 (초) - 1초로 더 단축
        
        # Django의 Camera 모델에서 카메라 정보 초기 로드
        self._load_cameras_from_db()
        
        self._last_access = {}
        self._optimized_urls = {}
        
        # 디버깅용 로깅 추가
        logger.info(f"카메라 매니저 초기화 완료, 캐시 상태: {self._url_cache}")
        
        # 성능 및 오류 관련 설정
        self.connection_timeout = 15  # 연결 타임아웃 시간 (초) 증가
        self.reconnect_attempts = 5   # 재연결 시도 횟수 증가
        
        # 카메라 연결 상태 추적
        self._camera_status = {}  # 카메라 ID를 키로, 상태를 값으로 저장
        
        # 시스템 시작 시 모든 카메라 초기화
        self._initialize_all_cameras()
        
        # 카메라 모니터링 스레드 시작
        self._monitor_stop_event = threading.Event()
        self._monitor_thread = threading.Thread(target=self._monitor_cameras, daemon=True)
        self._monitor_thread.start()
        logger.info("카메라 모니터링 스레드가 시작되었습니다.")
    
    def _initialize_all_cameras(self):
        """시스템 시작 시 모든 카메라를 강제로 초기화합니다."""
        try:
            logger.info("모든 카메라 초기화 시작...")
            
            # 캐시된 모든 카메라 URL 확인
            for camera_id, url in self._url_cache.items():
                if not url:
                    continue
                    
                logger.info(f"카메라 {camera_id} 초기화 시도 중...")
                # 각 카메라 연결 테스트
                cap = self.get_optimized_capture(camera_id)
                if cap:
                    # 성공적으로 연결된 경우
                    ret, frame = cap.read()
                    if ret and frame is not None:
                        logger.info(f"카메라 {camera_id} 초기화 성공")
                        self._camera_status[camera_id] = "online"
                    else:
                        logger.warning(f"카메라 {camera_id} 연결됐으나 프레임을 읽을 수 없음")
                        self._camera_status[camera_id] = "error"
                    cap.release()
                else:
                    logger.warning(f"카메라 {camera_id} 초기화 실패")
                    self._camera_status[camera_id] = "offline"
            
            logger.info("모든 카메라 초기화 완료")
            
        except Exception as e:
            logger.error(f"카메라 초기화 중 오류 발생: {e}")
    
    def _monitor_cameras(self):
        """주기적으로 모든 등록된 카메라의 연결 상태를 확인하고 필요시 재연결하는 모니터링 스레드"""
        check_interval = 30  # 30초마다 모든 카메라 상태 확인
        
        while not self._monitor_stop_event.is_set():
            try:
                # 현재 등록된 모든 카메라 확인
                logger.info("카메라 상태 모니터링 중...")
                self._refresh_camera_cache_if_needed()  # 최신 카메라 정보 유지
                
                for camera_id in list(self._url_cache.keys()):
                    # 모니터링 도중 멈춤 요청이 있으면 빠져나감
                    if self._monitor_stop_event.is_set():
                        break
                    
                    camera_id_str = str(camera_id)  # 문자열 형태로 통일
                    
                    try:
                        # 간단한 연결 테스트
                        url = self.get_camera_url(camera_id)
                        if not url:
                            logger.warning(f"카메라 {camera_id} URL을 찾을 수 없음")
                            self._camera_status[camera_id_str] = "offline"
                            continue
                        
                        # OpenCV로 빠른 연결 테스트
                        cap = cv2.VideoCapture(url, cv2.CAP_FFMPEG)
                        if not cap.isOpened():
                            logger.warning(f"카메라 {camera_id} 연결 실패, 상태: offline")
                            self._camera_status[camera_id_str] = "offline"
                            # 실패한 경우 URL 캐시에서 제거하여 다음 요청 시 DB에서 다시 로드하도록 함
                            if camera_id_str in self._url_cache:
                                del self._url_cache[camera_id_str]
                        else:
                            # 프레임을 읽을 수 있는지 확인
                            ret, frame = cap.read()
                            if ret and frame is not None:
                                logger.info(f"카메라 {camera_id} 연결 상태: online")
                                self._camera_status[camera_id_str] = "online"
                            else:
                                logger.warning(f"카메라 {camera_id} 프레임을 읽을 수 없음, 상태: error")
                                self._camera_status[camera_id_str] = "error"
                                # URL 캐시 갱신
                                if camera_id_str in self._url_cache:
                                    del self._url_cache[camera_id_str]
                        
                        # 항상 자원 해제
                        cap.release()
                        
                    except Exception as e:
                        logger.error(f"카메라 {camera_id} 모니터링 중 오류: {e}")
                        self._camera_status[camera_id_str] = "error"
                
                # 모니터링 결과 로깅
                logger.info(f"카메라 상태 모니터링 결과: {self._camera_status}")
                
            except Exception as e:
                logger.error(f"카메라 모니터링 스레드 오류: {e}")
            
            # 다음 점검까지 대기
            for _ in range(check_interval):
                if self._monitor_stop_event.is_set():
                    break
                time.sleep(1)
    
    def get_camera_status(self, camera_id: Union[int, str]) -> str:
        """카메라의 현재 연결 상태를 반환합니다."""
        camera_id_str = str(camera_id)
        return self._camera_status.get(camera_id_str, "unknown")
    
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
                    camera_id = str(camera.camera_id)  # 항상 문자열로 저장
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
            logger.info(f"현재 캐시 상태: {self._url_cache}")
            
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
        
        # 모든 카메라 초기화 재시도
        self._initialize_all_cameras()
        
        return True
    
    def force_reconnect(self, camera_id: Union[int, str]) -> bool:
        """특정 카메라의 연결을 강제로 재시도합니다."""
        try:
            camera_id_str = str(camera_id)
            logger.info(f"카메라 {camera_id_str} 강제 재연결 시도...")
            
            # URL 캐시에서 제거하여 DB에서 다시 로드하도록 함
            if camera_id_str in self._url_cache:
                del self._url_cache[camera_id_str]
                
            # 카메라 URL 다시 가져오기
            url = self.get_camera_url(camera_id_str)
            if not url:
                logger.warning(f"카메라 {camera_id_str} URL을 찾을 수 없어 재연결 실패")
                self._camera_status[camera_id_str] = "offline"
                return False
                
            # 연결 테스트
            cap = self.get_optimized_capture(camera_id_str)
            if cap:
                ret, frame = cap.read()
                cap.release()
                
                if ret and frame is not None:
                    logger.info(f"카메라 {camera_id_str} 재연결 성공")
                    self._camera_status[camera_id_str] = "online"
                    return True
                else:
                    logger.warning(f"카메라 {camera_id_str} 재연결됐으나 프레임을 읽을 수 없음")
                    self._camera_status[camera_id_str] = "error"
                    return False
            else:
                logger.warning(f"카메라 {camera_id_str} 재연결 실패")
                self._camera_status[camera_id_str] = "offline"
                return False
                
        except Exception as e:
            logger.error(f"카메라 {camera_id} 재연결 중 오류: {e}")
            return False
            
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
        
        logger.info(f"카메라 URL 요청: {camera_key}, 현재 캐시: {self._url_cache}")
            
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
        """단순화된 RTSP 스트림 옵션"""
        return {
            # 최소한의 필수 옵션만 유지
            'rtsp_transport': 'udp',       # UDP 사용 (더 빠름)
            'buffer_size': '4048576',      # 버퍼 크기 1MB (기본값)
            'fflags': 'nobuffer',          # 버퍼링 사용하지 않음 (실시간성 확보)
            'flags': 'low_delay',          # 낮은 지연 설정
        }
    
    def create_stream_url(self, rtsp_url: str) -> str:
        """추가 옵션을 제거한 기본 URL 반환"""
        return rtsp_url  # 전체 URL 그대로 사용
    
    def get_optimized_capture(self, camera_number: Union[int, str]) -> Optional[cv2.VideoCapture]:
        """최적화된 OpenCV 캡처 객체 생성"""
        # 카메라 URL 가져오기
        rtsp_url = self.get_camera_url(camera_number)
        if not rtsp_url:
            return None
            
        # 향상된 VideoCapture 생성
        try:
            # FFMPEG 옵션 문자열 설정
            options_str = ' '.join([f"{k};{v}" for k, v in self.get_stream_options().items()])
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
                # FPS 제한 제거 (최대 프레임 확보)
                cap.set(cv2.CAP_PROP_FPS, 10)            # 0으로 설정하여 제한 없음
                
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

    def __del__(self):
        """객체 소멸 시 자원 정리"""
        try:
            if hasattr(self, '_monitor_stop_event'):
                self._monitor_stop_event.set()
                
            if hasattr(self, '_monitor_thread') and self._monitor_thread.is_alive():
                self._monitor_thread.join(timeout=2.0)
                
            logger.info("카메라 매니저 자원 정리 완료")
        except Exception as e:
            logger.error(f"카메라 매니저 정리 중 오류: {e}")