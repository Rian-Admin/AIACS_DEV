import os
from pathlib import Path
import django_heroku
import sys
from datetime import datetime

# Build paths inside the project like this: BASE_DIR / 'subdir'.
BASE_DIR = Path(__file__).resolve().parent.parent

# 로그 디렉토리 생성
log_dir = os.path.join(BASE_DIR, 'logs')
if not os.path.exists(log_dir):
    os.makedirs(log_dir)

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

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(BASE_DIR)

# settings.py 파일의 LOGGING 부분을 아래와 같이 수정하거나 추가합니다

LOGGING = {
    'version': 1,
    'disable_existing_loggers': True,
    'formatters': {
        'verbose': {
            'format': '{levelname} {asctime} {module} {message}',
            'style': '{',
        },
        'simple': {
            'format': '{levelname} {message}',
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
            'level': 'ERROR',
            'class': 'logging.StreamHandler',
            'filters': ['suppress_opencv_ffmpeg'],
            'formatter': 'simple',
        },
        'file': {
            'level': 'ERROR',
            'class': 'logging.FileHandler',
            'filename': os.path.join(log_dir, 'error.log'),
            'filters': ['suppress_opencv_ffmpeg'],
            'formatter': 'verbose',
        },
    },
    'loggers': {
        '': {
            'handlers': ['null'],
            'level': 'CRITICAL',
            'propagate': False,
        },
        'django': {
            'handlers': ['console', 'file'],
            'level': 'ERROR',
            'propagate': False,
        },
    },
}