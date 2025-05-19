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
import threading

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
    
    # 카메라를 ID 기준으로 숫자형 정렬
    sorted_cameras = sorted(cameras, key=lambda camera: int(camera.camera_id) if str(camera.camera_id).isdigit() else camera.camera_id)
    
    return render(request, 'detection/camera_management.html', {'cameras': sorted_cameras})

@csrf_exempt
def add_camera(request):
    """카메라 추가 API"""
    if request.method == 'POST':
        try:
            logger.info("카메라 추가 API 호출 시작")
            
            # 요청 바디 디버깅
            try:
                request_body = request.body.decode('utf-8')
                logger.info(f"요청 바디: {request_body}")
                data = json.loads(request.body)
            except json.JSONDecodeError:
                logger.error("JSON 파싱 실패")
                return JsonResponse({
                    'status': 'error',
                    'message': 'JSON 파싱 실패'
                }, status=400)
                
            logger.info(f"파싱된 데이터: {data}")
            
            # 필수 필드 검증
            required_fields = ['camera_id', 'rtsp_address']
            missing_fields = [field for field in required_fields if not data.get(field)]
            if missing_fields:
                logger.error(f"필수 필드 누락: {missing_fields}")
                return JsonResponse({
                    'status': 'error',
                    'message': f'필수 필드가 누락되었습니다: {", ".join(missing_fields)}'
                }, status=400)
            
            # 외부 파일 경로 처리
            rtsp_address = data.get('rtsp_address')
            external_path = data.get('external_path')
            
            logger.info(f"처리 전 RTSP 주소: {rtsp_address}, 외부 경로: {external_path}")
            
            # file:// 프로토콜로 시작하는 경로 처리
            if rtsp_address and rtsp_address.startswith('file://'):
                rtsp_address = rtsp_address[7:]  # file:// 부분 제거
                logger.info(f"file:// 제거 후 RTSP 주소: {rtsp_address}")
            
            # 외부 비디오 파일 경로가 제공된 경우
            if external_path:
                # file:// 프로토콜로 시작하는 경로 처리
                if external_path.startswith('file://'):
                    external_path = external_path[7:]  # file:// 부분 제거
                
                # 경로 유효성 검사
                if not os.path.exists(external_path):
                    logger.error(f"외부 경로가 존재하지 않음: {external_path}")
                    return JsonResponse({
                        'status': 'error', 
                        'message': f'제공된 경로가 존재하지 않습니다: {external_path}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(external_path)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    logger.error(f"지원되지 않는 파일 형식: {ext}")
                    return JsonResponse({
                        'status': 'error',
                        'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                    }, status=400)
                
                # 외부 경로를 RTSP 주소로 설정
                rtsp_address = external_path
            
            # 입력된 파일 경로 존재 확인 (rtsp_address가 실제 파일 경로인 경우)
            if rtsp_address and os.path.isabs(rtsp_address) and not rtsp_address.startswith('rtsp://'):
                if not os.path.exists(rtsp_address):
                    logger.error(f"입력된 파일 경로가 존재하지 않음: {rtsp_address}")
                    return JsonResponse({
                        'status': 'error',
                        'message': f'입력한 파일 경로가 존재하지 않습니다: {rtsp_address}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(rtsp_address)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    logger.error(f"지원되지 않는 파일 형식: {ext}")
                    return JsonResponse({
                        'status': 'error',
                        'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                    }, status=400)
            
            logger.info(f"최종 데이터: camera_id={data.get('camera_id')}, rtsp_address={rtsp_address}")
            
            # 카메라 ID 중복 검사
            from ..models import Camera
            if Camera.objects.filter(camera_id=data.get('camera_id')).exists():
                logger.error(f"중복된 카메라 ID: {data.get('camera_id')}")
                return JsonResponse({
                    'status': 'error',
                    'message': f'이미 존재하는 카메라 ID입니다: {data.get("camera_id")}'
                }, status=400)
            
            # Camera 객체 생성
            try:
                camera = Camera(
                    camera_id=data.get('camera_id'),
                    wind_turbine_id=data.get('wind_turbine_id', ''),
                    viewing_angle=data.get('viewing_angle', 0),
                    installation_direction=data.get('installation_direction', '북'),
                    installation_height=data.get('installation_height', 0),
                    rtsp_address=rtsp_address,
                    status=data.get('status', 'active')
                )
                
                logger.info(f"카메라 객체 생성 완료, 저장 시도")
                camera.save()
                logger.info(f"카메라 저장 완료")
            except Exception as cam_error:
                logger.error(f"카메라 객체 생성/저장 중 오류: {cam_error}")
                import traceback
                logger.error(traceback.format_exc())
                return JsonResponse({
                    'status': 'error',
                    'message': f'카메라 저장 중 오류가 발생했습니다: {str(cam_error)}'
                }, status=500)
            
            # 카메라 매니저 캐시 초기화 (새 카메라가 바로 인식되도록)
            try:
                from ..frame.camera_manager import CameraManager
                CameraManager.get_instance()._load_cameras_from_db()
                logger.info("카메라 매니저 캐시 초기화 완료")
            except Exception as cm_error:
                logger.error(f"카메라 매니저 캐시 초기화 중 오류: {cm_error}")
            
            logger.info(f"카메라 추가됨: {camera.camera_id}")
            return JsonResponse({'status': 'success'})
        except json.JSONDecodeError as je:
            logger.error(f"JSON 파싱 오류: {je}, 원본 요청: {request.body}")
            return JsonResponse({'status': 'error', 'message': f'잘못된 JSON 형식: {str(je)}'}, status=400)
        except Exception as e:
            logger.error(f"카메라 추가 오류: {e}")
            import traceback
            logger.error(f"상세 오류: {traceback.format_exc()}")
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
            # 먼저 카메라 존재 여부 확인
            try:
                camera = Camera.objects.get(camera_id=camera_id)
                logger.info(f"삭제 대상 카메라 {camera_id} 확인됨: {camera.rtsp_address}")
            except Camera.DoesNotExist:
                logger.warning(f"삭제 요청된 카메라 {camera_id}가 데이터베이스에 존재하지 않습니다.")
                return JsonResponse({
                    'status': 'error', 
                    'message': f'카메라 ID {camera_id}가 존재하지 않습니다.'
                }, status=404)
                
            # 트랜잭션 시작
            with connection.cursor() as cursor:
                # 트랜잭션 시작
                cursor.execute("BEGIN")
                
                try:
                    # 0. 삭제해야 할 탐지 정보 ID 목록 조회
                    cursor.execute("""
                    SELECT detection_id FROM camera_detectioninfo
                    WHERE camera_id_id = %s
                    """, [camera_id])
                    
                    detection_ids = [row[0] for row in cursor.fetchall()]
                    logger.info(f"카메라 {camera_id}와 관련된 탐지 정보 ID: {len(detection_ids)}개")
                    
                    # 1. 퇴치 기록 삭제
                    if detection_ids:
                        # SQL 안전하게 생성
                        placeholders = ','.join(['%s'] * len(detection_ids))
                        deterrent_query = f"""
                        DELETE FROM camera_deterrentrecord
                        WHERE detection_id_id IN ({placeholders})
                        """
                        
                        cursor.execute(deterrent_query, detection_ids)
                        
                        deterrent_count = cursor.rowcount
                        logger.info(f"카메라 {camera_id}와 관련된 {deterrent_count}개의 퇴치 기록이 삭제되었습니다.")
                    else:
                        deterrent_count = 0
                        logger.info(f"카메라 {camera_id}와 관련된 탐지 정보가 없어 퇴치 기록 삭제를 건너뜁니다.")
                    
                    # 2. BBInfo 테이블에서 이 카메라의 탐지 정보와 관련된 모든 데이터 삭제
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
                    
                    # 3. 탐지 정보 삭제
                    cursor.execute("""
                    DELETE FROM camera_detectioninfo 
                    WHERE camera_id_id = %s
                    """, [camera_id])
                    
                    detection_count = cursor.rowcount
                    logger.info(f"카메라 {camera_id}와 관련된 {detection_count}개의 탐지 기록이 삭제되었습니다.")
                    
                    # 3-1. DetectionVideo 테이블에서 이 카메라와 관련된 모든 데이터 삭제
                    cursor.execute("""
                    DELETE FROM camera_detectionvideo 
                    WHERE camera_id = %s
                    """, [camera_id])
                    
                    video_count = cursor.rowcount
                    logger.info(f"카메라 {camera_id}와 관련된 {video_count}개의 동영상 기록이 삭제되었습니다.")
                    
                    # 4. 카메라 삭제
                    cursor.execute("""
                    DELETE FROM camera_camera 
                    WHERE camera_id = %s
                    """, [camera_id])
                    
                    camera_count = cursor.rowcount
                    logger.info(f"카메라 {camera_id} 삭제 결과: {camera_count}개 행 영향받음")
                    
                    if camera_count > 0:
                        # 트랜잭션 커밋
                        cursor.execute("COMMIT")
                        logger.info(f"카메라 삭제 성공: {camera_id} (트랜잭션 커밋됨)")
                        
                        # 카메라 매니저 캐시 초기화
                        try:
                            from ..frame.camera_manager import CameraManager
                            cm = CameraManager.get_instance()
                            
                            # 카메라 캐시에서 삭제
                            if str(camera_id) in cm._url_cache:
                                del cm._url_cache[str(camera_id)]
                                logger.info(f"카메라 {camera_id}가 URL 캐시에서 제거되었습니다.")
                            
                            # 카메라 상태에서 삭제
                            if str(camera_id) in cm._camera_status:
                                del cm._camera_status[str(camera_id)]
                                logger.info(f"카메라 {camera_id}가 상태 추적에서 제거되었습니다.")
                            
                            # 초기화 목록에서 삭제
                            with cm._global_init_lock:
                                if str(camera_id) in cm._cameras_initializing:
                                    cm._cameras_initializing.remove(str(camera_id))
                                    logger.info(f"카메라 {camera_id}가 초기화 목록에서 제거되었습니다.")
                            
                            # 모든 캐시 초기화
                            cm._last_db_check = 0
                            cm.force_refresh()  # 강제 새로고침
                            logger.info(f"카메라 매니저 캐시가 성공적으로 초기화되었습니다.")
                        except Exception as e:
                            logger.warning(f"카메라 캐시 초기화 중 오류 발생: {e}")
                        
                        # 결과에 삭제된 레코드 수 추가
                        return JsonResponse({
                            'status': 'success',
                            'details': {
                                'camera_count': camera_count,
                                'detection_count': detection_count,
                                'bb_count': bb_count,
                                'deterrent_count': deterrent_count if 'deterrent_count' in locals() else 0,
                                'video_count': video_count
                            }
                        })
                    else:
                        # 삭제할 카메라가 없으면 롤백
                        cursor.execute("ROLLBACK")
                        logger.warning(f"카메라 {camera_id}를 찾을 수 없어 삭제 실패 (트랜잭션 롤백)")
                        return JsonResponse({'status': 'error', 'message': f'카메라 ID {camera_id}를 찾을 수 없습니다.'}, status=404)
                    
                except Exception as inner_e:
                    # 오류 발생 시 롤백
                    cursor.execute("ROLLBACK")
                    logger.error(f"카메라 삭제 중 오류 발생 - 롤백됨: {inner_e}")
                    import traceback
                    logger.error(traceback.format_exc())
                    return JsonResponse({'status': 'error', 'message': f'카메라 삭제 처리 중 오류: {str(inner_e)}'}, status=500)
                
        except Exception as e:
            logger.error(f"카메라 삭제 처리 시작 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return JsonResponse({'status': 'error', 'message': f'카메라 삭제 처리 시작 중 오류: {str(e)}'}, status=500)
    
    return JsonResponse({'status': 'error', 'message': '유효하지 않은 요청 방식입니다.'}, status=405)

@csrf_exempt
def clear_camera_cache(request):
    """카메라 캐시 강제 비우기 API"""
    if request.method == 'POST':
        try:
            # CameraManager 인스턴스를 가져와 캐시 갱신
            from ..frame.camera_manager import CameraManager
            cm = CameraManager.get_instance()
            
            # 요청 데이터 파싱 (특정 카메라만 초기화할 경우)
            camera_id = None
            try:
                data = json.loads(request.body)
                camera_id = data.get('camera_id')
            except:
                # 요청 본문이 없거나 JSON이 아닌 경우 무시
                pass
                
            if camera_id:
                # 특정 카메라만 초기화
                logger.info(f"카메라 {camera_id} 캐시 초기화 요청")
                # _url_cache에서 특정 카메라 URL은 유지하되 초기화는 강제로 실행
                if str(camera_id) in cm._url_cache:
                    # 해당 카메라가 초기화 중인지 확인
                    if str(camera_id) in cm._cameras_initializing:
                        logger.warning(f"카메라 {camera_id}는 이미 초기화 중입니다.")
                        return JsonResponse({
                            'status': 'warning', 
                            'message': f'카메라 {camera_id}는 이미 초기화 중입니다.'
                        })
                    
                    # 초기화 실행 (별도 스레드로)
                    threading.Thread(
                        target=cm._initialize_camera,
                        args=(str(camera_id),),
                        daemon=True
                    ).start()
                    
                    logger.info(f"카메라 {camera_id} 캐시 초기화가 요청되었습니다.")
                    return JsonResponse({
                        'status': 'success',
                        'message': f'카메라 {camera_id} 캐시 초기화가 시작되었습니다.'
                    })
                else:
                    logger.warning(f"카메라 {camera_id}를 찾을 수 없습니다.")
                    return JsonResponse({
                        'status': 'error',
                        'message': f'카메라 {camera_id}를 찾을 수 없습니다.'
                    }, status=404)
            else:
                # 모든 카메라 캐시 초기화
                logger.info("모든 카메라 캐시 초기화 요청")
                
                # 이미 초기화 중인지 확인
                with cm._global_init_lock:
                    if cm._cameras_initializing:
                        logger.warning(f"이미 카메라 초기화가 진행 중입니다: {cm._cameras_initializing}")
                        return JsonResponse({
                            'status': 'warning',
                            'message': f'이미 카메라 초기화가 진행 중입니다: {cm._cameras_initializing}'
                        })
                
                # 캐시 강제 갱신
                cm.force_refresh()
                
                # 별도 스레드에서 모든 카메라 초기화
                threading.Thread(
                    target=cm._initialize_all_cameras,
                    daemon=True
                ).start()
                
                logger.info("카메라 캐시 초기화가 요청되었습니다.")
                return JsonResponse({
                    'status': 'success',
                    'message': '모든 카메라 캐시 초기화가 시작되었습니다.'
                })
                
        except Exception as e:
            logger.error(f"카메라 캐시 비우기 오류: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return JsonResponse({'status': 'error', 'message': str(e)}, status=400)
            
    return JsonResponse({'status': 'error', 'message': 'Invalid request method'}, status=405)

@csrf_exempt
def upload_video(request):
    """비디오 파일 업로드 API"""
    if request.method == 'POST':
        try:
            logger.debug("비디오 파일 업로드 API 호출 시작")
            
            # 요청 정보 디버깅
            logger.debug(f"POST 데이터: {request.POST}")
            logger.debug(f"FILES 데이터: {request.FILES}")
            
            # 경로 직접 사용 옵션 확인
            use_original_path = request.POST.get('use_original_path')
            
            # 외부 경로가 직접 제공된 경우
            if use_original_path and 'external_path' in request.POST:
                external_path = request.POST.get('external_path')
                logger.debug(f"외부 경로 사용: {external_path}")
                
                # file:// 프로토콜로 시작하는 경로 처리
                if external_path.startswith('file://'):
                    external_path = external_path[7:]  # file:// 부분 제거
                    logger.debug(f"프로토콜 제거 후 경로: {external_path}")
                
                # 경로 유효성 검사
                if not os.path.exists(external_path):
                    logger.error(f"외부 경로 존재하지 않음: {external_path}")
                    return JsonResponse({
                        'status': 'error',
                        'message': f'제공된 경로가 존재하지 않습니다: {external_path}'
                    }, status=400)
                
                # 지원되는 비디오 파일 형식 확인
                _, ext = os.path.splitext(external_path)
                supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
                if ext.lower() not in supported_extensions:
                    logger.error(f"지원되지 않는 파일 형식: {ext}")
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
                logger.error("비디오 파일이 제공되지 않음")
                return JsonResponse({
                    'status': 'error', 
                    'message': '비디오 파일이 제공되지 않았습니다.'
                }, status=400)
            
            video_file = request.FILES['video_file']
            logger.debug(f"업로드된 파일 정보: 이름={video_file.name}, 크기={video_file.size}")
            
            # 파일 확장자 확인
            filename = video_file.name
            _, ext = os.path.splitext(filename)
            
            # 지원되는 비디오 파일 형식 확인
            supported_extensions = ['.mp4', '.avi', '.mov', '.mkv', '.wmv']
            if ext.lower() not in supported_extensions:
                logger.error(f"지원되지 않는 파일 형식: {ext}")
                return JsonResponse({
                    'status': 'error',
                    'message': f'지원되지 않는 파일 형식입니다. 지원되는 형식: {", ".join(supported_extensions)}'
                }, status=400)
            
            # 안전한 파일명 생성
            import uuid
            safe_filename = f"{uuid.uuid4().hex}{ext}"
            file_path = os.path.join(VIDEO_UPLOAD_DIR, safe_filename)
            
            logger.debug(f"저장할 파일 경로: {file_path}")
            
            # 업로드 디렉토리 존재 확인 및 생성
            os.makedirs(os.path.dirname(file_path), exist_ok=True)
            
            # 파일 저장
            try:
                with open(file_path, 'wb+') as destination:
                    for chunk in video_file.chunks():
                        destination.write(chunk)
                logger.info(f"파일 저장 완료: {file_path}")
            except Exception as save_err:
                logger.error(f"파일 저장 중 오류 발생: {save_err}")
                import traceback
                logger.error(traceback.format_exc())
                return JsonResponse({
                    'status': 'error',
                    'message': f'파일 저장 중 오류가 발생했습니다: {str(save_err)}'
                }, status=500)
            
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
            import traceback
            logger.error(traceback.format_exc())
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