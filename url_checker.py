#!/usr/bin/env python
import requests
import sys
import os
import csv
import time
from datetime import datetime

# 테스트할 기본 URL (로컬 개발 서버 또는 실제 서버 주소)
BASE_URL = "http://localhost:8000"

# 로그 파일 설정
LOG_DIR = "logs"
LOG_FILE = f"{LOG_DIR}/url_check_results_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"

# 테스트할 URL 패턴 목록 (urls.py에서 추출)
URLS_TO_CHECK = [
    # 메인 뷰
    {"url": "/", "method": "GET", "description": "메인 페이지"},
    
    # 카메라 스트림 (카메라 ID가 필요)
    {"url": "/camera/1/", "method": "GET", "description": "카메라 스트림 (ID: 1)"},
    
    # API - 감지 정보
    {"url": "/api/detections/recent/", "method": "GET", "description": "최근 감지 결과"},
    {"url": "/api/detections/stats/", "method": "GET", "description": "감지 통계"},
    {"url": "/api/db/status/", "method": "GET", "description": "DB 상태"},
    {"url": "/api/cameras/", "method": "GET", "description": "카메라 목록"},
    
    # 카메라 관리 뷰
    {"url": "/camera-management/", "method": "GET", "description": "카메라 관리 페이지"},
    {"url": "/api/camera/get/1/", "method": "GET", "description": "카메라 정보 조회 (ID: 1)"},
    
    # API - 카메라 재연결
    {"url": "/api/camera/status/", "method": "GET", "description": "모든 카메라 상태 조회"},
    {"url": "/api/camera/status/1/", "method": "GET", "description": "특정 카메라 상태 조회 (ID: 1)"},
    {"url": "/api/camera/reconnect/1/", "method": "POST", "description": "카메라 재연결 (ID: 1)"},
    {"url": "/api/camera/reconnect-all/", "method": "POST", "description": "모든 카메라 재연결"},
    
    # 스피커 관리 뷰 및 API
    {"url": "/speaker-management/", "method": "GET", "description": "스피커 관리 페이지"},
    {"url": "/camera/speaker/status/", "method": "GET", "description": "스피커 상태 조회"},
    {"url": "/camera/speaker/connect/", "method": "POST", "description": "스피커 연결"},
    {"url": "/camera/speaker/disconnect/", "method": "POST", "description": "스피커 연결 해제"},
    
    # DB 관리 뷰
    {"url": "/db-management/", "method": "GET", "description": "DB 관리 페이지"},
    {"url": "/bird-analysis/", "method": "GET", "description": "조류 분석 페이지"},
    
    # API - 데이터베이스 
    {"url": "/api/detections/filtered/", "method": "GET", "description": "필터링된 감지 결과"},
    {"url": "/api/detection/bb-info/1/", "method": "GET", "description": "감지 바운딩 박스 정보 (ID: 1)"},
    {"url": "/api/bird-analysis/data/", "method": "GET", "description": "조류 분석 데이터"},
    
    # API - 모델 관리
    {"url": "/model-management/", "method": "GET", "description": "모델 관리 페이지"},
    
    # API - 데이터 내보내기
    {"url": "/api/export-database-csv/", "method": "GET", "description": "데이터베이스 CSV 내보내기"},
    {"url": "/api/export-detections-csv/", "method": "GET", "description": "감지 결과 CSV 내보내기"},
    {"url": "/api/export-bird-analysis-csv/", "method": "GET", "description": "조류 분석 CSV 내보내기"},
    {"url": "/api/export-cameras-csv/", "method": "GET", "description": "카메라 정보 CSV 내보내기"},
    {"url": "/api/export-birds-csv/", "method": "GET", "description": "조류 정보 CSV 내보내기"},
    {"url": "/api/export-deterrents-csv/", "method": "GET", "description": "퇴치기 정보 CSV 내보내기"},
    
    # API - YOLO 모델 관리
    {"url": "/api/yolo/info/", "method": "GET", "description": "YOLO 모델 정보"},
    {"url": "/api/yolo/available-models/", "method": "GET", "description": "사용 가능한 YOLO 모델 목록"},
    
    # API - 조류퇴치기 제어 (슬래시 추가)
    {"url": "/api/controller/enable/", "method": "POST", "description": "조류퇴치기 활성화"},
    {"url": "/api/controller/disable/", "method": "POST", "description": "조류퇴치기 비활성화"},
    
    # API - PTZ 카메라 제어
    {"url": "/api/ptz/position/1/", "method": "GET", "description": "PTZ 카메라 위치 조회 (ID: 1)"},
]

def ensure_log_directory():
    """로그 디렉토리가 존재하는지 확인하고, 없으면 생성"""
    if not os.path.exists(LOG_DIR):
        os.makedirs(LOG_DIR)

def check_url(url_info):
    """URL을 체크하고 결과를 반환"""
    full_url = f"{BASE_URL}{url_info['url']}"
    method = url_info["method"]
    description = url_info["description"]
    
    start_time = time.time()
    
    try:
        if method == "GET":
            response = requests.get(full_url, timeout=10)
        elif method == "POST":
            # POST 요청의 경우 빈 데이터로 테스트
            response = requests.post(full_url, json={}, timeout=10)
        else:
            return {
                "url": full_url,
                "method": method,
                "description": description,
                "status": "지원되지 않는 메소드",
                "status_code": None,
                "response_time": 0,
                "error": "Unsupported method"
            }
        
        response_time = time.time() - start_time
        
        # 상태 코드 확인
        if response.status_code < 400:
            status = "성공"
        else:
            status = "실패"
            
        return {
            "url": full_url,
            "method": method,
            "description": description,
            "status": status,
            "status_code": response.status_code,
            "response_time": round(response_time, 2),
            "error": None
        }
        
    except requests.exceptions.ConnectionError as e:
        return {
            "url": full_url,
            "method": method,
            "description": description,
            "status": "연결 오류",
            "status_code": None,
            "response_time": round(time.time() - start_time, 2),
            "error": "Connection error"
        }
    except requests.exceptions.Timeout as e:
        return {
            "url": full_url,
            "method": method,
            "description": description,
            "status": "시간 초과",
            "status_code": None,
            "response_time": round(time.time() - start_time, 2),
            "error": "Timeout"
        }
    except Exception as e:
        return {
            "url": full_url,
            "method": method,
            "description": description,
            "status": "오류",
            "status_code": None,
            "response_time": round(time.time() - start_time, 2),
            "error": str(e)
        }

def save_results_to_csv(results):
    """결과를 CSV 파일로 저장"""
    with open(LOG_FILE, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            "url", "method", "description", "status", "status_code", "response_time", "error"
        ])
        writer.writeheader()
        writer.writerows(results)
    
    print(f"결과가 {LOG_FILE}에 저장되었습니다.")

def main():
    """메인 함수"""
    print(f"Django URL 체크 스크립트 시작 - 기본 URL: {BASE_URL}")
    print(f"총 {len(URLS_TO_CHECK)}개의 URL을 테스트합니다...")
    
    ensure_log_directory()
    
    results = []
    success_count = 0
    
    for idx, url_info in enumerate(URLS_TO_CHECK, 1):
        print(f"[{idx}/{len(URLS_TO_CHECK)}] {url_info['method']} {url_info['url']} 테스트 중...")
        result = check_url(url_info)
        results.append(result)
        
        # 결과 출력
        status_str = f"상태: {result['status']}"
        if result['status_code']:
            status_str += f" (코드: {result['status_code']})"
        
        print(f"  - {status_str}, 응답 시간: {result['response_time']}초")
        
        if result['status'] == "성공":
            success_count += 1
        
        # 너무 빠른 요청으로 서버에 부하가 가지 않도록 약간의 지연
        time.sleep(0.5)
    
    # 요약 출력
    print("\n결과 요약:")
    print(f"총 URL 수: {len(URLS_TO_CHECK)}")
    print(f"성공: {success_count}")
    print(f"실패: {len(URLS_TO_CHECK) - success_count}")
    print(f"성공률: {(success_count / len(URLS_TO_CHECK) * 100):.1f}%")
    
    # CSV 저장
    save_results_to_csv(results)

if __name__ == "__main__":
    # 커맨드 라인에서 기본 URL 변경 가능
    if len(sys.argv) > 1:
        BASE_URL = sys.argv[1]
    
    main() 