#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 퇴치 기록 더미 데이터 생성 스크립트
# DB에 랜덤한 퇴치 기록 데이터를 생성하여 삽입합니다.

import os
import sys
import random
import datetime
import django
from django.utils import timezone

# Django 설정 로드
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'config.settings')
django.setup()

# 모델 임포트
from camera.models import (
    DetectionInfo, Camera, DeterrentRecord, 
    AcousticCannon, Speaker, SoundCode
)

def create_deterrent_dummy_data(count=30):
    """
    랜덤한 퇴치 기록 더미 데이터를 생성하여 DB에 삽입합니다.
    
    Args:
        count: 생성할 더미 데이터 수
    """
    print(f"퇴치 기록 더미 데이터 {count}개 생성 시작...")
    
    # 기존 카메라 확인 또는 생성
    cameras = list(Camera.objects.all())
    if not cameras:
        print("카메라 데이터가 없습니다. 더미 카메라 생성 중...")
        for i in range(1, 4):
            Camera.objects.create(
                camera_id=i,
                wind_turbine_id=f"WT{i}",
                viewing_angle=random.uniform(30.0, 120.0),
                installation_direction=random.choice(['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW']),
                installation_height=random.uniform(30.0, 100.0),
                rtsp_address=f"rtsp://example.com/camera{i}",
                status="Active"
            )
        cameras = list(Camera.objects.all())
    
    # 기존 음향 대포 확인 또는 생성
    cannons = list(AcousticCannon.objects.all())
    if not cannons:
        print("음향 대포 데이터가 없습니다. 더미 대포 생성 중...")
        for i in range(1, 4):
            AcousticCannon.objects.create(
                cannon_id=f"C{i}",
                wind_turbine_id=f"WT{i}",
                installation_location=f"Tower {i}",
                last_lpg_charge_date=timezone.now().date() - datetime.timedelta(days=random.randint(1, 30)),
                status="Active",
                installation_date=timezone.now().date() - datetime.timedelta(days=random.randint(60, 365))
            )
        cannons = list(AcousticCannon.objects.all())
    
    # 기존 스피커 확인 또는 생성
    speakers = list(Speaker.objects.all())
    if not speakers:
        print("스피커 데이터가 없습니다. 더미 스피커 생성 중...")
        for i in range(1, 4):
            Speaker.objects.create(
                speaker_id=f"SP{i}",
                wind_turbine_id=f"WT{i}",
                installation_location=f"Tower {i}",
                status="Active"
            )
        speakers = list(Speaker.objects.all())
    
    # 기존 사운드 코드 확인 또는 생성
    sounds = list(SoundCode.objects.all())
    if not sounds:
        print("사운드 코드 데이터가 없습니다. 더미 사운드 생성 중...")
        sound_types = ["새 경보음", "맹금류 울음소리", "포식자 소리", "고주파 경보음", "초음파"]
        for i, sound_type in enumerate(sound_types, 1):
            SoundCode.objects.create(
                sound_id=i,
                sound_type=sound_type,
                file_path=f"/sounds/{sound_type.replace(' ', '_').lower()}.mp3"
            )
        sounds = list(SoundCode.objects.all())
    
    # 기존 탐지 정보 확인
    detections = list(DetectionInfo.objects.all())
    if not detections:
        print("탐지 정보가 없습니다. 더미 탐지 정보 생성 중...")
        for i in range(1, count + 1):
            camera = random.choice(cameras)
            # 최근 30일 내의 랜덤한 시간
            detection_time = timezone.now() - datetime.timedelta(
                days=random.randint(0, 30),
                hours=random.randint(0, 23),
                minutes=random.randint(0, 59)
            )
            DetectionInfo.objects.create(
                detection_id=i,
                detection_time=detection_time,
                bb_count=random.randint(1, 5),
                camera_id=camera
            )
        detections = list(DetectionInfo.objects.all())
    
    # 기존 퇴치 기록 삭제 (선택적)
    # DeterrentRecord.objects.all().delete()
    # print("기존 퇴치 기록이 삭제되었습니다.")
    
    # 퇴치 기록 생성
    created_count = 0
    for i in range(count):
        try:
            # 랜덤 감지 정보 선택
            detection = random.choice(detections)
            
            # 퇴치 시작 시간 (감지 시간과 가까워야 함)
            deterrent_start_time = detection.detection_time + datetime.timedelta(
                seconds=random.randint(1, 5)
            )
            
            # 퇴치 종료 시간 (시작 시간 이후)
            deterrent_end_time = deterrent_start_time + datetime.timedelta(
                minutes=random.randint(1, 10)
            )
            
            # 음향 대포 발사 시간
            cannon_firing_time = deterrent_start_time + datetime.timedelta(
                seconds=random.randint(1, 3)
            )
            
            # 랜덤 장비 선택
            cannon = random.choice(cannons)
            speaker = random.choice(speakers)
            sound = random.choice(sounds)
            
            # 성공 여부 (80% 확률로 성공)
            is_success = random.random() < 0.8
            
            # 퇴치 기록 생성
            DeterrentRecord.objects.create(
                detection_id=detection,
                detection_distance=random.uniform(50.0, 500.0),  # 50m ~ 500m
                object_count=detection.bb_count,
                deterrent_start_time=deterrent_start_time,
                deterrent_end_time=deterrent_end_time,
                cannon_id=cannon,
                cannon_firing_time=cannon_firing_time,
                speaker_id=speaker,
                sound_id=sound,
                is_success=is_success
            )
            created_count += 1
            
            if created_count % 10 == 0:
                print(f"{created_count}개의 퇴치 기록이 생성되었습니다.")
                
        except Exception as e:
            print(f"퇴치 기록 생성 중 오류 발생: {e}")
    
    print(f"총 {created_count}개의 퇴치 기록 더미 데이터가 생성되었습니다.")

if __name__ == '__main__':
    # 커맨드 라인 인자로 생성할 데이터 수를 받을 수 있음
    count = 50
    if len(sys.argv) > 1:
        try:
            count = int(sys.argv[1])
        except ValueError:
            print("올바른 숫자를 입력하세요. 기본값 50개를 사용합니다.")
    
    create_deterrent_dummy_data(count) 