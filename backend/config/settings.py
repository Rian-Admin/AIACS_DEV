import os
from pathlib import Path
import sys
from datetime import datetime

# Build paths inside the project like this: BASE_DIR / 'subdir'.
BASE_DIR = Path(__file__).resolve().parent.parent

# 로그 디렉토리 생성
log_dir = os.path.join(BASE_DIR, 'logs')
if not os.path.exists(log_dir):
    os.makedirs(log_dir)

# 비디오 녹화 디렉토리 생성
video_recordings_dir = os.path.join(BASE_DIR, 'video_recordings')
if not os.path.exists(video_recordings_dir):
    os.makedirs(video_recordings_dir)

# 기본 설정
SECRET_KEY = 'd=j7y_#hw(dx315rzsmr@iqmk+d#jkvyus60)$by7=vyl=v*s^'
DEBUG = True  # 디버그 모드 활성화
ALLOWED_HOSTS = ['*']  # 모든 호스트 허용
TIMEOUT = 60
# 앱 설정
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'corsheaders',
    'camera',
    'radar_api', 
    'rest_framework',
    'weather',
]

# 미들웨어 설정
MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
    'corsheaders.middleware.CorsMiddleware',
]

# 서버 포트 설정
PORT = int(os.environ.get('PORT', 8000))  # Django 서버는 8000번 포트 사용

# CORS 설정 - 개발 및 프로덕션 환경 모두 지원
CORS_ALLOW_ALL_ORIGINS = True  # 개발 단계에서는 모든 오리진 허용
CORS_ALLOWED_ORIGINS = [
    "http://localhost:3000",
    "http://127.0.0.1:3000",
    "http://192.168.219.215:3000",
]

CORS_ALLOW_CREDENTIALS = True
CORS_ALLOW_METHODS = [
    'DELETE',
    'GET',
    'OPTIONS',
    'PATCH',
    'POST',
    'PUT',
]

# 비디오 스트리밍에 필요한 헤더 허용
CORS_ALLOW_HEADERS = [
    'accept',
    'accept-encoding',
    'authorization',
    'content-type',
    'dnt',
    'origin',
    'user-agent',
    'x-csrftoken',
    'x-requested-with',
    'range',  # 범위 요청을 위한 헤더 허용
]

# 응답에 포함될 수 있는 헤더 설정
CORS_EXPOSE_HEADERS = [
    'accept-ranges',
    'content-length',
    'content-range',
    'content-type',
    'content-disposition',
]

# X-Frame-Options 설정 (iframe 허용)
X_FRAME_OPTIONS = 'SAMEORIGIN'

# CSRF 설정
CSRF_TRUSTED_ORIGINS = [
    "http://localhost:3000",
    "http://127.0.0.1:3000",
    "http://192.168.219.215:3000",
]

# CSRF 설정
CSRF_COOKIE_NAME = "csrftoken"
CSRF_COOKIE_HTTPONLY = False  # JavaScript에서 접근 가능하도록

# URL 설정
ROOT_URLCONF = 'config.urls'

# 템플릿 설정
TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [os.path.join(BASE_DIR, 'templates')],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]

# WSGI 설정
WSGI_APPLICATION = 'config.wsgi.application'

# 데이터베이스 설정
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.postgresql',
        'NAME': 'bird_detection',
        'USER': 'postgres',
        'PASSWORD': '1234',
        'HOST': '127.0.0.1',
        'PORT': '5432',
        'OPTIONS': {
            'client_encoding': 'UTF8',
        }
    }
}

# 인증 설정
AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',
    },
]

# 국제화 설정
LANGUAGE_CODE = 'en-us'

# 시간 관련 설정
def get_safe_year():
    try:
        year = datetime.now().year
        return year if year >= 2000 else 2024
    except:
        return 2024

USE_TZ = True
TIME_ZONE = 'Asia/Seoul'
SAFE_YEAR = get_safe_year()

# 정적 파일 설정
STATICFILES_DIRS = [
    os.path.join(BASE_DIR, 'static'),  # 실제 정적 파일이 있는 경로로 수정
]
STATIC_URL = '/static/'

# 미디어 파일 설정 (업로드 및 녹화된 비디오)
MEDIA_URL = '/media/'
MEDIA_ROOT = BASE_DIR

# 비디오 MIME 타입 설정 추가
FILE_UPLOAD_PERMISSIONS = 0o644
# MIME 타입 맵핑 설정
MIME_TYPES = {
    'mp4': 'video/mp4',
    'webm': 'video/webm',
    'ogg': 'video/ogg',
    'avi': 'video/x-msvideo',
    'mov': 'video/quicktime',
    'wmv': 'video/x-ms-wmv',
    'flv': 'video/x-flv',
    'mkv': 'video/x-matroska',
    'mpg': 'video/mpeg',
    'mpeg': 'video/mpeg',
}

# 정적 파일 서빙 설정
STATICFILES_FINDERS = [
    'django.contrib.staticfiles.finders.FileSystemFinder',
    'django.contrib.staticfiles.finders.AppDirectoriesFinder',
]

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(BASE_DIR)

# settings.py 파일의 LOGGING 부분을 아래와 같이 수정하거나 추가합니다

LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,  # 기존 로거를 비활성화하지 않음
    'formatters': {
        'verbose': {
            'format': '{levelname} {asctime} {module} {message}',
            'style': '{',
        },
        'simple': {
            'format': '{levelname} {message}',
            'style': '{',
        },
        'detailed': {
            'format': '{levelname} {asctime} {module} {process:d} {thread:d} {message}',
            'style': '{',
        },
    },
    'filters': {
        'suppress_opencv_ffmpeg': {
            '()': 'django.utils.log.CallbackFilter',
            'callback': lambda record: 'h264' not in record.getMessage() and 'opencv' not in record.getMessage().lower(),
        },
    },
    'handlers': {
        'null': {
            'class': 'logging.NullHandler',
        },
        'console': {
            'level': 'DEBUG',  # 레벨을 DEBUG로 변경
            'class': 'logging.StreamHandler',
            'filters': ['suppress_opencv_ffmpeg'],
            'formatter': 'detailed',
        },
        'file': {
            'level': 'DEBUG',  # 레벨을 DEBUG로 변경
            'class': 'logging.FileHandler',
            'filename': os.path.join(log_dir, 'error.log'),
            'filters': ['suppress_opencv_ffmpeg'],
            'formatter': 'verbose',
        },
        'camera_console': {
            'level': 'DEBUG',
            'class': 'logging.StreamHandler',
            'formatter': 'detailed',
        },
        'camera_file': {
            'level': 'DEBUG',
            'class': 'logging.FileHandler',
            'filename': os.path.join(log_dir, 'camera.log'),
            'formatter': 'detailed',
        },
    },
    'loggers': {
        '': {
            'handlers': ['console', 'file'],  # 기본 핸들러 추가
            'level': 'WARNING',
            'propagate': True,
        },
        'django': {
            'handlers': ['console', 'file'],
            'level': 'WARNING',
            'propagate': False,
        },
        'camera': {
            'handlers': ['camera_console', 'camera_file'],
            'level': 'DEBUG',
            'propagate': False,
        },
        'camera.views.camera_management': {
            'handlers': ['camera_console', 'camera_file'],
            'level': 'DEBUG',
            'propagate': False,
        },
    },
}