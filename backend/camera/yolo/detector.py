import torch
import time
import cv2
import numpy as np
from ultralytics import YOLO
import os
from threading import Lock, Thread
import threading
import queue
import math
import sys

# 모듈 경로 추가
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from pantilt.controller import PanTiltController

os.environ['ULTRALYTICS_SILENT'] = '1'

# 팬틸트 관련 상수
CAMERA_FOV = 48  # 카메라 화각(도)
MAX_PAN_ANGLE = 60  # 최대 좌우 회전 각도(도)
SCAN_STEP = 10  # 스캔 모드 회전 단계(도)
SCAN_INTERVAL = 5  # 자동 스캔 모드 전환 전 대기 시간(초)
TRACK_INTERVAL = 1.0  # 추적 계산 간격(초)

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
                # 커스텀 모델 로드 시도
                print("[YOLO] 모델 로드 시도:", self.model_path)
                self.model = YOLO(self.model_path)
                print("[YOLO] 모델 로드 성공")
            except Exception as custom_model_error:
                print(f"[YOLO] 커스텀 모델 로드 실패: {custom_model_error}")
                self.model = None
            
            # 감지 설정
            self.conf_threshold = 0.4  # 기본 confidence threshold
            
            # GPU 사용 가능 여부 확인
            self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
            print(f"[YOLO] 사용 중인 장치: {self.device}")
            
            # 성능 모니터링을 위한 변수들
            self.inference_times = []
            self.total_processed = 0
            self.frame_size = (640, 640)  # 기본 입력 크기
            
            # 비동기 처리를 위한 큐 및 스레드 설정
            self.detection_queue = queue.PriorityQueue(maxsize=30)  # 우선순위 큐로 변경, 최신 프레임 우선 처리
            self.result_dict = {}  # 카메라 ID별 최신 감지 결과 저장
            self.result_lock = threading.Lock()  # 결과 딕셔너리 접근 동기화
            self.is_running = True
            
            # Guard Zone 설정을 위한 변수
            self.guard_zones = {}  # 카메라 ID별 guard zone 저장
            self.guard_zones_enabled = {}  # 카메라 ID별 guard zone 활성화 상태
            
            # 감지 작업자 스레드 시작
            self.detection_thread = Thread(target=self._process_detection_queue, daemon=True)
            self.detection_thread.start()
                
        except Exception as e:
            # 예외가 발생해도 최소한의 기능을 위한 더미 모델 설정
            print(f"[YOLO] ObjectDetector 초기화 오류: {e}")
            import traceback
            print(traceback.format_exc())
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
            self.guard_zones = {}
            self.guard_zones_enabled = {}
            self.is_running = False
    
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
                            # camera_id를 전달하여 가드존 필터링이 적용되도록 함
                            result = self._detect_objects_internal(frame, camera_id)
                            
                            if result and hasattr(result, 'boxes') and len(result.boxes) > 0:
                                print(f"[ASYNC_DETECT] 카메라 {camera_id}: {len(result.boxes)}개 객체 감지")
                            
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
            # 모델이 없는 경우 빈 결과 반환
            print(f"[YOLO] 모델이 None이거나 프레임이 None입니다: model={self.model is not None}, frame={frame is not None}")
            return None
            
        try:
            # 프레임 크기 저장
            original_height, original_width = frame.shape[:2] 
            
            # 카메라 ID를 문자열로 통일
            camera_id_str = str(camera_id) if camera_id is not None else None
            
            # 시작 시간 기록
            start_time = time.time()
            
            try:
                # YOLO 모델 실행 - conf 임계값 적용
                results = self.model(frame, conf=self.conf_threshold, verbose=False)
            except Exception as model_error:
                print(f"[YOLO] 모델 실행 오류: {model_error}")
                return None
            
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
                
                try:
                    # Guard Zone 필터링 (설정되어 있고 활성화된 경우)
                    has_guard_zone = (camera_id_str is not None and 
                        camera_id_str in self.guard_zones and 
                        len(self.guard_zones[camera_id_str]) > 0)
                    
                    is_enabled = self.guard_zones_enabled.get(camera_id_str, False)
                    
                    if has_guard_zone and is_enabled:
                        # 감지된 박스가 있는 경우에만 필터링 적용
                        if hasattr(result, 'boxes') and len(result.boxes) > 0:
                            # 필터링 전 박스 개수
                            original_boxes = len(result.boxes)
                            
                            # 조건에 맞는 박스만 필터링할 새 인덱스 목록
                            keep_indices = []
                            
                            # 각 감지된 객체에 대해
                            for i, box in enumerate(result.boxes):
                                if not hasattr(box, 'xyxy') or box.xyxy is None or len(box.xyxy) == 0:
                                    continue
                                    
                                # 박스 좌표 가져오기 (x1, y1, x2, y2)
                                try:
                                    xyxy = box.xyxy[0].cpu().numpy()
                                except Exception as e:
                                    continue
                                
                                # 박스 중심점 계산
                                center_x = (xyxy[0] + xyxy[2]) / 2
                                center_y = (xyxy[1] + xyxy[3]) / 2
                                
                                # 정규화된 중심점 (0~1 범위로)
                                norm_center_x = center_x / original_width
                                norm_center_y = center_y / original_height
                                
                                # 어느 Guard Zone에든 포함되는지 확인
                                in_guard_zone = False
                                for zone_idx, zone in enumerate(self.guard_zones[camera_id_str]):
                                    # zone: x, y, width, height (정규화된 좌표, 0~1 범위)
                                    zone_x = zone['x']
                                    zone_y = zone['y']
                                    zone_w = zone['width']
                                    zone_h = zone['height']
                                    
                                    # 가드존 영역 계산
                                    zone_x1 = zone_x
                                    zone_y1 = zone_y
                                    zone_x2 = zone_x + zone_w
                                    zone_y2 = zone_y + zone_h
                                    
                                    # 객체 중심점이 가드존 내부에 있는지 확인
                                    in_current_zone = (zone_x1 <= norm_center_x <= zone_x2 and
                                                      zone_y1 <= norm_center_y <= zone_y2)
                                    
                                    if in_current_zone:
                                        in_guard_zone = True
                                        break
                                
                                # Guard Zone 내부에 있는 경우 유지
                                if in_guard_zone:
                                    keep_indices.append(i)
                            
                            # 필터링된 박스로 결과 업데이트
                            if len(keep_indices) < original_boxes:
                                try:
                                    # 유지할 박스만 선택
                                    result.boxes = result.boxes[keep_indices]
                                    # 관련 배열도 함께 업데이트 (해당 속성이 있을 경우)
                                    if hasattr(result, 'masks') and result.masks is not None:
                                        result.masks = result.masks[keep_indices]
                                    if hasattr(result, 'keypoints') and result.keypoints is not None:
                                        result.keypoints = result.keypoints[keep_indices]
                                    
                                    print(f"[GuardZone] 필터링 결과: {original_boxes}개 -> {len(keep_indices)}개 객체 유지")
                                except Exception as filter_error:
                                    print(f"[GuardZone] 필터링 업데이트 오류: {filter_error}")
                except Exception as guard_zone_error:
                    print(f"[GuardZone] 가드존 적용 중 오류 발생: {guard_zone_error}")
                
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
                
                # 가드존 시각화 (활성화된 경우)
                if camera_id_str is not None and camera_id_str in self.guard_zones and self.guard_zones_enabled.get(camera_id_str, False):
                    visualized_frame = self._draw_guard_zones(frame, camera_id_str, original_width, original_height)
                    # 시각화된 프레임을 결과 객체에 저장
                    if not hasattr(result, 'visualized_frame'):
                        result.visualized_frame = visualized_frame
                
                return result
            else:
                return None
                
        except Exception as e:
            import traceback
            print(f"[YOLO] 내부 객체 감지 오류: {e}")
            print(traceback.format_exc())
            return None
    
    def _draw_guard_zones(self, frame, camera_id, original_width, original_height):
        """가드존을 프레임에 시각화하는 메서드
        
        Args:
            frame: 원본 프레임
            camera_id: 카메라 ID
            original_width: 프레임 너비
            original_height: 프레임 높이
            
        Returns:
            numpy.ndarray: 가드존이 그려진 프레임
        """
        try:
            # 프레임 복사본 생성 (원본을 유지하기 위해)
            visualized_frame = frame.copy()
            
            # 가드존 목록 가져오기
            guard_zones = self.guard_zones.get(camera_id, [])
            
            # 각 가드존에 대해
            for zone_idx, zone in enumerate(guard_zones):
                # 정규화된 좌표 (0~1)를 픽셀 좌표로 변환
                x1 = int(zone['x'] * original_width)
                y1 = int(zone['y'] * original_height)
                w = int(zone['width'] * original_width)
                h = int(zone['height'] * original_height)
                x2 = x1 + w
                y2 = y1 + h
                
                # 빨간색 네모 그리기 (BGR 형식, 빨간색은 (0, 0, 255))
                cv2.rectangle(visualized_frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                
                # "guardzone" 텍스트 추가
                # 텍스트 크기와 두께 설정
                font_scale = 0.5
                text_thickness = 1
                font = cv2.FONT_HERSHEY_SIMPLEX
                
                # 텍스트 크기 측정
                text = "guardzone"
                (text_width, text_height), _ = cv2.getTextSize(text, font, font_scale, text_thickness)
                
                # 텍스트 배경 사각형 (가독성 향상)
                text_bg_x1 = x1
                text_bg_y1 = y1 - text_height - 10
                text_bg_x2 = x1 + text_width + 10
                text_bg_y2 = y1
                
                # 텍스트 위치가 프레임 상단을 벗어나면 네모 아래에 표시
                if text_bg_y1 < 0:
                    text_bg_y1 = y2
                    text_bg_y2 = y2 + text_height + 10
                
                # 텍스트 배경 사각형 그리기
                cv2.rectangle(visualized_frame, 
                              (text_bg_x1, text_bg_y1), 
                              (text_bg_x2, text_bg_y2), 
                              (0, 0, 255), 
                              -1)  # -1은 채우기
                
                # 텍스트 그리기 (흰색)
                cv2.putText(visualized_frame, 
                            text, 
                            (text_bg_x1 + 5, text_bg_y2 - 5), 
                            font, 
                            font_scale, 
                            (255, 255, 255), 
                            text_thickness)
            
            return visualized_frame
            
        except Exception as e:
            print(f"[GuardZone] 가드존 시각화 오류: {e}")
            # 오류 발생 시 원본 프레임 반환
            return frame
    
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
            
            # 카메라 ID를 문자열로 통일 (비동기 큐 저장용)
            camera_id_str = str(camera_id)
                
            # 우선순위 큐에 항목 추가 (QueueItem 객체 사용)
            sequence = int(time.time() * 1000)  # 밀리초 타임스탬프를 순서 번호로 사용
            item = QueueItem(priority, sequence, camera_id_str, frame_copy, callback)
            
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
    
    def set_guard_zone(self, camera_id, guard_zones, enabled=True):
        """카메라의 Guard Zone 설정
        
        Args:
            camera_id: 카메라 식별자
            guard_zones: Guard Zone 목록 (각 zone은 x, y, width, height 키를 가진 딕셔너리)
            enabled: Guard Zone 활성화 여부
            
        Returns:
            bool: 설정 성공 여부
        """
        if not camera_id:
            return False
            
        try:
            # 카메라 ID를 문자열로 통일
            camera_id_str = str(camera_id)
            
            # guard_zones가 비어있는지 확인
            if not guard_zones or len(guard_zones) == 0:
                print(f"[GuardZone] 경고: 카메라 {camera_id_str}에 빈 가드존 설정됨")
                
            # 값이 0~1 범위인지 확인
            for i, zone in enumerate(guard_zones):
                for key in ['x', 'y', 'width', 'height']:
                    if key not in zone:
                        print(f"[GuardZone] 오류: zone[{i}]에 {key} 키가 없음")
                        return False
                    if not (0.0 <= zone[key] <= 1.0):
                        print(f"[GuardZone] 오류: zone[{i}].{key}={zone[key]}가 0~1 범위를 벗어남")
                        return False
            
            # Guard Zone 정보 저장 (문자열 키로 저장)
            self.guard_zones[camera_id_str] = guard_zones
            self.guard_zones_enabled[camera_id_str] = enabled
            
            # 설정 정보 로그
            print(f"[GuardZone] 카메라 {camera_id_str}: {len(guard_zones)}개 영역 설정됨, 활성화={enabled}")
            
            return True
        except Exception as e:
            print(f"[GuardZone] Guard Zone 설정 오류: {e}")
            return False
    
    def get_guard_zone(self, camera_id):
        """카메라의 Guard Zone 정보 가져오기
        
        Args:
            camera_id: 카메라 식별자
            
        Returns:
            dict: Guard Zone 정보 (zones, enabled)
        """
        # 카메라 ID를 문자열로 통일
        camera_id_str = str(camera_id)
        
        if not camera_id_str or camera_id_str not in self.guard_zones:
            print(f"[GuardZone] 카메라 {camera_id_str}의 가드존 정보 없음")
            return {'guard_zones': [], 'enabled': False}
            
        enabled = self.guard_zones_enabled.get(camera_id_str, False)
        zones = self.guard_zones.get(camera_id_str, [])
        print(f"[GuardZone] 카메라 {camera_id_str} 가드존 정보 반환: {len(zones)}개 영역, 활성화={enabled}")
        
        return {
            'guard_zones': zones,
            'enabled': enabled
        }
    
    def enable_guard_zone(self, camera_id, enabled=True):
        """Guard Zone 활성화/비활성화
        
        Args:
            camera_id: 카메라 식별자
            enabled: 활성화 여부
            
        Returns:
            bool: 설정 성공 여부
        """
        if not camera_id:
            return False
        
        # 카메라 ID를 문자열로 통일
        camera_id_str = str(camera_id)
        
        # 가드존이 설정되어 있지 않으면 오류 로그
        if camera_id_str not in self.guard_zones:
            print(f"[GuardZone] 오류: 카메라 {camera_id_str}에 설정된 가드존이 없습니다")
            return False
            
        self.guard_zones_enabled[camera_id_str] = enabled
        print(f"[GuardZone] 카메라 {camera_id_str} 가드존 {enabled and '활성화' or '비활성화'} 됨")
        return True

    def detect_objects(self, frame, camera_id=None):
        """동기식 객체 감지 (하위 호환성 유지)"""
        return self._detect_objects_internal(frame, camera_id)
    
    def get_average_fps(self):
        """평균 FPS 계산"""
        if not self.inference_times:
            return 0
        avg_ms = sum(self.inference_times) / len(self.inference_times)
        return 1000 / avg_ms if avg_ms > 0 else 0
    
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
            "async_mode": True,
            "guard_zones_count": sum(len(zones) for zones in self.guard_zones.values())
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

    def get_latest_frame(self, camera_id):
        """특정 카메라의 가드존이 그려진 최신 프레임 가져오기"""
        with self.result_lock:
            result = self.result_dict.get(camera_id)
            if result and hasattr(result, 'visualized_frame'):
                return result.visualized_frame
            return None

class BirdTracker:
    """조류 추적 및 팬틸트 카메라 제어 클래스"""
    
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
                    cls._instance = BirdTracker()
        return cls._instance
    
    def __init__(self):
        """초기화"""
        if BirdTracker._instance is not None:
            return
            
        # 객체 감지기 인스턴스 가져오기
        self.detector = ObjectDetector.get_instance()
        
        # 팬틸트 컨트롤러 가져오기
        self.pantilt = PanTiltController.get_instance()
        
        # 팬틸트 상태 정보
        self.current_pan_angle = 0  # 현재 팬 각도 (좌우)
        self.current_tilt_angle = 0  # 현재 틸트 각도 (상하) - 미사용
        self.home_pan_angle = 0  # 홈 포지션(0도) 설정
        self.is_tracking = False  # 추적 모드 활성화 여부
        self.is_scanning = False  # 스캔 모드 활성화 여부
        self.scan_direction = 1  # 스캔 방향 (1: 우측, -1: 좌측)
        
        # 추적 관련 정보
        self.last_detection_time = 0  # 마지막 감지 시간
        self.last_track_time = 0  # 마지막 추적 계산 시간
        self.tracked_positions = []  # 추적된 위치 기록 (최근 N개)
        self.max_positions = 5  # 위치 평균 계산에 사용할 최대 위치 수
        
        # 동작 제어를 위한 스레드 및 잠금
        self.lock = threading.Lock()
        self.running = True
        self.tracking_thread = Thread(target=self._tracking_loop, daemon=True)
        self.tracking_thread.start()
        
        # 현재 팬틸트 위치 동기화
        self._sync_position_from_hardware()
        
        # 모션 콜백 등록
        self.pantilt.add_motion_callback(self._on_motion_update)
        
        print("[BirdTracker] 초기화 완료")
    
    def _sync_position_from_hardware(self):
        """하드웨어의 현재 위치로 동기화"""
        position = self.pantilt.get_position()
        self.current_pan_angle = position["pan"]
        self.current_tilt_angle = position["tilt"]
    
    def _on_motion_update(self, position):
        """하드웨어 모션 업데이트 이벤트 처리"""
        with self.lock:
            self.current_pan_angle = position["pan"]
            self.current_tilt_angle = position["tilt"]
    
    def _tracking_loop(self):
        """추적 및 스캔 동작을 처리하는 메인 루프"""
        while self.running:
            try:
                current_time = time.time()
                
                # 추적 모드가 활성화된 경우
                if self.is_tracking:
                    # 추적 계산 간격 확인
                    if current_time - self.last_track_time >= TRACK_INTERVAL:
                        self._process_tracking()
                        self.last_track_time = current_time
                
                # 추적 모드가 비활성화된 경우 (객체 감지되지 않음)
                else:
                    # 마지막 감지 후 일정 시간이 지났고, 현재 홈 위치가 아니라면 홈으로 복귀
                    if (not self.is_scanning and 
                        current_time - self.last_detection_time >= SCAN_INTERVAL and 
                        abs(self.current_pan_angle - self.home_pan_angle) > 0.5):
                        self._return_to_home()
                    
                    # 홈 위치에서 일정 시간 객체가 감지되지 않으면 스캔 모드 활성화
                    elif (not self.is_scanning and 
                          current_time - self.last_detection_time >= SCAN_INTERVAL * 2 and 
                          abs(self.current_pan_angle - self.home_pan_angle) <= 0.5):
                        self._start_scanning()
                    
                    # 스캔 모드가 활성화된 경우 스캔 수행
                    elif self.is_scanning:
                        self._perform_scan()
                
                # 잠시 대기
                time.sleep(0.1)
                
            except Exception as e:
                import traceback
                print(f"[BirdTracker] 추적 루프 오류: {e}")
                print(traceback.format_exc())
                time.sleep(1.0)  # 오류 발생 시 잠시 대기
    
    def _process_tracking(self):
        """객체 감지 결과를 바탕으로 추적 처리"""
        with self.lock:
            # 마지막으로 감지된 객체 가져오기
            latest_result = self.detector.get_latest_result("main")  # 메인 카메라 결과 가져오기
            
            if latest_result is None or not hasattr(latest_result, 'boxes') or len(latest_result.boxes) == 0:
                # 객체가 감지되지 않았으면 추적 모드 비활성화
                if self.is_tracking:
                    print("[BirdTracker] 객체 감지되지 않음, 추적 중지")
                    self.is_tracking = False
                return
            
            # 객체가 감지됨 - 추적 모드 활성화 및 시간 갱신
            self.is_tracking = True
            self.is_scanning = False
            self.last_detection_time = time.time()
            
            # 감지된 객체들의 중심 위치 계산
            frame_width = latest_result.orig_shape[1]
            center_positions = []
            
            for box in latest_result.boxes:
                if not hasattr(box, 'xyxy') or box.xyxy is None or len(box.xyxy) == 0:
                    continue
                
                # 박스 좌표 가져오기 (x1, y1, x2, y2)
                try:
                    xyxy = box.xyxy[0].cpu().numpy()
                except Exception:
                    continue
                
                # 중심점 계산
                center_x = (xyxy[0] + xyxy[2]) / 2
                
                # 클래스가 조류인지 확인 (클래스 ID 또는 이름으로)
                is_bird = True  # 기본값: 모든 객체 추적
                if hasattr(box, 'cls') and box.cls is not None and len(box.cls) > 0:
                    cls_id = int(box.cls[0].item())
                    class_name = self.detector.model.names.get(cls_id, "")
                    # 조류 클래스만 필터링 (클래스 이름에 'bird'가 포함된 경우)
                    is_bird = ('bird' in class_name.lower())
                
                if is_bird:
                    center_positions.append(center_x)
            
            # 감지된 조류가 없으면 종료
            if not center_positions:
                return
            
            # 중심점 평균 계산
            avg_center_x = sum(center_positions) / len(center_positions)
            
            # 정규화된 위치 (-1 ~ 1 범위, 0이 중앙)
            normalized_pos = (avg_center_x / frame_width) * 2 - 1
            
            # 추적 위치 목록에 추가
            self.tracked_positions.append(normalized_pos)
            if len(self.tracked_positions) > self.max_positions:
                self.tracked_positions.pop(0)
            
            # 최근 위치들의 평균 계산
            avg_position = sum(self.tracked_positions) / len(self.tracked_positions)
            
            # 이동 필요성 판단 (중앙에서 ±0.2 이상 벗어나면 이동)
            threshold = 0.2
            movement_amount = 0
            
            if avg_position < -threshold:  # 왼쪽으로 치우침
                # 왼쪽으로 이동 (음수 각도)
                movement_amount = -abs(avg_position) * 5  # 위치에 비례한 이동량
            elif avg_position > threshold:  # 오른쪽으로 치우침
                # 오른쪽으로 이동 (양수 각도)
                movement_amount = abs(avg_position) * 5  # 위치에 비례한 이동량
            
            # 이동이 필요한 경우
            if abs(movement_amount) > 0:
                # 새 각도 계산
                new_angle = self.current_pan_angle + movement_amount
                
                # 제한 범위 내로 조정
                new_angle = max(self.home_pan_angle - MAX_PAN_ANGLE, min(self.home_pan_angle + MAX_PAN_ANGLE, new_angle))
                
                # 이동 실행
                if abs(new_angle - self.current_pan_angle) > 0.5:  # 최소 이동량 이상일 때만 실행
                    self._move_to_pan_angle(new_angle)
                    print(f"[BirdTracker] 팬 이동: {self.current_pan_angle:.1f}° → {new_angle:.1f}° (위치: {avg_position:.2f})")
    
    def _return_to_home(self):
        """홈 위치(0도)로 복귀"""
        try:
            print(f"[BirdTracker] 홈 위치({self.home_pan_angle}°)로 복귀 중...")
            self._move_to_pan_angle(self.home_pan_angle)
        except Exception as e:
            print(f"[BirdTracker] 홈 복귀 오류: {e}")
    
    def _start_scanning(self):
        """자동 스캔 모드 시작"""
        with self.lock:
            self.is_scanning = True
            self.scan_direction = 1  # 오른쪽으로 시작
            print("[BirdTracker] 자동 스캔 모드 시작")
    
    def _perform_scan(self):
        """스캔 패턴에 따라 카메라 이동"""
        with self.lock:
            if not self.is_scanning:
                return
            
            # 다음 위치 계산
            next_angle = self.current_pan_angle + (SCAN_STEP * self.scan_direction)
            
            # 범위 확인 및 방향 전환
            if next_angle > self.home_pan_angle + MAX_PAN_ANGLE:
                # 우측 한계 도달, 방향 전환
                self.scan_direction = -1
                next_angle = self.home_pan_angle + MAX_PAN_ANGLE
            elif next_angle < self.home_pan_angle - MAX_PAN_ANGLE:
                # 좌측 한계 도달, 방향 전환
                self.scan_direction = 1
                next_angle = self.home_pan_angle - MAX_PAN_ANGLE
            
            # 이동 실행
            self._move_to_pan_angle(next_angle)
            print(f"[BirdTracker] 스캔 중: {next_angle}° (방향: {self.scan_direction > 0 and '→' or '←'})")
            
            # 스캔 단계 사이 대기
            time.sleep(2.0)
    
    def _move_to_pan_angle(self, angle):
        """팬 각도로 카메라 이동
        
        Args:
            angle: 이동할 팬 각도 (도)
            
        Returns:
            bool: 이동 성공 여부
        """
        try:
            # 팬틸트 컨트롤러를 통해 이동
            success = self.pantilt.move_to(pan_angle=angle)
            
            if success:
                # 하드웨어 위치 동기화는 모션 콜백을 통해 처리됨
                print(f"[BirdTracker] 팬 이동 요청: {angle:.1f}°")
            else:
                print(f"[BirdTracker] 팬 이동 실패: {angle:.1f}°")
                
            return success
        except Exception as e:
            print(f"[BirdTracker] 이동 명령 오류: {e}")
            return False
    
    def set_home_position(self):
        """현재 위치를 홈 포지션(0도)으로 설정"""
        with self.lock:
            # 현재 하드웨어 위치 가져오기
            position = self.pantilt.get_position()
            self.home_pan_angle = position["pan"]
            self.current_pan_angle = position["pan"]
            print(f"[BirdTracker] 홈 포지션이 {self.home_pan_angle}°로 설정됨")
            return True
    
    def get_tracking_status(self):
        """현재 추적 상태 정보 반환"""
        with self.lock:
            # 하드웨어 위치 동기화
            position = self.pantilt.get_position()
            self.current_pan_angle = position["pan"]
            
            return {
                "is_tracking": self.is_tracking,
                "is_scanning": self.is_scanning,
                "current_pan_angle": self.current_pan_angle,
                "home_pan_angle": self.home_pan_angle,
                "last_detection_time": self.last_detection_time,
                "relative_angle": self.current_pan_angle - self.home_pan_angle,
                "hardware_connected": position["connected"]
            }
    
    def calculate_distance(self, bird_width_pixels, frame_width):
        """조류 크기를 기반으로 거리 계산 (추정)
        
        Args:
            bird_width_pixels: 조류 bbox의 너비 (픽셀)
            frame_width: 프레임 너비 (픽셀)
            
        Returns:
            float: 추정 거리 (미터)
        """
        # 가정: 조류의 실제 너비를 약 30cm로 가정
        # 단순한 삼각법 사용 (실제 구현에서는 보정 필요)
        ASSUMED_BIRD_WIDTH = 0.3  # 미터
        
        try:
            # 조류가 차지하는 화각 비율 계산
            bird_angular_width = (bird_width_pixels / frame_width) * CAMERA_FOV
            
            # 각도를 라디안으로 변환
            bird_angular_width_rad = math.radians(bird_angular_width)
            
            # 거리 계산 (d = w / (2 * tan(θ/2)))
            if bird_angular_width_rad > 0:
                distance = ASSUMED_BIRD_WIDTH / (2 * math.tan(bird_angular_width_rad / 2))
                return distance
            else:
                return None
        except Exception as e:
            print(f"[BirdTracker] 거리 계산 오류: {e}")
            return None
    
    def calculate_relative_position(self, bird_center_x, frame_width):
        """조류의 상대적 위치와 방위각 계산
        
        Args:
            bird_center_x: 조류 중심점 X 좌표 (픽셀)
            frame_width: 프레임 너비 (픽셀)
            
        Returns:
            dict: 상대적 위치 정보
        """
        try:
            # 프레임 중심으로부터의 각도 계산
            rel_position = (bird_center_x / frame_width) * 2 - 1  # -1 ~ 1 범위
            frame_angle = rel_position * (CAMERA_FOV / 2)  # 카메라 화각의 절반 만큼 스케일링
            
            # 전체 방위각 계산 (현재 팬 각도 + 프레임 내 각도)
            azimuth = self.current_pan_angle + frame_angle
            
            return {
                "relative_position": rel_position,  # -1 ~ 1 범위
                "frame_angle": frame_angle,  # 프레임 내에서의 각도
                "azimuth": azimuth  # 전체 방위각
            }
        except Exception as e:
            print(f"[BirdTracker] 위치 계산 오류: {e}")
            return {
                "relative_position": 0,
                "frame_angle": 0,
                "azimuth": self.current_pan_angle
            }
    
    def enable_tracking(self, enabled=True):
        """추적 모드 활성화/비활성화"""
        with self.lock:
            was_enabled = self.is_tracking
            self.is_tracking = enabled
            
            if enabled and not was_enabled:
                print("[BirdTracker] 추적 모드 활성화")
                self.is_scanning = False
                self.last_track_time = time.time()
            elif not enabled and was_enabled:
                print("[BirdTracker] 추적 모드 비활성화")
                self.tracked_positions = []
            
            return True
    
    def enable_scanning(self, enabled=True):
        """스캔 모드 활성화/비활성화"""
        with self.lock:
            was_enabled = self.is_scanning
            self.is_scanning = enabled
            
            if enabled and not was_enabled:
                print("[BirdTracker] 스캔 모드 활성화")
                self.is_tracking = False
                self.scan_direction = 1
            elif not enabled and was_enabled:
                print("[BirdTracker] 스캔 모드 비활성화")
                self._return_to_home()
            
            return True
    
    def stop(self):
        """추적기 정지 및 자원 정리"""
        self.running = False
        if hasattr(self, 'tracking_thread') and self.tracking_thread.is_alive():
            self.tracking_thread.join(timeout=1.0)
            
        # 모션 콜백 제거
        self.pantilt.remove_motion_callback(self._on_motion_update)
        
        print("[BirdTracker] 정지됨")