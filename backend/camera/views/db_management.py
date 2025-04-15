from django.shortcuts import render
from django.http import JsonResponse, HttpResponse
from django.views.decorators.csrf import csrf_exempt
import datetime
import random
from django.db.models import Count
from django.utils import timezone
from ..models import (
    DetectionInfo, Camera, BirdClass, DeterrentRecord,
    BirdAdditionalInfo, BirdMigrationCode, BirdProtectionCode,
    AcousticCannon, Speaker, SoundCode, BBInfo
)
from ..utils import create_automatic_deterrent_records
from ..db import db_adapter
import logging
import csv
import json
import zipfile
import io
import tempfile
import os
import pytz  # pytz 추가

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

# 한국 시간대 정의
KST = pytz.timezone('Asia/Seoul')

def db_management_view(request):
    """데이터베이스 관리 페이지"""
    # 기본 통계 데이터 준비
    total_detections = DetectionInfo.objects.count()
    
    # 오늘의 감지 수
    today = datetime.datetime.now().date()
    today_start = datetime.datetime.combine(today, datetime.time.min)
    today_end = datetime.datetime.combine(today, datetime.time.max)
    today_detections = DetectionInfo.objects.filter(
        detection_time__range=(today_start, today_end)
    ).count()
    
    # 카메라 수
    cameras_count = Camera.objects.count()
    
    # DB 어댑터 상태 확인
    try:
        db_status_info = db_adapter.get_adapter_status()
        db_status = "활성" if db_status_info['event_loop_running'] else "비활성"
        db_status_color = "success" if db_status_info['event_loop_running'] else "danger"
    except Exception as e:
        logger.error(f"DB 상태 확인 오류: {e}")
        db_status = "오류"
        db_status_color = "warning"
    
    # 최근 감지 기록 조회 (최대 20개)
    # 시간 간격을 고려하여 객체 탐지 정보를 조회
    detections = DetectionInfo.objects.select_related('camera_id').order_by('-detection_time')[:100]
    
    # 자동으로 신규 퇴치 기록 생성 (50% 확률로 자동 생성)
    if random.random() < 0.5:
        count = random.randint(1, 4)  # 1~4개 랜덤 생성
        try:
            created_count = create_automatic_deterrent_records(count)
            if created_count > 0:
                logger.info(f"새로운 퇴치 기록 {created_count}개가 자동으로 생성되었습니다.")
        except Exception as e:
            logger.error(f"퇴치 기록 자동 생성 오류: {e}")
    
    # 카메라 목록
    cameras = Camera.objects.all()
    
    # 조류 클래스 정보
    try:
        birds = BirdClass.objects.select_related('protection_status_code', 'migration_code').all()
        logger.info(f"조류 클래스 정보를 가져왔습니다. {birds.count()}개 클래스")
    except Exception as e:
        logger.error(f"조류 클래스 정보 가져오기 오류: {e}")
        birds = []  # 오류 발생 시 빈 리스트로 처리
    
    # 퇴치 기록
    deterrents = DeterrentRecord.objects.select_related('detection_id', 'cannon_id', 'speaker_id', 'sound_id').order_by('-deterrent_start_time')[:20]
    
    # 바운딩 박스 정보
    try:
        bb_infos = BBInfo.objects.select_related('detection_info_id_id', 'class_id').order_by('-record_id')[:100]
        
        # 바운딩 박스 영역 계산
        for bb in bb_infos:
            bb.bb_width = bb.bb_right - bb.bb_left
            bb.bb_height = bb.bb_bottom - bb.bb_top
            bb.bb_area = bb.bb_width * bb.bb_height
            
        logger.info(f"바운딩 박스 정보를 가져왔습니다. {len(bb_infos)}개")
    except Exception as e:
        logger.error(f"바운딩 박스 정보 가져오기 오류: {e}")
        bb_infos = []
    
    context = {
        'total_detections': total_detections,
        'today_detections': today_detections,
        'cameras_count': cameras_count,
        'db_status': db_status,
        'db_status_color': db_status_color,
        'detections': detections,
        'cameras': cameras,
        'birds': birds,
        'deterrents': deterrents,
        'bb_infos': bb_infos
    }
    
    return render(request, 'detection/db_management.html', context)

def bird_analysis_view(request):
    """조류 분석 페이지"""
    # 조류 클래스 정보 조회
    try:
        birds = BirdClass.objects.select_related('protection_status_code', 'migration_code').all()
    except Exception as e:
        logger.error(f"조류 클래스 정보 조회 오류: {e}")
        birds = []
    
    context = {
        'birds': birds
    }
    
    return render(request, 'detection/bird_analysis.html', context)

@csrf_exempt
def get_filtered_detections(request):
    """날짜 및 카메라별 감지 기록 필터링 API"""
    try:
        # 파라미터 가져오기
        date_from = request.GET.get('date_from')
        date_to = request.GET.get('date_to')
        camera_id = request.GET.get('camera_id')
        page = int(request.GET.get('page', 1))
        per_page = int(request.GET.get('per_page', 50))  # 기본값 50개
        
        # 정렬 방식
        sort_by = request.GET.get('sort_by', 'id')  # 기본값은 ID순
        
        # 디버깅용 로그 추가
        logger.info(f"필터링 요청 수신: date_from={date_from}, date_to={date_to}, camera_id={camera_id}, page={page}, per_page={per_page}, sort_by={sort_by}")
        
        # 기본 쿼리셋
        queryset = DetectionInfo.objects.select_related('camera_id').all()
        
        # 필터 적용
        if date_from:
            try:
                date_from_obj = datetime.datetime.strptime(date_from, '%Y-%m-%d').replace(hour=0, minute=0, second=0)
                queryset = queryset.filter(detection_time__gte=date_from_obj)
                logger.info(f"시작일 필터 적용: {date_from_obj}")
            except ValueError:
                logger.warning(f"시작일 형식 오류: {date_from}")
                pass
        
        if date_to:
            try:
                date_to_obj = datetime.datetime.strptime(date_to, '%Y-%m-%d').replace(hour=23, minute=59, second=59)
                queryset = queryset.filter(detection_time__lte=date_to_obj)
                logger.info(f"종료일 필터 적용: {date_to_obj}")
            except ValueError:
                logger.warning(f"종료일 형식 오류: {date_to}")
                pass
        
        if camera_id:
            queryset = queryset.filter(camera_id=camera_id)
            logger.info(f"카메라 ID 필터 적용: {camera_id}")
        
        # 정렬 방식 적용
        if sort_by == 'id':
            queryset = queryset.order_by('detection_id')  # ID 오름차순
            logger.info("ID 순서로 정렬 적용")
        elif sort_by == 'id_desc':
            queryset = queryset.order_by('-detection_id')  # ID 내림차순
            logger.info("ID 역순으로 정렬 적용")
        elif sort_by == 'date_desc':
            queryset = queryset.order_by('-detection_time')  # 날짜 내림차순
            logger.info("날짜 내림차순으로 정렬 적용")
        elif sort_by == 'date':
            queryset = queryset.order_by('detection_time')  # 날짜 오름차순
            logger.info("날짜 오름차순으로 정렬 적용")
        else:
            queryset = queryset.order_by('detection_id')  # 기본: ID 오름차순
            logger.info("기본 정렬(ID 순서) 적용")
        
        # 총 레코드 수
        total_records = queryset.count()
        
        # 총 페이지 수 계산 (올림)
        total_pages = max(1, (total_records + per_page - 1) // per_page)
        
        logger.info(f"총 레코드 수: {total_records}, 총 페이지 수: {total_pages}")
        
        # 페이지가 범위를 벗어나면 적절히 조정
        if page < 1:
            page = 1
        elif page > total_pages and total_pages > 0:
            page = total_pages
        
        # 페이지네이션 적용
        start_idx = (page - 1) * per_page
        end_idx = start_idx + per_page
        page_records = queryset[start_idx:end_idx]
        
        logger.info(f"페이지네이션: 현재 페이지 {page}/{total_pages}, 레코드 범위 {start_idx}-{end_idx}")
        
        # 결과 포맷팅
        detections = []
        for detection in page_records:
            detections.append({
                'detection_id': detection.detection_id,
                'detection_time': detection.detection_time.isoformat() if detection.detection_time else None,
                'bb_count': detection.bb_count,
                'camera_id': detection.camera_id.camera_id if detection.camera_id else None,
            })
        
        # 응답 데이터에 페이지 정보 추가
        response_data = {
            'status': 'success',
            'detections': detections,
            'total_records': total_records,
            'total_pages': total_pages,
            'current_page': page,
            'per_page': per_page
        }
        
        logger.info(f"응답: {len(detections)}건의 데이터 반환")
        return JsonResponse(response_data)
        
    except Exception as e:
        logger.error(f"감지 기록 필터링 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500)

@csrf_exempt
def get_detection_bb_info(request, detection_id):
    """특정 감지 ID에 대한 바운딩 박스 정보를 반환하는 API"""
    try:
        bb_info = BBInfo.objects.select_related('class_id').filter(
            detection_info_id=detection_id
        )
        
        result = []
        for bb in bb_info:
            result.append({
                'record_id': bb.record_id,
                'class_id': bb.class_id.class_id,
                'class_name': bb.class_id.bird_name_ko,
                'bb_left': bb.bb_left,
                'bb_top': bb.bb_top,
                'bb_right': bb.bb_right,
                'bb_bottom': bb.bb_bottom
            })
        
        return JsonResponse({
            'status': 'success',
            'bb_info': result
        })
        
    except Exception as e:
        logger.error(f"바운딩 박스 정보 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500)

@csrf_exempt
def get_bird_analysis_data(request):
    """조류 분석을 위한 바운딩 박스 데이터 API"""
    try:
        # 요청에서 필터 매개변수 추출
        date_from = request.GET.get('date_from')
        date_to = request.GET.get('date_to')
        bird_class = request.GET.get('bird_class')
        
        # 페이지네이션 매개변수 추출
        page = int(request.GET.get('page', 1))
        per_page = int(request.GET.get('per_page', 1000))  # 기본값을 1000으로 설정 (모든 데이터 가져오기)
        get_all = request.GET.get('get_all', 'false').lower() == 'true'  # 'get_all' 파라미터가 true이면 모든 데이터 반환
        
        # 기본 쿼리셋 생성
        bb_query = BBInfo.objects.select_related('detection_info_id', 'class_id', 'detection_info_id__camera_id')
        
        # 필터 적용
        if date_from:
            try:
                date_from_obj = datetime.datetime.strptime(date_from, '%Y-%m-%d').replace(hour=0, minute=0, second=0)
                date_from_obj = date_from_obj.replace(tzinfo=timezone.get_current_timezone())
                bb_query = bb_query.filter(detection_info_id__detection_time__gte=date_from_obj)
            except ValueError:
                pass
        
        if date_to:
            try:
                date_to_obj = datetime.datetime.strptime(date_to, '%Y-%m-%d').replace(hour=23, minute=59, second=59)
                date_to_obj = date_to_obj.replace(tzinfo=timezone.get_current_timezone())
                bb_query = bb_query.filter(detection_info_id__detection_time__lte=date_to_obj)
            except ValueError:
                pass
        
        if bird_class:
            bb_query = bb_query.filter(class_id=bird_class)
        
        # 전체 레코드 수 계산
        total_records = bb_query.count()
        total_pages = max(1, (total_records + per_page - 1) // per_page)
        
        # 정렬 적용
        bb_query = bb_query.order_by('-detection_info_id__detection_time')
        
        # 페이지네이션 적용 (get_all이 true인 경우 모든 데이터 반환)
        if not get_all:
            start_idx = (page - 1) * per_page
            end_idx = start_idx + per_page
            bb_info = bb_query[start_idx:end_idx]
        else:
            bb_info = bb_query.all()  # 모든 데이터 반환
        
        # 응답을 위한 데이터 구성
        bb_data = []
        for bb in bb_info:
            bb_data.append({
                'record_id': bb.record_id,
                'detection_id': bb.detection_info_id.detection_id,
                'detection_time': bb.detection_info_id.detection_time.isoformat() if bb.detection_info_id.detection_time else None,
                'class_id': bb.class_id.class_id,
                'class_name': bb.class_id.bird_name_ko,
                'bb_left': bb.bb_left,
                'bb_top': bb.bb_top,
                'bb_right': bb.bb_right,
                'bb_bottom': bb.bb_bottom,
                'camera_id': bb.detection_info_id.camera_id.camera_id if bb.detection_info_id.camera_id else None
            })
        
        # 페이지네이션 정보 포함
        return JsonResponse({
            'status': 'success',
            'bb_data': bb_data,
            'total_records': total_records,
            'total_pages': total_pages,
            'current_page': page,
            'per_page': per_page
        })
        
    except Exception as e:
        logger.error(f"조류 분석 데이터 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500)

def export_cameras_csv(request):
    """카메라 정보를 CSV로 내보내기 - 모든 필드 포함 (견고성 개선)"""
    try:
        response = HttpResponse(content_type='text/csv; charset=utf-8-sig')
        response['Content-Disposition'] = 'attachment; filename="cameras.csv"'
        
        # BOM 추가
        response.write('\ufeff')
        
        writer = csv.writer(response, quoting=csv.QUOTE_ALL)  # 모든 필드 인용 처리
        writer.writerow(['카메라 ID', '풍력 터빈 ID', '시야각', '설치 방향', '설치 높이', 'RTSP 주소', '상태', '감지 건수'])
        
        for camera in Camera.objects.order_by('camera_id').all():
            try:
                # 이 카메라로 감지된 건수 계산
                detection_count = DetectionInfo.objects.filter(camera_id=camera.camera_id).count()
                
                writer.writerow([
                    str(camera.camera_id) if camera.camera_id else '',
                    str(camera.wind_turbine_id) if camera.wind_turbine_id else '',
                    str(camera.viewing_angle) if camera.viewing_angle is not None else '',
                    str(camera.installation_direction) if camera.installation_direction else '',
                    str(camera.installation_height) if camera.installation_height is not None else '',
                    str(camera.rtsp_address) if camera.rtsp_address else '',
                    str(camera.status) if camera.status else '',
                    str(detection_count)
                ])
            except Exception as e:
                # 행 처리 중 오류 발생시 건너뛰기
                logger.error(f"카메라 CSV 행 처리 오류: {str(e)}")
                continue
        
        return response
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"카메라 정보 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'카메라 정보 내보내기 오류: {str(e)}'}, status=500)

def export_database_csv(request):
    """전체 데이터베이스를 ZIP 파일로 내보내기 (방식 개선)"""
    try:
        # 고유한 임시 디렉토리 이름 생성
        timestamp = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
        temp_dir = tempfile.mkdtemp(prefix=f'db_export_{timestamp}_')
        
        try:
            # 각 CSV 파일 먼저 디스크에 직접 생성 (메모리 사용 최소화)
            
            # 1. 감지 정보 CSV 파일 생성
            detections_file = os.path.join(temp_dir, 'detections.csv')
            with open(detections_file, 'w', newline='', encoding='utf-8-sig') as f:
                writer = csv.writer(f, quoting=csv.QUOTE_ALL)
                writer.writerow(['감지 ID', '감지 시간', '바운딩 박스 수', '카메라 ID', '카메라 풍력 터빈 ID', '카메라 시야각', '카메라 설치 방향', '카메라 설치 높이', 'RTSP 주소', '상태'])
                
                # 감지 ID 순서로 정렬 - 배치 처리로 메모리 효율성 높이기
                for detection in DetectionInfo.objects.select_related('camera_id').order_by('detection_id').iterator():
                    try:
                        # 시간을 KST로 변환
                        detection_time = ''
                        if detection.detection_time:
                            # 시간대 정보가 없다면 UTC로 가정하고 KST로 변환
                            if detection.detection_time.tzinfo is None:
                                aware_time = pytz.utc.localize(detection.detection_time)
                            else:
                                aware_time = detection.detection_time
                            
                            # KST로 변환
                            kst_time = aware_time.astimezone(KST)
                            detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                            
                        writer.writerow([
                            str(detection.detection_id) if detection.detection_id else '',
                            detection_time,
                            str(detection.bb_count) if detection.bb_count is not None else '0',
                            str(detection.camera_id.camera_id) if detection.camera_id else '',
                            str(detection.camera_id.wind_turbine_id) if detection.camera_id and detection.camera_id.wind_turbine_id else '',
                            str(detection.camera_id.viewing_angle) if detection.camera_id and detection.camera_id.viewing_angle else '',
                            str(detection.camera_id.installation_direction) if detection.camera_id and detection.camera_id.installation_direction else '',
                            str(detection.camera_id.installation_height) if detection.camera_id and detection.camera_id.installation_height else '',
                            str(detection.camera_id.rtsp_address) if detection.camera_id and detection.camera_id.rtsp_address else '',
                            str(detection.camera_id.status) if detection.camera_id and detection.camera_id.status else ''
                        ])
                    except Exception as e:
                        logger.error(f"Detection CSV 행 처리 오류: {str(e)}")
                        continue
            
            # 2. 바운딩 박스 정보 CSV 파일 생성
            bb_file = os.path.join(temp_dir, 'bounding_boxes.csv')
            with open(bb_file, 'w', newline='', encoding='utf-8-sig') as f:
                writer = csv.writer(f, quoting=csv.QUOTE_ALL)
                writer.writerow(['바운딩 박스 ID', '감지 ID', '감지 시간', '조류 클래스 ID', '조류 한글명', '학명', '조류 코드', '보호 상태', '이동성 유형', '위험도', 
                                '왼쪽 (BB Left)', '위 (BB Top)', '오른쪽 (BB Right)', '아래 (BB Bottom)', '너비', '높이', '면적', '카메라 ID'])
                
                # 배치 단위로 처리
                for bb in BBInfo.objects.select_related('detection_info_id', 'class_id', 'detection_info_id__camera_id').order_by('detection_info_id__detection_id', 'record_id').iterator():
                    try:
                        width = bb.bb_right - bb.bb_left if bb.bb_right is not None and bb.bb_left is not None else 0
                        height = bb.bb_bottom - bb.bb_top if bb.bb_bottom is not None and bb.bb_top is not None else 0
                        area = width * height
                        
                        # 시간을 KST로 변환
                        detection_time = ''
                        if bb.detection_info_id and bb.detection_info_id.detection_time:
                            dt = bb.detection_info_id.detection_time
                            if dt.tzinfo is None:
                                aware_time = pytz.utc.localize(dt)
                            else:
                                aware_time = dt
                            kst_time = aware_time.astimezone(KST)
                            detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                        
                        protection_status = ''
                        if bb.class_id and bb.class_id.protection_status_code:
                            protection_status = bb.class_id.protection_status_code.description
                        
                        migration_code = ''
                        if bb.class_id and bb.class_id.migration_code:
                            migration_code = bb.class_id.migration_code.description
                        
                        writer.writerow([
                            str(bb.record_id) if bb.record_id else '',
                            str(bb.detection_info_id.detection_id) if bb.detection_info_id else '',
                            detection_time,
                            str(bb.class_id.class_id) if bb.class_id else '',
                            str(bb.class_id.bird_name_ko) if bb.class_id and bb.class_id.bird_name_ko else '',
                            str(bb.class_id.scientific_name) if bb.class_id and bb.class_id.scientific_name else '',
                            str(bb.class_id.bird_type_code) if bb.class_id and bb.class_id.bird_type_code else '',
                            str(protection_status),
                            str(migration_code),
                            str(bb.class_id.risk_score) if bb.class_id and bb.class_id.risk_score is not None else '',
                            str(bb.bb_left) if bb.bb_left is not None else '',
                            str(bb.bb_top) if bb.bb_top is not None else '',
                            str(bb.bb_right) if bb.bb_right is not None else '',
                            str(bb.bb_bottom) if bb.bb_bottom is not None else '',
                            str(width),
                            str(height),
                            str(area),
                            str(bb.detection_info_id.camera_id.camera_id) if bb.detection_info_id and bb.detection_info_id.camera_id else ''
                        ])
                    except Exception as e:
                        logger.error(f"Bounding Box CSV 행 처리 오류: {str(e)}")
                        continue
            
            # 5. 퇴치 기록 CSV 파일 생성
            deterrents_file = os.path.join(temp_dir, 'deterrents.csv')
            with open(deterrents_file, 'w', newline='', encoding='utf-8-sig') as f:
                writer = csv.writer(f, quoting=csv.QUOTE_ALL)
                writer.writerow([
                    '퇴치 ID', '감지 ID', '감지 시간', '카메라 ID', '거리', '개체수', 
                    '퇴치 시작 시간', '퇴치 종료 시간', '대포 ID', '대포 풍력 터빈 ID', '대포 설치 위치',
                    '발사 시간', '스피커 ID', '소리 종류', '성공 여부'
                ])
                
                for deterrent in DeterrentRecord.objects.select_related('detection_id', 'cannon_id', 'speaker_id', 'sound_id', 'detection_id__camera_id').order_by('detection_id__detection_id', 'deterrent_id').iterator():
                    try:
                        # 시간값들을 KST로 변환
                        detection_time = ''
                        if deterrent.detection_id and deterrent.detection_id.detection_time:
                            dt = deterrent.detection_id.detection_time
                            if dt.tzinfo is None:
                                aware_time = pytz.utc.localize(dt)
                            else:
                                aware_time = dt
                            kst_time = aware_time.astimezone(KST)
                            detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                        
                        # 퇴치 시작 시간 변환
                        deterrent_start_time = ''
                        if deterrent.deterrent_start_time:
                            dt = deterrent.deterrent_start_time
                            if dt.tzinfo is None:
                                aware_time = pytz.utc.localize(dt)
                            else:
                                aware_time = dt
                            kst_time = aware_time.astimezone(KST)
                            deterrent_start_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                        
                        # 퇴치 종료 시간 변환
                        deterrent_end_time = ''
                        if deterrent.deterrent_end_time:
                            dt = deterrent.deterrent_end_time
                            if dt.tzinfo is None:
                                aware_time = pytz.utc.localize(dt)
                            else:
                                aware_time = dt
                            kst_time = aware_time.astimezone(KST)
                            deterrent_end_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                        
                        # 발사 시간 변환
                        cannon_firing_time = ''
                        if deterrent.cannon_firing_time:
                            dt = deterrent.cannon_firing_time
                            if dt.tzinfo is None:
                                aware_time = pytz.utc.localize(dt)
                            else:
                                aware_time = dt
                            kst_time = aware_time.astimezone(KST)
                            cannon_firing_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                        
                        writer.writerow([
                            str(deterrent.deterrent_id) if deterrent.deterrent_id else '',
                            str(deterrent.detection_id.detection_id) if deterrent.detection_id else '',
                            detection_time,
                            str(deterrent.detection_id.camera_id.camera_id) if deterrent.detection_id and deterrent.detection_id.camera_id else '',
                            str(deterrent.detection_distance) if deterrent.detection_distance is not None else '',
                            str(deterrent.object_count) if deterrent.object_count is not None else '',
                            deterrent_start_time,
                            deterrent_end_time,
                            str(deterrent.cannon_id.cannon_id) if deterrent.cannon_id else '',
                            str(deterrent.cannon_id.wind_turbine_id) if deterrent.cannon_id and deterrent.cannon_id.wind_turbine_id else '',
                            str(deterrent.cannon_id.installation_location) if deterrent.cannon_id and deterrent.cannon_id.installation_location else '',
                            cannon_firing_time,
                            str(deterrent.speaker_id.speaker_id) if deterrent.speaker_id else '',
                            str(deterrent.sound_id.sound_type) if deterrent.sound_id and deterrent.sound_id.sound_type else '',
                            '성공' if deterrent.is_success else '실패'
                        ])
                    except Exception as e:
                        logger.error(f"Deterrent CSV 행 처리 오류: {str(e)}")
                        continue
            
            # ZIP 파일 생성 (하나씩 파일 추가)
            zip_filename = os.path.join(temp_dir, "database_export.zip")
            with zipfile.ZipFile(zip_filename, 'w') as zipf:
                for filename in ['detections.csv', 'bounding_boxes.csv', 'cameras.csv', 'bird_classes.csv', 'deterrents.csv', 'summary.csv']:
                    filepath = os.path.join(temp_dir, filename)
                    if os.path.exists(filepath):
                        # 압축률을 낮추고 빠른 압축 설정
                        zipf.write(filepath, arcname=filename, compress_type=zipfile.ZIP_DEFLATED, compresslevel=1)
            
            # ZIP 파일 읽어서 응답 생성
            with open(zip_filename, 'rb') as f:
                response = HttpResponse(f.read(), content_type='application/zip')
                response['Content-Disposition'] = 'attachment; filename="database_export.zip"'
                return response
                
        finally:
            # 임시 디렉토리 정리 - 예외가 발생하더라도 항상 실행
            import shutil
            try:
                if os.path.exists(temp_dir):
                    shutil.rmtree(temp_dir)
            except Exception as e:
                logger.error(f"임시 디렉토리 삭제 오류: {str(e)}")
                
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"전체 데이터베이스 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'데이터베이스 내보내기 오류: {str(e)}'}, status=500)

def export_detections_csv(request):
    """감지 기록을 CSV로 내보내기 (필터링 지원) - 견고성 개선"""
    try:
        # 쿼리 파라미터에서 필터 조건 가져오기
        date_from = request.GET.get('date_from')
        date_to = request.GET.get('date_to')
        camera_id = request.GET.get('camera_id')
        
        # 응답 헤더 설정 (UTF-8 BOM 추가)
        response = HttpResponse(content_type='text/csv; charset=utf-8-sig')
        response['Content-Disposition'] = 'attachment; filename="detections.csv"'
        
        # BOM 추가
        response.write('\ufeff')
        
        # 필터링 적용
        detections = DetectionInfo.objects.select_related('camera_id').all()
        
        if date_from:
            try:
                date_from_obj = datetime.datetime.strptime(date_from, '%Y-%m-%d').replace(hour=0, minute=0, second=0)
                detections = detections.filter(detection_time__gte=date_from_obj)
            except ValueError:
                pass
        
        if date_to:
            try:
                date_to_obj = datetime.datetime.strptime(date_to, '%Y-%m-%d').replace(hour=23, minute=59, second=59)
                detections = detections.filter(detection_time__lte=date_to_obj)
            except ValueError:
                pass
        
        if camera_id:
            detections = detections.filter(camera_id=camera_id)
        
        # 감지 ID 순서로 정렬에서 날짜 내림차순으로 변경
        detections = detections.order_by('-detection_time')
        
        # CSV 작성 - 관련 테이블 정보 포함하여 더 많은 데이터 제공
        writer = csv.writer(response, quoting=csv.QUOTE_ALL)  # 모든 필드 인용 처리
        writer.writerow(['감지 ID', '감지 시간', '바운딩 박스 수', '카메라 ID', '카메라 풍력 터빈 ID', '카메라 시야각', '카메라 설치 방향', '카메라 설치 높이', 'RTSP 주소', '상태'])
        
        for detection in detections:
            try:
                # 시간을 KST로 변환
                detection_time = ''
                if detection.detection_time:
                    # 시간대 정보가 없다면 UTC로 가정하고 KST로 변환
                    if detection.detection_time.tzinfo is None:
                        aware_time = pytz.utc.localize(detection.detection_time)
                    else:
                        aware_time = detection.detection_time
                    
                    # KST로 변환
                    kst_time = aware_time.astimezone(KST)
                    detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                writer.writerow([
                    str(detection.detection_id) if detection.detection_id else '',
                    detection_time,
                    str(detection.bb_count) if detection.bb_count is not None else '0',
                    str(detection.camera_id.camera_id) if detection.camera_id else '',
                    str(detection.camera_id.wind_turbine_id) if detection.camera_id and detection.camera_id.wind_turbine_id else '',
                    str(detection.camera_id.viewing_angle) if detection.camera_id and detection.camera_id.viewing_angle else '',
                    str(detection.camera_id.installation_direction) if detection.camera_id and detection.camera_id.installation_direction else '',
                    str(detection.camera_id.installation_height) if detection.camera_id and detection.camera_id.installation_height else '',
                    str(detection.camera_id.rtsp_address) if detection.camera_id and detection.camera_id.rtsp_address else '',
                    str(detection.camera_id.status) if detection.camera_id and detection.camera_id.status else ''
                ])
            except Exception as e:
                # 행 처리 중 오류 발생시 건너뛰기
                logger.error(f"감지 CSV 행 처리 오류: {str(e)}")
                continue
        
        return response
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"감지 정보 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'감지 정보 내보내기 오류: {str(e)}'}, status=500)

def export_bird_analysis_csv(request):
    """조류 분석 데이터를 CSV로 내보내기 (필터링 지원) - 견고성 개선"""
    try:
        # 필터 조건 가져오기
        date_from = request.GET.get('date_from')
        date_to = request.GET.get('date_to')
        bird_class = request.GET.get('bird_class')
        
        # 응답 헤더 설정 (UTF-8 BOM 추가)
        response = HttpResponse(content_type='text/csv; charset=utf-8-sig')
        response['Content-Disposition'] = 'attachment; filename="bird_analysis.csv"'
        
        # BOM 추가
        response.write('\ufeff')
        
        # 필터링 적용
        bb_query = BBInfo.objects.select_related('detection_info_id', 'class_id', 'detection_info_id__camera_id', 'class_id__protection_status_code', 'class_id__migration_code').all()
        
        if date_from:
            try:
                date_from_obj = datetime.datetime.strptime(date_from, '%Y-%m-%d').replace(hour=0, minute=0, second=0)
                date_from_obj = date_from_obj.replace(tzinfo=timezone.get_current_timezone())
                bb_query = bb_query.filter(detection_info_id__detection_time__gte=date_from_obj)
            except ValueError:
                pass
        
        if date_to:
            try:
                date_to_obj = datetime.datetime.strptime(date_to, '%Y-%m-%d').replace(hour=23, minute=59, second=59)
                date_to_obj = date_to_obj.replace(tzinfo=timezone.get_current_timezone())
                bb_query = bb_query.filter(detection_info_id__detection_time__lte=date_to_obj)
            except ValueError:
                pass
        
        if bird_class:
            bb_query = bb_query.filter(class_id=bird_class)
        
        # 감지 ID 순서로 정렬 후 바운딩 박스 ID 순서로 정렬
        bb_query = bb_query.order_by('detection_info_id__detection_id', 'record_id')
        
        # CSV 작성 - 더 많은 필드와 관련 테이블 정보 포함
        writer = csv.writer(response, quoting=csv.QUOTE_ALL)  # 모든 필드 인용 처리
        writer.writerow(['바운딩 박스 ID', '감지 ID', '감지 시간', '조류 클래스 ID', '조류 한글명', '학명', '조류 코드', '보호 상태', '이동성 유형', '위험도', 
                        '왼쪽 (BB Left)', '위 (BB Top)', '오른쪽 (BB Right)', '아래 (BB Bottom)', '너비', '높이', '면적', '카메라 ID', '풍력 터빈 ID'])
        
        for bb in bb_query:
            try:
                width = bb.bb_right - bb.bb_left if bb.bb_right is not None and bb.bb_left is not None else 0
                height = bb.bb_bottom - bb.bb_top if bb.bb_bottom is not None and bb.bb_top is not None else 0
                area = width * height
                
                # 시간을 KST로 변환
                detection_time = ''
                if bb.detection_info_id and bb.detection_info_id.detection_time:
                    # 시간대 정보가 없다면 UTC로 가정하고 KST로 변환
                    dt = bb.detection_info_id.detection_time
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    
                    # KST로 변환
                    kst_time = aware_time.astimezone(KST)
                    detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                protection_status = ''
                if bb.class_id and bb.class_id.protection_status_code:
                    protection_status = bb.class_id.protection_status_code.description
                
                migration_code = ''
                if bb.class_id and bb.class_id.migration_code:
                    migration_code = bb.class_id.migration_code.description
                
                writer.writerow([
                    str(bb.record_id) if bb.record_id else '',
                    str(bb.detection_info_id.detection_id) if bb.detection_info_id else '',
                    detection_time,
                    str(bb.class_id.class_id) if bb.class_id else '',
                    str(bb.class_id.bird_name_ko) if bb.class_id and bb.class_id.bird_name_ko else '',
                    str(bb.class_id.scientific_name) if bb.class_id and bb.class_id.scientific_name else '',
                    str(bb.class_id.bird_type_code) if bb.class_id and bb.class_id.bird_type_code else '',
                    str(protection_status),
                    str(migration_code),
                    str(bb.class_id.risk_score) if bb.class_id and bb.class_id.risk_score is not None else '',
                    str(bb.bb_left) if bb.bb_left is not None else '',
                    str(bb.bb_top) if bb.bb_top is not None else '',
                    str(bb.bb_right) if bb.bb_right is not None else '',
                    str(bb.bb_bottom) if bb.bb_bottom is not None else '',
                    str(width),
                    str(height),
                    str(area),
                    str(bb.detection_info_id.camera_id.camera_id) if bb.detection_info_id and bb.detection_info_id.camera_id else '',
                    str(bb.detection_info_id.camera_id.wind_turbine_id) if bb.detection_info_id and bb.detection_info_id.camera_id else ''
                ])
            except Exception as e:
                # 행 처리 중 오류 발생시 건너뛰기
                logger.error(f"조류 분석 CSV 행 처리 오류: {str(e)}")
                continue
        
        return response
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"조류 분석 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'조류 분석 내보내기 오류: {str(e)}'}, status=500)

def export_birds_csv(request):
    """조류 클래스 정보를 CSV로 내보내기 - 더 많은 정보 제공 (견고성 개선)"""
    try:
        response = HttpResponse(content_type='text/csv; charset=utf-8-sig')
        response['Content-Disposition'] = 'attachment; filename="birds.csv"'
        
        # BOM 추가
        response.write('\ufeff')
        
        writer = csv.writer(response, quoting=csv.QUOTE_ALL)  # 모든 필드 인용 처리
        writer.writerow(['ID', '조류 코드', '한글명', '학명', '보호 상태 코드', '보호 상태 설명', '이동성 코드', '이동성 유형', '위험도', '감지 건수'])
        
        for bird in BirdClass.objects.select_related('protection_status_code', 'migration_code').order_by('class_id').all():
            try:
                # 이 조류가 감지된 건수 계산
                detection_count = BBInfo.objects.filter(class_id=bird.class_id).count()
                
                writer.writerow([
                    str(bird.class_id) if bird.class_id else '',
                    str(bird.bird_type_code) if bird.bird_type_code else '',
                    str(bird.bird_name_ko) if bird.bird_name_ko else '',
                    str(bird.scientific_name) if bird.scientific_name else '',
                    str(bird.protection_status_code.code) if bird.protection_status_code and bird.protection_status_code.code else '',
                    str(bird.protection_status_code.description) if bird.protection_status_code and bird.protection_status_code.description else '',
                    str(bird.migration_code.code) if bird.migration_code and bird.migration_code.code else '',
                    str(bird.migration_code.description) if bird.migration_code and bird.migration_code.description else '',
                    str(bird.risk_score) if bird.risk_score is not None else '',
                    str(detection_count)
                ])
            except Exception as e:
                # 행 처리 중 오류 발생시 건너뛰기
                logger.error(f"조류 CSV 행 처리 오류: {str(e)}")
                continue
        
        return response
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"조류 정보 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'조류 정보 내보내기 오류: {str(e)}'}, status=500)

def export_deterrents_csv(request):
    """퇴치 기록을 CSV로 내보내기 - 모든 관련 정보 포함 (견고성 개선)"""
    try:
        response = HttpResponse(content_type='text/csv; charset=utf-8-sig')
        response['Content-Disposition'] = 'attachment; filename="deterrents.csv"'
        
        # BOM 추가
        response.write('\ufeff')
        
        writer = csv.writer(response, quoting=csv.QUOTE_ALL)  # 모든 필드 인용 처리
        writer.writerow([
            '퇴치 ID', '감지 ID', '감지 시간', '카메라 ID', '거리', '개체수', 
            '퇴치 시작 시간', '퇴치 종료 시간', '대포 ID', '대포 풍력 터빈 ID', '대포 설치 위치', '대포 최근 충전일',
            '발사 시간', '스피커 ID', '스피커 풍력 터빈 ID', '스피커 설치 위치',
            '소리 ID', '소리 종류', '소리 파일 경로', '성공 여부'
        ])
        
        # 감지 ID 순서로 정렬 후 퇴치 ID 순서로 정렬
        for deterrent in DeterrentRecord.objects.select_related('detection_id', 'cannon_id', 'speaker_id', 'sound_id', 'detection_id__camera_id').order_by('detection_id__detection_id', 'deterrent_id').all():
            try:
                # 시간값들을 KST로 변환
                detection_time = ''
                if deterrent.detection_id and deterrent.detection_id.detection_time:
                    dt = deterrent.detection_id.detection_time
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    kst_time = aware_time.astimezone(KST)
                    detection_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                # 퇴치 시작 시간 변환
                deterrent_start_time = ''
                if deterrent.deterrent_start_time:
                    dt = deterrent.deterrent_start_time
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    kst_time = aware_time.astimezone(KST)
                    deterrent_start_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                # 퇴치 종료 시간 변환
                deterrent_end_time = ''
                if deterrent.deterrent_end_time:
                    dt = deterrent.deterrent_end_time
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    kst_time = aware_time.astimezone(KST)
                    deterrent_end_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                # 발사 시간 변환
                cannon_firing_time = ''
                if deterrent.cannon_firing_time:
                    dt = deterrent.cannon_firing_time
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    kst_time = aware_time.astimezone(KST)
                    cannon_firing_time = kst_time.strftime('%Y-%m-%d %H:%M:%S')
                
                # 대포 충전일 변환
                lpg_charge_date = ''
                if deterrent.cannon_id and deterrent.cannon_id.last_lpg_charge_date:
                    dt = deterrent.cannon_id.last_lpg_charge_date
                    if dt.tzinfo is None:
                        aware_time = pytz.utc.localize(dt)
                    else:
                        aware_time = dt
                    kst_time = aware_time.astimezone(KST)
                    lpg_charge_date = kst_time.strftime('%Y-%m-%d')
                
                writer.writerow([
                    str(deterrent.deterrent_id) if deterrent.deterrent_id else '',
                    str(deterrent.detection_id.detection_id) if deterrent.detection_id else '',
                    detection_time,
                    str(deterrent.detection_id.camera_id.camera_id) if deterrent.detection_id and deterrent.detection_id.camera_id else '',
                    str(deterrent.detection_distance) if deterrent.detection_distance is not None else '',
                    str(deterrent.object_count) if deterrent.object_count is not None else '',
                    deterrent_start_time,
                    deterrent_end_time,
                    str(deterrent.cannon_id.cannon_id) if deterrent.cannon_id else '',
                    str(deterrent.cannon_id.wind_turbine_id) if deterrent.cannon_id and deterrent.cannon_id.wind_turbine_id else '',
                    str(deterrent.cannon_id.installation_location) if deterrent.cannon_id and deterrent.cannon_id.installation_location else '',
                    lpg_charge_date,
                    cannon_firing_time,
                    str(deterrent.speaker_id.speaker_id) if deterrent.speaker_id else '',
                    str(deterrent.speaker_id.wind_turbine_id) if deterrent.speaker_id and deterrent.speaker_id.wind_turbine_id else '',
                    str(deterrent.speaker_id.installation_location) if deterrent.speaker_id and deterrent.speaker_id.installation_location else '',
                    str(deterrent.sound_id.sound_id) if deterrent.sound_id else '',
                    str(deterrent.sound_id.sound_type) if deterrent.sound_id and deterrent.sound_id.sound_type else '',
                    str(deterrent.sound_id.file_path) if deterrent.sound_id and deterrent.sound_id.file_path else '',
                    '성공' if deterrent.is_success else '실패'
                ])
            except Exception as e:
                # 행 처리 중 오류 발생시 건너뛰기
                logger.error(f"퇴치 CSV 행 처리 오류: {str(e)}")
                continue
        
        return response
    except Exception as e:
        # 전체 함수 예외 처리
        logger.error(f"퇴치 기록 내보내기 오류: {str(e)}")
        return JsonResponse({'status': 'error', 'message': f'퇴치 기록 내보내기 오류: {str(e)}'}, status=500)

@csrf_exempt
def clear_database(request):
    """데이터베이스 초기화 API"""
    if request.method != 'POST':
        return JsonResponse({'status': 'error', 'message': '잘못된 요청 방식입니다.'}, status=405)
    
    try:
        data = json.loads(request.body)
        db_type = data.get('db_type', 'all')
        
        if db_type == 'detections':
            # 감지 기록만 삭제 (BBInfo는 DetectionInfo를 참조하므로 자동으로 삭제됨)
            detections_count = DetectionInfo.objects.count()
            bb_count = BBInfo.objects.count()
            DetectionInfo.objects.all().delete()
            message = f'감지 기록 {detections_count}건과 바운딩 박스 정보 {bb_count}건이 성공적으로 초기화되었습니다.'
        
        elif db_type == 'bird_analysis':
            # 바운딩 박스 정보만 삭제
            bb_count = BBInfo.objects.count()
            BBInfo.objects.all().delete()
            message = f'조류 분석 데이터(바운딩 박스) {bb_count}건이 성공적으로 초기화되었습니다.'
        
        else:  # 'all'
            # 모든 운영 데이터 삭제 (기본 설정 데이터 유지)
            deterrents_count = DeterrentRecord.objects.count()
            bb_count = BBInfo.objects.count()
            detections_count = DetectionInfo.objects.count()
            
            # 삭제 순서 중요 (외래 키 참조 고려)
            DeterrentRecord.objects.all().delete()
            BBInfo.objects.all().delete()
            DetectionInfo.objects.all().delete()
            
            message = f'데이터베이스가 성공적으로 초기화되었습니다. 퇴치 기록 {deterrents_count}건, 바운딩 박스 {bb_count}건, 감지 기록 {detections_count}건이 삭제되었습니다.'
        
        return JsonResponse({'status': 'success', 'message': message})
    
    except Exception as e:
        logger.error(f"데이터베이스 초기화 오류: {e}")
        return JsonResponse({'status': 'error', 'message': f'초기화 중 오류가 발생했습니다: {str(e)}'}, status=500) 