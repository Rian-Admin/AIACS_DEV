# 기상 정보 시스템 사용 설명서

이 문서는 조류 감시 시스템에 추가된 기상 정보 기능의 설치 및 사용법을 설명합니다.

## 1. 개요

기상 정보 시스템은 기상청 지상 관측자료 API를 활용하여 현재 날씨 정보를 수집하고 표시합니다. 이 시스템은 다음과 같은 기능을 제공합니다:

- 실시간 온도, 습도, 풍속, 강수량 등 기상 정보 표시
- 시정, 기압, 이슬점온도 등 상세 기상 정보 제공
- 과거 24시간 동안의 날씨 변화 그래프 표시

## 2. 설치 및 설정

### 2.1 API 키 설정

기상청 API를 사용하기 위해서는 API 키가 필요합니다. 이미 예제 API 키가 설정되어 있으나, 필요한 경우 발급받은 API 키로 변경할 수 있습니다.

API 키는 `backend/weather/utils.py` 파일의 `API_KEY` 변수에 설정합니다:

```python
API_KEY = "발급받은서비스키입력"  # 여기에 발급받은 키를 입력
```

### 2.2 관측소 설정

지상 관측자료를 가져올 관측소를 설정할 수 있습니다. 기본값은 `0`으로 전체 관측소의 데이터를 가져옵니다. 특정 관측소의 데이터만 가져오려면 해당 관측소 번호를 설정합니다.

```python
STN = "0"  # 전체 지점 (특정 지점 번호로 변경 가능)
```

여러 관측소의 데이터를 가져오려면 콜론(:)으로 구분하여 여러 번호를 지정할 수 있습니다(예: "108:112:156").

### 2.3 데이터베이스 마이그레이션

날씨 앱이 추가된 후 데이터베이스 마이그레이션을 실행합니다:

```bash
cd backend
python manage.py makemigrations weather
python manage.py migrate weather
```

## 3. API 엔드포인트

### 3.1 최신 날씨 정보 가져오기

**URL**: `/api/weather/`  
**Method**: GET  
**응답 예시**:
```json
{
  "current": {
    "temperature": 22.5,
    "humidity": 65,
    "wind_speed": 2.5,
    "wind_direction": "남동",
    "precipitation": 0,
    "precipitation_type": "없음",
    "air_quality": "좋음",
    "pressure": 1015.2,
    "dew_point": 18.3,
    "location_name": "서울"
  },
  "forecast": {
    "temperature": 22.5,
    "humidity": 65,
    "wind_speed": 2.5,
    "precipitation": 0,
    "air_quality": "좋음"
  },
  "timestamp": "2023-06-15T09:30:00Z",
  "update_time": "2023-06-15T09:30:00Z"
}
```

### 3.2 날씨 정보 수동 업데이트

**URL**: `/api/weather/update/`  
**Method**: POST  
**응답 예시**:
```json
{
  "message": "날씨 정보가 성공적으로 업데이트되었습니다."
}
```

### 3.3 날씨 이력 정보 가져오기

**URL**: `/api/weather/history/`  
**Method**: GET  
**응답 예시**:
```json
[
  {
    "temperature": 22.5,
    "humidity": 65,
    "wind_speed": 2.5,
    "precipitation": 0,
    "timestamp": "2023-06-15T09:00:00Z"
  },
  {
    "temperature": 21.8,
    "humidity": 68,
    "wind_speed": 2.2,
    "precipitation": 0,
    "timestamp": "2023-06-15T08:00:00Z"
  }
  // ... 추가 데이터
]
```

## 4. 자동 업데이트 설정

날씨 정보는 프론트엔드에서 다음과 같이 자동으로 업데이트됩니다:

- 현재 날씨 정보: 10분마다 자동 갱신
- 날씨 이력 정보: 1시간마다 자동 갱신

백엔드에서 날씨 정보의 자동 업데이트를 위해서는 스케줄러 설정이 필요합니다. 이는 다음 방법으로 설정할 수 있습니다:

### 4.1 Cron Job 설정 (리눅스/유닉스 시스템)

```bash
# crontab -e 명령으로 편집기 열기
*/30 * * * * cd /path/to/project && python3 manage.py shell -c "from weather.utils import update_weather_database; update_weather_database()"
```

위 설정은 30분마다 날씨 정보를 자동으로 업데이트합니다.

### 4.2 Windows 작업 스케줄러 설정

1. 작업 스케줄러 열기
2. '기본 작업 만들기' 선택
3. 원하는 이름 입력 (예: "날씨 데이터 업데이트")
4. 트리거: 30분마다 실행되도록 설정
5. 작업: `python %프로젝트경로%\backend\manage.py shell -c "from weather.utils import update_weather_database; update_weather_database()"`

## 5. 문제 해결

### 5.1 날씨 정보가 표시되지 않는 경우

1. 백엔드 서버가 실행 중인지 확인
2. API 키가 올바르게 설정되었는지 확인
3. 백엔드 로그에서 에러 메시지 확인
4. 브라우저 개발자 도구에서 네트워크 요청 확인

### 5.2 특정 지점의 날씨 정보 확인

특정 관측소의 데이터를 확인하려면 다음 절차를 따르세요:

1. `backend/weather/utils.py` 파일의 `STN` 변수를 원하는 지점 번호로 설정
2. 백엔드 서버 재시작
3. `/api/weather/update/` API 호출하여 데이터 갱신

## 6. 라이센스 및 관련 정보

- 기상 데이터 출처: 기상청 지상 관측자료 API
- 참고 문서: 국제기상전보식(2012)
- 데이터 이용 약관: [기상청 홈페이지](https://www.kma.go.kr/) 참조 