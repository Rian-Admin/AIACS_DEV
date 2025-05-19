# detection/models.py
from django.db import models

# 1. 탐지 정보 테이블 (detection_info)
class DetectionInfo(models.Model):
    detection_id = models.AutoField(primary_key=True)
    detection_time = models.DateTimeField()
    bb_count = models.IntegerField()
    camera_id = models.ForeignKey('Camera', on_delete=models.CASCADE)

    def __str__(self):
        return f"탐지 ID: {self.detection_id} - {self.detection_time}"

# 감지 영상 테이블 (detection_video)
class DetectionVideo(models.Model):
    camera = models.ForeignKey('Camera', on_delete=models.CASCADE)
    file_path = models.CharField(max_length=255)
    record_time = models.DateTimeField()
    duration = models.FloatField()
    
    def __str__(self):
        return f"영상: {self.camera} - {self.record_time}"
        
    class Meta:
        ordering = ['-record_time']

class BBInfo(models.Model):
    record_id = models.AutoField(primary_key=True)
    detection_info_id = models.ForeignKey('DetectionInfo', on_delete=models.CASCADE)
    class_id = models.ForeignKey('BirdClass', on_delete=models.CASCADE)
    bb_left = models.FloatField()
    bb_top = models.FloatField()
    bb_right = models.FloatField()
    bb_bottom = models.FloatField()

    def __str__(self):
        return f"BB ID: {self.detection_info_id} - {self.record_id}"

# 2. 퇴치 기록 테이블 (deterrent_record)
class DeterrentRecord(models.Model):
    deterrent_id = models.AutoField(primary_key=True)
    detection_id = models.ForeignKey(DetectionInfo, on_delete=models.CASCADE)
    detection_distance = models.FloatField()
    object_count = models.IntegerField()
    deterrent_start_time = models.DateTimeField()
    deterrent_end_time = models.DateTimeField()
    cannon_id = models.ForeignKey('AcousticCannon', on_delete=models.CASCADE)
    cannon_firing_time = models.DateTimeField()
    speaker_id = models.ForeignKey('Speaker', on_delete=models.CASCADE)
    sound_id = models.ForeignKey('SoundCode', on_delete=models.CASCADE)
    is_success = models.BooleanField()

    def __str__(self):
        return f"퇴치 ID: {self.deterrent_id} - {self.deterrent_start_time}"

# 3. 중요도 코드 테이블 (importance_code)
class ImportanceCode(models.Model):
    importance_code = models.CharField(max_length=10, primary_key=True)
    description = models.CharField(max_length=255)
    size_criteria_for_firing = models.CharField(max_length=255)
    priority_level = models.IntegerField()

    def __str__(self):
        return f"{self.description} (우선순위: {self.priority_level})"

# 4. 조류 클래스 테이블 (bird_class)
class BirdClass(models.Model):
    class_id = models.AutoField(primary_key=True)
    bird_type_code = models.CharField(max_length=50)
    bird_name_ko = models.CharField(max_length=100)
    scientific_name = models.CharField(max_length=100)
    protection_status_code = models.ForeignKey('BirdProtectionCode', on_delete=models.PROTECT)
    migration_code = models.ForeignKey('BirdMigrationCode', on_delete=models.PROTECT)
    risk_score = models.FloatField()

    def __str__(self):
        return self.bird_name_ko

# 5. 조류 부가정보 테이블 (bird_additional_info)
class BirdAdditionalInfo(models.Model):
    info_id = models.AutoField(primary_key=True)
    bird_class_id = models.ForeignKey(BirdClass, on_delete=models.CASCADE)
    habitat = models.CharField(max_length=255)
    breeding_start_month = models.IntegerField()
    breeding_end_month = models.IntegerField()
    frequency_jan = models.FloatField()
    frequency_feb = models.FloatField()
    frequency_mar = models.FloatField()
    frequency_apr = models.FloatField()
    frequency_may = models.FloatField()
    frequency_jun = models.FloatField()
    frequency_jul = models.FloatField()
    frequency_aug = models.FloatField()
    frequency_sep = models.FloatField()
    frequency_oct = models.FloatField()
    frequency_nov = models.FloatField()
    frequency_dec = models.FloatField()

    def __str__(self):
        return f"{self.bird_class_id.bird_name_ko}의 부가 정보"

# 6. 철새 텃새 코드 테이블 (birdd_migration_code)
class BirdMigrationCode(models.Model):
    code = models.CharField(max_length=10, primary_key=True)
    code_value = models.IntegerField(help_text="0: 철새, 1: 텃새")
    description = models.CharField(max_length=255)

    def __str__(self):
        return self.description

# 7. 조류 코드 테이블 (bird_protection_code)
class BirdProtectionCode(models.Model):
    code = models.CharField(max_length=10, primary_key=True)
    code_value = models.IntegerField(help_text="0: 일반 새, 1: 보호종, 2: 천연기념물")
    description = models.CharField(max_length=255)

    def __str__(self):
        return self.description

# 8. 카메라 테이블 (camera)
class Camera(models.Model):
    camera_id = models.IntegerField(primary_key=True)
    wind_turbine_id = models.CharField(max_length=100)
    viewing_angle = models.FloatField()
    installation_direction = models.CharField(max_length=50)
    installation_height = models.FloatField()
    rtsp_address = models.CharField(max_length=255)
    status = models.CharField(max_length=50)

    def __str__(self):
        return f"카메라 {self.camera_id}"

# 9. 음향 대포 테이블 (acoustic_cannon)
class AcousticCannon(models.Model):
    cannon_id = models.CharField(max_length=100, primary_key=True)
    wind_turbine_id = models.CharField(max_length=100)
    installation_location = models.CharField(max_length=255)
    last_lpg_charge_date = models.DateField()
    status = models.CharField(max_length=50)
    installation_date = models.DateField()

    def __str__(self):
        return f"음향대포 {self.cannon_id}"

# 10. 스피커 테이블 (speaker)
class Speaker(models.Model):
    speaker_id = models.CharField(max_length=100, primary_key=True)
    wind_turbine_id = models.CharField(max_length=100)
    installation_location = models.CharField(max_length=255)
    status = models.CharField(max_length=50)

    def __str__(self):
        return f"스피커 {self.speaker_id}"

# 11. 사운드 코드 테이블 (sound_code)
class SoundCode(models.Model):
    sound_id = models.AutoField(primary_key=True)
    sound_type = models.CharField(max_length=100)
    file_path = models.CharField(max_length=255)

    def __str__(self):
        return self.sound_type