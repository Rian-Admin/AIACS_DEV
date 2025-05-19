from django.urls import path
from . import views

urlpatterns = [
    path('api/weather/current/', views.get_weather, name='get_weather'),
    path('api/weather/update/', views.update_weather, name='update_weather'),
    path('api/weather/history/', views.get_weather_history, name='get_weather_history'),
] 