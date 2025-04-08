from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from django.utils import timezone

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
                    {"error": "날씨 정보를 가져올 수 없습니다."},
                    status=status.HTTP_404_NOT_FOUND
                )
        
        # 날씨 데이터를 JSON 형식으로 변환
        weather_info = {
            "current": {
                "temperature": weather_data.temperature,
                "humidity": weather_data.humidity,
                "wind_speed": weather_data.wind_speed,
                "wind_direction": weather_data.wind_direction,
                "precipitation": weather_data.precipitation,
                "precipitation_type": weather_data.precipitation_type,
                "air_quality": weather_data.air_quality
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
            {"error": f"서버 오류: {str(e)}"},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR
        )

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
