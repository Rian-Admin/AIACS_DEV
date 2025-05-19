from django.contrib import admin
from django.urls import path, include, re_path
from django.conf import settings
from django.conf.urls.static import static
from django.http import FileResponse, HttpResponse
import os
import mimetypes

def serve_video_file(request, path):
    """비디오 파일을 단순하게 서빙하는 뷰"""
    # 파일 경로 생성
    file_path = os.path.join(settings.BASE_DIR, 'video_recordings', path)
    
    # 파일이 존재하는지 확인
    if not os.path.exists(file_path):
        return HttpResponse(f"파일을 찾을 수 없습니다: {path}", status=404)
    
    # 파일 확장자 기반으로 MIME 타입 설정
    _, ext = os.path.splitext(file_path)
    ext = ext.lower().lstrip('.')
    content_type = settings.MIME_TYPES.get(ext, 'application/octet-stream')
    
    # 단순 파일 응답 생성 (Range 요청 지원 없음)
    response = FileResponse(open(file_path, 'rb'), content_type=content_type)
    response['Content-Disposition'] = f'inline; filename="{os.path.basename(file_path)}"'
    return response

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('camera.urls')),  
    path('', include('weather.urls')), 
    path('api/radar/', include('radar_api.urls')),
    # 비디오 파일 서빙을 위한 커스텀 URL 패턴
    path('video_recordings/<path:path>', serve_video_file, name='serve_video'),
]

# 개발 환경에서 미디어 파일 접근을 위한 설정
if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
    
    # 비디오 녹화 파일 접근을 위한 추가 설정
    # 경로 설정 및 디렉토리 확인
    video_recordings_path = os.path.join(settings.BASE_DIR, 'video_recordings')
    # 경로 확인을 위한 출력
    print(f"비디오 녹화 파일 경로: {video_recordings_path}")
    
    # 해당 디렉토리가 존재하는지 확인
    if os.path.exists(video_recordings_path):
        print(f"비디오 디렉토리 확인됨: {video_recordings_path}")
    else:
        print(f"경고: 비디오 디렉토리를 찾을 수 없음: {video_recordings_path}")
        # 대체 경로 시도
        corrected_video_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'video_recordings')
        if os.path.exists(corrected_video_path):
            print(f"대체 비디오 디렉토리 찾음: {corrected_video_path}")
            video_recordings_path = corrected_video_path
        else:
            # 폴더 생성 시도
            try:
                os.makedirs(video_recordings_path, exist_ok=True)
                print(f"비디오 디렉토리 생성됨: {video_recordings_path}")
            except Exception as e:
                print(f"비디오 디렉토리 생성 실패: {e}")
    
    # 정적 파일 URL 추가 (Django의 기본 static 서빙 사용)
    urlpatterns += static('/video_recordings/', document_root=video_recordings_path)
