import os
import json
import shutil
import datetime
from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings
from ..yolo.detector import ObjectDetector

# 모델 관리 페이지 렌더링
def model_management_view(request):
    """모델 관리 페이지 렌더링"""
    return render(request, 'detection/model_management.html')

# 사용 가능한 모델 목록 조회 API
@csrf_exempt
def get_available_models(request):
    """사용 가능한 모델 목록 반환"""
    try:
        # 모델 파일이 저장된 디렉토리 경로 (기본 경로)
        model_dir = os.path.join(settings.BASE_DIR, 'camera', 'yolo', 'models')
        
        # 추가 모델 디렉토리 (models_dir 환경 변수가 설정된 경우)
        models_dir = os.environ.get('YOLO_MODELS_DIR')
        if models_dir:
            if os.path.exists(models_dir):
                model_dir = models_dir
        
        # 현재 모델 경로 가져오기
        detector = ObjectDetector.get_instance()
        current_model_path = detector.model_path if detector else None
        
        models = []
        
        # 모델 디렉토리에서 .pt 파일 찾기
        if os.path.exists(model_dir):
            for filename in os.listdir(model_dir):
                if filename.endswith('.pt'):
                    model_path = os.path.join(model_dir, filename)
                    model_stat = os.stat(model_path)
                    model_info = {
                        'path': model_path,
                        'name': filename,
                        'size': round(model_stat.st_size / (1024 * 1024), 2),  # MB 단위
                        'uploaded_date': datetime.datetime.fromtimestamp(model_stat.st_mtime).strftime('%Y-%m-%d %H:%M:%S'),
                        'is_current': model_path == current_model_path
                    }
                    models.append(model_info)
        
        return JsonResponse({
            'status': 'success',
            'models': models
        })
    except Exception as e:
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        })

# 모델 업로드 API
@csrf_exempt
def upload_model(request):
    """새 모델 파일 업로드"""
    if request.method != 'POST':
        return JsonResponse({
            'status': 'error',
            'message': '잘못된 요청 메소드입니다.'
        })
    
    try:
        # 모델 파일 읽기
        if 'model_file' not in request.FILES:
            return JsonResponse({
                'status': 'error',
                'message': '모델 파일이 제공되지 않았습니다.'
            })
        
        model_file = request.FILES['model_file']
        
        # 파일 확장자 검사
        if not model_file.name.lower().endswith('.pt'):
            return JsonResponse({
                'status': 'error',
                'message': '.pt 확장자의 PyTorch 모델 파일만 허용됩니다.'
            })
        
        # 파일 크기 검사 (100MB 제한)
        if model_file.size > 100 * 1024 * 1024:
            return JsonResponse({
                'status': 'error',
                'message': '파일 크기는 100MB 이하여야 합니다.'
            })
        
        # 모델 파일이 저장될 디렉토리 가져오기
        model_dir = os.path.join(settings.BASE_DIR, 'camera', 'yolo', 'models')
        
        # 환경 변수에 설정된 모델 디렉토리 있는지 확인
        models_dir = os.environ.get('YOLO_MODELS_DIR')
        if models_dir:
            if os.path.exists(models_dir):
                model_dir = models_dir
        
        # 디렉토리가 없으면 생성
        os.makedirs(model_dir, exist_ok=True)
        
        # 이름 가져오기
        model_name = request.POST.get('name', '')
        # 설명 가져오기
        model_description = request.POST.get('description', '')
        
        # 타임스탬프 생성
        timestamp = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
        
        # 파일명 생성 (이름이 제공된 경우 사용)
        if model_name:
            # 파일명에 사용될 수 없는 문자 제거
            safe_name = ''.join(c if c.isalnum() or c in '_-' else '_' for c in model_name)
            filename = f"{safe_name}_{timestamp}.pt"
        else:
            # 원본 파일명 사용 (확장자 제외)
            orig_name = os.path.splitext(model_file.name)[0]
            safe_name = ''.join(c if c.isalnum() or c in '_-' else '_' for c in orig_name)
            filename = f"{safe_name}_{timestamp}.pt"
        
        # 파일 저장 경로
        file_path = os.path.join(model_dir, filename)
        
        # 파일 저장
        with open(file_path, 'wb+') as destination:
            for chunk in model_file.chunks():
                destination.write(chunk)
        
        # 모델 정보 파일 생성 (설명이 있는 경우)
        if model_description:
            info_file_path = os.path.join(model_dir, f"{os.path.splitext(filename)[0]}_info.json")
            with open(info_file_path, 'w', encoding='utf-8') as info_file:
                json.dump({
                    'name': model_name,
                    'description': model_description,
                    'uploaded_date': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
                    'original_filename': model_file.name
                }, info_file, ensure_ascii=False, indent=2)
        
        return JsonResponse({
            'status': 'success',
            'filename': filename,
            'path': file_path,
            'size': round(os.path.getsize(file_path) / (1024 * 1024), 2)  # MB 단위
        })
        
    except Exception as e:
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        })

# 모델 전환 API
@csrf_exempt
def switch_model(request):
    """다른 모델로 전환"""
    if request.method != 'POST':
        return JsonResponse({
            'status': 'error',
            'message': '잘못된 요청 메소드입니다.'
        })
    
    try:
        # 요청 데이터 파싱
        data = json.loads(request.body)
        new_model_path = data.get('model_path')
        
        if not new_model_path:
            return JsonResponse({
                'status': 'error',
                'message': '모델 경로가 제공되지 않았습니다.'
            })
        
        # 파일 존재 여부 확인
        if not os.path.exists(new_model_path):
            return JsonResponse({
                'status': 'error',
                'message': '지정된 경로에 모델 파일이 존재하지 않습니다.'
            })
        
        # 파일 확장자 확인
        if not new_model_path.lower().endswith('.pt'):
            return JsonResponse({
                'status': 'error',
                'message': 'PyTorch 모델 파일(.pt)만 지원됩니다.'
            })
        
        # 현재 모델과 같은지 확인
        detector = ObjectDetector.get_instance()
        if detector.model_path == new_model_path:
            return JsonResponse({
                'status': 'info',
                'message': '이미 해당 모델을 사용 중입니다.'
            })
        
        # 모델 재로드
        try:
            # 현재 모델 인스턴스 가져오기
            detector = ObjectDetector.get_instance()
            
            # 모델 로드 메서드 호출
            detector.load_model(new_model_path)
            
            return JsonResponse({
                'status': 'success',
                'message': '모델이 성공적으로 전환되었습니다.',
                'new_model': new_model_path
            })
            
        except Exception as e:
            return JsonResponse({
                'status': 'error',
                'message': f'모델 전환 중 오류가 발생했습니다: {str(e)}'
            })
            
    except Exception as e:
        return JsonResponse({
            'status': 'error',
            'message': str(e)
        })

# 모델 삭제 API
@csrf_exempt
def delete_model(request):
    """모델 파일 삭제"""
    if request.method != 'POST':
        return JsonResponse({
            'status': 'error',
            'message': '잘못된 요청 메소드입니다.'
        })
    
    try:
        # 요청 데이터 파싱
        data = json.loads(request.body)
        model_path = data.get('model_path')
        
        if not model_path:
            return JsonResponse({
                'status': 'error',
                'message': '모델 경로가 제공되지 않았습니다.'
            })
        
        # 파일 존재 여부 확인
        if not os.path.exists(model_path):
            return JsonResponse({
                'status': 'error',
                'message': '지정된 경로에 모델 파일이 존재하지 않습니다.'
            })
        
        # 파일 확장자 확인
        if not model_path.lower().endswith('.pt'):
            return JsonResponse({
                'status': 'error',
                'message': 'PyTorch 모델 파일(.pt)만 삭제할 수 있습니다.'
            })
        
        # 현재 사용 중인 모델인지 확인
        detector = ObjectDetector.get_instance()
        if detector.model_path == model_path:
            return JsonResponse({
                'status': 'error',
                'message': '현재 사용 중인 모델은 삭제할 수 없습니다. 다른 모델로 전환 후 삭제해주세요.'
            })
        
        # 관련 정보 파일 경로 생성
        info_file_path = os.path.splitext(model_path)[0] + '_info.json'
        
        # 모델 파일 삭제
        os.remove(model_path)
        
        # 관련 정보 파일이 있으면 함께 삭제
        if os.path.exists(info_file_path):
            os.remove(info_file_path)
        
        return JsonResponse({
            'status': 'success',
            'message': '모델이 성공적으로 삭제되었습니다.',
            'deleted_path': model_path
        })
            
    except Exception as e:
        return JsonResponse({
            'status': 'error',
            'message': f'모델 삭제 중 오류가 발생했습니다: {str(e)}'
        }) 