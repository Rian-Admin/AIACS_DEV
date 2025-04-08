# 조류충돌방지시스템(AIACS) - 통합 프로젝트

## 프로젝트 개요

이 프로젝트는 조류충돌방지시스템(AIACS, Avian Intelligent Anti-Collision System)으로, 레이더, 카메라, 기상 데이터를 통합하여 조류 충돌을 감지하고 방지하는 시스템입니다. Django 백엔드와 React 프론트엔드로 구성되어 있으며, YOLO 객체 탐지 모델을 사용하여 조류를 식별합니다.

## 시스템 아키텍처

### 백엔드 (Django)

백엔드는 Django 프레임워크를 기반으로 구축되었으며, 다음과 같은 주요 앱으로 구성되어 있습니다:

1. **카메라 앱 (camera)**
   - 실시간 비디오 스트리밍 및 처리
   - YOLO 모델을 사용한 조류 탐지
   - 비디오 프레임 분석 및 저장

2. **레이더 API (radar_api)**
   - 레이더 데이터 수집 및 처리
   - 조류 이동 경로 추적
   - 레이더 관련 프로토콜 및 통신

3. **기상 정보 (weather)**
   - 기상 데이터 수집 및 분석
   - 날씨 예측 및 조류 활동 상관관계 분석
   - 외부 기상 API 연동

### 프론트엔드 (React)

프론트엔드는 React와 Material-UI를 사용하여 구현되었으며, 다음과 같은 주요 기능을 제공합니다:

1. **대시보드 (Dashboard)**
   - 시스템 핵심 지표 실시간 모니터링
   - 통합 데이터 시각화

2. **모니터링 화면**
   - 카메라 모니터링 (실시간 스트림 및 탐지)
   - 레이더 모니터링 (조류 이동 경로 시각화)
   - 비디오 분석 (녹화된 영상 분석 결과)

3. **제어 시스템**
   - 방어 시스템 제어
   - 레이더 제어
   - 설정 관리

4. **데이터 분석**
   - 조류 활동 분석
   - 기상 데이터 분석
   - 알림 및 이벤트 로그

## 기술 스택

### 백엔드
- **웹 프레임워크**: Django 3.2+
- **데이터베이스**: PostgreSQL
- **API**: Django REST Framework
- **AI/ML**: PyTorch, YOLO, OpenCV
- **기타 라이브러리**: NumPy, Pillow, Ultralytics

### 프론트엔드
- **프레임워크**: React 18
- **상태 관리**: Zustand
- **UI 라이브러리**: Material-UI (MUI)
- **데이터 시각화**: D3.js, Plotly.js
- **지도**: Leaflet
- **HTTP 클라이언트**: Axios
- **라우팅**: React Router

## 설치 및 실행 방법

### 백엔드 설치

```bash
# 프로젝트 클론
git clone https://github.com/your-repo/AIACS_WEB.git
cd AIACS_WEB/backend

# 가상 환경 생성 및 활성화
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# 의존성 설치
pip install -r requirements.txt

# 데이터베이스 마이그레이션
python manage.py makemigrations
python manage.py migrate

# 개발 서버 실행
python manage.py runserver
```

### 프론트엔드 설치

```bash
# 프론트엔드 디렉토리로 이동
cd ../frontend

# 의존성 설치
npm install

# 개발 서버 실행
npm start
```

## Docker 실행 (선택 사항)

Jetson AGX Orin 또는 다른 환경에서 Docker를 사용해 실행할 수 있습니다:

```bash
# Docker Compose를 사용한 실행
docker-compose up -d --build
```

## 주요 기능

### 1. 조류 탐지 및 식별
- YOLO 객체 탐지 모델을 사용한 실시간 조류 식별
- 레이더 데이터와 카메라 영상 통합을 통한 정확한 위치 파악

### 2. 레이더 모니터링
- 조류 이동 경로 실시간 추적
- 충돌 위험 예측 및 알림

### 3. 기상 데이터 분석
- 현재 및 예측 기상 조건 모니터링
- 기상 조건과 조류 활동의 상관관계 분석

### 4. 방어 시스템 제어
- 조류 충돌 위험 감지 시 자동/수동 방어 시스템 작동
- 다양한 방어 메커니즘 통합 제어

### 5. 데이터 분석 및 리포팅
- 조류 활동 패턴 분석
- 위험 구역 식별 및 시각화
- 장기간 데이터 트렌드 분석

## 디렉토리 구조

```
AIACS_WEB/
├── backend/                # Django 백엔드
│   ├── camera/             # 카메라 앱 (비디오 처리)
│   ├── config/             # Django 설정
│   ├── radar_api/          # 레이더 데이터 API
│   ├── weather/            # 기상 데이터 앱
│   ├── video_uploads/      # 업로드된 비디오 저장
│   ├── manage.py           # Django 관리 스크립트
│   └── requirements.txt    # 파이썬 의존성
│
├── frontend/               # React 프론트엔드
│   ├── public/             # 정적 파일
│   ├── src/                # 소스 코드
│   │   ├── api/            # API 통신 모듈
│   │   ├── components/     # 재사용 가능한 컴포넌트
│   │   ├── scenes/         # 페이지 컴포넌트
│   │   ├── store/          # 상태 관리
│   │   ├── theme/          # UI 테마
│   │   └── utils/          # 유틸리티 함수
│   ├── package.json        # 노드 의존성
│   └── README.md           # 프론트엔드 문서
│
└── README.md               # 프로젝트 문서
```

## 개발 가이드

### 백엔드 개발

1. **새 API 엔드포인트 추가**
   - 적절한 앱 내 `urls.py` 및 `views.py` 파일에 엔드포인트 추가
   - 필요한 경우 새 모델 정의 (`models.py`)

2. **YOLO 모델 업데이트**
   - 새 모델 파일을 `backend/` 디렉토리에 배치
   - `camera/yolo/detector.py` 파일에서 참조 업데이트

### 프론트엔드 개발

1. **새 컴포넌트 추가**
   - `src/components/` 디렉토리에 새 컴포넌트 생성
   - 컴포넌트 스타일은 MUI 테마 활용

2. **새 페이지 추가**
   - `src/scenes/` 디렉토리에 새 페이지 컴포넌트 생성
   - `App.js`에 라우팅 설정 추가

3. **API 호출 추가**
   - `src/api/` 디렉토리에 새 API 함수 추가

## 트러블슈팅

### 백엔드 문제

- **YOLO 모델 로딩 오류**
  - CUDA 버전 확인 (CUDA 11.4 권장)
  - 충분한 GPU 메모리 확보

- **데이터베이스 연결 오류**
  - PostgreSQL 서비스 실행 확인
  - 환경 변수 설정 확인

### 프론트엔드 문제

- **API 연결 오류**
  - CORS 설정 확인
  - 백엔드 서버 실행 확인
  - API 엔드포인트 경로 확인

- **리액트 빌드 오류**
  - Node.js 버전 확인 (v14+ 권장)
  - npm 캐시 정리: `npm cache clean --force`

## 기여하기

1. 이 저장소를 포크합니다.
2. 새 기능 브랜치를 생성합니다: `git checkout -b feature/new-feature`
3. 변경 사항을 커밋합니다: `git commit -m 'Add new feature'`
4. 브랜치를 푸시합니다: `git push origin feature/new-feature`
5. 풀 리퀘스트를 제출합니다.

## 라이센스

이 프로젝트는 [라이센스 정보]에 따라 라이센스가 부여됩니다. 