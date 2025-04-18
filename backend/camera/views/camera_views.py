#C:\Users\AAA\Documents\GitHub\AIACS\AIACS_WEB\backend\camera\views\camera_views.py
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
_camera_init_done = False  # 시스템 초기화 완료 여부 추적

def initialize_all_cameras():
    """모든 카메라를 강제로 초기화하는 함수 - 시스템 시작 시 호출"""
    global _camera_init_done
    
    if _camera_init_done:
        return  # 이미 초기화되었으면 재실행하지 않음
    
    print("시스템 시작 시 모든 카메라 강제 초기화...")
    
    try:
        # 카메라 매니저 인스턴스 가져오기
        camera_manager = CameraManager.get_instance()
        
        # DB에서 모든 카메라 가져오기
        db_cameras = Camera.objects.all()
        
        # 각 카메라를 초기화하고 카메라 캐시에 추가
        for camera in db_cameras:
            camera_id = camera.camera_id
            print(f"카메라 {camera_id} 초기화 중...")
            
            # VideoCamera 객체 생성 및 캐시에 추가
            try:
                # 이미 캐시에 있는 경우 건너뜀
                if str(camera_id) in _camera_cache:
                    print(f"카메라 {camera_id}는 이미 캐시에 있음")
                    continue
                
                # 새 카메라 인스턴스 생성 및 캐시에 추가
                camera_obj = VideoCamera(camera_id)
                _camera_cache[str(camera_id)] = camera_obj
                print(f"카메라 {camera_id} 초기화 성공")
                
                # 잠시 대기하여 시스템에 과부하가 걸리지 않도록 함
                time.sleep(0.5)
                
            except Exception as e:
                print(f"카메라 {camera_id} 초기화 중 오류 발생: {e}")
        
        # 백업 방식으로 1-4 확인 (DB에 없는 경우)
        if not db_cameras:
            for camera_id in range(1, 5):  # 1부터 4까지 확인
                if camera_manager.get_camera_url(camera_id) is not None:
                    # 캐시에 없으면 추가
                    if str(camera_id) not in _camera_cache:
                        try:
                            camera_obj = VideoCamera(camera_id)
                            _camera_cache[str(camera_id)] = camera_obj
                            print(f"카메라 {camera_id} 초기화 성공 (백업 방식)")
                            time.sleep(0.5)
                        except Exception as e:
                            print(f"카메라 {camera_id} 초기화 중 오류 발생: {e}")
        
        _camera_init_done = True
        print("모든 카메라 초기화 완료!")
        
    except Exception as e:
        print(f"카메라 초기화 중 오류 발생: {e}")
        import traceback
        traceback.print_exc()

def get_cached_camera(camera_id):
    """캐시된 카메라 객체 반환 또는 새로 생성"""
    global _camera_cache
    
    # 시스템 시작 시 모든 카메라 초기화 (처음 호출될 때만)
    if not _camera_init_done:
        initialize_all_cameras()
    
    # 문자열로 변환하여 일관성 유지
    camera_id_str = str(camera_id)
    
    if camera_id_str not in _camera_cache:
        try:
            # 카메라 매니저에서 URL 확인
            camera_manager = CameraManager.get_instance()
            url = camera_manager.get_camera_url(camera_id)
            
            if url:
                print(f"카메라 {camera_id} 캐시에 없음 - 새로 생성 중...")
                _camera_cache[camera_id_str] = VideoCamera(camera_id)
            else:
                print(f"카메라 {camera_id}의 URL을 찾을 수 없음")
                return None
        except Exception as e:
            print(f"카메라 {camera_id} 생성 중 오류: {e}")
            return None
    
    # 카메라 객체의 스트림 핸들러가 None이면 재연결 시도
    camera = _camera_cache[camera_id_str]
    if camera and hasattr(camera, 'stream_handler') and camera.stream_handler is None:
        print(f"카메라 {camera_id}의 스트림 핸들러가 없음 - 재생성 중...")
        try:
            # 캐시에서 제거하고 다시 생성
            del _camera_cache[camera_id_str]
            _camera_cache[camera_id_str] = VideoCamera(camera_id)
            camera = _camera_cache[camera_id_str]
        except Exception as e:
            print(f"카메라 {camera_id} 재생성 중 오류: {e}")
    
    return _camera_cache[camera_id_str]

def index(request):
    """메인 대시보드 페이지"""
    # 시스템 시작 시 모든 카메라 초기화 (처음 호출될 때만)
    if not _camera_init_done:
        initialize_all_cameras()
    
    camera_manager = CameraManager.get_instance()
    
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
        
        # 카메라 객체가 없으면 오류 프레임 반환
        if camera is None:
            empty_frame = np.zeros((480, 640, 3), dtype=np.uint8)
            cv2.putText(empty_frame, f"카메라 {camera_id}: 찾을 수 없음", 
                       (50, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            _, jpeg = cv2.imencode('.jpg', empty_frame, [cv2.IMWRITE_JPEG_QUALITY, 85])
            return HttpResponse(jpeg.tobytes(), content_type='image/jpeg')
        
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
    """최적화된 프레임 제너레이터 - FPS 제한 제거"""
    try:
        last_frame_time = time.time()
        min_interval = 0.005  # 초당 최대 200FPS까지 허용 (CPU 부하 방지를 위한 최소 제한)
        
        while True:
            # 현재 시간 체크
            current_time = time.time()
            elapsed = current_time - last_frame_time
            
            # 최소 시간 간격 보장 (CPU 과부하 방지)
            if elapsed < min_interval:
                time.sleep(0.001)  # 최소 대기
                continue
            
            # 프레임 가져오기 - 항상 최신 프레임
            frame = camera.get_frame(higher_quality=False)
            last_frame_time = time.time()
            
            if frame:
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            else:
                # 빈 프레임일 경우 짧게 대기
                time.sleep(0.01)
    except Exception as e:
        print(f"프레임 생성 오류: {e}")
        pass