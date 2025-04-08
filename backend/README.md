# Jetson AGX Orin용 백엔드 Docker 배포 가이드

이 문서는 Jetson AGX Orin Developer Kit(L4T R35.4.1, CUDA 11.4)에서 Docker를 이용하여 백엔드를 실행하는 방법을 설명합니다.

## 시스템 사양

- **하드웨어**: Jetson AGX Orin Developer Kit
- **OS**: Ubuntu 20.04.6 LTS
- **L4T 버전**: R35 (REVISION: 4.1)
- **CUDA**: 11.4.315
- **TensorRT**: 8.5.2.2
- **cuDNN**: 8.6.0

## 사전 요구사항

- Docker 및 Docker Compose가 설치되어 있어야 합니다.
- NVIDIA 컨테이너 런타임이 설치되어 있어야 합니다.

## 설치 및 실행 단계

### 1. NVIDIA 컨테이너 툴킷 설치 (아직 설치하지 않은 경우)

```bash
# NVIDIA Container Toolkit 저장소 추가
sudo apt-get update
sudo apt-get install -y apt-transport-https ca-certificates curl gnupg
distribution=$(. /etc/os-release;echo $ID$VERSION_ID)
curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add -
curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list

# 패키지 리스트 업데이트 및 설치
sudo apt-get update
sudo apt-get install -y nvidia-docker2

# Docker 서비스 재시작
sudo systemctl restart docker
```

### 2. 프로젝트 실행

```bash
# 프로젝트 디렉토리로 이동
cd /path/to/backend/directory

# Docker 컨테이너 빌드 및 실행
docker-compose up -d --build
```

### 3. 실행 확인

브라우저에서 `http://localhost:8000` 또는 `http://서버IP:8000`에 접속하여 백엔드가 정상적으로 실행되는지 확인합니다.

## 데이터베이스 초기화 (필요한 경우)

시스템을 처음 실행하는 경우 Django 마이그레이션이 자동으로 실행됩니다. 추가적인 초기화가 필요한 경우:

```bash
# 웹 컨테이너에 접속
docker-compose exec web bash

# 마이그레이션 실행
python manage.py makemigrations camera
python manage.py migrate

# 슈퍼유저 생성 (선택 사항)
python manage.py createsuperuser
```

## 성능 최적화 팁

### YOLO 모델 TensorRT 최적화

Jetson에서 더 빠른 추론을 위해 TensorRT로 YOLO 모델을 최적화할 수 있습니다:

```python
from ultralytics import YOLO

# 모델 로드
model = YOLO('yolo12_27_best.pt')

# TensorRT로 모델 내보내기
model.export(format='engine', device=0)
```

### 메모리 관리

Jetson AGX Orin은 충분한 메모리(61GB)를 가지고 있으므로 메모리 관리에 큰 제약은 없습니다. 그러나 GPU 메모리를 효율적으로 사용하려면:

```bash
# Jetson 상태 모니터링
sudo tegrastats
```

## 로그 확인

```bash
# 모든 서비스 로그 확인
docker-compose logs -f

# 웹 서비스 로그만 확인
docker-compose logs -f web

# 데이터베이스 서비스 로그만 확인
docker-compose logs -f db
```

## 컨테이너 종료

```bash
# 모든 컨테이너 종료
docker-compose down

# 볼륨까지 함께 제거 (데이터베이스 데이터 삭제)
docker-compose down -v
``` 