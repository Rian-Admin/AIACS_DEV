from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
import logging

# 로그 설정
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

def azimuth_setting_view(request):
    """방위각 설정 페이지"""
    # 필요한 데이터 처리 로직 추가
    # 방위각 데이터를 DB에서 가져오는 코드가 들어갈 수 있음
    # 현재는 간단하게 템플릿만 렌더링
    return render(request, 'detection/azimuth_setting.html')