#C:\Users\AAA\Documents\GitHub\AIACS\AIACS_WEB\backend\camera\views\camera_views.py
from django.shortcuts import render
from django.http import StreamingHttpResponse, HttpResponse
from ..frame.video_camera import VideoCamera
from ..frame.camera_manager import CameraManager
import time
import numpy as np
import cv2
from ..models import Camera, DetectionVideo
import threading
import logging
import os
import datetime
from django.utils import timezone
from django.conf import settings
from django.core.files.storage import FileSystemStorage
from django.http import JsonResponse
from pathlib import Path

logger = logging.getLogger(__name__)

# 카메라 객체 캐싱 (매번 새로 생성하지 않도록)
_camera_cache = {}
_camera_init_done = False  # 시스템 초기화 완료 여부 추적
_initialization_lock = threading.Lock()  # 초기화 프로세스 동기화를 위한 락

def initialize_all_cameras():
    """모든 카메라를 강제로 초기화하는 함수 - 시스템 시작 시 호출"""
    global _camera_init_done, _initialization_lock, _camera_cache
    
    # 이미 초기화 중인지 확인
    with _initialization_lock:
        if _camera_init_done:
            logger.info("카메라 시스템이 이미 초기화되어 있습니다.")
            return  # 이미 초기화되었으면 재실행하지 않음
    
        print("시스템 시작 시 모든 카메라 강제 초기화...")
        
        # 동기화된 초기화 작업을 수행
        _initialize_cameras_internal()
        
        # 초기화 완료 표시
        _camera_init_done = True

def _initialize_cameras_internal():
    """실제 카메라 초기화 작업을 수행하는 내부 함수"""
    try:
        # 카메라 매니저 인스턴스 가져오기
        camera_manager = CameraManager.get_instance()
        
        # DB에서 모든 카메라 가져오기
        db_cameras = Camera.objects.all()
        
        if not db_cameras:
            print("등록된 카메라가 없습니다.")
            return
            
        # 카메라 초기화 순서 최적화 (기본 카메라 먼저)
        camera_ids = sorted([camera.camera_id for camera in db_cameras])
        
        # 각 카메라를 초기화하고 카메라 캐시에 추가
        for camera_id in camera_ids:
            # 초기화 시작 로그
            print(f"카메라 {camera_id} 초기화 중...")
            
            # 이미 캐시에 있는 경우 건너뜀
            if str(camera_id) in _camera_cache and _camera_cache[str(camera_id)] is not None:
                if hasattr(_camera_cache[str(camera_id)], 'stream_handler') and _camera_cache[str(camera_id)].stream_handler is not None:
                    print(f"카메라 {camera_id}는 이미 캐시에 있음")
                    continue
            
            # 새 카메라 인스턴스 생성 및 캐시에 추가
            try:
                camera_obj = VideoCamera(camera_id)
                
                # 성공적으로 초기화되었는지 확인
                if hasattr(camera_obj, 'stream_handler') and camera_obj.stream_handler is not None:
                    _camera_cache[str(camera_id)] = camera_obj
                    print(f"카메라 {camera_id} 초기화 성공")
                else:
                    print(f"카메라 {camera_id} 스트림 핸들러 초기화 실패")
                
                # 잠시 대기하여 시스템에 과부하가 걸리지 않도록 함
                time.sleep(1.0)
                
            except Exception as e:
                print(f"카메라 {camera_id} 초기화 중 오류 발생: {e}")
        
        # 모든 카메라 초기화 완료
        print("모든 카메라 초기화 완료!")
        
    except Exception as e:
        print(f"카메라 초기화 중 오류 발생: {e}")
        import traceback
        traceback.print_exc()

def get_cached_camera(camera_id):
    """캐시된 카메라 객체 반환 또는 새로 생성"""
    global _camera_cache, _camera_init_done
    
    # 시스템 시작 시 모든 카메라 초기화 (처음 호출될 때만)
    if not _camera_init_done:
        # 별도 스레드에서 초기화 (웹 서버 시작 차단 방지)
        threading.Thread(target=initialize_all_cameras, daemon=True).start()
        # 초기화 중인 것으로 표시 (다음 호출 시 다시 초기화하지 않도록)
        _camera_init_done = True
    
    # 문자열로 변환하여 일관성 유지
    camera_id_str = str(camera_id)
    
    if camera_id_str not in _camera_cache or _camera_cache[camera_id_str] is None:
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
    
    # 카메라 ID를 숫자형으로 정렬 (문자열로 된 숫자도 올바르게 정렬)
    sorted_camera_ids = sorted(available_cameras.keys(), key=lambda x: int(x) if str(x).isdigit() else x)
    
    # 정렬된 카메라 정보를 담을 새 딕셔너리
    sorted_camera_info = {}
    for camera_id in sorted_camera_ids:
        sorted_camera_info[camera_id] = available_cameras[camera_id]
    
    return render(request, 'detection/index.html', {
        'camera_info': sorted_camera_info,
        'cameras': sorted_camera_ids
    })

def ptz_controller(request):
    """PTZ 조류 추적 컨트롤러 페이지"""
    # 시스템 시작 시 모든 카메라 초기화 (처음 호출될 때만)
    if not _camera_init_done:
        initialize_all_cameras()
    
    # DB에서 모든 카메라 가져오기
    db_cameras = Camera.objects.all()
    
    # 카메라 목록 준비
    cameras = []
    for camera in db_cameras:
        cameras.append({
            'camera_id': camera.camera_id,
            'installation_direction': camera.installation_direction,
            'rtsp_address': camera.rtsp_address,
            'status': camera.status
        })
    
    return render(request, 'detection/ptz_controller.html', {
        'cameras': cameras
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

def detection_videos_view(request):
    """감지 영상 목록 페이지"""
    # 필터 파라미터 가져오기
    date_filter = request.GET.get('date')
    camera_filter = request.GET.get('camera')
    
    # 기본 쿼리셋
    videos_query = DetectionVideo.objects.all().order_by('-record_time')
    
    # 필터 적용
    if date_filter:
        # 날짜 형식 변환
        try:
            filter_date = datetime.datetime.strptime(date_filter, '%Y-%m-%d').date()
            # 해당 날짜의 시작과 끝
            date_start = datetime.datetime.combine(filter_date, datetime.time.min).replace(tzinfo=timezone.get_current_timezone())
            date_end = datetime.datetime.combine(filter_date, datetime.time.max).replace(tzinfo=timezone.get_current_timezone())
            videos_query = videos_query.filter(record_time__range=(date_start, date_end))
        except ValueError:
            pass  # 유효하지 않은 날짜 형식
    
    if camera_filter and camera_filter.isdigit():
        videos_query = videos_query.filter(camera__camera_id=camera_filter)
    
    # 파일 경로 처리 (절대 경로에서 상대 URL 경로로 변환)
    videos = []
    for video in videos_query:
        # 서버의 파일시스템 경로에서 웹 접근 가능한 URL로 변환
        video_path = video.file_path
        
        # 파일 이름만 추출
        if isinstance(video_path, str):
            # 윈도우 경로에서 파일명만 추출
            filename = os.path.basename(video_path)
        else:
            # Path 객체에서 파일명만 추출
            filename = os.path.basename(str(video_path))
        
        # 새 URL 경로 생성 - /video_recordings/ 경로로 통일
        video.file_path = f'/video_recordings/{filename}'
        
        # 파일이 실제로 존재하는지 확인 (디버깅용)
        video_absolute_path = os.path.join(settings.BASE_DIR, 'video_recordings', filename)
        if not os.path.exists(video_absolute_path):
            print(f"경고: 비디오 파일이 존재하지 않습니다: {video_absolute_path}")
        
        videos.append(video)
    
    # 모든 카메라 정보
    cameras = Camera.objects.all()
    
    return render(request, 'detection/detection_videos.html', {
        'videos': videos,
        'cameras': cameras,
        'date_filter': date_filter,
        'camera_filter': camera_filter
    })