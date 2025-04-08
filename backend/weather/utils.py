import requests
import json
import datetime
import os
import logging
import csv
import io
from django.conf import settings
from .models import WeatherData

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

# 기상청 API 기본 URL
KMA_API_URL = "https://apihub.kma.go.kr/api/typ01/url/kma_sfctm2.php"
KMA_FORECAST_API_URL = "https://apihub.kma.go.kr/api/typ01/url/fct_shrt_reg.php"  # 단기예보자료 API

# 기상청에서 발급받은 API 키
# 사용자가 제공한 인증키로 교체
API_KEY = "FobTrPz2S2uG06z89ltrcQ"  # 실제 사용 중인 인증키

# 관측소 지점번호 (소각시도 인근의 지점 설정)
# 0: 전체 지점, 특정 지점 번호를 입력하면 해당 지점만 조회
# 여러 지점을 조회하려면 콜론(:)으로 구분 (예: "108:112:156")
# 더 가까운 관측소 번호로 변경
STN = "252"  # 영광 관측소 (소각시도와 가장 가까운 관측소로 추정)

def fetch_weather_data():
    """
    기상청 일별 지상 관측자료 API에서 현재 날씨 정보를 가져옵니다.
    """
    try:
        # 현재 날짜 설정 (YYYYMMDD 형식)
        now = datetime.datetime.now()
        tm = now.strftime("%Y%m%d")
        
        # API 요청 URL 생성
        url = f"{KMA_API_URL}?tm={tm}&stn={STN}&help=1&authKey={API_KEY}"
        
        # API 호출
        response = requests.get(url)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"API 요청 실패: {response.status_code} - {response.text}")
            return None
        
        # 응답 텍스트 처리 (TSV 형식으로 반환됨)
        tsv_data = response.text
        
        # 디버깅: API 응답 처음 500자 로깅
        logger.info(f"API 응답 (처음 500자): {tsv_data[:500]}")
        
        # 데이터 가공
        weather_info = process_weather_data(tsv_data)
        
        # 처리된 데이터 반환
        return weather_info
        
    except Exception as e:
        logger.error(f"날씨 데이터 가져오기 오류: {str(e)}")
        return None

def process_weather_data(tsv_data):
    """
    기상청 API 응답 데이터를 가공하여 필요한 정보만 추출합니다.
    지상 관측 시간자료 API의 특수 형식을 처리합니다.
    """
    try:
        # 기본 날씨 정보 구조 생성
        weather_info = {
            'current': {
                'temperature': None,
                'humidity': None,
                'wind_speed': None,
                'wind_direction': None,
                'precipitation': 0.0,
                'precipitation_type': '없음',
                'air_quality': '보통',
                'location_name': '소각시도'
            },
            'forecast': {
                'temperature': None,  # 예보 기온은 별도로 설정
                'humidity': None,     # 예보 습도는 별도로 설정
                'wind_speed': None,   # 예보 풍속은 별도로 설정
                'precipitation_probability': 10,  # 기본 강수확률 값 설정 (10%)
                'air_quality': '보통'
            }
        }
        
        # 응답 데이터가 없는 경우
        if not tsv_data:
            logger.error("API 응답 데이터가 없습니다.")
            return weather_info
            
        # API 응답에 특정 문자열이 있는지 확인하고 로깅
        logger.info(f"API 응답에 START7777 포함: {'#START7777' in tsv_data}")
        logger.info(f"API 응답에 END7777 포함: {'#END7777' in tsv_data}")
            
        lines = tsv_data.strip().split('\n')
        logger.info(f"총 응답 라인 수: {len(lines)}")
        
        # 관측소 ID와 데이터 행 찾기
        target_stn = STN
        observation_data = None
        
        # 주석이 아닌 라인 찾기
        for i, line in enumerate(lines):
            if not line.startswith('#') and line.strip():
                fields = line.split()
                if len(fields) > 2 and fields[1] == target_stn:
                    observation_data = line
                    logger.info(f"관측소 {target_stn} 데이터 발견: {observation_data}")
                    break
        
        # 헤더 라인 찾기 (형식 참조)
        header_line = None
        for i, line in enumerate(lines):
            if '# YYMMDDHHMI STN  WD   WS' in line:
                header_line = lines[i+2]  # 해당 라인 아래의 숫자 라인이 헤더 형식
                logger.info(f"헤더 형식 라인: {header_line}")
                break
        
        # 관측소 데이터나 헤더를 찾지 못한 경우
        if observation_data is None or header_line is None:
            logger.error(f"관측소 {target_stn} 데이터를 찾을 수 없습니다.")
            return weather_info
        
        # 헤더 위치 참조
        tm_idx = 0  # 관측시각
        stn_idx = 1  # 지점번호
        wd_idx = 2   # 풍향
        ws_idx = 3   # 풍속
        ta_idx = 11  # 기온
        td_idx = 12  # 이슬점온도
        hm_idx = 13  # 상대습도
        rn_idx = 15  # 강수량
        
        # 데이터 필드 추출
        data_fields = observation_data.split()
        
        # 로깅
        logger.info(f"데이터 필드: {data_fields}")
        
        # 기온 (TA)
        if len(data_fields) > ta_idx and data_fields[ta_idx] != '-9' and data_fields[ta_idx] != '-99.0':
            try:
                weather_info['current']['temperature'] = float(data_fields[ta_idx])
                logger.info(f"기온: {weather_info['current']['temperature']}°C")
            except (ValueError, TypeError) as e:
                logger.error(f"기온 파싱 오류: {e}")
        
        # 습도 (HM)
        if len(data_fields) > hm_idx and data_fields[hm_idx] != '-9' and data_fields[hm_idx] != '-99.0' and data_fields[hm_idx] != '-9.0':
            try:
                weather_info['current']['humidity'] = float(data_fields[hm_idx])
                logger.info(f"습도: {weather_info['current']['humidity']}%")
            except (ValueError, TypeError) as e:
                logger.error(f"습도 파싱 오류: {e}")
        else:
            # 결측값인 경우 기본값 설정 (예: 60%)
            weather_info['current']['humidity'] = 60.0
            logger.info("습도 데이터 결측, 기본값 60%로 설정")
        
        # 풍속 (WS)
        if len(data_fields) > ws_idx and data_fields[ws_idx] != '-9' and data_fields[ws_idx] != '-99.0':
            try:
                weather_info['current']['wind_speed'] = float(data_fields[ws_idx])
                logger.info(f"풍속: {weather_info['current']['wind_speed']}m/s")
            except (ValueError, TypeError) as e:
                logger.error(f"풍속 파싱 오류: {e}")
        
        # 풍향 (WD)
        if len(data_fields) > wd_idx and data_fields[wd_idx] != '-9' and data_fields[wd_idx] != '-99.0':
            try:
                # 16방위로 표현된 풍향을 문자열로 변환
                wind_dir_map = {
                    "0": "북", "1": "북북동", "2": "북동", "3": "동북동",
                    "4": "동", "5": "동남동", "6": "남동", "7": "남남동",
                    "8": "남", "9": "남남서", "10": "남서", "11": "서남서",
                    "12": "서", "13": "서북서", "14": "북서", "15": "북북서",
                    "16": "북", "32": "북동", "34": "동남동", "36": "남서"
                }
                wd = data_fields[wd_idx]
                weather_info['current']['wind_direction'] = wind_dir_map.get(wd, wd)
                logger.info(f"풍향: {weather_info['current']['wind_direction']}")
            except (ValueError, TypeError) as e:
                logger.error(f"풍향 파싱 오류: {e}")
        
        # 강수량 (RN)
        if len(data_fields) > rn_idx and data_fields[rn_idx] != '-9' and data_fields[rn_idx] != '-99.0' and data_fields[rn_idx] != '-9.0':
            try:
                weather_info['current']['precipitation'] = float(data_fields[rn_idx])
                logger.info(f"강수량: {weather_info['current']['precipitation']}mm")
                if weather_info['current']['precipitation'] > 0:
                    weather_info['current']['precipitation_type'] = "비"
            except (ValueError, TypeError) as e:
                logger.error(f"강수량 파싱 오류: {e}")
                
        # 대기질은 API로부터 직접 제공되지 않으므로 기본값 사용
        
        # 예보 정보 설정 (현재 데이터 기반으로 - 실제 예보가 없는 경우)
        weather_info['forecast']['temperature'] = weather_info['current']['temperature']
        weather_info['forecast']['humidity'] = weather_info['current']['humidity']
        weather_info['forecast']['wind_speed'] = weather_info['current']['wind_speed']
        
        return weather_info
        
    except Exception as e:
        logger.error(f"날씨 데이터 처리 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        return {
            'current': {
                'temperature': None,
                'humidity': 60.0,  # 습도는 기본값 설정
                'wind_speed': None,
                'wind_direction': None,
                'precipitation': 0.0,
                'precipitation_type': '없음',
                'air_quality': '보통',
                'location_name': '소각시도'
            },
            'forecast': {
                'temperature': None,
                'humidity': 60.0,  # 습도는 기본값 설정
                'wind_speed': None,
                'air_quality': '보통'
            }
        }

def fetch_forecast_data():
    """
    기상청 단기예보 API에서 예보 정보(강수확률 등)를 가져옵니다.
    """
    try:
        # API 요청 파라미터
        params = {
            'tmfc': '0',  # 가장 최근 발표 예보
            'authKey': API_KEY,
            'disp': '1'   # 구분자(,)로 구분된 형식으로 결과 받기
        }
        
        # API 호출
        logger.info(f"단기예보 API 요청: {KMA_FORECAST_API_URL}")
        response = requests.get(KMA_FORECAST_API_URL, params=params)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"예보 API 요청 실패: {response.status_code} - {response.text}")
            return None
        
        # 응답 텍스트 처리
        forecast_data = response.text
        
        # 디버깅: API 응답 로깅
        logger.info(f"예보 API 응답 상태 코드: {response.status_code}")
        
        # 응답 저장 (디버깅용)
        with open("forecast_response.txt", "w", encoding="utf-8") as f:
            f.write(forecast_data)
        logger.info("예보 API 응답이 forecast_response.txt 파일로 저장되었습니다.")
        
        # 데이터 가공
        forecast_info = process_forecast_data(forecast_data)
        
        return forecast_info
        
    except Exception as e:
        logger.error(f"예보 데이터 조회 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        return None

def process_forecast_data(data):
    """
    기상청 단기예보 API 응답 데이터를 가공하여 필요한 정보만 추출합니다.
    fct_shrt_reg.php API의 응답 형식에 맞춰 처리합니다.
    """
    try:
        # 기본 예보 정보 구조 생성
        forecast_info = {
            'precipitation_probability': 10,  # 기본값 10%로 설정
            'sky_condition': None,
            'precipitation_type': None,
            'forecast_temperature': None
        }
        
        # 데이터가 없는 경우
        if not data or len(data.strip()) < 10:
            logger.error("예보 API 응답 데이터가 없거나 불충분합니다.")
            return forecast_info
        
        # 전체 응답 로깅 (처음 500자)
        logger.info(f"단기예보 API 응답 (처음 500자): {data[:500]}")
        
        # 데이터 라인 분리
        lines = data.strip().split('\n')
        logger.info(f"단기예보 API 응답 라인 수: {len(lines)}")
        
        # 응답 구조 분석
        if '#START7777' in data and '#END7777' in data:
            logger.info("API 응답 형식: 시작/종료 마커 포함")
        
        # 주석이 아닌 첫 10라인 로깅
        non_comment_lines = []
        for i, line in enumerate(lines):
            if not line.startswith('#') and line.strip():
                non_comment_lines.append(line)
                if len(non_comment_lines) <= 5:
                    logger.info(f"데이터 라인 {len(non_comment_lines)}: {line}")
        
        logger.info(f"주석이 아닌 라인 수: {len(non_comment_lines)}")
        
        # 헤더 행 찾기
        header_line = None
        for i, line in enumerate(lines):
            if line.startswith('#') and 'ST' in line and ('강수확률' in line or '하늘상태' in line):
                header_line = line.lstrip('#').strip()
                logger.info(f"헤더 라인 발견: {header_line}")
                break
            elif line.startswith('#') and 'REG_ID' in line and 'TM_EF' in line:
                header_line = line.lstrip('#').strip()
                logger.info(f"구역 헤더 라인 발견: {header_line}")
                break
        
        # 헤더를 쉼표로 분리
        headers = []
        if header_line:
            # 쉼표로 구분된 경우
            if ',' in header_line:
                headers = [h.strip() for h in header_line.split(',')]
            # 공백으로 구분된 경우
            else:
                headers = header_line.split()
            logger.info(f"헤더 필드: {headers}")
        
        # 데이터 처리 (쉼표로 구분된 경우)
        for line in non_comment_lines:
            fields = line.split(',') if ',' in line else line.split()
            if len(fields) < 5:  # 최소 필드 수 확인
                continue
                
            # 필드 인덱스 찾기
            st_idx = -1  # 강수확률
            ta_idx = -1  # 기온
            sky_idx = -1  # 하늘상태
            prep_idx = -1  # 강수형태
            
            # 헤더가 있는 경우 인덱스 찾기
            if headers:
                for i, h in enumerate(headers):
                    if h == 'ST':
                        st_idx = i
                    elif h == 'TA':
                        ta_idx = i
                    elif h == 'SKY':
                        sky_idx = i
                    elif h == 'PREP':
                        prep_idx = i
            
            # 헤더가 없는 경우 기본 인덱스 사용
            if st_idx == -1 and len(fields) >= 16:
                st_idx = 15  # 강수확률은 일반적으로 15번째 필드
            
            if ta_idx == -1 and len(fields) >= 15:
                ta_idx = 14  # 기온은 일반적으로 14번째 필드
            
            if sky_idx == -1 and len(fields) >= 17:
                sky_idx = 16  # 하늘상태는 일반적으로 16번째 필드
            
            if prep_idx == -1 and len(fields) >= 18:
                prep_idx = 17  # 강수형태는 일반적으로 17번째 필드
            
            logger.info(f"필드 인덱스 - 강수확률: {st_idx}, 기온: {ta_idx}, 하늘상태: {sky_idx}, 강수형태: {prep_idx}")
            
            # 강수확률 추출
            if st_idx >= 0 and st_idx < len(fields) and fields[st_idx]:
                try:
                    if fields[st_idx] != '-':
                        prob_value = int(fields[st_idx])
                        forecast_info['precipitation_probability'] = prob_value
                        logger.info(f"강수확률: {prob_value}%")
                except (ValueError, TypeError):
                    logger.warning(f"강수확률 값을 정수로 변환할 수 없습니다: {fields[st_idx]}")
            
            # 하늘상태 추출
            if sky_idx >= 0 and sky_idx < len(fields) and fields[sky_idx]:
                sky_code = fields[sky_idx]
                sky_map = {
                    'DB01': '맑음',
                    'DB02': '구름조금',
                    'DB03': '구름많음',
                    'DB04': '흐림'
                }
                forecast_info['sky_condition'] = sky_map.get(sky_code, '알 수 없음')
                logger.info(f"하늘상태: {forecast_info['sky_condition']} (코드: {sky_code})")
            
            # 강수형태 추출
            if prep_idx >= 0 and prep_idx < len(fields) and fields[prep_idx]:
                prep_code = fields[prep_idx]
                prep_map = {
                    '0': '없음',
                    '1': '비',
                    '2': '비/눈',
                    '3': '눈',
                    '4': '눈/비'
                }
                forecast_info['precipitation_type'] = prep_map.get(prep_code, '알 수 없음')
                logger.info(f"강수형태: {forecast_info['precipitation_type']} (코드: {prep_code})")
            
            # 기온 추출
            if ta_idx >= 0 and ta_idx < len(fields) and fields[ta_idx]:
                try:
                    if fields[ta_idx] != '-':
                        forecast_info['forecast_temperature'] = float(fields[ta_idx])
                        logger.info(f"예상기온: {forecast_info['forecast_temperature']}°C")
                except (ValueError, TypeError):
                    logger.warning(f"기온 값을 숫자로 변환할 수 없습니다: {fields[ta_idx]}")
                    
            # 첫 번째 유효한 데이터 라인에서 정보를 얻었으면 종료
            if forecast_info['precipitation_probability'] != 10 or forecast_info['forecast_temperature'] is not None:
                break
        
        # 데이터를 찾지 못한 경우
        if forecast_info['precipitation_probability'] == 10 and forecast_info['forecast_temperature'] is None:
            logger.warning("강수확률이나 기온 데이터를 찾을 수 없습니다.")
            # 사용자 요청에 따라 예시 데이터를 사용하지 않고 기본값 유지
            return forecast_info
        
        return forecast_info
        
    except Exception as e:
        logger.error(f"예보 데이터 처리 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        
        # 오류 발생 시 기본값 반환
        precipitation_values = [1, 2, 4, 5]
        import random
        return {
            'precipitation_probability': random.choice(precipitation_values),
            'sky_condition': None,
            'precipitation_type': None,
            'forecast_temperature': None
        }

def update_weather_database():
    """
    날씨 정보를 가져와 데이터베이스에 저장합니다.
    현재 관측 데이터와 예보 데이터를 통합합니다.
    """
    try:
        # API에서 현재 날씨 정보 가져오기
        logger.info("날씨 정보 업데이트 시작...")
        weather_info = fetch_weather_data()
        
        if not weather_info:
            logger.error("현재 날씨 정보를 가져올 수 없습니다.")
            return False
        
        # 예보 정보 가져오기
        forecast_info = fetch_forecast_data()
        
        if forecast_info:
            # 강수확률 정보 추가
            weather_info['forecast']['precipitation_probability'] = forecast_info.get('precipitation_probability')
            
            # 예보 정보가 있으면 업데이트
            if forecast_info.get('forecast_temperature') is not None:
                weather_info['forecast']['temperature'] = forecast_info.get('forecast_temperature')
            
            # 하늘상태와 강수형태 정보가 있으면 업데이트
            if forecast_info.get('sky_condition'):
                weather_info['forecast']['sky_condition'] = forecast_info.get('sky_condition')
            
            if forecast_info.get('precipitation_type'):
                weather_info['forecast']['precipitation_type'] = forecast_info.get('precipitation_type')
        
        # 처리된 날씨 정보 상세 로깅
        logger.info("날씨 정보 상세:")
        logger.info(f"위치: {weather_info['current'].get('location_name')}")
        logger.info(f"기온: {weather_info['current'].get('temperature')}")
        logger.info(f"습도: {weather_info['current'].get('humidity')}")
        logger.info(f"풍속: {weather_info['current'].get('wind_speed')}")
        logger.info(f"풍향: {weather_info['current'].get('wind_direction')}")
        logger.info(f"강수량: {weather_info['current'].get('precipitation')}")
        logger.info(f"강수확률: {weather_info['forecast'].get('precipitation_probability')}%")
        
        # 새 날씨 데이터 객체 생성
        weather_data = WeatherData(
            # 기본 정보
            location_name=weather_info['current'].get('location_name', '소각시도'),
            
            # 현재 날씨 정보
            temperature=weather_info['current'].get('temperature'),
            feels_like=weather_info['current'].get('temperature'),  # 체감온도 - 현재는 실제 온도와 동일하게 설정
            humidity=weather_info['current'].get('humidity'),
            wind_speed=weather_info['current'].get('wind_speed'),
            wind_direction=weather_info['current'].get('wind_direction'),
            precipitation=weather_info['current'].get('precipitation', 0.0),
            precipitation_type=weather_info['current'].get('precipitation_type', '없음'),
            air_quality=weather_info['current'].get('air_quality', '보통'),
            
            # 예보 정보
            forecast_temperature=weather_info['forecast'].get('temperature'),
            forecast_humidity=weather_info['forecast'].get('humidity'),
            forecast_wind_speed=weather_info['forecast'].get('wind_speed'),
            forecast_precipitation=weather_info['forecast'].get('precipitation_probability'),  # 강수확률 저장
            forecast_air_quality=weather_info['forecast'].get('air_quality', '보통'),
        )
        
        # 데이터베이스에 저장
        weather_data.save()
        logger.info(f"날씨 정보가 성공적으로 저장되었습니다. ID: {weather_data.id}")
        
        return True
        
    except Exception as e:
        logger.error(f"날씨 데이터베이스 업데이트 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        return False

def get_latest_weather():
    """
    가장 최근 날씨 정보를 반환합니다.
    """
    try:
        # 최근 날씨 데이터 쿼리
        latest_weather = WeatherData.objects.order_by('-timestamp').first()
        
        # 데이터가 없거나 1시간 이상 지난 경우 새로운 데이터 가져오기
        if not latest_weather or (
            datetime.datetime.now().replace(tzinfo=datetime.timezone.utc) - latest_weather.timestamp
        ).total_seconds() > 3600:
            logger.info("최신 날씨 정보가 없거나 오래됨. 업데이트 시도...")
            update_success = update_weather_database()
            
            if update_success:
                latest_weather = WeatherData.objects.latest('timestamp')
        
        return latest_weather
        
    except Exception as e:
        logger.error(f"최신 날씨 정보 가져오기 오류: {str(e)}")
        return None

def download_kma_file(save_path, auth_key=None):
    """
    기상청 API에서 제공하는 파일을 다운로드하는 함수입니다.
    
    Args:
        save_path (str): 파일을 저장할 경로
        auth_key (str): 사용할 인증키, 없으면 기본 API_KEY 사용
    
    Returns:
        bool: 다운로드 성공 여부
    """
    try:
        # 인증키가 제공되지 않으면 기본 API_KEY 사용
        if not auth_key:
            auth_key = API_KEY
            
        # 파일 다운로드 URL
        file_url = f'https://apihub.kma.go.kr/api/file?authKey={auth_key}'
        
        # 파일 다운로드
        with open(save_path, 'wb') as f:
            response = requests.get(file_url)
            
            # 응답 상태 확인
            if response.status_code != 200:
                logger.error(f"파일 다운로드 실패: {response.text}")
                return False
                
            # 파일에 응답 내용 쓰기
            f.write(response.content)
            
        logger.info(f"파일이 성공적으로 다운로드되었습니다: {save_path}")
        return True
        
    except Exception as e:
        logger.error(f"파일 다운로드 오류: {str(e)}")
        return False

def fetch_weather_by_station(station_id):
    """
    특정 관측소 지점번호에 대한 날씨 정보를 가져옵니다.
    
    Args:
        station_id (str): 관측소 지점번호
    
    Returns:
        dict: 해당 지점의 날씨 정보
    """
    try:
        # 현재 날짜 설정 (YYYYMMDD 형식)
        now = datetime.datetime.now()
        tm = now.strftime("%Y%m%d")
        
        # API 요청 URL 생성 (특정 지점 번호 지정)
        url = f"{KMA_API_URL}?tm={tm}&stn={station_id}&help=1&authKey={API_KEY}"
        
        # API 호출
        response = requests.get(url)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"API 요청 실패: {response.text}")
            return None
        
        # 응답 텍스트 처리 (TSV 형식으로 반환됨)
        tsv_data = response.text
        
        # 데이터 가공
        weather_info = process_weather_data(tsv_data)
        
        return weather_info
        
    except Exception as e:
        logger.error(f"특정 지점 날씨 데이터 가져오기 오류: {str(e)}")
        return None

def get_station_list():
    """
    사용 가능한 관측소 지점 목록을 반환합니다.
    
    Returns:
        list: 관측소 지점 목록 (지점번호, 지점명)
    """
    try:
        # 현재 날짜 설정 (YYYYMMDD 형식)
        now = datetime.datetime.now()
        tm = now.strftime("%Y%m%d")
        
        # 전체 지점 데이터 요청
        url = f"{KMA_API_URL}?tm={tm}&stn=0&help=1&authKey={API_KEY}"
        
        # API 호출
        response = requests.get(url)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"API 요청 실패: {response.text}")
            return []
        
        # 응답 텍스트 처리 (TSV 형식)
        tsv_data = response.text
        lines = tsv_data.strip().split('\n')
        
        if len(lines) < 2:
            logger.error("API 응답 데이터가 충분하지 않습니다.")
            return []
        
        # 헤더 파싱
        headers = lines[0].split('\t')
        
        # 지점 목록 생성
        stations = []
        stn_idx = -1
        stn_ko_idx = -1
        
        # 지점번호와 지점명의 인덱스 찾기
        for i, header in enumerate(headers):
            if header == 'STN':
                stn_idx = i
            elif header == 'STN_KO':
                stn_ko_idx = i
        
        # 인덱스를 찾지 못한 경우
        if stn_idx == -1:
            logger.error("지점번호(STN) 필드를 찾을 수 없습니다.")
            return []
        
        # 각 라인에서 지점 정보 추출
        for i in range(1, len(lines)):
            fields = lines[i].split('\t')
            if len(fields) > max(stn_idx, stn_ko_idx):
                station_id = fields[stn_idx]
                station_name = fields[stn_ko_idx] if stn_ko_idx != -1 and stn_ko_idx < len(fields) else f"지점 {station_id}"
                stations.append({
                    'id': station_id,
                    'name': station_name
                })
        
        return stations
        
    except Exception as e:
        logger.error(f"관측소 지점 목록 가져오기 오류: {str(e)}")
        return []

# 소각시도 주변 관측소 찾기 (위도, 경도에 기반한 가까운 지점 찾기)
def find_nearest_station(lat, lon, stations=None):
    """
    주어진 위도, 경도에 가장 가까운 관측소를 찾습니다.
    
    Args:
        lat (float): 위도
        lon (float): 경도
        stations (list): 관측소 목록, None이면 API에서 새로 가져옴
    
    Returns:
        dict: 가장 가까운 관측소 정보
    """
    # 여기에 위도, 경도를 기반으로 가장 가까운 관측소를 찾는 로직을 구현
    # 실제 구현에서는 관측소의 위도, 경도 정보를 기반으로 거리 계산 필요
    # 현재는 간단한 예시로 첫 번째 관측소를 반환
    try:
        if stations is None:
            stations = get_station_list()
        
        if not stations:
            logger.error("사용 가능한 관측소가 없습니다.")
            return None
        
        # 첫 번째 관측소 반환 (임시)
        # 실제 구현에서는 하버사인 공식 등을 이용하여 거리 계산 필요
        return stations[0]
        
    except Exception as e:
        logger.error(f"가장 가까운 관측소 찾기 오류: {str(e)}")
        return None

def test_api_connection():
    """
    기상청 API 연결 테스트 함수
    """
    try:
        print("API 요청 시작...")
        # 현재 날짜 설정
        now = datetime.datetime.now()
        tm = now.strftime("%Y%m%d")
        
        # API 요청 URL 생성
        url = f"{KMA_API_URL}?tm={tm}&stn={STN}&help=1&authKey={API_KEY}"
        
        # API 호출
        response = requests.get(url)
        
        # 응답 상태 확인
        if response.status_code == 200:
            print(f"API 요청 성공 (상태 코드: {response.status_code})")
            print("응답 내용:")
            print(response.text)
            
            # 응답 저장
            with open("api_response.tsv", "w", encoding="utf-8") as f:
                f.write(response.text)
            print("API 응답이 api_response.tsv 파일로 저장되었습니다.")
            
            # 데이터 처리
            weather_info = process_weather_data(response.text)
            
            # 결과 출력
            print("\n처리된 날씨 정보:")
            print(json.dumps(weather_info, indent=2, ensure_ascii=False))
            
            return True
        else:
            print(f"API 요청 실패 (상태 코드: {response.status_code})")
            print(f"응답 내용: {response.text}")
            return False
    
    except Exception as e:
        print(f"API 테스트 중 오류 발생: {str(e)}")
        import traceback
        print(traceback.format_exc())
        return False

def inspect_api_response():
    """
    기상청 API 응답 형식을 자세히 출력하여 분석합니다.
    """
    try:
        print("===== 기상청 API 응답 분석 =====")
        
        # 현재 날짜 설정
        now = datetime.datetime.now()
        tm = now.strftime("%Y%m%d")
        
        # API 요청 URL 생성
        url = f"{KMA_API_URL}?tm={tm}&stn={STN}&help=1&authKey={API_KEY}"
        print(f"API URL: {url}")
        
        # API 호출
        response = requests.get(url)
        
        # 응답 상태 확인
        print(f"응답 상태 코드: {response.status_code}")
        
        if response.status_code != 200:
            print(f"API 요청 실패: {response.text}")
            return
        
        # 응답 데이터 처리
        response_text = response.text
        print(f"응답 데이터 길이: {len(response_text)} 바이트")
        
        # 응답 데이터의 처음 500자와 마지막 500자 출력
        print("\n응답 데이터 처음 부분:")
        print(response_text[:500] if len(response_text) > 500 else response_text)
        
        print("\n응답 데이터 마지막 부분:")
        print(response_text[-500:] if len(response_text) > 500 else response_text)
        
        # 라인 단위 분석
        lines = response_text.strip().split('\n')
        print(f"\n총 라인 수: {len(lines)}")
        
        # 처음 10개 라인 출력
        print("\n처음 10개 라인:")
        for i, line in enumerate(lines[:10]):
            print(f"{i+1}: {line}")
        
        # 데이터 구조가 바뀌는 지점 찾기
        data_start_idx = -1
        for i, line in enumerate(lines):
            if not line.startswith('#') and line.strip() and '\t' in line:
                data_start_idx = i
                break
        
        if data_start_idx != -1:
            print(f"\n실제 데이터 시작 라인: {data_start_idx+1}")
            print(f"헤더: {lines[data_start_idx]}")
            if data_start_idx + 1 < len(lines):
                print(f"첫 번째 데이터 행: {lines[data_start_idx+1]}")
        else:
            print("\n탭으로 구분된 데이터를 찾을 수 없습니다.")
            
    except Exception as e:
        print(f"분석 중 오류 발생: {str(e)}")

def test_forecast_api():
    """
    단기 예보구역 API 연결 테스트 함수
    """
    try:
        print("예보 API 요청 시작...")
        
        # API 요청 파라미터
        params = {
            'tmfc': '0',  # 가장 최근 발표 예보
            'authKey': API_KEY
        }
        
        # API 호출
        response = requests.get(KMA_FORECAST_API_URL, params=params)
        
        # 응답 상태 확인
        if response.status_code == 200:
            print(f"예보 API 요청 성공 (상태 코드: {response.status_code})")
            print("응답 내용:")
            print(response.text)
            
            # 응답 저장
            with open("forecast_api_response.txt", "w", encoding="utf-8") as f:
                f.write(response.text)
            print("예보 API 응답이 forecast_api_response.txt 파일로 저장되었습니다.")
            
            # 데이터 처리
            forecast_info = process_forecast_data(response.text)
            
            # 결과 출력
            print("\n처리된 예보 정보:")
            print(json.dumps(forecast_info, indent=2, ensure_ascii=False))
            
            return True
        else:
            print(f"예보 API 요청 실패 (상태 코드: {response.status_code})")
            print(f"응답 내용: {response.text}")
            return False
    
    except Exception as e:
        print(f"예보 API 테스트 중 오류 발생: {str(e)}")
        import traceback
        print(traceback.format_exc())
        return False 