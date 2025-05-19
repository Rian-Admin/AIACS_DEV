import requests
import json
import datetime
import os
import logging
import csv
import io
from django.conf import settings
from .models import WeatherData

# 로그 활성화
logger = logging.getLogger(__name__)

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
        
        logger.info(f"API 요청 URL: {url}")
        
        # API 호출
        response = requests.get(url, timeout=10)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"API 요청 실패: {response.status_code} - {response.text}")
            return None
        
        # 응답 텍스트 처리 (TSV 형식으로 반환됨)
        tsv_data = response.text
        
        # 디버깅: API 응답 저장
        with open("api_response.tsv", "w", encoding="utf-8") as f:
            f.write(tsv_data)
        logger.info("API 응답이 api_response.tsv 파일로 저장되었습니다.")
        
        # 디버깅: API 응답 처음 500자 로깅
        logger.info(f"API 응답 (처음 500자): {tsv_data[:500]}")
        
        # 데이터 가공
        weather_info = process_weather_data(tsv_data)
        
        # 처리된 데이터 반환
        return weather_info
        
    except Exception as e:
        logger.error(f"날씨 데이터 가져오기 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        
        # 오류 발생 시 기본 날씨 정보 제공 (프론트엔드 테스트용)
        return get_default_weather_info()

def get_default_weather_info():
    """
    API 연결 실패 시 기본 날씨 정보를 제공합니다. 더 이상 더미 데이터를 사용하지 않습니다.
    """
    return {
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
            'temperature': None,
            'humidity': None,
            'wind_speed': None,
            'precipitation_probability': 0,
            'air_quality': '보통',
            'sky_condition': None
        }
    }

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
                'location_name': '소각시도 (영광 관측소)',
                'visibility': None,  # 시정 정보 추가
            },
            'forecast': {
                'temperature': None,  # 예보 기온은 별도로 설정
                'humidity': None,     # 예보 습도는 별도로 설정
                'wind_speed': None,   # 예보 풍속은 별도로 설정
                'precipitation_probability': 0,  # 기본 강수확률 값 설정
                'air_quality': '보통',
                'sky_condition': None
            }
        }
        
        # 응답 데이터가 없는 경우
        if not tsv_data:
            logger.error("API 응답 데이터가 없습니다.")
            return get_default_weather_info()
            
        # API 응답에 특정 문자열이 있는지 확인하고 로깅
        logger.info(f"API 응답에 START7777 포함: {'#START7777' in tsv_data}")
        logger.info(f"API 응답에 END7777 포함: {'#END7777' in tsv_data}")
            
        lines = tsv_data.strip().split('\n')
        logger.info(f"총 응답 라인 수: {len(lines)}")
        
        # 데이터 라인 찾기 (주석이 아닌 라인)
        data_lines = []
        header_line = None
        
        for line in lines:
            if not line.startswith('#') and line.strip():
                # 공백으로 구분된 데이터 라인
                data_lines.append(line)
                logger.info(f"데이터 라인: {line}")
            elif line.startswith('# YYMMDDHHMI') or line.startswith('#  TM     :'):
                # 헤더 라인
                header_line = line
                logger.info(f"헤더 라인: {line}")
        
        if not data_lines:
            logger.error("API 응답에서 데이터 라인을 찾을 수 없습니다.")
            return get_default_weather_info()
        
        # 가장 최근 데이터 라인 사용
        latest_data = data_lines[0]
        fields = latest_data.split()
        
        # 필드 인덱스 매핑 (API 응답 형식에 따라 조정)
        try:
            if len(fields) >= 15:
                # 풍향 (WD)
                if fields[2] != '-9' and fields[2] != '-99.0':
                    wind_dir_map = {
                        "0": "북", "1": "북북동", "2": "북동", "3": "동북동",
                        "4": "동", "5": "동남동", "6": "남동", "7": "남남동",
                        "8": "남", "9": "남남서", "10": "남서", "11": "서남서",
                        "12": "서", "13": "서북서", "14": "북서", "15": "북북서",
                        "16": "북", "32": "북동", "34": "동남동", "36": "남서"
                    }
                    weather_info['current']['wind_direction'] = wind_dir_map.get(fields[2], fields[2])
                
                # 풍속 (WS)
                if fields[3] != '-9' and fields[3] != '-99.0':
                    weather_info['current']['wind_speed'] = float(fields[3])
                
                # 기온 (TA) - 일반적으로 11번째 필드
                if len(fields) > 11 and fields[11] != '-9' and fields[11] != '-99.0':
                    weather_info['current']['temperature'] = float(fields[11])
                
                # 습도 (HM) - 일반적으로 13번째 필드
                if len(fields) > 13 and fields[13] != '-9' and fields[13] != '-99.0':
                    weather_info['current']['humidity'] = float(fields[13])
                else:
                    weather_info['current']['humidity'] = 60.0  # 기본값
                
                # 강수량 (RN) - 일반적으로 15번째 필드
                if len(fields) > 15:
                    if fields[15] != '-9' and fields[15] != '-99.0' and fields[15] != '-9.0':
                        try:
                            weather_info['current']['precipitation'] = float(fields[15])
                            if float(fields[15]) > 0:
                                weather_info['current']['precipitation_type'] = '비'
                        except (ValueError, IndexError) as e:
                            logger.error(f"강수량 파싱 오류: {e}")
                    else:
                        # 결측값(-9, -9.0, -99.0)인 경우 0으로 설정
                        weather_info['current']['precipitation'] = 0.0
                        logger.info("강수량 결측값이 발견되어 0으로 설정")
                
                # 일강수량 (RN_DAY) - 16번째 필드 (인덱스 16)
                if len(fields) > 16:
                    if fields[16] != '-9' and fields[16] != '-99.0' and fields[16] != '-9.0':
                        try:
                            weather_info['current']['precipitation'] = float(fields[16])
                            if float(fields[16]) > 0:
                                weather_info['current']['precipitation_type'] = '비'
                            logger.info(f"일강수량 설정: {weather_info['current']['precipitation']} mm")
                        except (ValueError, IndexError) as e:
                            logger.error(f"일강수량 파싱 오류: {e}")
                    else:
                        # 결측값(-9, -9.0, -99.0)인 경우 0으로 설정
                        weather_info['current']['precipitation'] = 0.0
                        logger.info("일강수량 결측값이 발견되어 0으로 설정")
                
                # 시정 (VS) - 33번째 필드 (실제 인덱스는 32)
                if len(fields) > 32 and fields[32] != '-9' and fields[32] != '-99.0' and fields[32] != '-9.0':
                    try:
                        # VS 값은 10m 단위로 제공됨 -> km 단위로 변환
                        vs_value = float(fields[32])
                        visibility_meters = vs_value * 10  # 10m 단위를 미터로 변환
                        visibility_km = visibility_meters / 1000  # 미터를 km로 변환
                        
                        # 소수점 1자리까지
                        weather_info['current']['visibility'] = round(visibility_km, 1)
                        
                        # 디버깅을 위한 자세한 로깅
                        print(f"시정(VS) 처리중: 원본 값={fields[32]}, 계산 후={weather_info['current']['visibility']}km (타입: {type(weather_info['current']['visibility']).__name__})")
                        logger.info(f"시정 원본 VS 값: {fields[32]} (10m 단위)")
                        logger.info(f"시정 미터 변환: {visibility_meters}m")
                        logger.info(f"시정 최종 km 변환: {weather_info['current']['visibility']}km")
                        
                        # 실제 API 응답에 있는 값을 사용하여 바로 디버그 출력
                        logger.info(f"API 응답의 VS 값: {vs_value}, 변환 후: {round(visibility_km, 1)}km")

                        # 확인: weather_info에 visibility 값이 제대로 설정되었는지
                        print(f"weather_info visibility 설정 후: {weather_info['current']['visibility']} km")
                        
                    except (ValueError, IndexError) as e:
                        logger.error(f"시정 파싱 오류: {e}")
                        logger.error(f"시정 필드 값: {fields[32] if len(fields) > 32 else '인덱스 없음'}")
                        # 오류 발생 시 기본값 설정하지 않음
                        logger.info("시정 파싱 오류 발생, 값을 설정하지 않음")
                    else:
                        # 시정 정보가 없는 경우 확인 메시지
                        logger.warning("시정(VS) 정보가 없거나 결측 값입니다.")
                        logger.info("시정 정보 결측, 값을 설정하지 않음")
        except (ValueError, IndexError) as e:
            logger.error(f"데이터 파싱 오류: {e}")
            # 오류 발생해도 일부 데이터라도 표시할 수 있도록 함
        
        # 필드 값 로깅
        logger.info(f"파싱된 날씨 정보: {weather_info}")
        
        # 예보 정보 설정 (현재 데이터 기반)
        if weather_info['current']['temperature'] is not None:
            weather_info['forecast']['temperature'] = weather_info['current']['temperature'] + 1.5  # 가정
        if weather_info['current']['humidity'] is not None:
            weather_info['forecast']['humidity'] = weather_info['current']['humidity']
        if weather_info['current']['wind_speed'] is not None:
            weather_info['forecast']['wind_speed'] = weather_info['current']['wind_speed']
        
        return weather_info
        
    except Exception as e:
        logger.error(f"날씨 데이터 처리 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        return get_default_weather_info()

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
        response = requests.get(KMA_FORECAST_API_URL, params=params, timeout=10)
        
        # 응답 상태 확인
        if response.status_code != 200:
            logger.error(f"예보 API 요청 실패: {response.status_code} - {response.text}")
            return get_default_forecast_info()
        
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
        return get_default_forecast_info()

def get_default_forecast_info():
    """
    예보 API 연결 실패 시 기본 예보 정보를 제공합니다. 더 이상 더미 데이터를 사용하지 않습니다.
    """
    return {
        'precipitation_probability': 0,
        'sky_condition': None,
        'precipitation_type': '없음',
        'forecast_temperature': None
    }

def process_forecast_data(data):
    """
    기상청 단기예보 API 응답 데이터를 가공하여 필요한 정보만 추출합니다.
    fct_shrt_reg.php API의 응답 형식에 맞춰 처리합니다.
    """
    try:
        # 기본 예보 정보 구조 생성
        forecast_info = {
            'precipitation_probability': 20,  # 기본값 설정
            'sky_condition': '맑음',
            'precipitation_type': '없음',
            'forecast_temperature': 27.0
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
        
        # 주석이 아닌 데이터 라인 찾기
        data_lines = []
        header_indices = {}
        
        for line in lines:
            if not line.startswith('#') and line.strip() and (',' in line or '\t' in line):
                if ',' in line:
                    data_lines.append(line.split(','))
                else:
                    data_lines.append(line.split('\t'))
            elif line.startswith('#') and ('REG_ID' in line or 'ST' in line):
                # 헤더 라인에서 필요한 인덱스 찾기
                header_line = line.lstrip('#').strip()
                
                if ',' in header_line:
                    headers = [h.strip() for h in header_line.split(',')]
                else:
                    headers = header_line.split()
                
                for i, h in enumerate(headers):
                    if h == 'ST' or h == 'POP':
                        header_indices['precipitation_probability'] = i
                    elif h == 'SKY':
                        header_indices['sky_condition'] = i
                    elif h == 'PREP' or h == 'PTY':
                        header_indices['precipitation_type'] = i
                    elif h == 'TA' or h == 'T3H':
                        header_indices['temperature'] = i
        
        if not data_lines:
            logger.warning("예보 API 응답에서 데이터 라인을 찾을 수 없습니다.")
            return forecast_info
        
        # 첫 번째 데이터 라인 사용 (가장 가까운 예보)
        fields = data_lines[0]
        logger.info(f"예보 데이터 라인: {fields}")
        
        # 인덱스 매핑에 따라 값 추출
        if 'precipitation_probability' in header_indices and header_indices['precipitation_probability'] < len(fields):
            value = fields[header_indices['precipitation_probability']]
            if value != '-' and value.strip():
                try:
                    forecast_info['precipitation_probability'] = int(value)
                except ValueError:
                    logger.warning(f"강수확률 값을 정수로 변환할 수 없습니다: {value}")
        
        if 'sky_condition' in header_indices and header_indices['sky_condition'] < len(fields):
            sky_code = fields[header_indices['sky_condition']]
            sky_map = {
                'DB01': '맑음', '1': '맑음',
                'DB02': '구름조금', '2': '구름조금',
                'DB03': '구름많음', '3': '구름많음',
                'DB04': '흐림', '4': '흐림'
            }
            forecast_info['sky_condition'] = sky_map.get(sky_code, '맑음')
        
        if 'precipitation_type' in header_indices and header_indices['precipitation_type'] < len(fields):
            prep_code = fields[header_indices['precipitation_type']]
            prep_map = {
                '0': '없음',
                '1': '비',
                '2': '비/눈',
                '3': '눈',
                '4': '눈/비'
            }
            forecast_info['precipitation_type'] = prep_map.get(prep_code, '없음')
        
        if 'temperature' in header_indices and header_indices['temperature'] < len(fields):
            value = fields[header_indices['temperature']]
            if value != '-' and value.strip():
                try:
                    forecast_info['forecast_temperature'] = float(value)
                except ValueError:
                    logger.warning(f"기온 값을 숫자로 변환할 수 없습니다: {value}")
        
        return forecast_info
        
    except Exception as e:
        logger.error(f"예보 데이터 처리 오류: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        return get_default_forecast_info()

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
        logger.info(f"시정: {weather_info['current'].get('visibility')} km")
        
        # 시정 값 디버깅 추가
        print(f"데이터베이스 저장 전 시정 값: {weather_info['current'].get('visibility')} km (타입: {type(weather_info['current'].get('visibility')).__name__})")
        
        # 새 날씨 데이터 객체 생성
        weather_data = WeatherData(
            # 기본 정보
            location_name=weather_info['current'].get('location_name', '소각시도 (영광 관측소)'),
            
            # 현재 날씨 정보
            temperature=weather_info['current'].get('temperature'),
            feels_like=weather_info['current'].get('temperature'),  # 체감온도 - 현재는 실제 온도와 동일하게 설정
            humidity=weather_info['current'].get('humidity'),
            wind_speed=weather_info['current'].get('wind_speed'),
            wind_direction=weather_info['current'].get('wind_direction'),
            precipitation=weather_info['current'].get('precipitation', 0.0),
            precipitation_type=weather_info['current'].get('precipitation_type', '없음'),
            air_quality=weather_info['current'].get('air_quality', '보통'),
            visibility=weather_info['current'].get('visibility'),  # 시정 정보 추가
            
            # 예보 정보
            forecast_temperature=weather_info['forecast'].get('temperature'),
            forecast_humidity=weather_info['forecast'].get('humidity'),
            forecast_wind_speed=weather_info['forecast'].get('wind_speed'),
            forecast_precipitation=weather_info['forecast'].get('precipitation_probability'),  # 강수확률 저장
            forecast_air_quality=weather_info['forecast'].get('air_quality', '보통'),
        )
        
        # 날씨 데이터 객체 생성 후 visibility 값 확인
        print(f"WeatherData 객체 생성 후 시정 값: {weather_data.visibility} km")
        
        # 데이터베이스에 저장
        weather_data.save()
        logger.info(f"날씨 정보가 성공적으로 저장되었습니다. ID: {weather_data.id}")
        
        # 저장 후 데이터베이스에서 조회하여 확인
        saved_data = WeatherData.objects.get(id=weather_data.id)
        print(f"데이터베이스 저장 후 시정 값: {saved_data.visibility} km")
        
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
            else:
                # 업데이트 실패 시 더미 데이터 생성 (프론트엔드 테스트용)
                default_info = get_default_weather_info()
                latest_weather = WeatherData(
                    location_name='소각시도 (영광 관측소)',
                    temperature=default_info['current']['temperature'],
                    humidity=default_info['current']['humidity'],
                    wind_speed=default_info['current']['wind_speed'],
                    wind_direction=default_info['current']['wind_direction'],
                    precipitation=default_info['current']['precipitation'],
                    air_quality='보통',
                    forecast_temperature=default_info['forecast']['temperature'],
                    forecast_humidity=default_info['forecast']['humidity'],
                    forecast_wind_speed=default_info['forecast']['wind_speed'],
                    forecast_precipitation=default_info['forecast']['precipitation_probability'],
                    forecast_air_quality='보통',
                )
                # 더미 데이터는 저장하지 않음
        
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
            response = requests.get(file_url, timeout=30)
            
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
        response = requests.get(url, timeout=10)
        
        # 응답 상태 확인
        if response.status_code == 200:
            print(f"API 요청 성공 (상태 코드: {response.status_code})")
            
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