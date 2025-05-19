from django.db import models

# Create your models here.

class WeatherData(models.Model):
    """기상청 API에서 가져온 날씨 정보를 저장하는 모델"""
    
    # 기본 정보
    timestamp = models.DateTimeField(auto_now_add=True)
    location_name = models.CharField(max_length=100, default="소각시도")
    
    # 현재 날씨 정보
    temperature = models.FloatField(verbose_name="현재 기온(°C)", null=True, blank=True)
    feels_like = models.FloatField(verbose_name="체감 온도(°C)", null=True, blank=True)
    humidity = models.FloatField(verbose_name="습도(%)", null=True, blank=True)
    wind_speed = models.FloatField(verbose_name="풍속(m/s)", null=True, blank=True)
    wind_direction = models.CharField(verbose_name="풍향", max_length=10, null=True, blank=True)
    precipitation = models.FloatField(verbose_name="강수량(mm)", null=True, blank=True)
    precipitation_type = models.CharField(verbose_name="강수 형태", max_length=20, null=True, blank=True)
    air_quality = models.CharField(verbose_name="대기질", max_length=20, null=True, blank=True)
    visibility = models.FloatField(verbose_name="시정(km)", null=True, blank=True)
    
    # 예보 정보
    forecast_temperature = models.FloatField(verbose_name="예보 기온(°C)", null=True, blank=True)
    forecast_humidity = models.FloatField(verbose_name="예보 습도(%)", null=True, blank=True)
    forecast_wind_speed = models.FloatField(verbose_name="예보 풍속(m/s)", null=True, blank=True)
    forecast_precipitation = models.FloatField(verbose_name="예보 강수확률(%)", null=True, blank=True)
    forecast_air_quality = models.CharField(verbose_name="예보 대기질", max_length=20, null=True, blank=True)
    
    # 추가 정보
    update_time = models.DateTimeField(auto_now=True)
    
    class Meta:
        verbose_name = "날씨 데이터"
        verbose_name_plural = "날씨 데이터"
        ordering = ['-timestamp']
    
    def __str__(self):
        return f"{self.location_name} 날씨 - {self.timestamp.strftime('%Y-%m-%d %H:%M')}"
