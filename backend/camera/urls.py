# urls.py
from django.urls import path
from .views.camera_views import index, camera_feed
from .views.api_views import get_recent_detections, get_detection_stats, get_db_status, get_cameras
from .views.api_views import set_yolo_conf, get_yolo_info, enable_controller, disable_controller
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
    azimuth_setting_view, add_azimuth, update_azimuth, delete_azimuth, get_azimuth
)

urlpatterns = [
    # 메인 뷰
    path('', index, name='index'),
    
    # 카메라 스트림
    path('camera/<str:camera_id>/', camera_feed, name='camera_feed'),
    
    # API - 감지 정보
    path('api/detections/recent/', get_recent_detections, name='recent_detections'),
    path('api/detections/stats/', get_detection_stats, name='detection_stats'),
    path('api/db/status/', get_db_status, name='db_status'),
    path('api/cameras/', get_cameras, name='get_cameras'),
    
    # 카메라 관리 뷰
    path('camera-management/', camera_management_view, name='camera_management'),
    path('api/camera/add/', add_camera, name='add_camera'),
    path('api/camera/update/<int:camera_id>/', update_camera, name='update_camera'),
    path('api/camera/delete/<int:camera_id>/', delete_camera, name='delete_camera'),
    path('api/camera/clear-cache/', clear_camera_cache, name='clear_camera_cache'),
    path('api/camera/upload-video/', upload_video, name='upload_video'),
    path('api/camera/get/<int:camera_id>/', get_camera, name='get_camera'),
    
    # 카메라 재연결 API 추가
    path('api/camera/reconnect/<str:camera_id>/', reconnect_camera, name='reconnect_camera'),
    path('api/camera/status/', get_camera_status, name='get_all_camera_status'),
    path('api/camera/status/<str:camera_id>/', get_camera_status, name='get_camera_status'),
    path('api/camera/reconnect-all/', reconnect_all_cameras, name='reconnect_all_cameras'),
    
    # 스피커 관리 뷰 및 API
    path('speaker-management/', speaker_management_view, name='speaker_management'),
    path('camera/speaker/status/', get_controller_status, name='speaker_status'),
    path('camera/speaker/play_sound/', play_sound, name='play_sound'),
    path('camera/speaker/stop_sound/', stop_sound, name='stop_sound'),
    path('camera/speaker/set_volume/', set_volume, name='set_volume'),
    path('camera/speaker/command/', send_tcp_command, name='send_tcp_command'),
    path('camera/speaker/connect/', connect_controller, name='connect_controller'),
    path('camera/speaker/disconnect/', disconnect_controller, name='disconnect_controller'),
    
    # DB 관리 뷰
    path('db-management/', db_management_view, name='db_management'),
    path('bird-analysis/', bird_analysis_view, name='bird_analysis'),
    
    # API - 데이터베이스 
    path('api/detections/filtered/', get_filtered_detections, name='filtered_detections'),
    path('api/detection/bb-info/<int:detection_id>/', get_detection_bb_info, name='detection_bb_info'),
    path('api/bird-analysis/data/', get_bird_analysis_data, name='bird_analysis_data'),
    
    # API - 모델 관리
    path('model-management/', model_management_view, name='model_management'),
    
    # API - 데이터 내보내기
    path('api/export-database-csv/', export_database_csv, name='export_database_csv'),
    path('api/export-detections-csv/', export_detections_csv, name='export_detections_csv'),
    path('api/export-bird-analysis-csv/', export_bird_analysis_csv, name='export_bird_analysis_csv'),
    path('api/export-cameras-csv/', export_cameras_csv, name='export_cameras_csv'),
    path('api/export-birds-csv/', export_birds_csv, name='export_birds_csv'),
    path('api/export-deterrents-csv/', export_deterrents_csv, name='export_deterrents_csv'),
    
    # API - DB 초기화
    path('api/clear-database/', clear_database, name='clear_database'),
    
    # API - 모델 관리
    path('api/yolo/upload-model/', upload_model, name='upload_model'),
    path('api/yolo/available-models/', get_available_models, name='available_models'),
    path('api/yolo/info/', get_yolo_info, name='yolo_info'),
    path('api/yolo/set-conf/', set_yolo_conf, name='set_yolo_conf'),
    path('api/yolo/switch-model/', switch_model, name='switch_model'),
    path('api/yolo/delete-model/', delete_model, name='delete_model'),
    
    # API - 조류퇴치기 제어
    path('api/controller/enable', enable_controller, name='enable_controller'),
    path('api/controller/disable', disable_controller, name='disable_controller'),
    
    # API - PTZ 카메라 제어
    path('api/ptz/control/', control_ptz, name='control_ptz'),
    path('api/ptz/position/<str:camera_id>/', get_ptz_position, name='get_ptz_position'),

    # 방위각 설정 뷰 및 API
    path('azimuth-setting/', azimuth_setting_view, name='azimuth_setting'),
    path('api/azimuth/add/', add_azimuth, name='add_azimuth'),
    path('api/azimuth/update/<int:azimuth_id>/', update_azimuth, name='update_azimuth'),
    path('api/azimuth/delete/<int:azimuth_id>/', delete_azimuth, name='delete_azimuth'),
    path('api/azimuth/get/<int:azimuth_id>/', get_azimuth, name='get_azimuth'),
]