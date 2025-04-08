from django.contrib import admin
from .models import (
    Camera, DetectionInfo, DeterrentRecord,
    BirdClass, BirdAdditionalInfo, BirdProtectionCode, BirdMigrationCode,
    ImportanceCode, AcousticCannon, Speaker, SoundCode
)

# Register your models here.
admin.site.register(Camera)
admin.site.register(DetectionInfo)
admin.site.register(DeterrentRecord)
admin.site.register(BirdClass)
admin.site.register(BirdAdditionalInfo)
admin.site.register(BirdProtectionCode)
admin.site.register(BirdMigrationCode)
admin.site.register(ImportanceCode)
admin.site.register(AcousticCannon)
admin.site.register(Speaker)
admin.site.register(SoundCode)
