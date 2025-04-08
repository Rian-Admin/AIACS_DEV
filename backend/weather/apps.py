from django.apps import AppConfig
import logging

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False


class WeatherConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'weather'
    
    def ready(self):
        """
        Django 앱이 시작될 때 호출됩니다.
        현재는 모든 초기화 작업이 비활성화되었습니다.
        """
        # 모든 코드를 주석 처리하여 아무것도 실행되지 않도록 함
        pass
