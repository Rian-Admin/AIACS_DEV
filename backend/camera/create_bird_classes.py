import os
import django
import sys

# Django 설정 모듈 경로 추가
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'config.settings')
django.setup()

from camera.models import BirdClass, BirdProtectionCode, BirdMigrationCode
from django.db import transaction

def create_bird_protection_code():
    """조류 보호 코드 기본 데이터 생성"""
    codes = [
        {'code': 'PC001', 'code_value': 0, 'description': '일반 새'},
        {'code': 'PC002', 'code_value': 1, 'description': '보호종'},
        {'code': 'PC003', 'code_value': 2, 'description': '천연기념물'}
    ]
    
    for code_data in codes:
        BirdProtectionCode.objects.get_or_create(
            code=code_data['code'],
            defaults={
                'code_value': code_data['code_value'],
                'description': code_data['description']
            }
        )
    
    print(f"조류 보호 코드 {len(codes)}개가 생성되었습니다.")
    return BirdProtectionCode.objects.get(code='PC001')  # 기본 보호 코드 반환

def create_bird_migration_code():
    """조류 철새/텃새 코드 기본 데이터 생성"""
    codes = [
        {'code': 'MC001', 'code_value': 0, 'description': '철새'},
        {'code': 'MC002', 'code_value': 1, 'description': '텃새'}
    ]
    
    for code_data in codes:
        BirdMigrationCode.objects.get_or_create(
            code=code_data['code'],
            defaults={
                'code_value': code_data['code_value'],
                'description': code_data['description']
            }
        )
    
    print(f"조류 철새/텃새 코드 {len(codes)}개가 생성되었습니다.")
    return BirdMigrationCode.objects.get(code='MC001')  # 기본 철새 코드 반환

def create_bird_classes():
    """조류 클래스 데이터 생성"""
    # 기본 코드 가져오기
    default_protection_code = create_bird_protection_code()
    default_migration_code = create_bird_migration_code()
    
    # 조류 클래스 데이터
    bird_data = [
        (0, "알락도요", "Common Sandpiper", "Actitis hypoleucos"),
        (1, "검은가슴물떼새", "Kentish Plover", "Anarhynchus alexandrinus"),
        (2, "검독수리", "Golden Eagle", "Aquila chrysaetos"),
        (3, "중대백로", "Great Egret", "Ardea alba"),
        (4, "붉은발슴새", "Flesh-footed Shearwater", "Ardenna carneipes"),
        (5, "슴새", "Streaked Shearwater", "Calonectris leucomelas"),
        (6, "매", "Peregrine Falcon", "Falco peregrinus"),
        (7, "검은머리물떼새", "Eurasian Oystercatcher", "Haematopus ostralegus"),
        (8, "재갈매기", "Common Gull", "Larus canus"),
        (9, "마도요", "Eurasian Curlew", "Numenius arquata"),
        (10, "물수리", "Osprey (subsp. carolinensis)", "Pandion haliaetus carolinensis"),
        (11, "가마우지", "Great Cormorant", "Phalacrocorax carbo"),
        (12, "저어새", "Eurasian Spoonbill", "Platalea leucorodia"),
        (13, "뿔제비갈매기", "Great Crested Tern", "Thalasseus bergii"),
        (14, "모래도요", "Sanderling", "Calidris alba"),
        (15, "다제비", "Fork-tailed Storm Petrel", "Hydrobates furcatus"),
        (16, "청도요", "Common Greenshank", "Tringa nebularia"),
        (17, "검둥오리", "Black Scoter", "Melanitta americana"),
        (18, "흰이마기러기", "Greater White-fronted Goose", "Anser albifrons"),
        (19, "새매", "Eurasian Sparrowhawk", "Accipiter nisus"),
        (20, "참매", "Northern Goshawk", "Astur gentilis"),
        (21, "황조롱이", "Common Kestrel", "Falco tinnunculus"),
        (22, "까마귀", "Carrion Crow", "Corvus corone"),
        (23, "검둥오리", "Common Scoter", "Melanitta nigra"),
        (24, "괭이갈매기", "Black-tailed Gull", "Larus crassirostris"),
        (25, "쇠갈매기", "Saunders's Gull", "Chroicocephalus saundersi"),
        (26, "알락꼬리마도요", "Far Eastern Curlew", "Numenius madagascariensis"),
        (27, "넓적부리도요", "Broad-billed Sandpiper", "Calidris falcinellus"),
        (28, "청둥오리", "Mallard", "Anas platyrhynchos"),
        (29, "큰기러기", "Tundra Bean Goose", "Anser serrirostris"),
        (30, "중백로", "Intermediate Egret", "Ardea intermedia"),
        (31, "쇠백로", "Little Egret", "Egretta garzetta"),
        (32, "노랑부리백로", "Chinese Egret", "Egretta eulophotes"),
        (33, "일본가마우지", "Japanese Cormorant", "Phalacrocorax capillatus"),
        (34, "검은머리저어새", "Black-faced Spoonbill", "Platalea minor")
    ]
    
    created_count = 0
    updated_count = 0
    
    with transaction.atomic():
        for bird_id, ko_name, en_name, scientific_name in bird_data:
            bird_type_code = f"BIRD{bird_id:03d}"
            risk_score = 0.5  # 기본 위험 점수
            
            # 실제 위험도/보호 종에 따라 설정 (예시)
            protection_code = default_protection_code
            migration_code = default_migration_code
            
            # 일부 종은 보호종 또는 천연기념물로 설정 (예시)
            if bird_id in [2, 6, 12, 32, 34]:  # 검독수리, 매, 저어새, 노랑부리백로, 검은머리저어새
                protection_code = BirdProtectionCode.objects.get(code='PC003')  # 천연기념물
                risk_score = 0.9
            elif bird_id in [10, 19, 20, 26]:  # 물수리, 새매, 참매, 알락꼬리마도요
                protection_code = BirdProtectionCode.objects.get(code='PC002')  # 보호종
                risk_score = 0.7
                
            # 일부 종은 텃새로 설정 (예시)
            if bird_id in [3, 11, 22, 24, 31]:  # 중대백로, 가마우지, 까마귀, 괭이갈매기, 쇠백로
                migration_code = BirdMigrationCode.objects.get(code='MC002')  # 텃새
            
            # 조류 클래스 생성 또는 업데이트
            bird, created = BirdClass.objects.update_or_create(
                class_id=bird_id,
                defaults={
                    'bird_type_code': bird_type_code,
                    'bird_name_ko': ko_name,
                    'scientific_name': scientific_name,
                    'protection_status_code': protection_code,
                    'migration_code': migration_code,
                    'risk_score': risk_score
                }
            )
            
            if created:
                created_count += 1
            else:
                updated_count += 1
    
    print(f"총 {len(bird_data)}개의 조류 클래스 데이터 중 {created_count}개가 새로 생성되고, {updated_count}개가 업데이트되었습니다.")

if __name__ == "__main__":
    create_bird_classes()
    print("조류 클래스 데이터 생성이 완료되었습니다.") 