from django.shortcuts import render
from django.http import StreamingHttpResponse, HttpResponse
from ..frame.video_camera import VideoCamera
from ..frame.camera_manager import CameraManager
import time
import numpy as np
import cv2
from ..models import Camera

# 카메라 객체 캐싱 (매번 새로 생성하지 않도록)
_camera_cache = {}

def get_cached_camera(camera_id):
    """캐시된 카메라 객체 반환 또는 새로 생성"""
    global _camera_cache
    
    if camera_id not in _camera_cache:
        _camera_cache[camera_id] = VideoCamera(camera_id)
    
    return _camera_cache[camera_id]

def index(request):
    """메인 대시보드 페이지"""
    camera_manager = CameraManager()
    
    # DB에서 모든 활성 카메라 가져오기
    db_cameras = Camera.objects.filter(status='active')
    
    available_cameras = {}
    # DB에 있는 모든 카메라 사용
    for camera in db_cameras:
        camera_id = camera.camera_id
        available_cameras[camera_id] = {
            "location": f"{camera.installation_direction} ({camera.viewing_angle}°)"
        }
    
    # 백업 방식으로 1-4 확인 (DB에 없는 경우)
    if not available_cameras:
        for camera_id in range(1, 5):  # 1부터 4까지 확인
            if camera_manager.get_camera_url(camera_id) is not None:
                available_cameras[camera_id] = {"location": f"카메라 {camera_id}"}
    
    return render(request, 'detection/index.html', {
        'camera_info': available_cameras,
        'cameras': sorted(available_cameras.keys())
    })

def camera_feed(request, camera_id):
    """카메라 스트림 - 성능 최적화"""
    # 싱글 프레임 요청 확인 (갱신 요청)
    is_single_frame = 't' in request.GET or 'refresh' in request.GET
    
    try:
        # 캐시된 카메라 객체 사용
        camera = get_cached_camera(camera_id)
        
        # 스트림 핸들러 확인
        if camera.stream_handler is None:
            empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
            cv2.putText(empty_frame, f"카메라 {camera_id}: 사용 불가", 
                       (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 85])
            return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
        
        # 싱글 프레임 요청 처리 (썸네일이나 갱신 요청)
        if is_single_frame:
            frame_data = camera.get_frame(higher_quality=True)
            if frame_data:
                return HttpResponse(frame_data, content_type='image/jpeg')
            else:
                # 빈 프레임 전송
                empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
                cv2.putText(empty_frame, f"카메라 {camera_id}: 프레임 없음", 
                           (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
                _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 85])
                return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
        
        # 최적화된 스트림 제공
        return StreamingHttpResponse(
            generate_frames(camera),
            content_type='multipart/x-mixed-replace; boundary=frame'
        )
    except Exception as e:
        # 오류 발생 시 오류 메시지와 함께 빈 프레임 반환
        empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
        cv2.putText(empty_frame, f"오류: {str(e)}", 
                   (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 85])
        return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')

def generate_frames(camera):
    """최적화된 프레임 제너레이터 - 10FPS로 고정"""
    try:
        last_frame_time = time.time()
        fps_target = 10.0  # 10FPS로 고정
        frame_interval = 1.0 / fps_target  # 프레임 간격 (초)
        
        while True:
            # 현재 시간 체크
            current_time = time.time()
            elapsed = current_time - last_frame_time
            
            # 목표 FPS 간격 보장
            if elapsed < frame_interval:
                # 대기 시간 계산
                sleep_time = frame_interval - elapsed
                time.sleep(sleep_time * 0.8)  # 정확한 타이밍을 위해 조금 덜 대기
                continue
            
            # 프레임 가져오기
            frame = camera.get_frame(higher_quality=False)
            last_frame_time = time.time()
            
            if frame:
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            else:
                # 빈 프레임일 경우 짧게 대기
                time.sleep(0.05)  # 50ms 대기
    except Exception as e:
        print(f"프레임 생성 오류: {e}")
        pass