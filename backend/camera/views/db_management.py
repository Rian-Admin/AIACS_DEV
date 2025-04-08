from django.shortcuts import render
from django.http import JsonResponse
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

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

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
        
        # 최대 100개 항목으로 제한
        bb_info = bb_query.order_by('-detection_info_id__detection_time')[:100]
        
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
        
        return JsonResponse({
            'status': 'success',
            'bb_data': bb_data
        })
        
    except Exception as e:
        logger.error(f"조류 분석 데이터 조회 오류: {e}")
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        }, status=500) 