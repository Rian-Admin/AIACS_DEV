from django.shortcuts import render, redirect
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
import logging
from ..hardware.bird_controller import BirdController
import socket

# 로그 설정
logger = logging.getLogger(__name__)

def speaker_management_view(request):
    """스피커 관리 페이지"""
    # 컨트롤러 상태 확인
    controller = BirdController.get_instance()
    status = controller.get_controller_status()
    
    context = {
        'controller_status': status,
        'is_enabled': status.get('is_enabled', False),
        'is_running': status.get('is_running', False),
        'current_volume': status.get('current_volume', 0),
        'is_playing': status.get('is_playing', False),
        'current_sound': status.get('current_sound', None)
    }
    
    return render(request, 'detection/speaker_management.html', context)

@csrf_exempt
def play_sound(request):
    """특정 음원 재생 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        data = json.loads(request.body)
        sound_index = int(data.get('sound_index', 0))
        repeat = bool(data.get('repeat', False))
        
        # 인덱스 범위 검증 (0-9)
        if not (0 <= sound_index <= 9):
            return JsonResponse({'status': 'error', 'message': '유효하지 않은 음원 인덱스입니다. 0-9 사이의 값이어야 합니다.'}, status=400)
        
        try:
            controller = BirdController.get_instance()
            result = controller.play_sound(sound_index, repeat)
            
            if result:
                return JsonResponse({
                    'status': 'success', 
                    'message': f'음원 {sound_index} 재생 시작 (반복: {"예" if repeat else "아니오"})'
                })
            else:
                return JsonResponse({
                    'status': 'error', 
                    'message': '음원 재생 실패 - 연결을 확인하세요'
                }, status=200)
        except Exception as play_error:
            logger.error(f"음원 재생 중 예외 발생: {play_error}")
            return JsonResponse({
                'status': 'error', 
                'message': f'음원 재생 오류: {str(play_error)}'
            }, status=200)
    
    except Exception as e:
        logger.error(f"음원 재생 요청 처리 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'요청 처리 실패: {str(e)}'
        }, status=200)

@csrf_exempt
def stop_sound(request):
    """음원 정지 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        try:
            controller = BirdController.get_instance()
            result = controller.stop_sound()
            
            if result:
                return JsonResponse({'status': 'success', 'message': '음원 재생 정지'})
            else:
                return JsonResponse({
                    'status': 'error', 
                    'message': '음원 정지 실패 - 연결을 확인하세요'
                }, status=200)
        except Exception as stop_error:
            logger.error(f"음원 정지 중 예외 발생: {stop_error}")
            return JsonResponse({
                'status': 'error', 
                'message': f'음원 정지 오류: {str(stop_error)}'
            }, status=200)
    
    except Exception as e:
        logger.error(f"음원 정지 요청 처리 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'요청 처리 실패: {str(e)}'
        }, status=200)

@csrf_exempt
def set_volume(request):
    """볼륨 설정 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        data = json.loads(request.body)
        volume = int(data.get('volume', 50))
        
        try:
            controller = BirdController.get_instance()
            result = controller.set_volume(volume)
            
            if result:
                return JsonResponse({'status': 'success', 'message': f'볼륨이 {volume}%로 설정됨'})
            else:
                return JsonResponse({
                    'status': 'error', 
                    'message': '볼륨 설정 실패 - 연결을 확인하세요'
                }, status=200)
        except Exception as volume_error:
            logger.error(f"볼륨 설정 중 예외 발생: {volume_error}")
            return JsonResponse({
                'status': 'error', 
                'message': f'볼륨 설정 오류: {str(volume_error)}'
            }, status=200)
    
    except Exception as e:
        logger.error(f"볼륨 설정 요청 처리 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'요청 처리 실패: {str(e)}'
        }, status=200)

@csrf_exempt
def get_controller_status(request):
    """컨트롤러 상태 조회 API"""
    try:
        controller = BirdController.get_instance()
        status = controller.get_controller_status()
        
        return JsonResponse({
            'status': 'success',
            'data': status
        })
    
    except Exception as e:
        logger.error(f"상태 조회 오류: {e}")
        return JsonResponse({
            'status': 'error', 
            'message': f'상태 조회 실패: {str(e)}'
        }, status=200)

@csrf_exempt
def send_tcp_command(request):
    """TCP 서버에 직접 명령어 전송 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        data = json.loads(request.body)
        ip_address = data.get('ip_address', '127.0.0.1')
        port = int(data.get('port', 9090))
        command_hex = data.get('command', '')
        
        if not command_hex:
            return JsonResponse({'status': 'error', 'message': '명령어가 비어있습니다'}, status=400)
        
        # 16진수 문자열을 바이트로 변환
        try:
            # 공백 및 콜론 등의 구분자 제거
            command_hex = command_hex.replace(' ', '').replace(':', '')
            command_bytes = bytes.fromhex(command_hex)
        except ValueError as e:
            return JsonResponse({'status': 'error', 'message': f'16진수 변환 오류: {str(e)}'}, status=400)
        
        # 소켓 생성 및 명령어 전송
        sock = None
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(3.0)  # 3초 타임아웃
            sock.connect((ip_address, port))
            sock.sendall(command_bytes)
            
            # 응답 대기 (옵션)
            try:
                response = sock.recv(1024)
                response_hex = response.hex()
                logger.info(f"TCP 응답: {response_hex}")
                return JsonResponse({
                    'status': 'success',
                    'message': '명령어 전송 성공',
                    'response': response_hex
                })
            except socket.timeout:
                # 응답이 없어도 명령은 전송됨
                return JsonResponse({
                    'status': 'success', 
                    'message': '명령어 전송 성공 (응답 없음)'
                })
        except ConnectionRefusedError:
            return JsonResponse({
                'status': 'error',
                'message': f'TCP 서버({ip_address}:{port})에 연결할 수 없습니다'
            }, status=500)
        except socket.error as e:
            return JsonResponse({
                'status': 'error',
                'message': f'소켓 오류: {str(e)}'
            }, status=500)
        finally:
            if sock:
                sock.close()
    
    except Exception as e:
        logger.error(f"TCP 명령어 전송 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': f'명령어 전송 실패: {str(e)}'
        }, status=500)

@csrf_exempt
def connect_controller(request):
    """스피커 서버 연결 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        data = json.loads(request.body)
        server_address = data.get('server_address', '192.168.219.15')
        server_port = int(data.get('server_port', 9090))
        
        # 기존 인스턴스 얻기
        controller = BirdController.get_instance()
        
        # 서버 주소 및 포트 설정 (인스턴스가 이미 생성된 경우 속성 업데이트)
        controller.server_address = server_address
        controller.server_port = server_port
        
        # 연결 시도
        try:
            success = controller.connect()
            
            if success:
                return JsonResponse({
                    'status': 'success',
                    'message': f'스피커 서버({server_address}:{server_port})에 연결되었습니다'
                })
            else:
                return JsonResponse({
                    'status': 'error',
                    'message': f'스피커 서버({server_address}:{server_port})에 연결할 수 없습니다'
                }, status=200)  # 클라이언트에서 정상 처리할 수 있도록 200 상태 코드 반환
        except Exception as connect_error:
            logger.error(f"연결 시도 중 예외 발생: {connect_error}")
            return JsonResponse({
                'status': 'error',
                'message': f'연결 오류: {str(connect_error)}'
            }, status=200)  # 클라이언트에서 정상 처리할 수 있도록 200 상태 코드 반환
    
    except Exception as e:
        logger.error(f"스피커 서버 연결 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': f'연결 요청 처리 실패: {str(e)}'
        }, status=200)  # 클라이언트에서 정상 처리할 수 있도록 200 상태 코드 반환

@csrf_exempt
def disconnect_controller(request):
    """스피커 서버 연결 해제 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다'}, status=400)
    
    try:
        try:
            controller = BirdController.get_instance()
            controller.disconnect()
            
            return JsonResponse({
                'status': 'success',
                'message': '스피커 서버 연결이 종료되었습니다'
            })
        except Exception as disconnect_error:
            logger.error(f"연결 해제 중 예외 발생: {disconnect_error}")
            return JsonResponse({
                'status': 'error',
                'message': f'연결 해제 중 오류: {str(disconnect_error)}'
            }, status=200)
    
    except Exception as e:
        logger.error(f"스피커 서버 연결 해제 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': f'연결 해제 요청 처리 실패: {str(e)}'
        }, status=200) 