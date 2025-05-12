from django.http import JsonResponse, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.db.models import Count, F
import json
import datetime
import logging
from ..frame.camera_manager import CameraManager
from ..db import db_adapter
from ..models import Camera, DetectionInfo
from datetime import datetime
from ..yolo.detector import ObjectDetector
from ..hardware.bird_controller import BirdController
from ..hardware.ptz_manager import PTZManager
import numpy as np
import cv2

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

def standardize_response(data, date_fields=None):
    """API 응답 표준화 헬퍼 함수"""
    if date_fields is None:
        date_fields = ['detection_time']
    
    if isinstance(data, dict):
        data_dict = dict(data)
        # 날짜 필드 표준화
        for field in date_fields:
            if field in data_dict and data_dict[field]:
                if isinstance(data_dict[field], datetime):
                    data_dict[field] = data_dict[field].isoformat()
                elif isinstance(data_dict[field], str):
                    if ' ' in data_dict[field]:
                        data_dict[field] = data_dict[field].replace(' ', 'T')
        return data_dict
    elif isinstance(data, list):
        return [standardize_response(item, date_fields) for item in data]
    else:
        return data

@csrf_exempt
def get_recent_detections(request):
    """최근 감지 결과"""
    try:
        camera_id_param = request.GET.get('camera_id')
        
        if camera_id_param is not None and camera_id_param != '':
            try:
                camera_id = int(camera_id_param)
                detections = db_adapter.get_detections(limit=50, camera_id=camera_id)
            except ValueError:
                return JsonResponse({
                    'error': f'Invalid camera_id: {camera_id_param}',
                    'detections': []
                }, status=400)
        else:
            detections = db_adapter.get_detections(limit=50)
        
        # 표준화된 응답 변환
        processed_detections = standardize_response(detections)
        
        return JsonResponse({
            'detections': processed_detections,
            'timestamp': datetime.now().isoformat()
        })
        
    except Exception as e:
        logger.error(f"감지 결과 조회 오류: {e}")
        return JsonResponse({
            'error': str(e),
            'detections': []
        }, status=500)

@csrf_exempt
def get_detection_stats(request):
    """감지 통계"""
    try:
        hours_param = request.GET.get('hours', '24')
        try:
            hours = int(hours_param)
        except ValueError:
            return JsonResponse({
                'error': f'Invalid hours parameter: {hours_param}',
                'stats': []
            }, status=400)
            
        stats = db_adapter.get_detection_stats(hours=hours)
        
        # 표준화된 응답 변환
        processed_stats = standardize_response(stats, date_fields=['first_seen', 'last_seen'])
        
        return JsonResponse({
            'stats': processed_stats,
            'period_hours': hours,
            'timestamp': datetime.now().isoformat()
        })
            
    except Exception as e:
        logger.error(f"감지 통계 조회 오류: {e}")
        return JsonResponse({
            'error': str(e),
            'message': 'Failed to get detection statistics'
        }, status=500)

@csrf_exempt
def get_db_status(request):
    """DB 상태"""
    try:
        adapter_status = db_adapter.get_adapter_status()
        return JsonResponse({
            'status': 'success',
            'db_adapter': adapter_status,
            'timestamp': datetime.now().isoformat()
        })
    except Exception as e:
        logger.error(f"DB 상태 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'error': str(e)
        }, status=500)

def get_cameras(request):
    """카메라 정보"""
    try:
        cameras = Camera.objects.all()
        camera_list = []
        
        for camera in cameras:
            camera_data = {
                'camera_id': camera.camera_id,
                'wind_turbine_id': camera.wind_turbine_id,
                'viewing_angle': camera.viewing_angle,
                'installation_direction': camera.installation_direction,
                'installation_height': camera.installation_height,
                'rtsp_address': camera.rtsp_address,
                'status': camera.status
            }
            camera_list.append(camera_data)
        
        return JsonResponse({'cameras': camera_list})
    except Exception as e:
        logger.error(f"카메라 정보 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'error': str(e),
            'cameras': []
        }, status=500)

@csrf_exempt
def get_radar_data(request):
    """레이더 데이터 API (404 오류 방지용)"""
    return JsonResponse({
        'status': 'success',
        'message': '레이더 데이터 API 준비 중',
        'timestamp': datetime.now().isoformat(),
        'data': []
    })

@csrf_exempt
def set_yolo_conf(request):
    """YOLO 모델의 confidence threshold 설정"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': 'POST 요청만 허용됩니다'}, status=405)
        
    try:
        # 요청에서 conf 값 파싱
        data = json.loads(request.body)
        conf = data.get('conf')
        
        if conf is None:
            return JsonResponse({
                'status': 'error', 
                'message': 'conf 값이 필요합니다'
            }, status=400)
            
        # 값 유효성 검증
        try:
            conf = float(conf)
            if not (0.0 <= conf <= 1.0):
                return JsonResponse({
                    'status': 'error', 
                    'message': 'conf 값은 0.0~1.0 사이의 값이어야 합니다'
                }, status=400)
        except ValueError:
            return JsonResponse({
                'status': 'error', 
                'message': 'conf 값은 숫자여야 합니다'
            }, status=400)
            
        # YOLO 모델 가져오기
        detector = ObjectDetector.get_instance()
        if detector is None:
            return JsonResponse({
                'status': 'error', 
                'message': 'YOLO 모델을 초기화할 수 없습니다'
            }, status=500)
            
        # conf 값 설정
        old_conf = detector.get_conf_threshold()
        result = detector.set_conf_threshold(conf)
        
        if result:
            logger.info(f"YOLO confidence threshold 변경됨: {old_conf} -> {conf}")
            return JsonResponse({
                'status': 'success',
                'old_conf': old_conf,
                'new_conf': conf
            })
        else:
            return JsonResponse({
                'status': 'error', 
                'message': 'conf 값 설정 실패'
            }, status=500)
            
    except json.JSONDecodeError:
        return JsonResponse({
            'status': 'error', 
            'message': '잘못된 JSON 형식'
        }, status=400)
    except Exception as e:
        logger.error(f"conf 설정 중 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'서버 오류: {str(e)}'
        }, status=500)

@csrf_exempt
def set_guard_zone(request):
    """YOLO 모델의 Guard Zone 설정"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': 'POST 요청만 허용됩니다'}, status=405)
        
    try:
        # 요청에서 데이터 파싱
        data = json.loads(request.body)
        camera_id = data.get('camera_id')
        guard_zones = data.get('guard_zones', [])
        enabled = data.get('enabled', True)
        
        print(f"[API] Guard Zone 설정 요청: 카메라={camera_id}, 영역={len(guard_zones)}개, 활성화={enabled}")
        
        if not camera_id:
            return JsonResponse({
                'status': 'error', 
                'message': '카메라 ID가 필요합니다'
            }, status=400)
            
        # 데이터 유효성 검증
        if not isinstance(guard_zones, list):
            return JsonResponse({
                'status': 'error', 
                'message': 'guard_zones는 배열이어야 합니다'
            }, status=400)
            
        # 각 guard zone의 좌표 검증
        for i, zone in enumerate(guard_zones):
            if not isinstance(zone, dict):
                return JsonResponse({
                    'status': 'error', 
                    'message': f'guard_zones[{i}]는 객체여야 합니다'
                }, status=400)
                
            for key in ['x', 'y', 'width', 'height']:
                if key not in zone:
                    return JsonResponse({
                        'status': 'error', 
                        'message': f'guard_zones[{i}]에 {key} 키가 없습니다'
                    }, status=400)
                    
                try:
                    value = float(zone[key])
                    if not (0.0 <= value <= 1.0):
                        return JsonResponse({
                            'status': 'error', 
                            'message': f'guard_zones[{i}].{key}는 0.0~1.0 사이의 값이어야 합니다'
                        }, status=400)
                    
                    # 값을 숫자로 저장
                    zone[key] = value
                except (ValueError, TypeError):
                    return JsonResponse({
                        'status': 'error', 
                        'message': f'guard_zones[{i}].{key}는 숫자여야 합니다'
                    }, status=400)
            
        # YOLO 모델 가져오기
        detector = ObjectDetector.get_instance()
        if detector is None:
            return JsonResponse({
                'status': 'error', 
                'message': 'YOLO 모델을 초기화할 수 없습니다'
            }, status=500)
            
        # Guard Zone 설정
        result = detector.set_guard_zone(camera_id, guard_zones, enabled)
        
        if result:
            logger.info(f"YOLO Guard Zone 설정됨: 카메라 {camera_id}, 영역 {len(guard_zones)}개, 활성화: {enabled}")
            
            # 설정 결과 확인
            current_settings = detector.get_guard_zone(camera_id)
            
            return JsonResponse({
                'status': 'success',
                'message': f'Guard Zone이 성공적으로 설정되었습니다 ({len(guard_zones)}개 영역)',
                'details': {
                    'camera_id': camera_id,
                    'zones_count': len(guard_zones),
                    'enabled': enabled,
                    'current_settings': current_settings
                }
            })
        else:
            return JsonResponse({
                'status': 'error', 
                'message': 'Guard Zone 설정 실패'
            }, status=500)
            
    except json.JSONDecodeError:
        return JsonResponse({
            'status': 'error', 
            'message': '잘못된 JSON 형식'
        }, status=400)
    except Exception as e:
        logger.error(f"Guard Zone 설정 중 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'서버 오류: {str(e)}'
        }, status=500)

def get_guard_zone(request, camera_id):
    """카메라의 Guard Zone 정보 가져오기"""
    try:
        # YOLO 모델 가져오기
        detector = ObjectDetector.get_instance()
        if detector is None:
            return JsonResponse({
                'status': 'error', 
                'message': 'YOLO 모델을 초기화할 수 없습니다'
            }, status=500)
            
        # Guard Zone 정보 가져오기
        guard_zone_info = detector.get_guard_zone(camera_id)
        
        # 결과 반환
        return JsonResponse({
            'status': 'success',
            'guard_zones': guard_zone_info['guard_zones'],
            'enabled': guard_zone_info['enabled']
        })
        
    except Exception as e:
        logger.error(f"Guard Zone 정보 조회 중 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'서버 오류: {str(e)}'
        }, status=500)

def preview_guard_zone(request, camera_id):
    """Guard Zone 미리보기 - 가드존이 그려진 프레임 직접 반환"""
    try:
        # YOLO 모델 인스턴스 가져오기
        detector = ObjectDetector.get_instance()
        if detector is None:
            empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
            cv2.putText(empty_frame, "YOLO 모델 초기화 불가", 
                       (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            _, jpeg = cv2.imencode('.jpg', empty_frame)
            return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
            
        # 카메라 인스턴스 가져오기
        from ..frame.video_camera import get_cached_camera
        camera = get_cached_camera(camera_id)
        
        if camera is None:
            empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
            cv2.putText(empty_frame, f"카메라 {camera_id}: 찾을 수 없음", 
                       (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            _, jpeg = cv2.imencode('.jpg', empty_frame)
            return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
            
        # 프레임 가져오기
        frame = None
        if hasattr(camera, 'stream_handler') and camera.stream_handler:
            frame = camera.stream_handler.get_frame_from_buffer()
        
        if frame is None:
            empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
            cv2.putText(empty_frame, f"카메라 {camera_id}: 프레임 없음", 
                       (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            _, jpeg = cv2.imencode('.jpg', empty_frame)
            return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
        
        # 프레임 크기 가져오기
        height, width = frame.shape[:2]
        
        # 가드존이 그려진 프레임 가져오기
        visualized_frame = detector._draw_guard_zones(frame, str(camera_id), width, height)
        
        # JPEG으로 인코딩하여 반환
        _, jpeg = cv2.imencode('.jpg', visualized_frame, [cv2.IMWRITE_JPEG_QUALITY, 90])
        return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
        
    except Exception as e:
        logger.error(f"가드존 미리보기 생성 오류: {e}")
        import traceback
        logger.error(traceback.format_exc())
        
        # 오류 발생 시 오류 메시지와 함께 빈 프레임 반환
        empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
        cv2.putText(empty_frame, f"오류: {str(e)}", 
                   (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 85])
        return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')

@csrf_exempt
def get_yolo_info(request):
    """YOLO 모델 정보 조회"""
    try:
        detector = ObjectDetector.get_instance()
        if detector is None:
            return JsonResponse({
                'status': 'error', 
                'message': 'YOLO 모델을 초기화할 수 없습니다'
            }, status=500)
            
        model_info = detector.get_model_info()
        
        return JsonResponse({
            'status': 'success',
            'model_info': model_info
        })
        
    except Exception as e:
        logger.error(f"YOLO 정보 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500)

@csrf_exempt
def enable_controller(request):
    """조류퇴치기 활성화 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식'}, status=405)
    
    try:
        controller = BirdController.get_instance()
        controller.enable_controller()
        
        return JsonResponse({
            'status': 'success',
            'message': '조류퇴치기가 활성화되었습니다'
        })
    except Exception as e:
        logger.error(f"조류퇴치기 활성화 오류: {str(e)}")
        return JsonResponse({
            'status': 'error',
            'message': f'오류: {str(e)}'
        }, status=500)

@csrf_exempt
def disable_controller(request):
    """조류퇴치기 비활성화 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식'}, status=405)
    
    try:
        controller = BirdController.get_instance()
        controller.disable_controller()
        
        return JsonResponse({
            'status': 'success',
            'message': '조류퇴치기가 비활성화되었습니다'
        })
    except Exception as e:
        logger.error(f"조류퇴치기 비활성화 오류: {str(e)}")
        return JsonResponse({
            'status': 'error',
            'message': f'오류: {str(e)}'
        }, status=500)

@csrf_exempt
def control_ptz(request):
    """PTZ 카메라 제어 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': 'POST 요청만 허용됩니다'}, status=405)
        
    try:
        # 요청 데이터 파싱
        data = json.loads(request.body)
        camera_id = data.get('camera_id')
        direction = data.get('direction')
        is_continuous = data.get('is_continuous', True)
        speed = data.get('speed', 0.7)
        
        # 필수 파라미터 검증
        if camera_id is None:
            return JsonResponse({
                'status': 'error',
                'message': 'camera_id가 필요합니다'
            }, status=400)
            
        if direction is None:
            return JsonResponse({
                'status': 'error',
                'message': 'direction이 필요합니다'
            }, status=400)
            
        # direction 값 검증
        valid_directions = ["LEFT", "RIGHT", "UP", "DOWN", "ZOOM_IN", "ZOOM_OUT"]
        if direction not in valid_directions:
            return JsonResponse({
                'status': 'error',
                'message': f'유효하지 않은 direction 값입니다. 유효한 값: {", ".join(valid_directions)}'
            }, status=400)
            
        # 카메라 정보 가져오기
        try:
            camera = Camera.objects.get(camera_id=camera_id)
        except Camera.DoesNotExist:
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 ID {camera_id}를 찾을 수 없습니다'
            }, status=404)
            
        # 카메라 RTSP URL 확인
        if not camera.rtsp_address:
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 ID {camera_id}의 RTSP 주소가 설정되지 않았습니다'
            }, status=400)
            
        # PTZ 매니저 인스턴스 가져오기
        ptz_manager = PTZManager.get_instance()
        
        # PTZ 세션 설정 (없는 경우 초기화)
        # 카메라 모델에 ONVIF 정보 필드가 없는 경우 기본값 사용
        if hasattr(camera, 'onvif_username') and camera.onvif_username:
            username = camera.onvif_username
        else:
            username = 'admin'
            
        if hasattr(camera, 'onvif_password') and camera.onvif_password:
            password = camera.onvif_password
        else:
            password = 'admin123'
            
        if hasattr(camera, 'onvif_port') and camera.onvif_port:
            port = camera.onvif_port
        else:
            port = 80
            
        # PTZ 세션 설정이 되어 있지 않으면 설정 시도
        if not ptz_manager.setup_onvif_ptz(camera_id, camera.rtsp_address, username, password, port):
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 ID {camera_id}의 PTZ 설정에 실패했습니다'
            }, status=500)
            
        # PTZ 제어 실행
        result = ptz_manager.control_ptz(camera_id, direction, is_continuous, speed)
        
        if result:
            action = "이동 시작" if is_continuous else "정지"
            return JsonResponse({
                'status': 'success',
                'message': f'카메라 {camera_id} PTZ {direction} {action} 명령이 성공적으로 전송되었습니다',
                'is_moving': ptz_manager.is_ptz_moving(camera_id)
            })
        else:
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 {camera_id} PTZ 제어 명령 전송에 실패했습니다'
            }, status=500)
            
    except json.JSONDecodeError:
        return JsonResponse({
            'status': 'error',
            'message': '잘못된 JSON 형식입니다'
        }, status=400)
    except Exception as e:
        logger.error(f"PTZ 제어 오류: {str(e)}")
        return JsonResponse({
            'status': 'error',
            'message': f'PTZ 제어 중 오류가 발생했습니다: {str(e)}'
        }, status=500)

@csrf_exempt
def get_ptz_position(request, camera_id):
    """PTZ 카메라 현재 위치 정보 가져오기 API"""
    try:
        # 카메라 정보 가져오기
        try:
            camera = Camera.objects.get(camera_id=camera_id)
        except Camera.DoesNotExist:
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 ID {camera_id}를 찾을 수 없습니다'
            }, status=404)
            
        # PTZ 매니저 인스턴스 가져오기
        ptz_manager = PTZManager.get_instance()
        
        # 위치 정보 가져오기
        position = ptz_manager.get_camera_position(camera_id)
        
        if position is not None:
            return JsonResponse({
                'status': 'success',
                'camera_id': camera_id,
                'position': position,
                'is_moving': ptz_manager.is_ptz_moving(camera_id)
            })
        else:
            return JsonResponse({
                'status': 'error',
                'message': f'카메라 {camera_id}의 위치 정보를 가져올 수 없습니다'
            }, status=500)
            
    except Exception as e:
        logger.error(f"PTZ 위치 정보 가져오기 오류: {str(e)}")
        return JsonResponse({
            'status': 'error',
            'message': f'PTZ 위치 정보 가져오기 중 오류가 발생했습니다: {str(e)}'
        }, status=500)

# 카메라 재연결 엔드포인트 추가
@csrf_exempt
def reconnect_camera(request, camera_id):
    """특정 카메라를 강제로 재연결하는 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 메소드'}, status=405)
    
    try:
        # 카메라 ID가 유효한지 확인
        try:
            camera = Camera.objects.get(camera_id=camera_id)
        except Camera.DoesNotExist:
            return JsonResponse({'status': 'error', 'message': f'카메라 {camera_id}를 찾을 수 없습니다'}, status=404)
        
        # 카메라 매니저를 통한 재연결 시도
        manager = CameraManager.get_instance()
        reconnect_result = manager.force_reconnect(camera_id)
        
        # VideoCamera 객체 캐시 갱신
        from .camera_views import get_cached_camera
        get_cached_camera(camera_id)
        
        if reconnect_result:
            return JsonResponse({
                'status': 'success', 
                'message': f'카메라 {camera_id} 재연결 성공',
                'camera_status': manager.get_camera_status(camera_id)
            })
        else:
            return JsonResponse({
                'status': 'warning', 
                'message': f'카메라 {camera_id} 재연결 시도했으나 실패함',
                'camera_status': manager.get_camera_status(camera_id)
            }, status=200)  # 실패해도 200 반환 (클라이언트 정보용)
            
    except Exception as e:
        import traceback
        print(traceback.format_exc())
        return JsonResponse({'status': 'error', 'message': str(e)}, status=500)

@csrf_exempt
def get_camera_status(request, camera_id=None):
    """카메라의 현재 연결 상태를 반환하는 API"""
    try:
        manager = CameraManager.get_instance()
        
        # 특정 카메라의 상태만 요청된 경우
        if camera_id is not None:
            status = manager.get_camera_status(camera_id)
            return JsonResponse({
                'status': 'success',
                'camera_id': camera_id,
                'connection_status': status
            })
        
        # 모든 카메라 상태 요청된 경우
        else:
            # DB에서 모든 카메라 가져오기
            cameras = Camera.objects.all()
            statuses = {}
            
            for camera in cameras:
                camera_id = camera.camera_id
                status = manager.get_camera_status(camera_id)
                statuses[str(camera_id)] = {
                    'status': status,
                    'rtsp_address': camera.rtsp_address,
                    'camera_desc': f"{camera.installation_direction} ({camera.viewing_angle}°)"
                }
            
            return JsonResponse({
                'status': 'success',
                'cameras': statuses
            })
            
    except Exception as e:
        import traceback
        print(traceback.format_exc())
        return JsonResponse({'status': 'error', 'message': str(e)}, status=500)

@csrf_exempt
def reconnect_all_cameras(request):
    """모든 카메라를 강제로 재연결하는 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 메소드'}, status=405)
    
    try:
        # DB에서 모든 카메라 가져오기
        cameras = Camera.objects.all()
        
        # 각 카메라 재연결 시도
        manager = CameraManager.get_instance()
        
        results = {}
        success_count = 0
        
        for camera in cameras:
            camera_id = camera.camera_id
            try:
                # 카메라 매니저를 통한 재연결 시도
                reconnect_result = manager.force_reconnect(camera_id)
                
                # VideoCamera 객체 캐시 갱신
                from .camera_views import get_cached_camera
                get_cached_camera(camera_id)
                
                if reconnect_result:
                    results[str(camera_id)] = "success"
                    success_count += 1
                else:
                    results[str(camera_id)] = "failed"
                
            except Exception as e:
                results[str(camera_id)] = f"error: {str(e)}"
        
        return JsonResponse({
            'status': 'success', 
            'message': f'{success_count}/{len(cameras)} 카메라 재연결 성공',
            'results': results
        })
            
    except Exception as e:
        import traceback
        print(traceback.format_exc())
        return JsonResponse({'status': 'error', 'message': str(e)}, status=500)