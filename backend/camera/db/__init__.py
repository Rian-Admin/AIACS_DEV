# 데이터베이스 어댑터 패키지 초기화
import threading
from .db_adapter import DetectionDatabaseAdapter

# 싱글톤 인스턴스 사용
db_adapter = DetectionDatabaseAdapter.get_instance()

# 이벤트 루프는 이미 _start_background_loop에서 자동으로 시작됨
# 더 이상 명시적으로 스레드 시작할 필요 없음