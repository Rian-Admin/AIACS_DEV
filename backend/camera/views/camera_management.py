from django.shortcuts import render, redirect
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
from django.core.cache import cache
import json
import os
from django.conf import settings
from ..models import Camera
import logging
from django.db import connection

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

# 비디오 파일 업로드를 위한 디렉토리 생성
VIDEO_UPLOAD_DIR = os.path.join(settings.BASE_DIR, 'video_uploads')
os.makedirs(VIDEO_UPLOAD_DIR, exist_ok=True)

def camera_management_view(request):
    """카메라 관리 페이지"""
    cameras = Camera.objects.all()
    
    # 카메라 객체에 표시용 RTSP 주소 추가
    for camera in cameras:
        # 실제 경로가 업로드 디렉토리 내부인 경우 표시 이름 변경
        if camera.rtsp_address and os.path.exists(camera.rtsp_address) and camera.rtsp_address.startswith(VIDEO_UPLOAD_DIR):
            # UUID 경로에서 원본 파일명을 추출하는 것은 불가능하므로 표시용 이름 생성
            filename = os.path.basename(camera.rtsp_address)
            camera.display_rtsp = f"업로드된 파일: {filename}"
        elif camera.rtsp_address and os.path.exists(camera.rtsp_address):
            # 외부 파일 경로인 경우 간단한 표시 이름
            filename = os.path.basename(camera.rtsp_address)
            camera.display_rtsp = f"외부 파일: {filename}"
        else:
            # RTSP URL이거나 존재하지 않는 파일인 경우 그대로 표시
            camera.display_rtsp = camera.rtsp_address
    
    return render(request, 'detection/camera_management.html', {'cameras': cameras})

@csrf_exempt
def add_camera(request):
    """카메라 추가 API"""
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            
            # 외부 파일 경로 처리
            rtsp_address = data.get('rtsp_address')
            external_path = data.get('external_path')
            
            # 외부 비디오 파일 경로가 제공된 경우
            if external_path:
                # 경로 유효성 검사
                if not os.path.exists(external_path):
                    return JsonResponse({
                        'status': 'error', 
                        'message': f'제공된 경로가 존재하지 않습니다: {external_path}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(external_path)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    return JsonResponse({
                        'status': 'error',
                        'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                    }, status=400)
                
                # 외부 경로를 RTSP 주소로 설정
                rtsp_address = external_path
            
            camera = Camera(
                camera_id=data.get('camera_id'),
                wind_turbine_id=data.get('wind_turbine_id'),
                viewing_angle=data.get('viewing_angle'),
                installation_direction=data.get('installation_direction'),
                installation_height=data.get('installation_height'),
                rtsp_address=rtsp_address,
                status=data.get('status', 'active')
            )
            camera.save()
            
            # 카메라 매니저 캐시 초기화 (새 카메라가 바로 인식되도록)
            from ..frame.camera_manager import CameraManager
            CameraManager.get_instance()._load_cameras_from_db()
            
            logger.info(f"카메라 추가됨: {camera.camera_id}")
            return JsonResponse({'status': 'success'})
        except Exception as e:
            logger.error(f"카메라 추가 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def update_camera(request, camera_id):
    """카메라 업데이트 API"""
    if request.method in ['PUT', 'POST']:
        try:
            data = json.loads(request.body)
            camera = Camera.objects.get(camera_id=camera_id)
            
            # 외부 파일 경로 처리
            external_path = data.get('external_path')
            if external_path:
                # 경로 유효성 검사
                if not os.path.exists(external_path):
                    return JsonResponse({
                        'status': 'error', 
                        'message': f'제공된 경로가 존재하지 않습니다: {external_path}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(external_path)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    return JsonResponse({
                        'status': 'error',
                        'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                    }, status=400)
                
                # 외부 경로를 RTSP 주소로 설정
                data['rtsp_address'] = external_path
            
            # URL 변경 여부 확인 (로깅용)
            url_changed = 'rtsp_address' in data and data['rtsp_address'] != camera.rtsp_address
            old_url = camera.rtsp_address

            # 필드 업데이트
            if 'wind_turbine_id' in data:
                camera.wind_turbine_id = data['wind_turbine_id']
            if 'viewing_angle' in data:
                camera.viewing_angle = data['viewing_angle']
            if 'installation_direction' in data:
                camera.installation_direction = data['installation_direction']
            if 'installation_height' in data:
                camera.installation_height = data['installation_height']
            if 'rtsp_address' in data:
                camera.rtsp_address = data['rtsp_address']
            if 'status' in data:
                camera.status = data['status']
                
            camera.save()
            
            # 카메라 매니저 캐시 갱신 (변경사항이 바로 적용되도록)
            from ..frame.camera_manager import CameraManager
            cm = CameraManager.get_instance()
            
            # URL이 변경된 경우 로그 출력
            if url_changed:
                logger.info(f"카메라 {camera_id}의 URL이 변경되었습니다: {old_url} -> {camera.rtsp_address}")
                # 강제 캐시 갱신 (중요: 추가된 부분)
                cm.force_refresh()
            else:
                # 일반 업데이트의 경우 표준 refresh
                cm._load_cameras_from_db()
            
            logger.info(f"카메라 업데이트됨: {camera_id}")
            return JsonResponse({'status': 'success'})
        except Camera.DoesNotExist:
            logger.warning(f"카메라 업데이트 시도 중 찾을 수 없음: {camera_id}")
            return JsonResponse({'status': 'error', 'message': f'Camera {camera_id} not found'}, status=404)
        except Exception as e:
            logger.error(f"카메라 업데이트 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def delete_camera(request, camera_id):
    """카메라 삭제 API"""
    if request.method in ['DELETE', 'POST']:
        try:
            # Django ORM 대신 직접 SQL 쿼리 사용
            with connection.cursor() as cursor:
                # 1. BBInfo 테이블에서 이 카메라의 탐지 정보와 관련된 모든 데이터 삭제
                cursor.execute("""
                DELETE FROM camera_bbinfo 
                WHERE detection_info_id_id IN (
                    SELECT detection_id 
                    FROM camera_detectioninfo 
                    WHERE camera_id_id = %s
                )
                """, [camera_id])
                
                bb_count = cursor.rowcount
                logger.info(f"카메라 {camera_id}와 관련된 {bb_count}개의 바운딩 박스 정보가 삭제되었습니다.")
                
                # 2. 탐지 정보 삭제
                cursor.execute("""
                DELETE FROM camera_detectioninfo 
                WHERE camera_id_id = %s
                """, [camera_id])
                
                detection_count = cursor.rowcount
                logger.info(f"카메라 {camera_id}와 관련된 {detection_count}개의 탐지 기록이 삭제되었습니다.")
                
                # 3. 카메라 삭제
                cursor.execute("""
                DELETE FROM camera_camera 
                WHERE camera_id = %s
                """, [camera_id])
                
                if cursor.rowcount > 0:
                    logger.info(f"카메라 삭제됨: {camera_id}")
                    
                    # 카메라 매니저 캐시 초기화
                    try:
                        from ..frame.camera_manager import CameraManager
                        cm = CameraManager.get_instance()
                        cm._url_cache.clear()
                        cm._last_db_check = 0
                    except Exception as e:
                        logger.warning(f"카메라 캐시 초기화 중 오류 발생: {e}")
                    
                    return JsonResponse({'status': 'success'})
                else:
                    return JsonResponse({'status': 'error', 'message': f'Camera {camera_id} not found'}, status=404)
                
        except Exception as e:
            logger.error(f"카메라 삭제 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def clear_camera_cache(request):
    """카메라 캐시 강제 비우기 API"""
    if request.method == 'POST':
        try:
            # CameraManager 인스턴스를 가져와 캐시 비우기
            from ..frame.camera_manager import CameraManager
            cm = CameraManager.get_instance()
            
            # 캐시 비우기
            cm._url_cache.clear()
            cm._last_db_check = 0  # 강제로 다음 호출 시 DB 조회하도록
            
            logger.info("카메라 캐시가 강제로 비워졌습니다.")
            return JsonResponse({'status': 'success'})
        except Exception as e:
            logger.error(f"카메라 캐시 비우기 오류: {e}")
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def upload_video(request):
    """비디오 파일 업로드 API"""
    if request.method == 'POST':
        try:
            # 경로 직접 사용 옵션 확인
            use_original_path = request.POST.get('use_original_path')
            
            # 외부 경로가 직접 제공된 경우
            if use_original_path and 'external_path' in request.POST:
                external_path = request.POST.get('external_path')
                
                # 경로 유효성 검사
                if not os.path.exists(external_path):
                    return JsonResponse({
                        'status': 'error',
                        'message': f'제공된 경로가 존재하지 않습니다: {external_path}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(external_path)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    return JsonResponse({
                        'status': 'error',
                        'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                    }, status=400)
                
                # 경로를 그대로 반환
                return JsonResponse({
                    'status': 'success',
                    'file_path': external_path,
                    'original_filename': os.path.basename(external_path),
                    'is_external': True
                })
            
            # 일반 파일 업로드
            if 'video_file' not in request.FILES:
                return JsonResponse({
                    'status': 'error', 
                    'message': '비디오 파일이 제공되지 않았습니다.'
                }, status=400)
            
            video_file = request.FILES['video_file']
            
            # 파일 확장자 확인
            filename = video_file.name
            _, ext = os.path.splitext(filename)
            
            # 지원되는 비디오 파일 형식 확인
            supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
            if ext.lower() not in supported_extensions:
                return JsonResponse({
                    'status': 'error',
                    'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                }, status=400)
            
            # 안전한 파일명 생성
            import uuid
            safe_filename = f"{uuid.uuid4().hex}{ext}"
            file_path = os.path.join(VIDEO_UPLOAD_DIR, safe_filename)
            
            # 파일 저장
            with open(file_path, 'wb+') as destination:
                for chunk in video_file.chunks():
                    destination.write(chunk)
            
            logger.info(f"비디오 파일 업로드됨: {filename} -> {file_path}")
            
            # 업로드된 파일의 경로 반환 (UI 표시용 짧은 이름 포함)
            return JsonResponse({
                'status': 'success',
                'file_path': file_path,
                'display_path': f"업로드된 파일: {filename}",  # UI에 표시할 짧은 이름
                'original_filename': filename,
                'is_external': False
            })
        
        except Exception as e:
            logger.error(f"비디오 파일 업로드 오류: {e}")
            return JsonResponse({
                'status': 'error',
                'message': str(e)
            }, status=500)
    
    return JsonResponse({
        'status': 'error',
        'message': 'Invalid request method'
    }, status=405)

@csrf_exempt
def get_camera(request, camera_id):
    """카메라 정보 가져오기 API"""
    try:
        camera = Camera.objects.get(camera_id=camera_id)
        
        # 카메라 정보를 JSON으로 변환
        camera_data = {
            'camera_id': camera.camera_id,
            'wind_turbine_id': camera.wind_turbine_id,
            'viewing_angle': camera.viewing_angle,
            'installation_direction': camera.installation_direction,
            'installation_height': camera.installation_height,
            'rtsp_address': camera.rtsp_address,
            'status': camera.status
        }
        
        return JsonResponse({
            'status': 'success',
            'camera': camera_data
        })
    except Camera.DoesNotExist:
        return JsonResponse({
            'status': 'error',
            'message': f'카메라 ID {camera_id}를 찾을 수 없습니다.'
        }, status=404)
    except Exception as e:
        logger.error(f"카메라 정보 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500) 