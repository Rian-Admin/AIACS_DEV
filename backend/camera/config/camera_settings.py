"""
카메라 관련 전역 설정값을 관리하는 모듈입니다.
이 파일에서 정의된 설정값들은 프로젝트 전체에서 일관되게 사용됩니다.
"""

# FPS 관련 설정
FPS_LIMIT = 10  # 카메라 및 비디오 스트림의 최대 FPS
RTSP_FPS = 10   # RTSP 스트림의 FPS 설정
VIDEO_FPS = 10  # 비디오 파일 재생 FPS

# 버퍼 관련 설정
FRAME_BUFFER_SIZE = 2  # 프레임 버퍼의 최대 크기

# 화질 및 크기 관련 설정
DEFAULT_STREAM_QUALITY = 80  # 기본 스트림 JPEG 품질(0-100)
HIGH_QUALITY = 92            # 고품질 스트림 JPEG 품질(0-100)
DEFAULT_FRAME_WIDTH = 640    # 기본 프레임 가로 크기
DEFAULT_FRAME_HEIGHT = 480   # 기본 프레임 세로 크기

# DB 업데이트 관련 설정
DB_UPDATE_INTERVAL = 5  # DB 업데이트 간격 (초)

# 기타 설정
FRAME_PROCESSING_INTERVAL = 0.033  # 프레임 처리 간격 (약 30FPS에 해당, 1/30초)
CHECK_INTERVAL = 0.2  # 결과 체크 간격 (초)