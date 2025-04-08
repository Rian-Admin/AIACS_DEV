import cv2
import threading
import time
import numpy as np
import os
from collections import deque
from ..yolo.detector import ObjectDetector

class StreamHandler:
    def __init__(self, camera_number, url):
        self.camera_number = camera_number
        self.url = url
        self.cap = None
        self.is_running = False
        self.thread = None
        
        # 버퍼 크기 설정
        self.frame_buffer = deque(maxlen=4)
        
        # 감지 결과 버퍼 추가
        self.detection_buffer = deque(maxlen=3)
        
        # 객체 감지 FPS 계산용 변수
        self.detection_fps = 0
        self.detection_count = 0
        self.detection_start_time = time.time()
        
        # 연결 재시도 설정
        self.max_retries = 10
        self.current_retry = 0
        self.retry_delay = 3
        
        # 비디오 파일 여부 확인
        self.is_video_file = self._is_video_file(url)
        self.is_rtsp = url.lower().startswith('rtsp://')
        self.video_fps = 10
        self.last_frame_time = 0
        
        # 프레임 안정성 관련 변수
        self.awaiting_iframe = False
        self.valid_frame_count = 0
        self.frame_health = 100
        
        # RTSP 스트림인 경우 저지연 모드를 위한 환경 변수 설정
        if self.is_rtsp:
            # TCP 사용 및 타임아웃 증가
            os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;tcp|timeout;120000000|stimeout;120000000"
            if '?' in url:
                print(f"카메라 {camera_number}: 쿼리 파라미터 포함된 RTSP URL 사용 - {url}")
        
        # 객체 감지기 가져오기
        self.detector = ObjectDetector.get_instance()
        
        # 객체 감지 스케줄링 관련 변수
        self.last_detection_time = 0
        self.detection_interval = 1
        
        # 마지막 감지 결과
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
        """비디오 캡처 초기화"""
        try:
            self.current_retry += 1
            
            # 이전 캡처 객체가 있다면 정리
            if self.cap is not None:
                self.cap.release()
                self.cap = None
                time.sleep(0.5)
            
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
                        self.video_fps = 30
                
                # RTSP 스트림인 경우 최적화 설정
                if self.is_rtsp:
                    # 버퍼 크기 설정
                    self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
                    # H264 코덱 명시적 지정
                    self.cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'H264'))
                else:
                    # 비RTSP 스트림/파일에 대한 설정
                    self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
                    try:
                        self.cap.set(cv2.CAP_PROP_HW_ACCELERATION, 1)
                    except:
                        pass
                
                self.current_retry = 0  # 성공 시 재시도 카운터 리셋
                print(f"카메라 {self.camera_number} 초기화 완료: {self.url}")
                return True
            else:
                print(f"카메라 {self.camera_number} 연결 실패: {self.url}")
                return False
                
        except Exception as e:
            print(f"카메라 초기화 오류: {e}")
            return False
    
    def _is_frame_valid(self, frame):
        """프레임이 유효한지 검사 - 최소한의 검사만 수행"""
        # 기본적인 NULL 및 빈 프레임 검사만 수행
        if frame is None or frame.size == 0:
            return False
        return True
    
    def _capture_frames(self):
        """프레임 캡처 스레드 - 성능 최적화"""
        empty_frame_count = 0
        last_processed_time = time.time()
        frame_target_duration = 1.0 / 10.0  # 10 FPS
        
        while self.is_running:
            try:
                # 캡처 상태 확인
                if self.cap is None or not self.cap.isOpened():
                    if self.current_retry < self.max_retries:
                        time.sleep(self.retry_delay)
                        self.init_capture()
                        continue
                    else:
                        # 빈 프레임 추가
                        self._add_empty_frame_to_buffer("연결 실패")
                        time.sleep(0.5)
                        continue
                
                # 프레임 캡처
                ret, frame = self.cap.read()
                
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
                        self.cap.release()
                        self.cap = None
                        empty_frame_count = 0
                        continue
                    
                    # 빈 프레임 추가
                    self._add_empty_frame_to_buffer("프레임 없음")
                    continue
                
                # 정상 프레임 처리
                empty_frame_count = 0
                
                # 현재 시간 확인
                current_time = time.time()
                
                # 10FPS로 제한 (스로우 모션 방지)
                time_since_last_frame = current_time - last_processed_time
                
                # 이 부분이 중요함: 10FPS 이하로만 처리
                if time_since_last_frame >= frame_target_duration:
                    # 버퍼에 프레임 추가 (순환 버퍼이므로 자동으로 오래된 것 제거)
                    self.frame_buffer.append(frame.copy())
                    last_processed_time = current_time
                    
                    # 객체 감지 요청 (일정 간격으로만)
                    if current_time - self.last_detection_time >= self.detection_interval:
                        # 우선순위 설정 (카메라 번호 기반)
                        priority = min(10, max(1, self.camera_number))
                        
                        # 비동기 감지 요청
                        self.detector.detect_objects_async(
                            self.camera_number, 
                            frame.copy(),
                            priority=priority
                        )
                        self.last_detection_time = current_time
                        
                        # 감지 카운터 증가
                        self.detection_count += 1
                        detection_elapsed = current_time - self.detection_start_time
                        if detection_elapsed >= 1.0:
                            self.detection_fps = self.detection_count / detection_elapsed
                            self.detection_count = 0
                            self.detection_start_time = current_time
                else:
                    # 너무 빨리 오는 프레임은 건너뛰기
                    pass
                
                # CPU 사용량을 낮추기 위한 최소 대기
                # 프레임 간 간격이 타겟보다 작으면 나머지 시간만큼 대기
                remaining_time = frame_target_duration - time_since_last_frame
                if remaining_time > 0:
                    time.sleep(remaining_time / 2)  # 정확한 타이밍을 위해 절반만 대기
                
            except Exception as e:
                print(f"프레임 캡처 중 오류: {e}")
                time.sleep(0.1)  # 오류 발생 시 짧게 대기
    
    def _add_empty_frame_to_buffer(self, message):
        """빈 프레임을 버퍼에 추가"""
        # 검은색 프레임 생성
        empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
        
        # 메시지 추가
        font = cv2.FONT_HERSHEY_COMPLEX
        text = f"camera {self.camera_number}: {message}"
        cv2.putText(empty_frame, text, (120, 240), font, 1, (255, 255, 255), 2) 
        # 버퍼에 추가
        self.frame_buffer.append(empty_frame)
    
    def get_frame_with_detections(self):
        """감지 결과가 적용된 최신 프레임 가져오기"""
        try:
            # 최신 프레임 가져오기
            frame = self.get_frame_from_buffer()
            if frame is None:
                return None, None
            
            # 최신 감지 결과 가져오기 (비동기 처리 결과)
            result = self.detector.get_latest_result(self.camera_number)
            
            # 결과 저장 (UI 외부에서 사용할 수 있도록)
            if result is not None:
                self.last_detection_result = result
            
            return frame, result
        
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
                
            # 가장 최근 프레임 반환 (복사 없이)
            return self.frame_buffer[-1]
            
        except Exception as e:
            return None
    
    def get_latest_detection(self):
        """최신 감지 결과 반환"""
        return self.last_detection_result
    
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