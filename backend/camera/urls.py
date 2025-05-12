# urls.py
from django.urls import path
from .views.camera_views import index, camera_feed, camera_snapshot
from .views.api_views import get_recent_detections, get_detection_stats, get_db_status, get_cameras
from .views.api_views import set_yolo_conf, get_yolo_info, enable_controller, disable_controller
from .views.api_views import set_guard_zone, get_guard_zone, preview_guard_zone
from .views import api_views
from .views.camera_management import camera_management_view, add_camera, update_camera, delete_camera, clear_camera_cache, upload_video, get_camera
from .views.db_management import (
    db_management_view, bird_analysis_view, get_filtered_detections, get_detection_bb_info, get_bird_analysis_data,
    export_database_csv, export_detections_csv, export_bird_analysis_csv, export_cameras_csv, export_birds_csv, export_deterrents_csv,
    clear_database
)
from .views.model_management import model_management_view, upload_model, get_available_models, switch_model, delete_model
from .views.api_views import control_ptz, get_ptz_position
from .views.api_views import reconnect_camera, get_camera_status, reconnect_all_cameras
from .views.speaker_management import (
    speaker_management_view, get_controller_status, play_sound, stop_sound, 
    set_volume, send_tcp_command, connect_controller, disconnect_controller
)
# 방위각 설정 뷰 함수 가져오기
from .views.azimuth_setting import (
    azimuth_setting_view
)

urlpatterns = [
    # 메인 페이지
    path('', index, name='index'),
    
    # 카메라 피드 및 스냅샷
    path('camera/snapshot/<str:camera_id>/', camera_snapshot, name='camera_snapshot'),
    path('camera/preview-guard-zone/<str:camera_id>/', preview_guard_zone, name='preview_guard_zone'),
    path('camera/<str:camera_id>/', camera_feed, name='camera_feed'),
    
    # API 엔드포인트
    path('api/detections/', get_recent_detections, name='get_recent_detections'),
    path('api/detection-stats/', get_detection_stats, name='get_detection_stats'),
    path('api/db-status/', get_db_status, name='get_db_status'),
    path('api/cameras/', get_cameras, name='get_cameras'),
    
    # YOLO 모델 설정
    path('api/yolo/info/', get_yolo_info, name='get_yolo_info'),
    path('api/yolo/set-conf/', set_yolo_conf, name='set_yolo_conf'),
    
    # Guard Zone 설정
    path('api/yolo/set-guard-zone/', set_guard_zone, name='set_guard_zone'),
    path('api/yolo/get-guard-zone/<str:camera_id>/', get_guard_zone, name='get_guard_zone'),
    
    # 카메라 관리
    path('camera-management/', camera_management_view, name='camera_management'),
    path('api/camera/add/', add_camera, name='add_camera'),
    path('api/camera/update/<str:camera_id>/', update_camera, name='update_camera'),
    path('api/camera/delete/<str:camera_id>/', delete_camera, name='delete_camera'),
    path('api/camera/get/<str:camera_id>/', get_camera, name='get_camera'),
    path('api/camera/clear-cache/<str:camera_id>/', clear_camera_cache, name='clear_camera_cache'),
    path('api/camera/upload-video/', upload_video, name='upload_video'),
    
    # 카메라 상태 관리
    path('api/camera/reconnect/<str:camera_id>/', reconnect_camera, name='reconnect_camera'),
    path('api/camera/status/<str:camera_id>/', get_camera_status, name='get_camera_status'),
    path('api/camera/reconnect-all/', reconnect_all_cameras, name='reconnect_all_cameras'),
    
    # PTZ 컨트롤
    path('api/camera/ptz/control/<str:camera_id>/', control_ptz, name='control_ptz'),
    path('api/camera/ptz/position/<str:camera_id>/', get_ptz_position, name='get_ptz_position'),
    
    # 데이터베이스 관리
    path('db-management/', db_management_view, name='db_management'),
    path('bird-analysis/', bird_analysis_view, name='bird_analysis'),
    path('api/detections/filtered/', get_filtered_detections, name='get_filtered_detections'),
    path('api/detections/bb-info/<int:detection_id>/', get_detection_bb_info, name='get_detection_bb_info'),
    path('api/bird-analysis/', get_bird_analysis_data, name='get_bird_analysis_data'),
    
    # 데이터 내보내기
    path('api/export/database/', export_database_csv, name='export_database_csv'),
    path('api/export/detections/', export_detections_csv, name='export_detections_csv'),
    path('api/export/bird-analysis/', export_bird_analysis_csv, name='export_bird_analysis_csv'),
    path('api/export/cameras/', export_cameras_csv, name='export_cameras_csv'),
    path('api/export/birds/', export_birds_csv, name='export_birds_csv'),
    path('api/export/deterrents/', export_deterrents_csv, name='export_deterrents_csv'),
    
    # 데이터베이스 관리
    path('api/db/clear/', clear_database, name='clear_database'),
    
    # 모델 관리
    path('model-management/', model_management_view, name='model_management'),
    path('api/models/upload/', upload_model, name='upload_model'),
    path('api/models/list/', get_available_models, name='get_available_models'),
    path('api/models/switch/<str:model_id>/', switch_model, name='switch_model'),
    path('api/models/delete/<str:model_id>/', delete_model, name='delete_model'),
    
    # 스피커 관리
    path('speaker-management/', speaker_management_view, name='speaker_management'),
    path('api/controller/status/', get_controller_status, name='get_controller_status'),
    path('api/controller/connect/', connect_controller, name='connect_controller'),
    path('api/controller/disconnect/', disconnect_controller, name='disconnect_controller'),
    path('api/speaker/play/<int:speaker_id>/<str:sound_id>/', play_sound, name='play_sound'),
    path('api/speaker/stop/<int:speaker_id>/', stop_sound, name='stop_sound'),
    path('api/speaker/volume/<int:speaker_id>/<int:volume>/', set_volume, name='set_volume'),
    path('api/controller/enable/', enable_controller, name='enable_controller'),
    path('api/controller/disable/', disable_controller, name='disable_controller'),
    path('api/controller/send-tcp/', send_tcp_command, name='send_tcp_command'),
    
    # 방위각 설정
    path('azimuth-setting/', azimuth_setting_view, name='azimuth_setting'),
]