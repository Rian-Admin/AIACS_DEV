from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from django.utils import timezone
import datetime

from .models import WeatherData
from .utils import get_latest_weather, update_weather_database

import json
import logging

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

@api_view(['GET'])
def get_weather(request):
    """
    최신 날씨 정보를 반환하는 API 엔드포인트
    """
    try:
        # 최신 날씨 정보 가져오기
        weather_data = get_latest_weather()
        
        if not weather_data:
            # 날씨 정보가 없으면 새로 가져오기 시도
            update_success = update_weather_database()
            if update_success:
                weather_data = get_latest_weather()
            
            # 여전히 데이터가 없으면 에러 반환
            if not weather_data:
                return Response(
                    {"error": "날씨 정보를 가져올 수 없습니다. 기상청 API 연결을 확인하세요."},
                    status=status.HTTP_404_NOT_FOUND
                )
        
        # 현재 시간 포맷팅 (한국 시간 기준)
        now = timezone.localtime()
        formatted_time = now.strftime("%Y. %m. %d. %p %I:%M:%S").replace('AM', '오전').replace('PM', '오후')
        
        # 날씨 상태 결정 (습도, 강수량 기반)
        weather_status = determine_weather_status(weather_data)
        
        # 시정 값 디버깅 (추가)
        print(f"API 반환 시정 값: {weather_data.visibility} km (타입: {type(weather_data.visibility).__name__})")
        logger.info(f"API 반환 시정 값: {weather_data.visibility} km (타입: {type(weather_data.visibility).__name__})")
        
        # 시정 값은 데이터베이스에서 반환된 그대로 사용
        visibility = weather_data.visibility
        
        # 날씨 데이터를 JSON 형식으로 변환
        weather_info = {
            "current": {
                "temperature": weather_data.temperature,
                "feels_like": weather_data.feels_like,
                "humidity": weather_data.humidity,
                "wind_speed": weather_data.wind_speed,
                "wind_direction": weather_data.wind_direction,
                "precipitation": weather_data.precipitation,
                "precipitation_type": weather_data.precipitation_type,
                "air_quality": weather_data.air_quality,
                "visibility": visibility,
                "weather_status": weather_status,  # 날씨 상태 추가 (아이콘 표시용)
                "observation_station": weather_data.location_name,  # 관측소 정보 추가
                "formatted_time": formatted_time,  # 포맷된 시간 추가
            },
            "forecast": {
                "temperature": weather_data.forecast_temperature,
                "humidity": weather_data.forecast_humidity,
                "wind_speed": weather_data.forecast_wind_speed,
                "precipitation_probability": weather_data.forecast_precipitation,
                "air_quality": weather_data.forecast_air_quality
            },
            "timestamp": weather_data.timestamp.isoformat(),
            "update_time": weather_data.update_time.isoformat() if weather_data.update_time else None
        }
        
        return Response(weather_info)
        
    except Exception as e:
        logger.error(f"날씨 정보 API 오류: {str(e)}")
        return Response(
            {"error": f"날씨 데이터 서비스 오류: {str(e)}"},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )

def determine_weather_status(weather_data):
    """
    날씨 데이터를 기반으로 현재 날씨 상태를 결정합니다.
    (맑음, 흐림, 비, 눈 등)
    """
    # 1. 강수 타입 확인
    if weather_data.precipitation_type and weather_data.precipitation_type not in ['없음', '', None]:
        return weather_data.precipitation_type
    
    # 2. 일강수량 확인 (0mm 초과인 경우 비로 간주)
    if weather_data.precipitation is not None and weather_data.precipitation > 0:
        return '비'
    
    # 3. 습도 확인
    if weather_data.humidity is not None and weather_data.humidity >= 80:
        return '흐림'
    
    # 4. 기본값
    return '맑음'

@api_view(['POST'])
@csrf_exempt
def update_weather(request):
    """
    날씨 정보를 수동으로 업데이트하는 API 엔드포인트
    """
    try:
        # 날씨 데이터 업데이트 시도
        update_success = update_weather_database()
        
        if update_success:
            return Response({"message": "날씨 정보가 성공적으로 업데이트되었습니다."})
        else:
            return Response(
                {"error": "날씨 정보 업데이트에 실패했습니다."},
                status=status.HTTP_500_INTERNAL_SERVER_ERROR
            )
            
    except Exception as e:
        logger.error(f"날씨 업데이트 API 오류: {str(e)}")
        return Response(
            {"error": f"서버 오류: {str(e)}"},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )

@api_view(['GET'])
def get_weather_history(request):
    """
    날씨 정보 이력을 반환하는 API 엔드포인트
    """
    try:
        # 최근 24시간 날씨 데이터 쿼리 (최대 24개 항목)
        weather_history = WeatherData.objects.order_by('-timestamp')[:24]
        
        # 날씨 데이터를 JSON 형식으로 변환
        history_data = []
        for weather in weather_history:
            history_data.append({
                "temperature": weather.temperature,
                "humidity": weather.humidity,
                "wind_speed": weather.wind_speed,
                "precipitation": weather.precipitation,
                "timestamp": weather.timestamp.isoformat()
            })
        
        return Response(history_data)
        
    except Exception as e:
        logger.error(f"날씨 이력 API 오류: {str(e)}")
        return Response(
            {"error": f"서버 오류: {str(e)}"},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )
