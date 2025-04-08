from django.urls import path
from .views import (
    RadarConnectionView, 
    RadarCommandView, 
    RadarStateView,
    RadarMessageView,
    CommandDetailView,
    RadarSimulationView,
    RadarOperationView,
    RadarPerformanceView,
    RadarStatusView,
    RadarTrackView,
    RadarPlotTrackView,
    track_delete_request, 
    get_deleted_tracks
)

urlpatterns = [
    path('connection/', RadarConnectionView.as_view(), name='radar_connection'),
    path('commands/', RadarCommandView.as_view(), name='radar_commands'),
    path('commands/<int:command_id>/', CommandDetailView.as_view(), name='command_detail'),
    path('state/', RadarStateView.as_view(), name='radar_state'),
    path('messages/', RadarMessageView.as_view(), name='radar_messages'),
    path('messages/<str:message_type>/', RadarMessageView.as_view(), name='radar_messages_type'),
    path('simulation/', RadarSimulationView.as_view(), name='radar_simulation'),
    path('operation/', RadarOperationView.as_view(), name='radar_operation'),
    path('performance/', RadarPerformanceView.as_view(), name='radar_performance'),
    path('status/', RadarStatusView.as_view(), name='radar_status'),
    path('track/', RadarTrackView.as_view(), name='radar_track'),
    path('plot_track/', RadarPlotTrackView.as_view(), name='radar_plot_track'),
    path('track/delete', track_delete_request, name='track_delete_request'),
    path('track/deleted', get_deleted_tracks, name='get_deleted_tracks'),
] 