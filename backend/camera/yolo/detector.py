import torch
import time
import cv2
import numpy as np
from ultralytics import YOLO
import os
from threading import Lock, Thread
import threading
import queue

os.environ['ULTRALYTICS_SILENT'] = '1'


class QueueItem:
    """우선순위 큐에 사용되는 항목을 위한 클래스"""
    def __init__(self, priority, sequence, camera_id, frame, callback):
        self.priority = priority
        self.sequence = sequence
        self.camera_id = camera_id
        self.frame = frame
        self.callback = callback
    
    def __lt__(self, other):
        # 우선순위(priority)로 먼저 비교하고, 그 다음 sequence로 비교
        return (self.priority, self.sequence) < (other.priority, other.sequence)


class ObjectDetector:
    """YOLO 기반 객체 감지 클래스 (비동기 처리 최적화)"""
    
    # 싱글톤 인스턴스
    _instance = None
    _lock = None
    
    @classmethod
    def get_instance(cls):
        """싱글톤 인스턴스 반환"""
        if cls._instance is None:
            import threading
            if cls._lock is None:
                cls._lock = threading.Lock()
                
            with cls._lock:
                if cls._instance is None:
                    cls._instance = ObjectDetector()
        return cls._instance
    
    def __init__(self):
        """YOLO 모델 초기화"""
        if ObjectDetector._instance is not None:
            return
            
        try:
            # 모델 로드 (ultralytics 라이브러리 사용)
            from ultralytics import YOLO
            
            # 기존 모델 경로
            self.model_path = 'yolo12_27_best.pt'  # 상대 경로 사용
            
            try:
                # 먼저 커스텀 모델 로드 시도
                self.model = YOLO(self.model_path)
            except Exception as custom_model_error:
                print(f"커스텀 모델 로드 실패: {custom_model_error}")
                pass
            
            # 감지 설정
            self.conf_threshold = 0.4  # 기본 confidence threshold
            
            # GPU 사용 가능 여부 확인
            self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
            
            # 성능 모니터링을 위한 변수들
            self.inference_times = []
            self.total_processed = 0
            self.frame_size = (640, 640)  # 기본 입력 크기
            
            # 비동기 처리를 위한 큐 및 스레드 설정
            self.detection_queue = queue.PriorityQueue(maxsize=30)  # 우선순위 큐로 변경, 최신 프레임 우선 처리
            self.result_dict = {}  # 카메라 ID별 최신 감지 결과 저장
            self.result_lock = threading.Lock()  # 결과 딕셔너리 접근 동기화
            self.is_running = True
            
            # Guard Zone 관련 변수 초기화
            self.guard_zones = {}  # 카메라별 가드존 정보를 저장하는 딕셔너리
            self.guard_zone_enabled = {}  # 카메라별 가드존 활성화 상태
            self.guard_zones_enabled = {}  # guard_zones_enabled도 호환성을 위해 추가
            self.guard_zone_lock = threading.Lock()  # 가드존 설정 동기화
            
            # 감지 작업자 스레드 시작
            self.detection_thread = Thread(target=self._process_detection_queue, daemon=True)
            self.detection_thread.start()
                
        except Exception as e:
            # 예외가 발생해도 최소한의 기능을 위한 더미 모델 설정
            print(f"ObjectDetector 초기화 오류: {e}")
            self.model = None
            self.model_path = "로드 실패"
            self.device = "없음"
            self.conf_threshold = 0.4
            self.inference_times = []
            self.total_processed = 0
            self.frame_size = (640, 640)
            self.detection_queue = queue.PriorityQueue(maxsize=30)
            self.result_dict = {}
            self.result_lock = threading.Lock()
            self.is_running = False
            
            # Guard Zone 관련 변수 초기화
            self.guard_zones = {}  # 카메라별 가드존 정보를 저장하는 딕셔너리
            self.guard_zone_enabled = {}  # 카메라별 가드존 활성화 상태 
            self.guard_zones_enabled = {}  # guard_zones_enabled도 호환성을 위해 추가
            self.guard_zone_lock = threading.Lock()  # 가드존 설정 동기화
    
    def _process_detection_queue(self):
        """큐에서 프레임을 가져와 객체 감지를 수행하는 스레드"""
        while self.is_running:
            item_processed = False  # 아이템 처리 여부 추적
            
            try:
                # 큐에서 항목 가져오기 (우선순위, 순서, 카메라ID, 프레임, 콜백)
                queue_item = self.detection_queue.get(timeout=0.1)
                
                try:
                    # 항목 처리 로직
                    if queue_item is None:
                        # None 항목 무시
                        print("큐에서 None 항목을 받았습니다.")
                    elif not isinstance(queue_item, QueueItem):
                        # 잘못된 형식의 항목 무시
                        print(f"잘못된 큐 항목 형식: {type(queue_item)}")
                    else:
                        # QueueItem에서 값 추출
                        camera_id = queue_item.camera_id
                        frame = queue_item.frame
                        callback = queue_item.callback
                        
                        # 프레임 유효성 검사
                        if frame is None:
                            print(f"프레임이 None입니다: 카메라 {camera_id}")
                        elif not isinstance(frame, np.ndarray):
                            print(f"유효하지 않은 프레임 타입: {type(frame)}, 카메라 {camera_id}")
                        else:
                            # 객체 감지 수행
                            result = self._detect_objects_internal(frame, camera_id)
                            
                            # 결과 저장
                            with self.result_lock:
                                self.result_dict[camera_id] = result
                            
                            # 콜백 실행 (있는 경우)
                            if callback is not None:
                                try:
                                    callback(result)
                                except Exception as callback_err:
                                    print(f"콜백 실행 오류: {callback_err}")
                                    
                    # 항목이 정상적으로 처리됨 표시
                    item_processed = True
                    
                except Exception as process_err:
                    # 항목 처리 중 발생한 오류 기록
                    import traceback
                    print(f"항목 처리 중 오류: {process_err}")
                    print(traceback.format_exc())
                    
                    # 오류가 발생해도 항목은 처리된 것으로 간주
                    item_processed = True
                
                finally:
                    # 항목이 처리됐으면 task_done() 호출 (한 번만 호출됨)
                    if item_processed:
                        self.detection_queue.task_done()
                
            except queue.Empty:
                # 큐가 비어 있으면 잠시 대기
                time.sleep(0.01)
            except Exception as e:
                import traceback
                print(f"큐 처리 주요 오류: {e}")
                print(traceback.format_exc())
                
                # 오류 후 잠시 대기
                time.sleep(0.1)
    
    def _detect_objects_internal(self, frame, camera_id=None):
        """내부적으로 사용되는 객체 감지 메서드"""
        if frame is None or self.model is None:
            return None
            
        try:
            # 프레임 크기 저장
            original_height, original_width = frame.shape[:2] 
            
            # 시작 시간 기록
            start_time = time.time()
            
            # YOLO 모델 실행 - conf 임계값 적용
            results = self.model(frame, conf=self.conf_threshold, verbose=False)
            
            # 추론 시간 기록
            inference_time = (time.time() - start_time) * 1000  # 밀리초 단위
            self.inference_times.append(inference_time)
            if len(self.inference_times) > 100:  # 최대 100개의 샘플만 유지
                self.inference_times.pop(0)
                
            # 처리된 프레임 수 증가
            self.total_processed += 1
            
            # results는 리스트
            if isinstance(results, list) and len(results) > 0:
                result = results[0]  # 첫 번째 결과 사용
                
                # orig_shape 속성 추가
                if not hasattr(result, 'orig_shape'):
                    result.orig_shape = (original_height, original_width)
                
                # 가드존 필터링 적용
                if camera_id is not None and hasattr(result, 'boxes') and len(result.boxes) > 0:
                    # 카메라 ID를 문자열로 변환
                    camera_id_str = str(camera_id)
                    
                    # 해당 카메라에 가드존이 설정되어 있고 활성화 되어 있는지 확인
                    has_guard_zone = (camera_id_str in self.guard_zones and 
                                     len(self.guard_zones[camera_id_str]) > 0)
                    is_enabled = self.guard_zone_enabled.get(camera_id_str, False) or self.guard_zones_enabled.get(camera_id_str, False)
                    
                    # 가드존이 활성화되어 있으면 필터링 적용
                    if has_guard_zone and is_enabled:
                        filtered_boxes = []
                        guard_zones = self.guard_zones[camera_id_str]
                        
                        # 각 바운딩 박스에 대해 하나 이상의 가드존 내에 있는지 확인
                        for box in result.boxes:
                            if box.xyxy is None or len(box.xyxy) == 0:
                                continue
                                
                            # 박스 좌표 가져오기
                            xyxy = box.xyxy[0].cpu().numpy()
                            x1, y1, x2, y2 = map(float, xyxy)
                            
                            # 박스 중심점 계산
                            center_x = (x1 + x2) / 2 / original_width
                            center_y = (y1 + y2) / 2 / original_height
                            
                            # 하나 이상의 가드존 내에 있는지 확인
                            is_in_guard_zone = False
                            for zone in guard_zones:
                                zone_x = zone['x']
                                zone_y = zone['y']
                                zone_width = zone['width']
                                zone_height = zone['height']
                                
                                # 중심점이 가드존 내에 있는지 확인
                                if (zone_x <= center_x <= zone_x + zone_width and 
                                    zone_y <= center_y <= zone_y + zone_height):
                                    is_in_guard_zone = True
                                    break
                            
                            # 가드존 내에 있으면 결과에 포함
                            if is_in_guard_zone:
                                filtered_boxes.append(box)
                        
                        # 필터링된 박스로 결과 업데이트
                        result.boxes = filtered_boxes
                
                # 감지된 객체 수 처리
                if hasattr(result, 'boxes') and len(result.boxes) > 0:
                    boxes_count = len(result.boxes)
                    
                    # 클래스별 객체 수 카운트
                    if hasattr(self.model, 'names'):
                        class_counts = {}
                        for box in result.boxes:
                            if hasattr(box, 'cls') and box.cls is not None and len(box.cls) > 0:
                                cls_id = int(box.cls[0].item())
                                class_name = self.model.names.get(cls_id, f"class_{cls_id}")
                                class_counts[class_name] = class_counts.get(class_name, 0) + 1
                
                return result
            else:
                return None
                
        except Exception as e:
            import traceback
            print(f"내부 객체 감지 오류: {e}")
            print(traceback.format_exc())
            return None
    
    def detect_objects_async(self, camera_id, frame, priority=10, callback=None):
        """비동기 객체 감지 요청
        
        Args:
            camera_id: 카메라 식별자
            frame: 처리할 프레임
            priority: 우선순위 (낮을수록 높은 우선순위, 기본값 10)
            callback: 감지 완료 시 호출할 콜백 함수
            
        Returns:
            bool: 요청 성공 여부
        """
        if not self.is_running or self.model is None:
            return False
            
        try:
            # 프레임 유효성 확인
            if frame is None:
                print(f"카메라 {camera_id}: 프레임이 None입니다")
                return False
                
            if not isinstance(frame, np.ndarray):
                print(f"카메라 {camera_id}: 유효하지 않은 프레임 타입 - {type(frame)}")
                return False
                
            # 대기열이 가득 찼는지 확인
            if self.detection_queue.full():
                # 가득 찬 경우 건너뜀
                return False
            
            # 안전한 프레임 복사본 만들기
            try:
                frame_copy = frame.copy()
            except Exception as copy_err:
                print(f"프레임 복사 오류: {copy_err}")
                return False
                
            # 카메라 ID가 None이면 기본값 할당
            if camera_id is None:
                camera_id = "unknown"
                
            # 우선순위 큐에 항목 추가 (QueueItem 객체 사용)
            sequence = int(time.time() * 1000)  # 밀리초 타임스탬프를 순서 번호로 사용
            item = QueueItem(priority, sequence, camera_id, frame_copy, callback)
            
            # QueueItem 객체를 큐에 추가
            self.detection_queue.put(item)
            return True
            
        except Exception as e:
            print(f"비동기 감지 요청 오류: {e}")
            import traceback
            print(traceback.format_exc())
            return False
    
    def get_latest_result(self, camera_id):
        """특정 카메라의 최신 객체 감지 결과 가져오기"""
        with self.result_lock:
            return self.result_dict.get(camera_id)
    
    def set_conf_threshold(self, conf):
        """Confidence threshold 설정"""
        if 0.0 <= conf <= 1.0:
            old_conf = self.conf_threshold
            self.conf_threshold = conf
            return True
        else:
            return False
            
    def get_conf_threshold(self):
        """현재 confidence threshold 반환"""
        return self.conf_threshold

    def detect_objects(self, frame, camera_id=None):
        """동기식 객체 감지 (하위 호환성 유지)"""
        return self._detect_objects_internal(frame, camera_id)
    
    def get_average_fps(self):
        """평균 FPS 계산"""
        if not self.inference_times:
            return 0
        avg_ms = sum(self.inference_times) / len(self.inference_times)
        calculated_fps = 1000 / avg_ms if avg_ms > 0 else 0
        
        # FPS 제한 적용 (실제 프레임 레이트 10으로 제한)
        return min(10.0, calculated_fps)
    
    def get_model_info(self):
        """모델 정보 반환"""
        try:
            avg_inference_time = sum(self.inference_times) / max(1, len(self.inference_times))
        except Exception:
            avg_inference_time = 0

        return {
            "model_path": self.model_path,
            "device": str(self.device),
            "model_loaded": self.model is not None,
            "avg_inference_time": avg_inference_time,
            "conf_threshold": self.conf_threshold,
            "total_processed": self.total_processed,
            "queue_size": self.detection_queue.qsize(),
            "async_mode": True
        }
        
    def stop(self):
        """객체 감지기 정지 및 자원 정리"""
        self.is_running = False
        if hasattr(self, 'detection_thread') and self.detection_thread.is_alive():
            self.detection_thread.join(timeout=1.0)

    def load_model(self, model_path):
        """새 모델 파일 로드"""
        if not os.path.exists(model_path):
            raise FileNotFoundError(f"모델 파일을 찾을 수 없습니다: {model_path}")
            
        try:
            # 처리 중인 큐 비우기
            with self.result_lock:
                # 큐에 남아 있는 작업 비우기
                self.result_dict = {}
                self.inference_times = []
                self.total_processed = 0
            
            # 새 모델 로드
            new_model = YOLO(model_path)
            
            # 모델 교체
            self.model = new_model
            self.model_path = model_path
            
            # 추론 시간 통계 초기화
            self.inference_times = []
            
            return True
            
        except Exception as e:
            # 오류 발생 시 원래 모델 유지
            print(f"새 모델 로드 중 오류 발생: {e}")
            raise

    def set_guard_zone(self, camera_id, guard_zones, enabled=True):
        """특정 카메라의 Guard Zone 설정
        
        Args:
            camera_id: 카메라 식별자
            guard_zones: 가드존 리스트 (각각은 x, y, width, height를 0~1 사이의 정규화된 값으로 포함)
            enabled: 가드존 활성화 여부
            
        Returns:
            bool: 설정 성공 여부
        """
        try:
            with self.guard_zone_lock:
                self.guard_zones[camera_id] = guard_zones
                self.guard_zone_enabled[camera_id] = enabled
                self.guard_zones_enabled[camera_id] = enabled  # 호환성을 위해 추가
            return True
        except Exception as e:
            import traceback
            print(f"Guard Zone 설정 오류: {e}")
            print(traceback.format_exc())
            return False
    
    def get_guard_zone(self, camera_id):
        """특정 카메라의 Guard Zone 정보 가져오기
        
        Args:
            camera_id: 카메라 식별자
            
        Returns:
            dict: 가드존 정보 (guard_zones, enabled)
        """
        with self.guard_zone_lock:
            return {
                'guard_zones': self.guard_zones.get(camera_id, []),
                'enabled': self.guard_zone_enabled.get(camera_id, False)
            }
    
    def _draw_guard_zones(self, frame, camera_id, width=None, height=None):
        """프레임에 Guard Zone 그리기
        
        Args:
            frame: 원본 프레임
            camera_id: 카메라 식별자
            width: 프레임 너비 (None이면 프레임에서 가져옴)
            height: 프레임 높이 (None이면 프레임에서 가져옴)
            
        Returns:
            numpy.ndarray: Guard Zone이 그려진 프레임
        """
        if frame is None:
            return frame
            
        try:
            # 프레임 복사본 생성
            visualized_frame = frame.copy()
            
            # 프레임 크기 가져오기
            if width is None or height is None:
                height, width = frame.shape[:2]
                
            # Guard Zone 정보 가져오기
            guard_zone_info = self.get_guard_zone(camera_id)
            zones = guard_zone_info['guard_zones']
            
            # 빨간색으로 설정 (BGR 형식)
            color = (0, 0, 255)
            
            # 각 가드존 그리기
            for i, zone in enumerate(zones):
                # 정규화된 좌표를 픽셀 좌표로 변환
                x = int(zone['x'] * width)
                y = int(zone['y'] * height)
                w = int(zone['width'] * width)
                h = int(zone['height'] * height)
                
                # 사각형 그리기
                cv2.rectangle(visualized_frame, (x, y), (x + w, y + h), color, 2)
                
                # 라벨 그리기 (간단하게 "ZONE"만 표시)
                label = f"ZONE {i+1}"
                cv2.putText(visualized_frame, label, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)
            
            return visualized_frame
            
        except Exception as e:
            import traceback
            print(f"Guard Zone 시각화 오류: {e}")
            print(traceback.format_exc())
            return frame  # 오류 발생 시 원본 프레임 반환