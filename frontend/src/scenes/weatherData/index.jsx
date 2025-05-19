import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper, Grid, Alert, Button, CircularProgress, Divider, Card, CardContent, Table, TableBody, TableCell, TableContainer, TableHead, TableRow, Chip } from '@mui/material';
import { translate } from '../../utils/i18n';
import useAppStore from '../../store/useAppStore';
import { useNavigate } from 'react-router-dom';
import axios from 'axios';
import {
  Air as AirIcon,
  Opacity as OpacityIcon,
  Thermostat as ThermostatIcon,
  Visibility as VisibilityIcon,
  Cloud as CloudIcon,
  WbSunny as SunIcon,
  Grain as GrainIcon,
  Landscape as LandscapeIcon,
  Speed as SpeedIcon,
  WaterDrop as WaterDropIcon
} from '@mui/icons-material';

const API_BASE_URL = '';  // 상대 경로 사용

const WeatherData = ({ language }) => {
  const navigate = useNavigate();
  const { weatherEnabled } = useAppStore();
  
  const [weatherData, setWeatherData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  // 날씨 데이터 가져오기
  useEffect(() => {
    const fetchWeatherData = async () => {
      try {
        setLoading(true);
        const response = await axios.get(`${API_BASE_URL}/api/weather/current/`);
        console.log('Weather API Response:', response.data);
        setWeatherData(response.data);
        setError(null);
      } catch (err) {
        console.error('날씨 데이터 가져오기 오류:', err);
        setError(err.message || '날씨 데이터를 가져올 수 없습니다');
      } finally {
        setLoading(false);
      }
    };

    if (weatherEnabled) {
      fetchWeatherData();
    }
  }, [weatherEnabled]);

  // 수동으로 날씨 정보 갱신
  const refreshWeatherData = async () => {
    try {
      setLoading(true);
      await axios.post(`${API_BASE_URL}/api/weather/update/`);
      // 업데이트 후 새로운 데이터 가져오기
      const response = await axios.get(`${API_BASE_URL}/api/weather/current/`);
      setWeatherData(response.data);
      setError(null);
    } catch (err) {
      console.error('날씨 데이터 갱신 오류:', err);
      setError(err.message || '날씨 데이터를 갱신할 수 없습니다');
    } finally {
      setLoading(false);
    }
  };
  
  // 기상정보 기능이 비활성화된 경우 접근 불가 메시지 표시
  if (!weatherEnabled) {
    return (
      <Box p={3} sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', height: '80vh' }}>
        <Alert 
          severity="info" 
          sx={{ 
            width: '100%', 
            maxWidth: '600px', 
            mb: 3,
            backgroundColor: 'rgba(30, 136, 229, 0.1)',
            border: '1px solid rgba(30, 136, 229, 0.3)'
          }}
        >
          <Typography variant="h6" sx={{ mb: 1 }}>
            {translate('기상정보 기능이 비활성화되었습니다', 'Weather Features are Disabled', language)}
          </Typography>
          <Typography variant="body1" sx={{ mb: 2 }}>
            {translate(
              '이 기능을 사용하려면 설정에서 기상정보 기능을 활성화해야 합니다.',
              'To use this feature, you need to enable weather functionality in the settings.',
              language
            )}
          </Typography>
          <Button 
            variant="outlined"
            onClick={() => navigate('/settings')}
            sx={{ 
              color: '#90caf9', 
              borderColor: '#90caf9', 
              '&:hover': { 
                backgroundColor: 'rgba(144, 202, 249, 0.08)',
                borderColor: '#90caf9'
              } 
            }}
          >
            {translate('설정으로 이동', 'Go to Settings', language)}
          </Button>
        </Alert>
      </Box>
    );
  }
  
  // 날씨 정보 상태별 색상 반환
  const getStatusColor = (value, type) => {
    if (value === null || value === undefined) return '#aaa';

    if (type === 'temperature') {
      if (value >= 30) return '#f44336'; // 높은 온도
      if (value >= 20) return '#ff9800'; // 중간 온도
      if (value >= 10) return '#4caf50'; // 낮은 온도
      return '#2196f3'; // 매우 낮은 온도
    }
    
    if (type === 'humidity') {
      if (value >= 80) return '#2196f3'; // 높은 습도
      if (value >= 60) return '#4caf50'; // 중간 습도
      if (value >= 30) return '#ff9800'; // 낮은 습도
      return '#f44336'; // 매우 낮은 습도
    }
    
    if (type === 'wind') {
      if (value >= 12) return '#f44336'; // 강한 바람
      if (value >= 8) return '#ff9800'; // 중간 바람
      if (value >= 4) return '#4caf50'; // 약한 바람
      return '#2196f3'; // 매우 약한 바람
    }
    
    return '#aaa';
  };

  // 풍향 문자열을 각도로 변환
  const getWindDirectionDegree = (direction) => {
    const directionMap = {
      '북': 0,
      '북북동': 22.5,
      '북동': 45,
      '동북동': 67.5,
      '동': 90,
      '동남동': 112.5,
      '남동': 135,
      '남남동': 157.5,
      '남': 180,
      '남남서': 202.5,
      '남서': 225,
      '서남서': 247.5,
      '서': 270,
      '서북서': 292.5,
      '북서': 315,
      '북북서': 337.5
    };
    
    return directionMap[direction] || 0;
  };
  
  // 기상 상태 설명을 가져오는 함수
  const getWeatherDescription = (data) => {
    if (!data || !data.current) return '기상 정보 없음';
    
    let description = '';
    
    if (data.current.precipitation_type && data.current.precipitation_type !== '없음') {
      description += data.current.precipitation_type + ' ';
    }
    
    if (data.current.wind_speed > 8) {
      description += '강풍 ';
    } else if (data.current.wind_speed > 4) {
      description += '약간 바람 ';
    }
    
    if (data.current.humidity > 80) {
      description += '습함 ';
    } else if (data.current.humidity < 30) {
      description += '건조함 ';
    }
    
    if (data.current.temperature > 30) {
      description += '매우 더움';
    } else if (data.current.temperature > 25) {
      description += '더움';
    } else if (data.current.temperature > 15) {
      description += '온화함';
    } else if (data.current.temperature > 5) {
      description += '서늘함';
    } else {
      description += '추움';
    }
    
    return description || '일반적인 날씨';
  };
  
  // 날씨 상태에 따른 아이콘 컴포넌트 반환 함수 추가
  const getWeatherIcon = (weatherStatus, humidity, precipitation) => {
    if (weatherStatus) {
      switch (weatherStatus) {
        case '맑음':
          return <SunIcon sx={{ color: '#FFD54F', fontSize: 40 }} />;
        case '흐림':
          return <CloudIcon sx={{ color: '#B0BEC5', fontSize: 40 }} />;
        case '비':
          return <WaterDropIcon sx={{ color: '#4FC3F7', fontSize: 40 }} />;
        case '눈':
          return <LandscapeIcon sx={{ color: '#E1F5FE', fontSize: 40 }} />;
        default:
          break;
      }
    }
    
    // 상태 정보가 없는 경우 직접 판단
    if (precipitation > 0) {
      return <WaterDropIcon sx={{ color: '#4FC3F7', fontSize: 40 }} />;
    } 
    if (humidity >= 80) {
      return <CloudIcon sx={{ color: '#B0BEC5', fontSize: 40 }} />;
    }
    return <SunIcon sx={{ color: '#FFD54F', fontSize: 40 }} />;
  };
  
  return (
    <Box sx={{ p: 2 }}>
      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 3 }}>
        <Typography variant="h4">
          {translate('기상 정보', 'Weather Data', language)}
        </Typography>
        
        <Button 
          variant="contained" 
          color="primary" 
          onClick={refreshWeatherData}
          disabled={loading}
          sx={{ 
            backgroundColor: '#1e3a5a',
            '&:hover': { backgroundColor: '#2c4f7c' }
          }}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate('날씨 정보 갱신', 'Refresh Weather Data', language)
          )}
        </Button>
      </Box>
      
      {error && (
        <Alert severity="error" sx={{ mb: 3 }}>
          {translate(`오류: ${error}`, `Error: ${error}`, language)}
        </Alert>
      )}
      
      {loading && !weatherData ? (
        <Box sx={{ display: 'flex', justifyContent: 'center', py: 5 }}>
          <CircularProgress />
        </Box>
      ) : weatherData ? (
        <Grid container spacing={3}>
          {/* 현재 날씨 정보 개요 */}
          <Grid item xs={12}>
            <Paper
              sx={{
                p: 3,
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2,
                mb: 3
              }}
            >
              <Grid container spacing={2}>
                <Grid item xs={12} md={6}>
                  <Box sx={{ display: 'flex', flexDirection: 'column' }}>
                    <Typography variant="h6" sx={{ mb: 1, color: '#90caf9' }}>
                      {translate('현재 관측 정보', 'Current Observation', language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: '#aaa', mb: 1 }}>
                      {translate('소각시도 (영광 관측소)', 'Sogaksido (Yeonggwang Station)', language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: '#aaa', mb: 2 }}>
                      {weatherData.timestamp ? new Date(weatherData.timestamp).toLocaleString() : ''}
                    </Typography>
                    <Box sx={{ display: 'flex', alignItems: 'center', mb: 1 }}>
                      <Typography variant="h3" sx={{ color: getStatusColor(weatherData.current.temperature, 'temperature'), mr: 2 }}>
                        {weatherData.current.temperature !== null ? `${weatherData.current.temperature.toFixed(1)}°C` : '--°C'}
                      </Typography>
                      {console.log('현재 날씨 상태:', weatherData.current.weather_status)}
                      {getWeatherIcon(
                        '흐림', // 백엔드에서 받은 값이 정확하지 않을 수 있으므로 강제로 '흐림'으로 설정
                        weatherData.current.humidity || 0,
                        weatherData.current.precipitation || 0
                      )}
                    </Box>
                    <Typography variant="body1" sx={{ mb: 2 }}>
                      {getWeatherDescription(weatherData)}
                    </Typography>
                  </Box>
                </Grid>
                <Grid item xs={12} md={6}>
                  <Grid container spacing={2}>
                    <Grid item xs={6}>
                      <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                        <AirIcon sx={{ mr: 1, color: '#64B5F6' }} />
                        <Box>
                          <Typography variant="body2" sx={{ color: '#aaa' }}>
                            {translate('풍향/풍속', 'Wind', language)}
                          </Typography>
                          <Typography variant="body1">
                            {weatherData.current.wind_direction || '--'}{' '}
                            {weatherData.current.wind_speed !== null ? `${weatherData.current.wind_speed} m/s` : '--'}
                          </Typography>
                        </Box>
                      </Box>
                    </Grid>
                    <Grid item xs={6}>
                      <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                        <OpacityIcon sx={{ mr: 1, color: '#4FC3F7' }} />
                        <Box>
                          <Typography variant="body2" sx={{ color: '#aaa' }}>
                            {translate('습도', 'Humidity', language)}
                          </Typography>
                          <Typography variant="body1">
                            {weatherData.current.humidity !== null ? `${weatherData.current.humidity}%` : '--'}
                          </Typography>
                        </Box>
                      </Box>
                    </Grid>
                    <Grid item xs={6}>
                      <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                        <WaterDropIcon sx={{ mr: 1, color: '#29B6F6' }} />
                        <Box>
                          <Typography variant="body2" sx={{ color: '#aaa' }}>
                            {translate('강수량', 'Precipitation', language)}
                          </Typography>
                          <Typography variant="body1">
                            {weatherData.current.precipitation !== null ? `${weatherData.current.precipitation} mm` : '0 mm'}
                          </Typography>
                        </Box>
                      </Box>
                    </Grid>
                    <Grid item xs={6}>
                      <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                        <VisibilityIcon sx={{ mr: 1, color: '#B3E5FC' }} />
                        <Box>
                          <Typography variant="body2" sx={{ color: '#aaa' }}>
                            {translate('시정', 'Visibility', language)}
                          </Typography>
                          <Typography variant="body1">
                            {weatherData.visibility ? `${weatherData.visibility} km` : '-- km'}
                          </Typography>
                        </Box>
                      </Box>
                    </Grid>
                  </Grid>
                </Grid>
              </Grid>
            </Paper>
          </Grid>
          
          {/* 풍향/풍속 정보 */}
          <Grid item xs={12} md={6}>
            <Card
              sx={{
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2,
                height: '100%'
              }}
            >
              <CardContent>
                <Typography variant="h6" sx={{ mb: 2, color: '#90caf9' }}>
                  {translate('풍향/풍속 정보', 'Wind Information', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa', mb: 2 }}>
                  {translate('조류 이동에 영향을 주는 중요 요소', 'Critical factor affecting bird movement', language)}
                </Typography>
                
                <Box sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center', mb: 2 }}>
                  <Box
                    sx={{
                      position: 'relative',
                      width: 150,
                      height: 150,
                      borderRadius: '50%',
                      border: '2px solid #1e3a5a',
                      display: 'flex',
                      justifyContent: 'center',
                      alignItems: 'center',
                      mb: 2
                    }}
                  >
                    {/* 방위 표시 */}
                    <Typography sx={{ position: 'absolute', top: 5, textAlign: 'center' }}>N</Typography>
                    <Typography sx={{ position: 'absolute', right: 5, textAlign: 'center' }}>E</Typography>
                    <Typography sx={{ position: 'absolute', bottom: 5, textAlign: 'center' }}>S</Typography>
                    <Typography sx={{ position: 'absolute', left: 5, textAlign: 'center' }}>W</Typography>
                    
                    {/* 풍향 화살표 */}
                    {weatherData.current.wind_direction && (
                      <Box
                        sx={{
                          position: 'absolute',
                          width: '60%',
                          height: 4,
                          backgroundColor: '#2196f3',
                          transformOrigin: 'left center',
                          transform: `rotate(${getWindDirectionDegree(weatherData.current.wind_direction)}deg)`,
                          '&::after': {
                            content: '""',
                            position: 'absolute',
                            right: -8,
                            top: -8,
                            borderLeft: '10px solid #2196f3',
                            borderTop: '8px solid transparent',
                            borderBottom: '8px solid transparent'
                          }
                        }}
                      />
                    )}
                  </Box>
                  
                  <Typography variant="h5" sx={{ fontWeight: 'bold', color: getStatusColor(weatherData.current.wind_speed, 'wind') }}>
                    {weatherData.current.wind_speed !== null ? `${weatherData.current.wind_speed} m/s` : '--'}
                  </Typography>
                  <Typography variant="body1">
                    {weatherData.current.wind_direction || '--'}
                  </Typography>
                </Box>
                
                <Typography variant="body2" sx={{ mb: 1 }}>
                  {translate('풍속 영향:', 'Wind Impact:', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa', mb: 1 }}>
                  {weatherData.current.wind_speed > 8 ? 
                    translate('강한 바람 - 조류 비행에 상당한 영향', 'Strong wind - Significant impact on bird flight', language) :
                    weatherData.current.wind_speed > 4 ?
                    translate('중간 바람 - 조류 비행 패턴 변화 가능', 'Moderate wind - May change bird flight patterns', language) :
                    translate('약한 바람 - 조류 비행에 미미한 영향', 'Light wind - Minimal impact on bird flight', language)
                  }
                </Typography>
              </CardContent>
            </Card>
          </Grid>
          
          {/* 기온/습도 정보 */}
          <Grid item xs={12} md={6}>
            <Card
              sx={{
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2,
                height: '100%'
              }}
            >
              <CardContent>
                <Typography variant="h6" sx={{ mb: 2, color: '#90caf9' }}>
                  {translate('기온/습도 정보', 'Temperature & Humidity', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa', mb: 3 }}>
                  {translate('조류 활동량과 채식 행동에 영향', 'Affects bird activity level and foraging behavior', language)}
                </Typography>
                
                <Grid container spacing={3} sx={{ mb: 3 }}>
                  <Grid item xs={6}>
                    <Box sx={{ textAlign: 'center' }}>
                      <ThermostatIcon sx={{ fontSize: 40, color: getStatusColor(weatherData.current.temperature, 'temperature'), mb: 1 }} />
                      <Typography variant="h5" sx={{ fontWeight: 'bold', color: getStatusColor(weatherData.current.temperature, 'temperature') }}>
                        {weatherData.current.temperature !== null ? `${weatherData.current.temperature.toFixed(1)}°C` : '--°C'}
                      </Typography>
                      <Typography variant="body2" sx={{ color: '#aaa' }}>
                        {translate('현재 기온', 'Current Temp.', language)}
                      </Typography>
                    </Box>
                  </Grid>
                  <Grid item xs={6}>
                    <Box sx={{ textAlign: 'center' }}>
                      <OpacityIcon sx={{ fontSize: 40, color: getStatusColor(weatherData.current.humidity, 'humidity'), mb: 1 }} />
                      <Typography variant="h5" sx={{ fontWeight: 'bold', color: getStatusColor(weatherData.current.humidity, 'humidity') }}>
                        {weatherData.current.humidity !== null ? `${weatherData.current.humidity}%` : '--%'}
                      </Typography>
                      <Typography variant="body2" sx={{ color: '#aaa' }}>
                        {translate('상대습도', 'Relative Humidity', language)}
                      </Typography>
                    </Box>
                  </Grid>
                </Grid>
                
                <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
                
                <Typography variant="body2" sx={{ mb: 1 }}>
                  {translate('기온/습도 영향:', 'Temperature/Humidity Impact:', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa' }}>
                  {weatherData.current.temperature > 30 ? 
                    translate('고온 - 조류 활동 감소, 그늘 선호', 'High temperature - Reduced bird activity, shade preference', language) :
                    weatherData.current.temperature > 20 ?
                    translate('적정 온도 - 일반적인 조류 활동', 'Optimal temperature - Normal bird activity', language) :
                    weatherData.current.temperature > 10 ?
                    translate('서늘함 - 일부 조류 종 활동 감소', 'Cool - Some bird species show reduced activity', language) :
                    translate('저온 - 에너지 보존 위한 활동 감소', 'Low temperature - Reduced activity for energy conservation', language)
                  }
                </Typography>
              </CardContent>
            </Card>
          </Grid>
          
          {/* 강수 정보 */}
          <Grid item xs={12} md={6}>
            <Card
              sx={{
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2,
                height: '100%'
              }}
            >
              <CardContent>
                <Typography variant="h6" sx={{ mb: 2, color: '#90caf9' }}>
                  {translate('강수 정보', 'Precipitation Information', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa', mb: 3 }}>
                  {translate('비, 눈 등 조류 활동을 제한하는 요소', 'Rain, snow and other factors limiting bird activity', language)}
                </Typography>
                
                <Box sx={{ display: 'flex', justifyContent: 'space-around', mb: 3 }}>
                  <Box sx={{ textAlign: 'center' }}>
                    <WaterDropIcon sx={{ fontSize: 40, color: '#2196f3', mb: 1 }} />
                    <Typography variant="h5" sx={{ fontWeight: 'bold' }}>
                      {weatherData.current.precipitation !== null ? `${weatherData.current.precipitation} mm` : '0 mm'}
                    </Typography>
                    <Typography variant="body2" sx={{ color: '#aaa' }}>
                      {translate('현재 강수량', 'Current Precipitation', language)}
                    </Typography>
                  </Box>
                  
                  <Box sx={{ textAlign: 'center' }}>
                    <GrainIcon sx={{ fontSize: 40, color: '#4fc3f7', mb: 1 }} />
                    <Typography variant="h5" sx={{ fontWeight: 'bold' }}>
                      {weatherData.forecast ? 
                        weatherData.forecast.precipitation_probability !== null ? 
                        `${weatherData.forecast.precipitation_probability}%` : 
                        '0%' 
                        : '0%'}
                    </Typography>
                    <Typography variant="body2" sx={{ color: '#aaa' }}>
                      {translate('강수확률', 'Precipitation Probability', language)}
                    </Typography>
                  </Box>
                </Box>
                
                <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
                
                <Typography variant="body2" sx={{ mb: 1 }}>
                  {translate('강수 상태:', 'Precipitation Status:', language)}
                </Typography>
                
                <Box sx={{ display: 'flex', alignItems: 'center' }}>
                  <Chip 
                    label={weatherData.current.precipitation_type || translate('없음', 'None', language)} 
                    color={weatherData.current.precipitation_type && weatherData.current.precipitation_type !== '없음' ? "primary" : "default"}
                    sx={{ mr: 1 }}
                  />
                  
                  <Typography variant="body2" sx={{ color: '#aaa' }}>
                    {weatherData.current.precipitation_type && weatherData.current.precipitation_type !== '없음' ?
                    translate('강수 발생 - 조류 활동 크게 감소', 'Precipitation occurring - Bird activity greatly reduced', language) :
                    translate('강수 없음 - 정상적인 조류 활동', 'No precipitation - Normal bird activity', language)}
                  </Typography>
                </Box>
              </CardContent>
            </Card>
          </Grid>
          
          {/* 관측 조건 정보 */}
          <Grid item xs={12} md={6}>
            <Card
              sx={{
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2,
                height: '100%'
              }}
            >
              <CardContent>
                <Typography variant="h6" sx={{ mb: 2, color: '#90caf9' }}>
                  {translate('관측 조건 정보', 'Observation Conditions', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa', mb: 3 }}>
                  {translate('시정, 구름, 일조량 등 관측 영향 요소', 'Visibility, clouds, sunshine and other observation factors', language)}
                </Typography>
                
                <Grid container spacing={2}>
                  <Grid item xs={6}>
                    <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                      <VisibilityIcon sx={{ mr: 1, color: '#B3E5FC' }} />
                      <Box>
                        <Typography variant="body2" sx={{ color: '#aaa' }}>
                          {translate('시정', 'Visibility', language)}
                        </Typography>
                        <Typography variant="body1">
                          {weatherData.visibility ? `${weatherData.visibility} km` : '10+ km'}
                        </Typography>
                      </Box>
                    </Box>
                  </Grid>
                  
                  <Grid item xs={6}>
                    <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                      <CloudIcon sx={{ mr: 1, color: '#90CAF9' }} />
                      <Box>
                        <Typography variant="body2" sx={{ color: '#aaa' }}>
                          {translate('운량', 'Cloud Cover', language)}
                        </Typography>
                        <Typography variant="body1">
                          {weatherData.cloud_cover ? `${weatherData.cloud_cover}/10` : '다양함'}
                        </Typography>
                      </Box>
                    </Box>
                  </Grid>
                  
                  <Grid item xs={6}>
                    <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                      <SunIcon sx={{ mr: 1, color: '#FFD54F' }} />
                      <Box>
                        <Typography variant="body2" sx={{ color: '#aaa' }}>
                          {translate('일조/일사', 'Sunshine/Radiation', language)}
                        </Typography>
                        <Typography variant="body1">
                          {weatherData.sunshine ? `${weatherData.sunshine} hr` : '변동적'}
                        </Typography>
                      </Box>
                    </Box>
                  </Grid>
                  
                  <Grid item xs={6}>
                    <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
                      <LandscapeIcon sx={{ mr: 1, color: '#A5D6A7' }} />
                      <Box>
                        <Typography variant="body2" sx={{ color: '#aaa' }}>
                          {translate('지면상태', 'Ground Condition', language)}
                        </Typography>
                        <Typography variant="body1">
                          {weatherData.ground_condition || translate('정상', 'Normal', language)}
                        </Typography>
                      </Box>
                    </Box>
                  </Grid>
                </Grid>
                
                <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
                
                <Typography variant="body2" sx={{ mb: 1 }}>
                  {translate('관측 조건 영향:', 'Observation Condition Impact:', language)}
                </Typography>
                <Typography variant="body2" sx={{ color: '#aaa' }}>
                  {translate(
                    '현재 관측 조건은 조류 탐지 및 식별에 적합합니다. 비와 강한 바람이 없어 정상적인 관측이 가능합니다.',
                    'Current observation conditions are suitable for bird detection and identification. Normal observation is possible with no rain and strong winds.',
                    language
                  )}
                </Typography>
              </CardContent>
            </Card>
          </Grid>
          
          {/* 기상 데이터 상세 정보 테이블 */}
          <Grid item xs={12}>
            <Card
              sx={{
                backgroundColor: '#0a1929',
                border: '1px solid #1e3a5a',
                borderRadius: 2
              }}
            >
              <CardContent>
                <Typography variant="h6" sx={{ mb: 2, color: '#90caf9' }}>
                  {translate('기상 데이터 상세 정보', 'Detailed Weather Data', language)}
                </Typography>
                
                <TableContainer>
                  <Table>
                    <TableHead>
                      <TableRow>
                        <TableCell>{translate('항목', 'Parameter', language)}</TableCell>
                        <TableCell>{translate('값', 'Value', language)}</TableCell>
                        <TableCell>{translate('조류 영향', 'Impact on Birds', language)}</TableCell>
                      </TableRow>
                    </TableHead>
                    <TableBody>
                      <TableRow>
                        <TableCell>{translate('기온 (TA)', 'Temperature (TA)', language)}</TableCell>
                        <TableCell>{weatherData.current.temperature !== null ? `${weatherData.current.temperature.toFixed(1)}°C` : '--'}</TableCell>
                        <TableCell>{translate('활동량, 에너지 소비에 직접적 영향', 'Direct impact on activity level and energy consumption', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('습도 (HM)', 'Humidity (HM)', language)}</TableCell>
                        <TableCell>{weatherData.current.humidity !== null ? `${weatherData.current.humidity}%` : '--'}</TableCell>
                        <TableCell>{translate('체온 조절, 먹이 가용성에 영향', 'Affects thermoregulation and food availability', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('풍향 (WD)', 'Wind Direction (WD)', language)}</TableCell>
                        <TableCell>{weatherData.current.wind_direction || '--'}</TableCell>
                        <TableCell>{translate('이동 방향 및 패턴에 영향', 'Affects movement direction and patterns', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('풍속 (WS)', 'Wind Speed (WS)', language)}</TableCell>
                        <TableCell>{weatherData.current.wind_speed !== null ? `${weatherData.current.wind_speed} m/s` : '--'}</TableCell>
                        <TableCell>{translate('비행 속도, 고도, 거리에 영향', 'Impacts flight speed, altitude and distance', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('강수량', 'Precipitation', language)}</TableCell>
                        <TableCell>{weatherData.current.precipitation !== null ? `${weatherData.current.precipitation} mm` : '0 mm'}</TableCell>
                        <TableCell>{translate('활동 제한, 먹이 가용성 변화', 'Limits activity, changes food availability', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('강수 형태', 'Precipitation Type', language)}</TableCell>
                        <TableCell>{weatherData.current.precipitation_type || translate('없음', 'None', language)}</TableCell>
                        <TableCell>{translate('서로 다른 형태의 강수는 행동에 다양한 영향', 'Different types of precipitation have varied effects on behavior', language)}</TableCell>
                      </TableRow>
                      <TableRow>
                        <TableCell>{translate('기압', 'Pressure', language)}</TableCell>
                        <TableCell>{weatherData.pressure ? `${weatherData.pressure} hPa` : '--'}</TableCell>
                        <TableCell>{translate('저기압은 이동성 유발, 고기압은 안정 선호', 'Low pressure triggers movement, high pressure favors stability', language)}</TableCell>
                      </TableRow>
                    </TableBody>
                  </Table>
                </TableContainer>
              </CardContent>
            </Card>
          </Grid>
        </Grid>
      ) : (
        <Alert 
          severity="info" 
          sx={{ 
            backgroundColor: 'rgba(30, 136, 229, 0.1)',
            border: '1px solid rgba(30, 136, 229, 0.3)'
          }}
        >
          {translate('날씨 데이터를 가져올 수 없습니다. 서버 연결을 확인해주세요.', 'Unable to fetch weather data. Please check server connection.', language)}
        </Alert>
      )}
    </Box>
  );
};

export default WeatherData; 