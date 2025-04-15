import os
import sys
import django
import logging
import traceback
from datetime import datetime

# Django 설정 모듈 설정
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'config.settings')
django.setup()

# 로깅 설정
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

# 필요한 모델 import
from django.utils import timezone
from camera.models import Camera, DetectionInfo, BBInfo, BirdClass

def test_direct_django_orm():
    """Django ORM을 직접 사용하여 레코드 생성"""
    try:
        print("Django ORM을 사용한 직접 테스트 시작...")
        
        # 1. 카메라 존재 확인
        camera_id = 1
        camera, created = Camera.objects.get_or_create(
            camera_id=camera_id,
            defaults={
                'wind_turbine_id': 'WT001',
                'viewing_angle': 120.0,
                'installation_direction': 'South',
                'installation_height': 10.0,
                'rtsp_address': 'rtsp://example.com/stream1',
                'status': 'active'
            }
        )
        
        if created:
            print(f"새 카메라 생성됨: {camera.camera_id}")
        else:
            print(f"기존 카메라 사용: {camera.camera_id}")
            
        # 2. 테스트용 DetectionInfo 레코드 생성
        detection_time = timezone.now()
        bb_count = 3
        
        detection_info = DetectionInfo.objects.create(
            camera_id=camera,
            detection_time=detection_time,
            bb_count=bb_count
        )
        
        print(f"DetectionInfo 생성 성공! ID: {detection_info.detection_id}")
        
        # 3. BirdClass 확인 (첫 번째 클래스 사용)
        bird_class = BirdClass.objects.first()
        if not bird_class:
            print("BirdClass가 없습니다. 테스트를 중단합니다.")
            return
            
        print(f"사용할 BirdClass: {bird_class.bird_name_ko} (ID: {bird_class.class_id})")
        
        # 4. BBInfo 레코드 생성
        for i in range(3):
            bb_info = BBInfo.objects.create(
                detection_info_id=detection_info,
                class_id=bird_class,
                bb_left=0.1 + (i * 0.1),
                bb_top=0.1 + (i * 0.1),
                bb_right=0.3 + (i * 0.1),
                bb_bottom=0.3 + (i * 0.1)
            )
            print(f"BBInfo {i+1} 생성 성공! ID: {bb_info.record_id}")
        
        print("테스트 완료! 데이터베이스에 레코드가 정상적으로 생성되었습니다.")
        
    except Exception as e:
        print(f"오류 발생: {e}")
        print(traceback.format_exc())
        
def test_psycopg_direct():
    """psycopg2를 사용하여 직접 SQL 쿼리로 삽입"""
    try:
        import psycopg2
        print("psycopg2 직접 SQL 테스트 시작...")
        
        # 연결 설정
        conn = psycopg2.connect(
            dbname="bird_detection",
            user="postgres",
            password="1234",
            host="localhost",
            port="5432"
        )
        
        # 자동 커밋 비활성화 (트랜잭션 사용)
        conn.autocommit = False
        
        try:
            with conn.cursor() as cursor:
                # DetectionInfo 삽입
                cursor.execute("""
                INSERT INTO camera_detectioninfo (camera_id_id, detection_time, bb_count)
                VALUES (1, %s, %s)
                RETURNING detection_id
                """, (datetime.now(), 2))
                
                detection_id = cursor.fetchone()[0]
                print(f"SQL 직접 삽입: 새 DetectionInfo ID = {detection_id}")
                
                # BirdClass ID 확인
                cursor.execute("SELECT class_id FROM camera_birdclass ORDER BY class_id LIMIT 1")
                bird_class_id = cursor.fetchone()[0]
                
                # BBInfo 삽입
                cursor.execute("""
                INSERT INTO camera_bbinfo (detection_info_id_id, class_id_id, bb_left, bb_top, bb_right, bb_bottom)
                VALUES (%s, %s, %s, %s, %s, %s)
                RETURNING record_id
                """, (detection_id, bird_class_id, 0.1, 0.1, 0.3, 0.3))
                
                record_id = cursor.fetchone()[0]
                print(f"SQL 직접 삽입: 새 BBInfo ID = {record_id}")
                
                # 트랜잭션 커밋
                conn.commit()
                print("트랜잭션 커밋 성공!")
                
        except Exception as e:
            conn.rollback()
            print(f"SQL 직접 삽입 오류: {e}")
            print(traceback.format_exc())
            
        finally:
            conn.close()
            
    except Exception as e:
        print(f"psycopg2 테스트 오류: {e}")
        print(traceback.format_exc())

def test_db_adapter():
    """DB 어댑터를 사용한 테스트"""
    try:
        from camera.db import db_adapter
        print("DB 어댑터 테스트 시작...")
        
        # 상태 확인
        adapter_status = db_adapter.get_adapter_status()
        print(f"DB 어댑터 상태: {adapter_status}")
        
        # DetectionInfo 저장
        detection_data = {
            'camera_id': 1, 
            'detection_time': timezone.now(),
            'bb_count': 5
        }
        
        result = db_adapter.save_detection_info(detection_data)
        
        if result:
            print(f"DB 어댑터: DetectionInfo 저장 성공! ID: {result.id}")
            
            # BBInfo 저장
            bb_data = {
                'detection_info_id_id': result.id,
                'bb_left': 0.2,
                'bb_top': 0.2,
                'bb_right': 0.5,
                'bb_bottom': 0.5,
                'class_id': BirdClass.objects.first().class_id,
                'class_name': 'test_bird'
            }
            
            bb_result = db_adapter.save_bb_info(bb_data)
            print(f"DB 어댑터: BBInfo 저장 결과: {bb_result}")
        else:
            print("DB 어댑터: DetectionInfo 저장 실패")
            
    except Exception as e:
        print(f"DB 어댑터 테스트 오류: {e}")
        print(traceback.format_exc())

if __name__ == "__main__":
    print("=" * 50)
    print("데이터베이스 삽입 테스트 시작...")
    print("=" * 50)
    
    print("\n1. Django ORM 직접 사용 테스트")
    test_direct_django_orm()
    
    print("\n2. psycopg2 직접 SQL 테스트")
    test_psycopg_direct()
    
    print("\n3. DB 어댑터 테스트")
    test_db_adapter()
    
    print("\n" + "=" * 50)
    print("테스트 완료!")
    print("=" * 50) 