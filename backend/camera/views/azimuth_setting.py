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

@csrf_exempt
def add_azimuth(request):
    """방위각 추가 API"""
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            
            # 여기에 방위각 데이터를 DB에 저장하는 코드 추가
            # 실제 데이터베이스 모델이 구현되면 아래 코드를 적절히 수정
            
            logger.info(f"방위각 추가됨: {data.get('target_id')}")
            return JsonResponse({'status': 'success'})
        except Exception as e:
            logger.error(f"방위각 추가 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def update_azimuth(request, azimuth_id):
    """방위각 업데이트 API"""
    if request.method in ['PUT', 'POST']:
        try:
            data = json.loads(request.body)
            
            # 여기에 방위각 데이터를 DB에서 업데이트하는 코드 추가
            # 실제 데이터베이스 모델이 구현되면 아래 코드를 적절히 수정
            
            logger.info(f"방위각 업데이트됨: {azimuth_id}")
            return JsonResponse({'status': 'success'})
        except Exception as e:
            logger.error(f"방위각 업데이트 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def delete_azimuth(request, azimuth_id):
    """방위각 삭제 API"""
    if request.method in ['DELETE', 'POST']:
        try:
            # 여기에 방위각 데이터를 DB에서 삭제하는 코드 추가
            # 실제 데이터베이스 모델이 구현되면 아래 코드를 적절히 수정
            
            logger.info(f"방위각 삭제됨: {azimuth_id}")
            return JsonResponse({'status': 'success'})
        except Exception as e:
            logger.error(f"방위각 삭제 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def get_azimuth(request, azimuth_id):
    """방위각 정보 가져오기 API"""
    try:
        # 여기에 방위각 데이터를 DB에서 가져오는 코드 추가
        # 실제 데이터베이스 모델이 구현되면 아래 코드를 적절히 수정
        
        # 샘플 데이터
        azimuth_data = {
            'id': azimuth_id,
            'target_id': 'sample_target',
            'target_type': 'wind_turbine',
            'azimuth_degree': 90.0,
            'adjustment_reason': 'initial_setup',
            'notes': '초기 설정입니다.',
            'applied_date': '2023-06-01'
        }
        
        return JsonResponse({
            'status': 'success',
            'azimuth': azimuth_data
        })
    except Exception as e:
        logger.error(f"방위각 정보 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500) 