import os
import cv2
import time
import logging
import threading
from typing import Dict, Optional, Union, Any
from django.core.cache import cache
from django.conf import settings

# 로그 설정
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

class CameraManager:
    # 싱글톤 인스턴스
    _instance = None
    _instance_lock = threading.Lock()
    
    # 카메라 초기화를 위한 락 딕셔너리
    _camera_locks = {}
    # 초기화 상태 추적
    _initialization_status = {}
    # 전역 초기화 락
    _global_init_lock = threading.Lock()
    # 현재 초기화 중인 카메라 ID 목록
    _cameras_initializing = set()
    
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
        os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;tcp"
        
        # 카메라 URL 캐시
        self._url_cache = {}
        self._cache_timeout = 3  # 캐시 만료 시간 (초) - 3초로 설정
        self._last_db_check = 0
        self._db_refresh_interval = 10  # DB 체크 간격 (초) - 10초로 설정
        
        # Django의 Camera 모델에서 카메라 정보 초기 로드
        self._load_cameras_from_db()
        
        self._last_access = {}
        self._optimized_urls = {}
        
        # 디버깅용 로깅 추가
        logger.info(f"카메라 매니저 초기화 완료, 캐시 상태: {self._url_cache}")
        
        # 성능 및 오류 관련 설정
        self.connection_timeout = 15  # 연결 타임아웃 시간 (초)
        self.reconnect_attempts = 5   # 재연결 시도 횟수
        
        # 카메라 연결 상태 추적
        self._camera_status = {}  # 카메라 ID를 키로, 상태를 값으로 저장
        
        # 시스템 시작 시 모든 카메라 초기화 (스레드로 실행)
        threading.Thread(target=self._initialize_all_cameras_thread, daemon=True).start()
        
        # 카메라 모니터링 스레드 시작
        self._monitor_stop_event = threading.Event()
        self._monitor_thread = threading.Thread(target=self._monitor_cameras, daemon=True)
        self._monitor_thread.start()
        logger.info("카메라 모니터링 스레드가 시작되었습니다.")
    
    def _initialize_all_cameras_thread(self):
        """별도 스레드에서 모든 카메라를 초기화합니다."""
        time.sleep(2)  # 시스템 시작 후 잠시 대기
        self._initialize_all_cameras()
    
    def _initialize_all_cameras(self):
        """시스템 시작 시 모든 카메라를 순차적으로 초기화합니다."""
        # 이미 초기화 중인지 확인
        with self._global_init_lock:
            if self._cameras_initializing:
                logger.info(f"이미 카메라 초기화가 진행 중입니다: {self._cameras_initializing}")
                return
            
            logger.info("모든 카메라 초기화 시작...")
            
            # 카메라 목록 가져오기
            camera_ids = list(self._url_cache.keys())
            
            for camera_id in camera_ids:
                # 이미 성공적으로 초기화되었는지 확인
                if self._camera_status.get(camera_id, "") == "online":
                    logger.info(f"카메라 {camera_id}는 이미 초기화되어 있습니다.")
                    continue
                
                # 하나씩 순차적으로 초기화 (병렬 초기화 방지)
                self._initialize_camera(camera_id)
                
                # 카메라 간 초기화 딜레이 (과부하 방지)
                time.sleep(1.0)
            
            logger.info("모든 카메라 초기화 완료")
    
    def _initialize_camera(self, camera_id):
        """단일 카메라를 초기화하는 메서드"""
        # 카메라별 락 생성
        if camera_id not in self._camera_locks:
            with self._global_init_lock:
                if camera_id not in self._camera_locks:
                    self._camera_locks[camera_id] = threading.Lock()
        
        # 이미 초기화 중인지 확인
        with self._global_init_lock:
            if camera_id in self._cameras_initializing:
                logger.info(f"카메라 {camera_id}는 이미 초기화 중입니다.")
                return False
            
            # 초기화 시작 표시
            self._cameras_initializing.add(camera_id)
        
        try:
            # URL 가져오기
            url = self._url_cache.get(camera_id)
            if not url:
                logger.warning(f"카메라 {camera_id}의 URL을 찾을 수 없습니다.")
                return False
            
            logger.info(f"카메라 {camera_id} 초기화 중...")
            
            if url and "?" in url:
                logger.info(f"카메라 {camera_id}: 쿼리 파라미터 포함된 RTSP URL 사용 - {url}")
            
            logger.info(f"카메라 {camera_id}: RTSP 스트림 초기화 - {url}")
            
            # 카메라 연결 테스트
            cap = self.get_optimized_capture(camera_id)
            success = False
            
            if cap:
                # 성공적으로 연결된 경우 프레임 읽기 시도
                ret, frame = cap.read()
                if ret and frame is not None:
                    success = True
                    self._camera_status[camera_id] = "online"
                    logger.info(f"카메라 {camera_id} 초기화 완료: {url}")
                    logger.info(f"카메라 {camera_id} 초기화 성공")
                else:
                    self._camera_status[camera_id] = "error"
                    logger.warning(f"카메라 {camera_id} 연결됐으나 프레임을 읽을 수 없음")
                
                # 리소스 해제
                cap.release()
            else:
                self._camera_status[camera_id] = "offline"
                logger.warning(f"카메라 {camera_id} 초기화 실패")
            
            return success
            
        except Exception as e:
            logger.error(f"카메라 {camera_id} 초기화 중 오류 발생: {e}")
            self._camera_status[camera_id] = "error"
            return False
        finally:
            # 초기화 완료 표시
            with self._global_init_lock:
                if camera_id in self._cameras_initializing:
                    self._cameras_initializing.remove(camera_id)
    
    def _monitor_cameras(self):
        """카메라 상태를 주기적으로 모니터링하는 스레드"""
        check_interval = 60  # 1분마다 체크 (너무 자주 체크하지 않도록)
        
        while not self._monitor_stop_event.is_set():
            time.sleep(check_interval)
            
            try:
                # 특별한 모니터링 작업이 필요한 경우 여기에 구현
                pass
            except Exception as e:
                logger.error(f"카메라 모니터링 중 오류: {e}")
    
    def _load_cameras_from_db(self):
        """데이터베이스에서 카메라 정보 로드"""
        try:
            # Django 모델에서 카메라 정보 가져오기
            from ..models import Camera
            
            # 모든 카메라 가져오기 (상태와 관계없이)
            cameras = Camera.objects.all()
            
            # 이전 캐시 보존 (완전히 비우지 않음)
            previous_cache = self._url_cache.copy()
            
            # URL 캐시 업데이트
            loaded_count = 0
            new_url_cache = {}
            
            for camera in cameras:
                try:
                    camera_id = str(camera.camera_id)  # 항상 문자열로 저장
                    rtsp_address = camera.rtsp_address
                    new_url_cache[camera_id] = rtsp_address
                    
                    # 기존에 없거나 변경된 경우만 로그 출력
                    if camera_id not in previous_cache or previous_cache[camera_id] != rtsp_address:
                        logger.info(f"카메라 {camera_id} 캐싱 (새로 추가 또는 변경): {rtsp_address}")
                    
                    loaded_count += 1
                except UnicodeDecodeError as ude:
                    logger.error(f"카메라 객체 처리 중 인코딩 오류: {ude}")
                    continue
                except Exception as ex:
                    logger.error(f"카메라 객체 처리 중 오류: {ex}")
                    continue
            
            # 새 캐시로 업데이트
            self._url_cache = new_url_cache
            
            # 마지막 DB 체크 시간 업데이트
            self._last_db_check = time.time()
            
            logger.info(f"데이터베이스에서 {loaded_count}/{len(cameras)} 개의 카메라 정보를 로드했습니다.")
            
            # 변경된 URL이 있으면 해당 카메라만 재초기화
            for camera_id, url in self._url_cache.items():
                if camera_id not in previous_cache or previous_cache[camera_id] != url:
                    # 이미 초기화 중이 아닌 경우에만 별도 스레드로 초기화
                    if camera_id not in self._cameras_initializing:
                        logger.info(f"카메라 {camera_id}의 URL이 변경되어 별도 스레드에서 재초기화합니다.")
                        threading.Thread(
                            target=self._initialize_camera, 
                            args=(camera_id,),
                            daemon=True
                        ).start()
            
        except Exception as e:
            logger.error(f"카메라 정보 로드 오류: {e}")
            import traceback
            logger.error(f"상세 오류 정보: {traceback.format_exc()}")
    
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
        self._last_db_check = 0  # 다음 호출 시 즉시 DB에서 다시 로드하도록 설정
        self._load_cameras_from_db()  # 즉시 DB에서 다시 로드
        
        return True
    
    def force_reconnect(self, camera_id):
        """특정 카메라에 대한 연결을 강제로 재시도합니다."""
        camera_id_str = str(camera_id)
        logger.info(f"카메라 {camera_id_str} 강제 재연결 시도")
        
        # 카메라 상태 초기화
        if camera_id_str in self._camera_status:
            self._camera_status[camera_id_str] = "reconnecting"
        
        # 해당 카메라가 초기화 중인 경우 대기
        if camera_id_str in self._cameras_initializing:
            logger.info(f"카메라 {camera_id_str}는 이미 초기화 중입니다. 완료될 때까지 대기합니다.")
            wait_count = 0
            while camera_id_str in self._cameras_initializing and wait_count < 6:
                time.sleep(5)  # 5초 간격으로 대기
                wait_count += 1
                logger.info(f"카메라 {camera_id_str} 초기화 대기 중... ({wait_count}/6)")
        
        # 강제 재연결 시도 (초기화 메서드 활용)
        success = self._initialize_camera(camera_id_str)
        logger.info(f"카메라 {camera_id_str} 강제 재연결 결과: {'성공' if success else '실패'}")
        
        return success
    
    def get_camera_url(self, camera_id):
        """카메라 ID에 해당하는 RTSP URL 반환"""
        # 항상 문자열 키로 사용
        camera_id_str = str(camera_id)
        
        # 필요한 경우 캐시 갱신
        self._refresh_camera_cache_if_needed()
        
        # 마지막 접근 시간 업데이트
        self._last_access[camera_id_str] = time.time()
        
        # URL 반환
        return self._url_cache.get(camera_id_str)
    
    def get_stream_options(self) -> Dict[str, str]:
        """RTSP 스트림 연결에 필요한 옵션 설정"""
        return {
            'rtsp_transport': 'tcp',       # TCP 사용 (더 안정적)
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