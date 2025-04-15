import cv2
import datetime
import time
import torch
import numpy as np
import threading
import logging
from django.utils import timezone

from ..yolo.detector import ObjectDetector
from ..stream.stream_handler import StreamHandler
from ..db import db_adapter
from ..hardware.bird_controller import BirdController
from ..utils.drawing import draw_detections

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

class VideoCamera:
    """카메라 스트림과 객체 감지 처리 - 성능 최적화"""
    
    def __init__(self, camera_number):
        # 문자열로 들어온 카메라 번호를 가능하면 숫자로 변환
        try:
            self.camera_number = int(camera_number)
        except (ValueError, TypeError):
            self.camera_number = camera_number
            
        self.stream_handler = None
        self._frame_count = 0
        self._last_results = None
        self._last_processed_result = None
        self._last_db_update = time.time()
        self._db_update_interval = 30  # DB 업데이트 간격 (초) - 30초로 늘림
        self._capture_timing_stats = []
        
        # 마지막 유효 프레임 저장 변수 초기화
        self._last_valid_frame = None
        self._last_frame_time = 0
        self._frame_cache_time = 0.005  # 캐시 시간 5ms로 단축 (거의 항상 새 프레임 사용)
        self._processing_lock = threading.Lock()

        try:
            from ..frame.camera_manager import CameraManager
            self.camera_manager = CameraManager.get_instance()
            self.rtsp_url = self.camera_manager.get_camera_url(camera_number)
            
            if not self.rtsp_url:
                logger.warning(f"카메라 {camera_number} 구성되지 않음")
                return
                
            # 스트림 URL 생성
            self.stream_url = self.camera_manager.create_stream_url(self.rtsp_url)
            
            # 객체 감지 모델 로드 (공유 인스턴스 사용)
            self.detector = ObjectDetector.get_instance() if 'ObjectDetector' in globals() else None
            
            # 스트림 핸들러 초기화
            self.stream_handler = StreamHandler(camera_number, self.stream_url)
            
            # DB 어댑터
            self.db = db_adapter
            
            # 조류퇴치기 컨트롤러 초기화
            self.bird_controller = BirdController()
            
            # 성능 모니터링
            self.stats = {
                'frames_processed': 0,
                'detection_count': 0,
                'db_updates': 0,
                'avg_process_time': 0,
                'total_process_time': 0,
                'last_fps': 0
            }
            
            # 결과 처리 및 DB 업데이트를 위한 스레드 시작
            self.is_running = True
            self.processing_thread = threading.Thread(target=self._process_results_thread, daemon=True)
            self.processing_thread.start()
            
            logger.info(f"카메라 {camera_number} 초기화 완료")
            
        except Exception as e:
            logger.error(f"카메라 {camera_number} 초기화 오류: {e}")
            # 예외 발생해도 진행
    
    def _process_results_thread(self):
        """감지 결과 처리 및 DB 업데이트를 위한 별도 스레드"""
        last_check_time = time.time()
        check_interval = 0.2  # 0.2초마다 새 결과 확인
        
        while self.is_running:
            try:
                current_time = time.time()
                
                # 주기적으로 감지 결과 확인
                if current_time - last_check_time >= check_interval:
                    last_check_time = current_time
                    
                    # 새 감지 결과 가져오기
                    if self.stream_handler:
                        result = self.detector.get_latest_result(self.camera_number)
                        
                        # 새 결과가 있으면 처리
                        if result is not None and result != self._last_processed_result:
                            self._last_results = result
                            self._last_processed_result = result
                            
                            # 잠금 획득
                            with self._processing_lock:
                                # DB 업데이트 및 조류퇴치기 제어 (주기적으로)
                                if current_time - self._last_db_update >= self._db_update_interval:
                                    if hasattr(result, 'boxes') and hasattr(result, 'orig_shape'):
                                        height, width = result.orig_shape
                                        self._process_detection_results(result.boxes, width, height)
                            
                            # 통계 업데이트
                            self.stats['detection_count'] += 1
                
                # 짧게 대기
                time.sleep(0.05)
                
            except Exception as e:
                logger.error(f"결과 처리 스레드 오류: {e}")
                time.sleep(1)  # 오류 발생 시 1초 대기
    
    def get_frame(self, higher_quality=False):
        """프레임 가져오기 - 최적화된 버전"""
        try:
            # URL 변경 여부 확인 및 필요시 스트림 핸들러 재초기화
            current_url = self.camera_manager.get_camera_url(self.camera_number)
            if current_url and current_url != self.rtsp_url:
                logger.info(f"카메라 {self.camera_number}의 URL이 변경되었습니다: {self.rtsp_url} -> {current_url}")
                self.rtsp_url = current_url
                # 기존 스트림 핸들러 정리
                if self.stream_handler:
                    self.stream_handler.stop()
                    # 스트림 핸들러가 완전히 정리될 시간 부여
                    time.sleep(0.2)
                # 새 URL로 재초기화 (stream_url도 원본 URL을 사용)
                self.stream_url = current_url  # self.camera_manager.create_stream_url() 사용하지 않음
                self.stream_handler = StreamHandler(self.camera_number, self.stream_url)
                # 마지막 유효 프레임 초기화
                self._last_valid_frame = None
                self._last_frame_time = 0
                logger.info(f"카메라 {self.camera_number} 스트림 핸들러 재초기화 완료")
            
            # 스트림 핸들러 확인
            if self.stream_handler is None:
                return self._create_empty_frame(f"카메라 {self.camera_number}: 초기화되지 않음")
            
            # 프로세스 시작 시간
            start_time = time.time()
            
            # 캐시된 프레임 사용 안함 - 항상 최신 프레임과 감지 결과 사용
            # 프레임 및 감지 결과 가져오기
            frame, latest_result = self.stream_handler.get_frame_with_detections()
            
            if frame is None:
                return self._create_empty_frame(f"카메라 {self.camera_number}: 프레임 없음")
            
            # 마지막 유효 프레임 저장 (디버깅을 위해)
            self._last_valid_frame = frame.copy()
            self._last_frame_time = time.time()
            
            # 바운딩 박스 그리기 - 최신 감지 결과 사용
            if latest_result and hasattr(latest_result, 'boxes') and hasattr(latest_result.boxes, '__len__') and len(latest_result.boxes) > 0:
                try:
                    frame = draw_detections(frame, latest_result, self.detector.model.names)
                except Exception as e:
                    logger.error(f"바운딩 박스 그리기 오류: {e}")
            
            # 감지 FPS 표시 (추가)
            if hasattr(self.stream_handler, 'detection_fps'):
                cv2.putText(
                    frame, 
                    f"Det FPS: {self.stream_handler.detection_fps:.1f}", 
                    (10, 30),
                    cv2.FONT_HERSHEY_COMPLEX, 
                    0.4, 
                    (0, 0, 0),
                    1
                )
            
            # 프레임 카운터 증가
            self._frame_count += 1
            
            # 처리 시간 측정
            process_time = time.time() - start_time
            self._capture_timing_stats.append(process_time)
            if len(self._capture_timing_stats) > 10:
                self._capture_timing_stats.pop(0)
            
            # 통계 업데이트
            self.stats['frames_processed'] += 1
            self.stats['total_process_time'] += process_time
            self.stats['avg_process_time'] = self.stats['total_process_time'] / max(1, self.stats['frames_processed'])
            
            # JPEG 인코딩 - higher_quality 변수에 따라 품질 조정
            # 썸네일(고품질) 또는 스트리밍(저품질) 여부에 따라 품질 조정
            quality = 92 if higher_quality else 80
            return self._encode_frame(frame, quality=quality)
            
        except Exception as e:
            logger.error(f"프레임 처리 중 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return self._create_empty_frame(f"오류: {str(e)}")
    
    def _encode_frame(self, frame, quality=80):
        """프레임을 JPEG 바이트로 인코딩합니다."""
        try:
            if frame is None:
                return self._create_empty_frame("프레임 데이터 없음")
            
            # JPEG 품질 설정 (낮은 품질 = 더 빠른 전송)
            _, jpeg = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, quality])
            return jpeg.tobytes()
        except Exception as e:
            logger.error(f"프레임 인코딩 오류: {e}")
            return self._create_empty_frame(f"인코딩 오류: {str(e)}")

    def _create_empty_frame(self, message="데이터 없음"):
        """오류 메시지가 포함된 빈 프레임을 생성합니다."""
        try:
            # 적절한 크기의 검은색 이미지 생성 (640x480)
            height, width = 480, 640
            empty_frame = np.zeros((height, width, 3), dtype=np.uint8)
            
            # 오류 메시지 표시
            font = cv2.FONT_HERSHEY_SIMPLEX
            font_scale = 0.7
            thickness = 2
            color = (255, 255, 255)  # 흰색 텍스트
            
            # 텍스트 크기 계산
            (text_width, text_height), _ = cv2.getTextSize(message, font, font_scale, thickness)
            
            # 텍스트 위치 계산 (중앙)
            position = ((width - text_width) // 2, (height + text_height) // 2)
            
            # 텍스트 그리기
            cv2.putText(empty_frame, message, position, font, font_scale, color, thickness)
            
            # 현재 시간 추가
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
            cv2.putText(empty_frame, timestamp, (10, height - 10), font, 0.5, (150, 150, 150), 1)
            
            # 카메라 ID 표시
            camera_text = f"카메라 {self.camera_number}"
            cv2.putText(empty_frame, camera_text, (width - 120, height - 10), font, 0.5, (150, 150, 150), 1)
            
            # JPEG로 인코딩
            _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 70])
            return jpeg.tobytes()
            
        except Exception as e:
            logger.error(f"빈 프레임 생성 오류: {e}")
            # 최소한의 바이트 데이터 반환
            return b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + b'\xff\xd8\xff\xe0\x00\x10JFIF\x00\x01\x01\x01\x00H\x00H\x00\x00\xff\xdb\x00C\x00\x08\x06\x06\x07\x06\x05\x08\x07\x07\x07\t\t\x08\n\x0c\x14\r\x0c\x0b\x0b\x0c\x19\x12\x13\x0f\x14\x1d\x1a\x1f\x1e\x1d\x1a\x1c\x1c $.\' ",#\x1c\x1c(7),01444\x1f\'9=82<.342\xff\xdb\x00C\x01\t\t\t\x0c\x0b\x0c\x18\r\r\x182!\x1c!22222222222222222222222222222222222222222222222222\xff\xc0\x00\x11\x08\x00\x01\x00\x01\x03\x01"\x00\x02\x11\x01\x03\x11\x01\xff\xc4\x00\x1f\x00\x00\x01\x05\x01\x01\x01\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\xff\xc4\x00\xb5\x10\x00\x02\x01\x03\x03\x02\x04\x03\x05\x05\x04\x04\x00\x00\x01}\x01\x02\x03\x00\x04\x11\x05\x12!1A\x06\x13Qa\x07"q\x142\x81\x91\xa1\x08#B\xb1\xc1\x15R\xd1\xf0$3br\x82\t\n\x16\x17\x18\x19\x1a%&\'()*456789:CDEFGHIJSTUVWXYZcdefghijstuvwxyz\x83\x84\x85\x86\x87\x88\x89\x8a\x92\x93\x94\x95\x96\x97\x98\x99\x9a\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xff\xc4\x00\x1f\x01\x00\x03\x01\x01\x01\x01\x01\x01\x01\x01\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\xff\xc4\x00\xb5\x11\x00\x02\x01\x02\x04\x04\x03\x04\x07\x05\x04\x04\x00\x01\x02w\x00\x01\x02\x03\x11\x04\x05!1\x06\x12AQ\x07aq\x13"2\x81\x08\x14B\x91\xa1\xb1\xc1\t#3R\xf0\x15br\xd1\n\x16$4\xe1%\xf1\x17\x18\x19\x1a&\'()*56789:CDEFGHIJSTUVWXYZcdefghijstuvwxyz\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x92\x93\x94\x95\x96\x97\x98\x99\x9a\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xff\xda\x00\x0c\x03\x01\x00\x02\x11\x03\x11\x00?\x00\xfe\xfe(\xa2\x8a\x00\xff\xd9' + b'\r\n'
    
    def _process_detection_results(self, boxes, frame_width, frame_height):
        """감지 결과 처리 - 조류퇴치 및 DB 업데이트"""
        current_time = time.time()
        
        # 감지된 객체가 있는지 확인
        if boxes is None or not hasattr(boxes, '__len__') or len(boxes) == 0:
            logger.info("감지된 객체 없음")
            return
            
        logger.info(f"_process_detection_results 호출됨: {len(boxes)}개 객체 감지")
        
        # 가장 큰 객체 찾기
        best_box = self._find_largest_box(boxes, frame_width, frame_height)
        
        # 조류퇴치기 제어 (객체가 있는 경우만)
        if best_box:
            self.bird_controller.process_detection(best_box, frame_width, frame_height)
        else:
            # 객체가 없으면 감지 상태 초기화
            self.bird_controller.reset_detection()
        
        # DB에 감지 정보 저장
        try:
            from ..models import Camera
            
            # 카메라 ID가 유효한지 확인
            camera_id = self.camera_number
            camera_exists = Camera.objects.filter(camera_id=camera_id).exists()
            
            if not camera_exists:
                logger.warning(f"카메라 ID {camera_id}가 데이터베이스에 존재하지 않습니다. 감지 정보 저장을 건너뜁니다.")
                return  # 감지 정보 저장 건너뛰기
            
            # DetectionInfo 테이블 업데이트
            detection_data = {
                'camera_id': self.camera_number,
                'detection_time': timezone.now(),
                'bb_count': len(boxes),
                'frame_width': frame_width,
                'frame_height': frame_height
            }
            
            # DB에 DetectionInfo 저장
            detection_info = self.db.save_detection_info(detection_data)
            
            # 각 객체의 바운딩 박스 정보를 BBInfo 테이블에 저장
            saved_box_count = 0
            for i, box in enumerate(boxes):
                try:
                    if box.xyxy is not None and len(box.xyxy) > 0:
                        # 좌표 추출 및 변환 (drawing.py와 동일한 방식 적용)
                        xyxy = box.xyxy[0].cpu().numpy()
                        x1_orig, y1_orig, x2_orig, y2_orig = xyxy  # 스케일링 없이 원본 좌표 사용
                        
                        # 좌표 범위 제한
                        x1_orig = max(0, min(x1_orig, frame_width-1))
                        y1_orig = max(0, min(y1_orig, frame_height-1))
                        x2_orig = max(0, min(x2_orig, frame_width-1))
                        y2_orig = max(0, min(y2_orig, frame_height-1))
                        
                        # 너비와 높이 계산
                        width = abs(x2_orig - x1_orig)
                        height = abs(y2_orig - y1_orig)
                        
                        # 너비나 높이가 너무 작은 바운딩 박스는 DB에 저장하지 않음
                        # 최소 크기는 1픽셀로 설정
                        if width < 1 or height < 1:
                            logger.warning(f"너무 작은 바운딩 박스 무시(박스 {i}): 너비={width:.4f}, 높이={height:.4f}")
                            continue
                        
                        # 좌표를 0~1 사이의 정규화된 값으로 변환
                        left_norm = x1_orig / frame_width if frame_width > 0 else 0
                        top_norm = y1_orig / frame_height if frame_height > 0 else 0
                        right_norm = x2_orig / frame_width if frame_width > 0 else 0
                        bottom_norm = y2_orig / frame_height if frame_height > 0 else 0
                        
                        # 신뢰도 및 클래스 정보 가져오기
                        confidence = float(box.conf[0]) if hasattr(box, 'conf') and box.conf is not None and len(box.conf) > 0 else 0
                        class_id = int(box.cls[0]) if hasattr(box, 'cls') and box.cls is not None and len(box.cls) > 0 else -1
                        class_name = self.detector.model.names[class_id] if class_id >= 0 and hasattr(self.detector.model, 'names') else "unknown"
                        
                        # BBInfo 테이블에 데이터 저장 (정규화된 좌표 사용)
                        bb_data = {
                            'detection_id': detection_info.id if detection_info else None,
                            'left': left_norm,
                            'top': top_norm,
                            'right': right_norm,
                            'bottom': bottom_norm,
                            'confidence': confidence,
                            'class_id': class_id,
                            'class_name': class_name
                        }
                        
                        # DB에 BBInfo 저장
                        self.db.save_bb_info(bb_data)
                        saved_box_count += 1
                except Exception as e:
                    logger.error(f"바운딩 박스 정보 저장 오류 (박스 {i}): {e}")
            
            # 마지막 DB 업데이트 시간 갱신
            self._last_db_update = current_time
            self.stats['db_updates'] += 1
            logger.info(f"DB 업데이트 완료: {saved_box_count}개 객체 감지 정보 저장 (총 {len(boxes)}개 중)")
            
        except Exception as e:
            logger.error(f"DB 업데이트 오류: {e}")
    
    def _find_largest_box(self, boxes, frame_width, frame_height):
        """가장 큰 객체의 바운딩 박스 찾기"""
        best_box = None
        max_area = 0
        
        for box in boxes:
            if box.xyxy is not None and len(box.xyxy) > 0:
                # 좌표 추출 및 변환 (drawing.py와 동일한 방식 적용)
                xyxy = box.xyxy[0].cpu().numpy()
                x1_orig, y1_orig, x2_orig, y2_orig = xyxy  # 스케일링 없이 원본 좌표 사용
                
                # 좌표 범위 제한
                x1_orig = max(0, min(x1_orig, frame_width-1))
                y1_orig = max(0, min(y1_orig, frame_height-1))
                x2_orig = max(0, min(x2_orig, frame_width-1))
                y2_orig = max(0, min(y2_orig, frame_height-1))
                
                # 너비와 높이 계산
                width = abs(x2_orig - x1_orig)
                height = abs(y2_orig - y1_orig)
                
                # 너비나 높이가 너무 작으면 무시
                if width < 1 or height < 1:
                    continue
                
                area = width * height
                if area > max_area:
                    max_area = area
                    best_box = [x1_orig, y1_orig, x2_orig, y2_orig]
        
        return best_box
    
    def get_status(self):
        """카메라 상태 정보"""
        stream_status = {"state": "NOT_INITIALIZED"}
        if self.stream_handler:
            stream_status = {
                "state": "ACTIVE",
            }
        
        return {
            "camera_id": self.camera_number,
            "stream": stream_status,
            "stats": self.stats,
            "detector": {
                "avg_process_time": self.stats['avg_process_time'],
                "detection_count": self.stats['detection_count'],
                "db_updates": self.stats['db_updates']
            }
        }

    def __del__(self):
        """객체가 소멸될 때 호출됨"""
        try:
            # 실행 중 플래그 해제
            self.is_running = False
            
            # 스트림 핸들러 정리
            if self.stream_handler:
                self.stream_handler.stop()
                
            # 메모리 정리
            self._last_results = None
            self._last_valid_frame = None
            
            # GPU 메모리 정리
            if torch.cuda.is_available():
                torch.cuda.empty_cache()
                
            logger.info(f"카메라 {self.camera_number} 자원 정리 완료")
        except Exception as e:
            logger.error(f"자원 정리 중 오류: {e}")