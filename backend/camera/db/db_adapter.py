import asyncio
from concurrent.futures import ThreadPoolExecutor
import time
import logging
import threading
from datetime import datetime
from .async_detection_db import AsyncDetectionDatabase

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

class DetectionDatabaseAdapter:
    """비동기 DB를 동기식 코드에서 사용하기 위한 어댑터"""
    
    _instance = None
    _lock = threading.Lock()
    
    @classmethod
    def get_instance(cls, db_config=None):
        """싱글톤 인스턴스 반환"""
        with cls._lock:
            if cls._instance is None:
                cls._instance = cls(db_config)
            return cls._instance
    
    def __init__(self, db_config=None):
        self.async_db = AsyncDetectionDatabase(db_config)
        self.loop = None
        self.executor = ThreadPoolExecutor(max_workers=4)
        self.startup_time = time.time()
        self._loop_running = False
        
        # 이벤트 루프 시작
        self._start_background_loop()
    
    def _start_background_loop(self):
        """백그라운드 스레드에서 이벤트 루프 시작"""
        if not self._loop_running:
            thread = threading.Thread(target=self._run_event_loop, daemon=True)
            thread.start()
            # 루프가 시작될 때까지 짧게 대기
            time.sleep(0.1)
            
    def _run_event_loop(self):
        """이벤트 루프 실행"""
        try:
            self.loop = asyncio.new_event_loop()
            asyncio.set_event_loop(self.loop)
            self._loop_running = True
            logger.info("비동기 이벤트 루프 시작")
            self.loop.run_forever()
        except Exception as e:
            logger.error(f"이벤트 루프 오류: {e}")
        finally:
            self._loop_running = False
            logger.info("비동기 이벤트 루프 종료")
            
    def _run_async(self, coro):
        """코루틴을 동기적으로 실행"""
        try:
            if self.loop is None or not self._loop_running:
                logger.warning("이벤트 루프가 실행 중이 아닙니다. 재시작 중...")
                self._start_background_loop()
                
            future = asyncio.run_coroutine_threadsafe(coro, self.loop)
            return future.result(timeout=10)  # 10초 타임아웃 추가
        except asyncio.TimeoutError:
            logger.error("비동기 작업 타임아웃")
            return False
        except Exception as e:
            logger.error(f"비동기 작업 실행 중 오류 발생: {e}")
            # 심각한 오류 시 이벤트 루프 재시작
            if self._loop_running and self.loop and self.loop.is_closed():
                self._loop_running = False
                self._start_background_loop()
            return False
    
    def _map_detection_to_model_structure(self, detection):
        """DB 응답을 DetectionInfo 모델 구조에 맞게 변환"""
        try:
            detection_dict = dict(detection)
            
            # 필드 이름 매핑
            if 'camera_id_id' in detection_dict:
                detection_dict['camera_id'] = detection_dict['camera_id_id']
            
            return detection_dict
            
        except Exception as e:
            logger.error(f"감지 데이터 매핑 오류: {e}")
            return detection  # 오류 시 원본 반환
        
    def insert_detection(self, class_id, class_name, confidence, count, first_detected, camera_id):
        """동기식 인터페이스로 비동기 함수 호출"""
        start_time = time.time()
        result = self._run_async(self.async_db.insert_detection(
            class_id, class_name, confidence, count, first_detected, camera_id
        ))
        elapsed = time.time() - start_time
        if elapsed > 0.5:  # 500ms 이상 걸리면 경고 로그 출력
            logger.warning(f"insert_detection 작업이 {elapsed:.2f}초 소요되었습니다.")
        return result
    
    def batch_insert_detections(self, detections):
        """여러 감지 결과를 배치로 처리하는 동기식 인터페이스"""
        try:
            logger.info(f"batch_insert_detections 호출됨 - {len(detections)}개 감지")
            result = self._run_async(self.async_db.batch_insert_detections(detections))
            logger.info(f"batch_insert_detections 결과: {result}")
            return result
        except Exception as e:
            logger.error(f"batch_insert_detections 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return False
        
    def get_detections(self, limit=20, camera_id=None):
        """DetectionInfo 모델과 일치하는 구조로 데이터 반환"""
        try:
            db_detections = self._run_async(self.async_db.get_detections(limit, camera_id))
            # 모델 구조에 맞게 변환
            model_detections = [self._map_detection_to_model_structure(detection) for detection in db_detections]
            return model_detections
        except Exception as e:
            logger.error(f"get_detections 오류: {e}")
            return []
        
    def get_detection_stats(self, hours=24):
        """동기식 인터페이스로 비동기 함수 호출 - 통계 데이터 반환"""
        try:
            stats = self._run_async(self.async_db.get_detection_stats(hours))
            
            # 날짜 필드 표준화
            processed_stats = []
            for stat in stats:
                stat_dict = dict(stat)
                
                # 날짜 필드 처리
                for date_field in ['first_seen', 'last_seen']:
                    if date_field in stat_dict and stat_dict[date_field]:
                        if isinstance(stat_dict[date_field], datetime):
                            stat_dict[date_field] = stat_dict[date_field]
                        
                processed_stats.append(stat_dict)
            
            return processed_stats
        except Exception as e:
            logger.error(f"get_detection_stats 오류: {e}")
            return []
    
    def get_adapter_status(self):
        """어댑터 상태 정보 반환"""
        uptime = time.time() - self.startup_time
        
        try:
            query_stats = self._run_async(self.async_db.get_query_stats())
        except:
            query_stats = {"error": "통계를 가져올 수 없음"}
        
        return {
            'uptime_seconds': int(uptime),
            'uptime_formatted': f"{int(uptime // 3600)}시간 {int((uptime % 3600) // 60)}분 {int(uptime % 60)}초",
            'thread_pool_size': self.executor._max_workers,
            'query_stats': query_stats,
            'event_loop_running': self._loop_running
        }
    
    def save_detection_info(self, detection_data):
        """DetectionInfo 모델에 감지 정보를 저장합니다."""
        try:
            logger.info(f"save_detection_info 호출됨: {detection_data}")
            # 동기적으로 실행하기 위해 _run_async 사용
            result = self._run_async(self.async_db.save_detection_info(detection_data))
            logger.info(f"save_detection_info 결과: {result}")
            return result
        except Exception as e:
            logger.error(f"save_detection_info 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return None
    
    def save_bb_info(self, bb_data):
        """BBInfo 모델에 바운딩 박스 정보를 저장합니다."""
        try:
            logger.info(f"save_bb_info 호출됨: {bb_data}")
            # 동기적으로 실행하기 위해 _run_async 사용
            result = self._run_async(self.async_db.save_bb_info(bb_data))
            logger.info(f"save_bb_info 결과: {result}")
            return result
        except Exception as e:
            logger.error(f"save_bb_info 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return None
        
    def __del__(self):
        """객체 소멸 시 리소스 정리"""
        try:
            if self.loop and self._loop_running:
                # 루프 중지 및 풀 닫기
                asyncio.run_coroutine_threadsafe(self.async_db.close(), self.loop)
                self.loop.call_soon_threadsafe(self.loop.stop)
            
            if self.executor:
                self.executor.shutdown(wait=False)
                
            logger.info("DB 어댑터 자원 정리 완료")
        except Exception as e:
            logger.error(f"DB 어댑터 정리 중 오류: {e}")

# 전역 인스턴스 생성
db_adapter = DetectionDatabaseAdapter.get_instance()