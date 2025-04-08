import datetime
import asyncio
import asyncpg
from typing import List, Dict, Optional, Any, Union
from functools import wraps
import time
import logging
from django.utils import timezone
import traceback
import sys

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

def measure_async_performance(func):
    """비동기 함수의 실행 시간을 측정하는 데코레이터"""
    @wraps(func)
    async def wrapper(*args, **kwargs):
        start_time = time.time()
        result = await func(*args, **kwargs)
        elapsed_time = time.time() - start_time
        
        if args and hasattr(args[0], '__class__'):
            class_name = args[0].__class__.__name__
            func_name = f"{class_name}.{func.__name__}"
        else:
            func_name = func.__name__
            
        print(f"[성능] {func_name} 실행 시간: {elapsed_time:.4f}초")
        return result
    return wrapper

def async_retry(attempts=3, delay=1, backoff=2):
    """비동기 함수에 재시도 로직을 추가하는 데코레이터"""
    def decorator(func):
        @wraps(func)
        async def wrapper(*args, **kwargs):
            last_exception = None
            for attempt in range(1, attempts + 1):
                try:
                    return await func(*args, **kwargs)
                except (asyncpg.exceptions.ConnectionDoesNotExistError,
                        asyncpg.exceptions.InterfaceError,
                        asyncpg.exceptions.TooManyConnectionsError) as e:
                    last_exception = e
                    if attempt < attempts:
                        wait_time = delay * (backoff ** (attempt - 1))
                        print(f"데이터베이스 연결 오류: {e}. {wait_time}초 후 재시도 ({attempt}/{attempts})")
                        await asyncio.sleep(wait_time)
                    else:
                        break
            raise last_exception
        return wrapper
    return decorator

class AsyncDetectionDatabase:
    """비동기 데이터베이스 연결과 처리를 담당하는 클래스"""
    
    def __init__(self, db_config=None):
        if db_config is None:
            self.db_config = {
                'database': 'bird_detection',
                'user': 'postgres',
                'password': '1234',
                'host': 'localhost',
                'port': 5432
            }
        else:
            # asyncpg는 'dbname' 대신 'database'를 사용합니다
            if 'dbname' in db_config:
                db_config['database'] = db_config.pop('dbname')
            self.db_config = db_config
            
        # 연결 풀 저장용 변수
        self._pool = None
        # 성능 측정용 변수
        self.query_counts = {
            'insert': 0,
            'select': 0,
            'total': 0
        }
        
    async def _get_pool(self) -> asyncpg.Pool:
        """연결 풀을 가져오거나 생성합니다."""
        if self._pool is None:
            try:
                # 연결 풀 생성 - 여러 동시 연결을 효율적으로 관리
                self._pool = await asyncpg.create_pool(
                    **self.db_config, 
                    min_size=5,     # 최소 연결 수
                    max_size=20,    # 최대 연결 수
                    command_timeout=60.0,       # 명령 실행 타임아웃 (초)
                    max_inactive_connection_lifetime=300.0,  # 비활성 연결 수명 (초)
                    max_queries=50000           # 연결당 최대 쿼리 수
                )
                print(f"PostgreSQL 연결 풀이 성공적으로 생성되었습니다. (min={5}, max={20})")
            except Exception as e:
                print(f"Database pool creation error: {e}")
                raise
        return self._pool
    
    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def insert_detection(self, class_id: int, class_name: str, confidence: float, 
                              count: int, first_detected: datetime.datetime, camera_id: int) -> bool:
        """감지 결과를 데이터베이스에 비동기적으로 저장합니다."""
        try:
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                # 수정된 쿼리 - camera_detectioninfo 테이블 사용
                query = """
                INSERT INTO camera_detectioninfo 
                (detection_time, bb_count, camera_id_id)
                VALUES ($1, $2, $3)
                """
                # 타임존 인식 datetime 사용
                detection_time = timezone.now()
                
                # 쿼리 실행
                await conn.execute(query, detection_time, count, camera_id)
                return True
                
        except Exception as e:
            print(f"Error inserting detection: {e}")
            import traceback
            print(traceback.format_exc())
            return False
    
    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def batch_insert_detections(self, detections: List[Dict]) -> bool:
        """여러 개의 감지 결과를 배치로 처리합니다."""
        if not detections:
            return True  # 빈 목록은 성공으로 처리
            
        try:
            print(f"비동기 batch_insert_detections 호출됨 - {len(detections)}개 감지")
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                # 수정된 배치 삽입 쿼리 - media_storage_path 포함
                query = """
                INSERT INTO camera_detectioninfo 
                (detection_time, bb_count, camera_id_id)
                SELECT d.first_detected, d.count, d.camera_id
                FROM unnest($1::timestamptz[], $2::int[], $3::text[]) 
                AS d(first_detected, count, camera_id)
                """
                
                # 타임존 인식 datetime 사용
                current_time = timezone.now()
                
                # 데이터 변환
                first_detected_times = [d['first_detected'] for d in detections]
                counts = [d['count'] for d in detections]
                
                # 카메라 ID 문자열 변환
                camera_ids = [str(d['camera_id']) for d in detections]
                
                # 배치 실행 전 데이터 로깅
                print(f"비동기 배치 삽입 준비 - 카메라: {camera_ids}")
                for i, d in enumerate(detections):
                    print(f"감지 {i+1}: 카메라={d['camera_id']}, 시간={first_detected_times[i]}")
                
                # 배치 실행
                await conn.execute(query, first_detected_times, counts, camera_ids)
                
                # 쿼리 카운트 증가
                self.query_counts['insert'] += 1
                self.query_counts['total'] += 1
                
                print(f"비동기 배치 삽입 성공 - {len(detections)}개 감지")
                return True
                    
        except Exception as e:
            print(f"Error in batch insert: {e}")
            print(traceback.format_exc())
            return False
    
    # camera_detectioninfo와 camera_birdclass를 사용하는 쿼리 수정

    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def get_detections(self, limit: int = 20, camera_id: Optional[int] = None) -> List[Dict[str, Any]]:
        """최근 감지 결과를 가져옵니다."""
        try:
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                if camera_id:
                    query = """
                    SELECT d.* 
                    FROM camera_detectioninfo d
                    WHERE d.camera_id_id = $1
                    ORDER BY d.detection_time DESC LIMIT $2
                    """
                    rows = await conn.fetch(query, camera_id, limit)
                else:
                    query = """
                    SELECT d.* 
                    FROM camera_detectioninfo d
                    ORDER BY d.detection_time DESC LIMIT $1
                    """
                    rows = await conn.fetch(query, limit)
                
                # 결과를 사전 형태로 변환
                result = []
                for row in rows:
                    row_dict = dict(row)
                    result.append(row_dict)
                    
                return result
                
        except Exception as e:
            print(f"Error retrieving detections: {e}")
            import traceback
            print(traceback.format_exc())
            return []

    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def get_detections_optimized(self, limit: int = 20, camera_id: Optional[int] = None) -> List[Dict[str, Any]]:
        """최적화된 감지 결과 조회 (불필요한 조인 제외)"""
        try:
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                if camera_id:
                    query = """
                    SELECT 
                        d.detection_id,
                        d.detection_time,
                        d.bb_count,
                        d.camera_id_id,
                        c.camera_id
                    FROM camera_detectioninfo d
                    LEFT JOIN camera_camera c ON d.camera_id_id = c.camera_id
                    WHERE d.camera_id_id = $1
                    ORDER BY d.detection_time DESC
                    LIMIT $2
                    """
                    rows = await conn.fetch(query, camera_id, limit)
                else:
                    query = """
                    SELECT 
                        d.detection_id,
                        d.detection_time,
                        d.bb_count,
                        d.camera_id_id,
                        c.camera_id
                    FROM camera_detectioninfo d
                    LEFT JOIN camera_camera c ON d.camera_id_id = c.camera_id
                    ORDER BY d.detection_time DESC
                    LIMIT $1
                    """
                    rows = await conn.fetch(query, limit)
                    
                # 결과를 사전 형태로 변환
                result = []
                for row in rows:
                    result.append(dict(row))
                    
                return result
                
        except Exception as e:
            print(f"Error retrieving optimized detections: {e}")
            import traceback
            print(traceback.format_exc())
            return []

    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def get_detection_stats(self, hours: int = 24) -> List[Dict[str, Any]]:
        """기간별 감지 통계를 가져옵니다."""
        try:
            pool = await self._get_pool()
            
            # 지정된 시간 이전 기준 시간
            hours_ago = timezone.now() - datetime.timedelta(hours=hours)
            
            async with pool.acquire() as conn:
                query = """
                SELECT 
                    d.camera_id_id,
                    c.camera_id as camera_name,
                    COUNT(d.detection_id) as detection_count,
                    MIN(d.detection_time) AS first_seen,
                    MAX(d.detection_time) AS last_seen
                FROM camera_detectioninfo d
                LEFT JOIN camera_camera c ON d.camera_id_id = c.camera_id
                WHERE d.detection_time >= $1
                GROUP BY d.camera_id_id, c.camera_id
                ORDER BY detection_count DESC
                """
                rows = await conn.fetch(query, hours_ago)
                
                # 결과 변환
                result = []
                for row in rows:
                    result.append(dict(row))
                    
                return result
                
        except Exception as e:
            print(f"Error retrieving detection stats: {e}")
            import traceback
            print(traceback.format_exc())
            return []
    
    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def save_detection_info(self, detection_data: Dict) -> Any:
        """DetectionInfo 모델에 감지 정보를 저장합니다."""
        try:
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                # 파라미터 추출
                try:
                    camera_id = int(detection_data.get('camera_id', 1))
                except (ValueError, TypeError):
                    camera_id = 1  # 변환 실패시 기본값 1 사용
                detection_time = detection_data.get('detection_time', timezone.now())
                bb_count = detection_data.get('bb_count', 0)
                
                # 카메라 ID가 존재하는지 먼저 확인
                check_camera_query = """
                SELECT camera_id FROM camera_camera 
                WHERE camera_id = $1
                """
                camera_exists = await conn.fetchrow(check_camera_query, camera_id)
                
                # 카메라 ID가 존재하지 않으면
                if not camera_exists:
                    logger.warning(f"카메라 ID {camera_id}가 존재하지 않습니다. 대체 카메라를 검색합니다.")
                    
                    # 존재하는 첫 번째 카메라 ID 가져오기
                    fallback_query = """
                    SELECT camera_id FROM camera_camera 
                    LIMIT 1
                    """
                    fallback_camera = await conn.fetchrow(fallback_query)
                    
                    if fallback_camera:
                        camera_id = fallback_camera['camera_id']
                        logger.info(f"대체 카메라 ID {camera_id}를 사용합니다.")
                    else:
                        logger.error("데이터베이스에 카메라가 없습니다. 감지 정보를 저장할 수 없습니다.")
                        return None
                
                # camera_detectioninfo 테이블에 데이터 삽입
                query = """
                INSERT INTO camera_detectioninfo 
                (camera_id_id, detection_time, bb_count)
                VALUES ($1, $2, $3)
                RETURNING detection_id
                """
                
                # 쿼리 실행
                row = await conn.fetchrow(query, camera_id, detection_time, bb_count)
                
                # 쿼리 카운트 증가
                self.query_counts['insert'] += 1
                self.query_counts['total'] += 1
                
                # 삽입된 레코드의 ID 및 기타 정보 반환
                if row:
                    detection_id = row['detection_id']
                    logger.info(f"DetectionInfo가 ID {detection_id}로 저장되었습니다.")
                    
                    # 객체 형태로 반환하기 위해 SimpleNamespace 사용
                    from types import SimpleNamespace
                    result = SimpleNamespace()
                    result.id = detection_id
                    result.camera_id = camera_id
                    result.detection_time = detection_time
                    result.bb_count = bb_count
                    return result
                
                return None
                
        except Exception as e:
            logger.error(f"감지 정보 저장 오류: {e}")
            print(traceback.format_exc())
            return None
    
    @async_retry(attempts=3, delay=1, backoff=2)
    @measure_async_performance
    async def save_bb_info(self, bb_data: Dict) -> bool:
        """BBInfo 모델에 바운딩 박스 정보를 저장합니다."""
        try:
            pool = await self._get_pool()
            
            async with pool.acquire() as conn:
                # 먼저 유효한 class_id인지 확인 (BirdClass 테이블에 존재하는지)
                check_query = """
                SELECT class_id FROM camera_birdclass 
                WHERE class_id = $1
                """
                
                # 문자열 class_id를 정수로 변환
                try:
                    class_id = int(bb_data.get('class_id', 1))  # 기본값으로 1 사용 (첫 번째 클래스)
                except (ValueError, TypeError):
                    class_id = 1  # 변환 실패시 기본값 1 사용
                
                # 존재하는 class_id인지 확인
                valid_class = await conn.fetchrow(check_query, class_id)
                
                # 존재하지 않는 class_id면 기본값 1 사용
                if not valid_class:
                    logger.warning(f"class_id {class_id}가 BirdClass 테이블에 존재하지 않아 기본값 1로 대체합니다.")
                    class_id = 1  # 기본값 1 사용
                
                # detection_id 확인 - NULL이 아닌지 검증
                detection_id = bb_data.get('detection_id')
                if detection_id is None:
                    logger.error(f"detection_id가 None입니다. BBInfo를 저장할 수 없습니다.")
                    print(f"오류: BBInfo 저장 실패 - detection_id가 NULL입니다. bb_data: {bb_data}")
                    
                    # 누락된 detection_id가 있으면 새로운 detection_info 레코드 생성
                    # 이 과정에서 카메라 ID 유효성이 검증됨
                    camera_id = bb_data.get('camera_id', 1)
                    detection_data = {
                        'camera_id': camera_id,
                        'detection_time': timezone.now(),
                        'bb_count': 1
                    }
                    logger.info(f"카메라 ID {camera_id}로 새 감지 정보를 생성합니다. 존재하지 않는 카메라인 경우 대체 카메라가 사용됩니다.")
                    detection_result = await self.save_detection_info(detection_data)
                    if detection_result:
                        detection_id = detection_result.id
                        logger.info(f"누락된 detection_id를 위한 새 DetectionInfo 생성됨: {detection_id}, 사용된 카메라 ID: {detection_result.camera_id}")
                    else:
                        logger.error("대체 DetectionInfo를 생성할 수 없습니다. 유효한 카메라가 없을 수 있습니다.")
                        return False
                
                # camera_bbinfo 테이블에 데이터 삽입 - DB 테이블 구조와 맞게 수정
                query = """
                INSERT INTO camera_bbinfo 
                (detection_info_id_id, bb_left, bb_top, bb_right, bb_bottom, class_id_id)
                VALUES ($1, $2, $3, $4, $5, $6)
                """
                
                # 파라미터 추출
                left = bb_data.get('left', 0)
                top = bb_data.get('top', 0)
                right = bb_data.get('right', 0)
                bottom = bb_data.get('bottom', 0)
                
                # 쿼리 실행
                try:
                    await conn.execute(query, detection_id, left, top, right, bottom, class_id)
                    
                    # 쿼리 카운트 증가
                    self.query_counts['insert'] += 1
                    self.query_counts['total'] += 1
                    
                    logger.info(f"BBInfo가 저장되었습니다. (Detection ID: {detection_id}, Class ID: {class_id})")
                    return True
                except asyncpg.exceptions.ForeignKeyViolationError as e:
                    logger.error(f"외래 키 제약조건 오류: {e}")
                    # 외래 키 오류가 발생해도 프로그램 계속 실행
                    return False
                
        except Exception as e:
            logger.error(f"바운딩 박스 정보 저장 오류: {e}")
            print(traceback.format_exc())
            return False
    
    async def get_query_stats(self) -> Dict[str, int]:
        """쿼리 통계 정보를 반환합니다."""
        return self.query_counts