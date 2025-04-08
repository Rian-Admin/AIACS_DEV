#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import sys
import warnings

# FFmpeg 경고 무시 설정
os.environ["OPENCV_FFMPEG_LOGLEVEL"] = "0"
warnings.filterwarnings("ignore", category=UserWarning)

# 표준 에러 리디렉션 (h264 디코더 경고 메시지 숨기기)
if __name__ == '__main__':
    # 로그 파일 생성, w+로 파일을 열어 기존 내용 삭제
    if not os.path.exists('logs'):
        os.makedirs('logs')
    # stderr 리디렉션 임시 비활성화 (디버깅용)
    # sys.stderr = open('logs/stderr.log', 'w+')

def main():
    """Run administrative tasks."""
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'config.settings')
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        ) from exc
    execute_from_command_line(sys.argv)


if __name__ == '__main__':
    main()
