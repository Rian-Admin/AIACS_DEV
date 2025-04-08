from .drawing import draw_detections

def create_automatic_deterrent_records(count=1):
    """
    자동으로 퇴치 기록을 생성하는 함수
    임시 구현 - 실제 퇴치 기록 생성 로직 필요
    """
    from ..models import DetectionInfo, DeterrentRecord, AcousticCannon, Speaker, SoundCode
    import random
    import datetime
    
    # 최근 감지 기록 가져오기
    recent_detections = DetectionInfo.objects.order_by('-detection_time')[:50]
    if not recent_detections:
        return 0
    
    # 가능한 퇴치 장비 가져오기
    try:
        cannons = list(AcousticCannon.objects.all())
        speakers = list(Speaker.objects.all())
        sounds = list(SoundCode.objects.all())
    except:
        # 모델이 없는 경우 빈 리스트로 설정
        cannons = []
        speakers = []
        sounds = []
    
    created_count = 0
    for _ in range(min(count, len(recent_detections))):
        # 랜덤 감지 기록 선택
        detection = random.choice(recent_detections)
        
        # 퇴치 기록 생성 시간 설정 (감지 시간 이후)
        start_time = detection.detection_time + datetime.timedelta(seconds=random.randint(1, 10))
        end_time = start_time + datetime.timedelta(seconds=random.randint(5, 30))
        
        # 퇴치 결과 랜덤 선택 (0: 실패, 1: 성공)
        result = random.choice([0, 1])
        
        # 퇴치 기록 생성
        record = DeterrentRecord(
            detection_id=detection,
            deterrent_start_time=start_time,
            deterrent_end_time=end_time,
            deterrent_result=result,
            cannon_id=random.choice(cannons) if cannons else None,
            speaker_id=random.choice(speakers) if speakers else None,
            sound_id=random.choice(sounds) if sounds else None,
        )
        record.save()
        created_count += 1
    
    return created_count