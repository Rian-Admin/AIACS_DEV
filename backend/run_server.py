import os
import sys

# 현재 디렉토리를 시스템 경로에 추가
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from waitress import serve
from config.wsgi import application
   
if __name__ == '__main__':
       serve(application, host='0.0.0.0', port=8000)