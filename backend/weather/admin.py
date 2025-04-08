from django.contrib import admin
from .models import WeatherData

@admin.register(WeatherData)
class WeatherDataAdmin(admin.ModelAdmin):
    list_display = ('location_name', 'timestamp', 'temperature', 'humidity', 'wind_speed', 'precipitation')
    list_filter = ('location_name', 'timestamp')
    search_fields = ('location_name',)
    readonly_fields = ('timestamp', 'update_time')
