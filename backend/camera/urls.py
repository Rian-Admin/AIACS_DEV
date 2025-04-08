# urls.py
from django.urls import path
from .views.camera_views import index, camera_feed
from .views.api_views import get_recent_detections, get_detection_stats, get_db_status, get_cameras
from .views.api_views import set_yolo_conf, get_yolo_info
from .views import api_views
from .views.camera_management import camera_management_view, add_camera, update_camera, delete_camera, clear_camera_cache, upload_video, get_camera
from .views.db_management import db_management_view, bird_analysis_view, get_filtered_detections, get_detection_bb_info, get_bird_analysis_data
from .views.model_management import model_management_view, upload_model, get_available_models, switch_model, delete_model
from .views.api_views import control_ptz, get_ptz_position
from .views.speaker_management import speaker_management_view, play_sound, stop_sound, set_volume, get_controller_status, connect_controller, disconnect_controller, send_tcp_command

urlpatterns = [
    # 메인 페이지 
    path('', index, name='index'),
    path('camera/<int:camera_id>/', camera_feed, name='camera_feed'),
    
    # 관리 인터페이스
    path('camera-management/', camera_management_view, name='camera_management'),
    path('db-management/', db_management_view, name='db_management'),
    path('bird-analysis/', bird_analysis_view, name='bird_analysis'),
    path('model-management/', model_management_view, name='model_management'),
    path('speaker-management/', speaker_management_view, name='speaker_management'),
    # API - 기존
    path('api/recent-detections/', get_recent_detections, name='recent_detections'),
    path('api/detection-stats/', get_detection_stats, name='detection_stats'),
    path('api/db-status/', get_db_status, name='db_status'),
    path('api/cameras/', get_cameras, name='get_cameras'),
    
    # API - 카메라 관리
    path('api/camera/add/', add_camera, name='add_camera'),
    path('api/camera/update/<str:camera_id>/', update_camera, name='update_camera'),
    path('api/camera/delete/<str:camera_id>/', delete_camera, name='delete_camera'),
    path('api/camera/clear-cache/', clear_camera_cache, name='clear_camera_cache'),
    path('api/camera/upload_video/', upload_video, name='upload_video'),
    path('api/camera/get/<str:camera_id>/', get_camera, name='get_camera'),
    
    # API - DB 관리
    path('api/detections-filtered/', get_filtered_detections, name='filtered_detections'),
    path('api/detection-bb-info/<str:detection_id>/', get_detection_bb_info, name='detection_bb_info'),
    path('api/bird-analysis/', get_bird_analysis_data, name='bird_analysis_data'),
    
    # API - 모델 관리
    path('api/yolo/upload-model/', upload_model, name='upload_model'),
    path('api/yolo/available-models/', get_available_models, name='available_models'),
    path('api/yolo/info/', get_yolo_info, name='yolo_info'),
    path('api/yolo/set-conf/', set_yolo_conf, name='set_yolo_conf'),
    path('api/yolo/switch-model/', switch_model, name='switch_model'),
    path('api/yolo/delete-model/', delete_model, name='delete_model'),
    
    # API - PTZ 카메라 제어
    path('api/ptz/control/', control_ptz, name='control_ptz'),
    path('api/ptz/position/<str:camera_id>/', get_ptz_position, name='get_ptz_position'),

     # 1. 프론트엔드 메인 패턴 - /camera/speaker/...
    path('camera/speaker/play_sound/', play_sound, name='camera_speaker_play'),
    path('camera/speaker/stop_sound/', stop_sound, name='camera_speaker_stop'),
    path('camera/speaker/set_volume/', set_volume, name='camera_speaker_volume'),
    path('camera/speaker/status/', get_controller_status, name='camera_speaker_status'),
    path('camera/speaker/connect/', connect_controller, name='camera_speaker_connect'),
    path('camera/speaker/disconnect/', disconnect_controller, name='camera_speaker_disconnect'),
    path('camera/speaker/send_tcp_command/', send_tcp_command, name='camera_speaker_send_command'),
    
    # 2. RESTful API 패턴 - /api/speaker/...
    path('api/speaker/play/', play_sound, name='play_sound'),
    path('api/speaker/stop/', stop_sound, name='stop_sound'),
    path('api/speaker/volume/', set_volume, name='set_volume'),
    path('api/speaker/status/', get_controller_status, name='get_controller_status'),

]