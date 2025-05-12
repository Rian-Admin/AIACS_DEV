import cv2
import threading
import time
import numpy as np
import os  # 환경 변수 설정을 위해 추가
from collections import deque
from ..yolo.detector import ObjectDetector

class StreamHandler:
    def __init__(self, camera_number, url):
        self.camera_number = camera_number
        self.url = url
        self.cap = None
        self.is_running = False
        self.thread = None
        
        # 버퍼 크기 축소
        self.frame_buffer = deque(maxlen=2)  # 최대 2개 프레임만 유지 (지연 더 감소)
        
        # 연결 재시도 설정 - 강화된 재연결 시스템
        self.max_retries = 20  # 재시도 횟수 증가
        self.current_retry = 0
        self.retry_delay = 3  # 재시도 간격 3초
        self.consecutive_failures = 0  # 연속 실패 횟수 추적
        self.last_reconnect_time = 0  # 마지막 재연결 시도 시간
        self.reconnect_cool_down = 10  # 연결 성공 후 재시도 쿨다운 시간 (초)
        self.connection_state = "disconnected"  # 연결 상태 추적
        
        # 비디오 파일 여부 확인
        self.is_video_file = self._is_video_file(url)
        self.is_rtsp = url.lower().startswith('rtsp://')
        self.video_fps = 10  # 비디오 파일의 기본 FPS
        self.last_frame_time = 0  # 마지막 프레임 시간
        
        # RTSP 스트림인 경우 저지연 모드를 위한 환경 변수 설정
        if self.is_rtsp:
            # 간결한 RTSP 설정으로 변경
            os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"
            # URL에 쿼리 파라미터(chnid 등)가 있는지 확인하고 출력
            if '?' in url:
                print(f"카메라 {camera_number}: 쿼리 파라미터 포함된 RTSP URL 사용 - {url}")
        
        # 마지막 감지 결과 (동기식 모드로 전환하더라도 호환성을 위해 유지)
        self.last_detection_result = None
        
        # 초기화 즉시 스트림 시작
        self.start()
    
    def _is_video_file(self, url):
        """URL이 비디오 파일인지 확인"""
        video_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
        return any(url.lower().endswith(ext) for ext in video_extensions)
    
    def start(self):
        """스트림 캡처 시작"""
        if self.thread is not None and self.thread.is_alive():
            return
            
        self.init_capture()
        
        if self.cap is None or not self.cap.isOpened():
            return
            
        # 스레드 생성 및 시작
        self.is_running = True
        self.thread = threading.Thread(target=self._capture_frames, daemon=True)
        self.thread.start()
    
    def init_capture(self):
        """비디오 캡처 초기화 - 강화된 안정성"""
        try:
            # 카메라 매니저에서 최신 URL 확인
            try:
                from ..frame.camera_manager import CameraManager
                manager = CameraManager.get_instance()
                updated_url = manager.get_camera_url(self.camera_number)
                
                # URL이 변경된 경우 업데이트
                if updated_url and updated_url != self.url:
                    print(f"카메라 {self.camera_number}: URL 업데이트됨 - {self.url} -> {updated_url}")
                    self.url = updated_url
            except Exception as e:
                print(f"카메라 URL 업데이트 중 오류: {e}")
            
            self.current_retry += 1
            
            # 이전 캡처 객체가 있다면 정리
            if self.cap is not None:
                self.cap.release()
                self.cap = None
                time.sleep(0.5)  # 완전히 정리될 시간을 줌
            
            # 연속 실패 횟수에 따라 재시도 간격 조정 (지수 백오프)
            if self.consecutive_failures > 3:
                # 점진적으로 재시도 간격 증가 (최대 30초)
                cooldown = min(30, self.retry_delay * (2 ** (self.consecutive_failures - 3)))
                print(f"카메라 {self.camera_number}: 연속 {self.consecutive_failures}회 실패, 대기 시간 {cooldown}초")
                time.sleep(cooldown - 3)  # 기본 재시도 간격 3초 고려
            
            # RTSP 스트림인 경우 적절한 백엔드와 옵션 사용
            if self.is_rtsp:
                print(f"카메라 {self.camera_number}: RTSP 스트림 초기화 - {self.url}")
                # FFmpeg 백엔드 강제 사용
                self.cap = cv2.VideoCapture(self.url, cv2.CAP_FFMPEG)
            else:
                # 일반 캡처 초기화
                self.cap = cv2.VideoCapture(self.url)
            
            # 캡처 설정 최적화
            if self.cap and self.cap.isOpened():
                # 비디오 파일인 경우 FPS 가져오기
                if self.is_video_file:
                    self.video_fps = self.cap.get(cv2.CAP_PROP_FPS)
                    if self.video_fps <= 0:
                        self.video_fps = 10  # 기본값
                
                # RTSP 스트림인 경우 최적화 설정
                if self.is_rtsp:
                    # 버퍼 크기 최소화 (지연 감소)
                    self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 2)
                    # H264 코덱 명시적 지정
                    self.cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'H264'))
                    # FPS 설정
                    self.cap.set(cv2.CAP_PROP_FPS, 10)
                else:
                    # 비RTSP 스트림/파일에 대한 설정
                    self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 3)
                    try:
                        self.cap.set(cv2.CAP_PROP_HW_ACCELERATION, 1)  # 하드웨어 가속 활성화
                    except:
                        pass
                
                # 성공 시 재설정
                self.current_retry = 0
                self.consecutive_failures = 0
                self.last_reconnect_time = time.time()
                self.connection_state = "connected"
                
                # 카메라 매니저 상태 업데이트
                try:
                    from ..frame.camera_manager import CameraManager
                    manager = CameraManager.get_instance()
                    manager._camera_status[str(self.camera_number)] = "online"
                except Exception:
                    pass
                
                print(f"카메라 {self.camera_number} 초기화 완료: {self.url}")
                return True
            else:
                # 연결 실패 처리
                self.consecutive_failures += 1
                self.connection_state = "failed"
                
                # 카메라 매니저 상태 업데이트
                try:
                    from ..frame.camera_manager import CameraManager
                    manager = CameraManager.get_instance()
                    manager._camera_status[str(self.camera_number)] = "offline"
                except Exception:
                    pass
                
                print(f"카메라 {self.camera_number} 연결 실패: {self.url} (시도 {self.current_retry}/{self.max_retries})")
                return False
                
        except Exception as e:
            self.consecutive_failures += 1
            self.connection_state = "error"
            print(f"카메라 초기화 오류: {e}")
            return False
    
    def _is_frame_valid(self, frame):
        """프레임이 유효한지 검사 - 최소한의 검사만 수행"""
        # 기본적인 NULL 및 빈 프레임 검사만 수행
        if frame is None or frame.size == 0:
            return False
        return True
    
    def _capture_frames(self):
        """프레임 캡처 스레드 - 성능 최적화 및 강화된 재연결"""
        empty_frame_count = 0
        frame_interval = 0.1  # 10 FPS 제한 (1초 / 10 = 0.1초)
        last_frame_time = time.time()
        connection_monitor_time = time.time()  # 연결 모니터링 타이머
        connection_check_interval = 10  # 10초마다 연결 상태 확인
        
        while self.is_running:
            current_time = time.time()
            
            # 주기적으로 연결 상태 확인 및 로깅
            if current_time - connection_monitor_time >= connection_check_interval:
                connection_monitor_time = current_time
                print(f"카메라 {self.camera_number} 연결 상태: {self.connection_state}, 재시도: {self.current_retry}/{self.max_retries}, 실패: {self.consecutive_failures}")
            
            # 모든 소스에 대해 동일한 FPS 제한 적용 (비디오 파일 포함)
            if current_time - last_frame_time < frame_interval:
                time.sleep(0.001)  # 아주 짧은 대기
                continue
            
            # 캡처 상태 확인
            if self.cap is None or not self.cap.isOpened():
                if self.current_retry < self.max_retries:
                    # 재연결 시도 간격 확인 (쿨다운 적용)
                    if current_time - self.last_reconnect_time >= self.retry_delay:
                        print(f"카메라 {self.camera_number} 재연결 시도 중...")
                        self.init_capture()
                    else:
                        # 대기 중임을 나타내는 메시지 추가
                        self._add_empty_frame_to_buffer(f"재연결 대기 중... ({int(self.retry_delay - (current_time - self.last_reconnect_time))}초)")
                    time.sleep(0.5)
                    continue
                else:
                    # 최대 재시도 횟수 초과 시 오류 프레임 추가하고 재설정
                    self._add_empty_frame_to_buffer("최대 재시도 횟수 초과")
                    
                    # 재시도 카운터 리셋하여 다시 시작할 수 있게 함
                    if current_time - self.last_reconnect_time >= 60:  # 1분 후 다시 시도
                        self.current_retry = 0
                        self.last_reconnect_time = current_time
                        print(f"카메라 {self.camera_number} 재시도 카운터 리셋")
                    
                    time.sleep(1)
                    continue
            
            # 프레임 캡처
            ret, frame = self.cap.read()
            last_frame_time = time.time()
            
            # 비디오 파일이 끝났는지 확인
            if self.is_video_file and not ret:
                # 비디오 파일 재시작
                self.cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                continue
      
            if not ret or frame is None or frame.size == 0:
                empty_frame_count += 1
                
                # 연속으로 빈 프레임이 많이 오면 연결 재설정
                if empty_frame_count > 5:
                    print(f"카메라 {self.camera_number}: 빈 프레임이 너무 많음, 연결 재설정")
                    self.connection_state = "reconnecting"
                    self.cap.release()
                    self.cap = None
                    empty_frame_count = 0
                    
                    # 카메라 매니저에서 강제 재연결 시도
                    try:
                        from ..frame.camera_manager import CameraManager
                        manager = CameraManager.get_instance()
                        manager.force_reconnect(self.camera_number)
                    except Exception as e:
                        print(f"카메라 매니저 재연결 시도 중 오류: {e}")
                    
                    continue
                
                # 빈 프레임 추가
                self._add_empty_frame_to_buffer("프레임 없음")
                continue
            
            # 정상 프레임 처리 - 유효성 검사 간소화
            empty_frame_count = 0
            
            # 연결 상태 업데이트
            if self.connection_state != "connected":
                self.connection_state = "connected"
                print(f"카메라 {self.camera_number}: 스트림 복구됨")
            
            # 버퍼에 프레임 추가 (순환 버퍼이므로 자동으로 오래된 것 제거)
            self.frame_buffer.append(frame.copy())  # 깊은 복사로 프레임 보존
            
            # 짧은 대기 (CPU 사용량 제한)
            time.sleep(0.001)
    
    def _add_empty_frame_to_buffer(self, message):
        """빈 프레임을 버퍼에 추가"""
        # 검은색 프레임 생성
        empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
        
        # 메시지 추가
        font = cv2.FONT_HERSHEY_COMPLEX
        text = f"camera {self.camera_number}: {message}"
        cv2.putText(empty_frame, text, (120, 240), font, 1, (255, 255, 255), 2) 
        
        # 현재 시간 추가
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        cv2.putText(empty_frame, timestamp, (10, 20), font, 0.5, (255, 255, 255), 1)
        
        # 상태 정보 추가
        state_text = f"상태: {self.connection_state} | 시도: {self.current_retry}/{self.max_retries}"
        cv2.putText(empty_frame, state_text, (10, 460), font, 0.5, (255, 255, 255), 1)
        
        # 버퍼에 추가
        self.frame_buffer.append(empty_frame)
    
    def get_frame_with_detections(self):
        """이전 버전과의 호환성을 위해 유지됨 - 단순히 프레임만 반환"""
        try:
            # 최신 프레임 가져오기
            frame = self.get_frame_from_buffer()
            if frame is None:
                return None, None
            
            # 비동기 감지 없이 프레임만 반환
            return frame, None
        
        except Exception as e:
            import traceback
            print(f"프레임 처리 오류: {e}")
            print(traceback.format_exc())
            return None, None
    
    def get_frame_from_buffer(self):
        """버퍼에서 최신 프레임 가져오기"""
        try:
            if not self.frame_buffer:
                return None
                
            # 가장 최근 프레임 반환 (깊은 복사로 반환)
            return self.frame_buffer[-1].copy()
            
        except Exception as e:
            return None
    
    def get_latest_detection(self):
        """최신 감지 결과 반환"""
        return self.last_detection_result
    
    def get_connection_state(self):
        """현재 연결 상태 반환"""
        return {
            "state": self.connection_state,
            "retries": self.current_retry,
            "max_retries": self.max_retries,
            "consecutive_failures": self.consecutive_failures
        }
    
    def force_reconnect(self):
        """강제로 카메라 재연결"""
        try:
            print(f"카메라 {self.camera_number} 강제 재연결 시도...")
            
            # 기존 연결 정리
            if self.cap:
                self.cap.release()
                self.cap = None
            
            # 재시도 카운터 초기화
            self.current_retry = 0
            self.consecutive_failures = 0
            self.last_reconnect_time = 0  # 즉시 재연결 시도하도록
            self.connection_state = "reconnecting"
            
            # 카메라 매니저에서 강제 URL 갱신
            try:
                from ..frame.camera_manager import CameraManager
                manager = CameraManager.get_instance()
                manager.force_reconnect(self.camera_number)
            except Exception as e:
                print(f"카메라 매니저 재연결 시도 중 오류: {e}")
            
            # 초기화 재시도
            return self.init_capture()
            
        except Exception as e:
            print(f"강제 재연결 중 오류: {e}")
            return False
    
    def stop(self):
        """스트림 중지 및 자원 정리"""
        self.is_running = False
        
        if self.thread and self.thread.is_alive():
            self.thread.join(timeout=0.5)
        
        if self.cap is not None and hasattr(self.cap, 'release'):
            self.cap.release()
            self.cap = None
        
        # 버퍼 비우기
        self.frame_buffer.clear()