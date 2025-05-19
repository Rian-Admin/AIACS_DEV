import { Box, Grid, Typography, useTheme, Paper, CircularProgress, Divider, Alert } from '@mui/material';
import { translate } from '../../../utils/i18n';
import { 
  Thermostat as ThermostatIcon,
  Air as AirIcon,
  Opacity as OpacityIcon,
  Visibility as VisibilityIcon,
  Warning as WarningIcon,
  Speed as SpeedIcon,
  WbSunny as SunnyIcon,
  NightsStay as NightIcon,
  Cloud as CloudIcon,
  Thunderstorm as StormIcon,
  AcUnit as SnowIcon,
  Water as RainIcon,
  ErrorOutline as ErrorIcon
} from '@mui/icons-material';

const WeatherWidget = ({ weatherData, weatherLoading, language }) => {
  // 날씨 상태에 따른 아이콘 반환
  const getWeatherIcon = (weatherStatus, humidity, precipitation) => {
    // 백엔드에서 제공하는 날씨 상태에 따라 아이콘 반환
    if (weatherStatus) {
      switch (weatherStatus) {
        case '맑음':
          return <SunnyIcon sx={{ color: '#FFD54F' }} />;
        case '흐림':
          return <CloudIcon sx={{ color: '#B0BEC5' }} />;
        case '비':
          return <StormIcon sx={{ color: '#4FC3F7' }} />;
        case '눈':
          return <SnowIcon sx={{ color: '#E1F5FE' }} />;
        case '비/눈':
        case '눈/비':
          return <StormIcon sx={{ color: '#7E57C2' }} />;
        default:
          break;
      }
    }
    
    // 백엔드에서 상태 정보가 없는 경우 직접 판단
    if (precipitation > 0) {
      return <RainIcon sx={{ color: '#4FC3F7' }} />;
    } 
    if (humidity >= 80) {
      return <CloudIcon sx={{ color: '#B0BEC5' }} />;
    }
    return <SunnyIcon sx={{ color: '#FFD54F' }} />;
  };

  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 2, 
        borderRadius: 2, 
        background: 'linear-gradient(135deg, #0a1929 0%, #0d47a1 100%)',
        border: '1px solid #1e3a5a',
      }}
    >
      <Box display="flex" justifyContent="space-between" alignItems="flex-start">
        <Typography variant="h6" fontWeight="bold">
          {translate('현재 날씨', 'Current Weather', language)}
        </Typography>
        <Typography variant="caption" color="#4fc3f7">
          {weatherData?.current?.formatted_time || (weatherData?.timestamp ? new Date(weatherData.timestamp).toLocaleTimeString() : '--:--')}
        </Typography>
      </Box>
      
      {/* 위치 및 관측소 정보 */}
      <Typography variant="body2" color="#aaa" sx={{ mb: 2 }}>
        {weatherLoading 
          ? translate('데이터 로드 중...', 'Loading data...', language)
          : weatherData?.current?.observation_station || '소각시도 (영광 관측소)'
        }
      </Typography>
      
      {weatherLoading ? (
        <Box display="flex" justifyContent="center" alignItems="center" py={2}>
          <CircularProgress size={30} />
        </Box>
      ) : weatherData === null ? (
        <Alert 
          severity="info" 
          icon={<ErrorIcon />}
          sx={{ 
            backgroundColor: 'rgba(13, 71, 161, 0.15)', 
            color: '#90CAF9',
            border: '1px solid rgba(13, 71, 161, 0.3)'
          }}
        >
          {translate(
            '날씨 데이터를 가져올 수 없습니다. 기상청 API에 연결할 수 없습니다.',
            'Unable to fetch weather data. Cannot connect to weather API.',
            language
          )}
        </Alert>
      ) : (
        <>
          <Box display="flex" justifyContent="space-between" alignItems="center">
            <Box>
              <Typography variant="h3">
                {weatherData?.current?.temperature !== null ? `${weatherData.current.temperature.toFixed(1)}°` : '--°'}
              </Typography>
            </Box>
            <Box sx={{ fontSize: '50px' }}>
              {getWeatherIcon(
                weatherData?.current?.weather_status,
                weatherData?.current?.humidity || 0,
                weatherData?.current?.precipitation || 0
              )}
            </Box>
          </Box>
          
          <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
          
          <Grid container spacing={1}>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <OpacityIcon sx={{ mr: 1, color: '#64B5F6' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('습도', 'Humidity', language)}: {weatherData?.current?.humidity !== null ? `${weatherData.current.humidity}%` : '--'}
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <AirIcon sx={{ mr: 1, color: '#81D4FA' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('풍속', 'Wind', language)}: {weatherData?.current?.wind_speed !== null ? `${weatherData.current.wind_speed} m/s` : '--'}
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <VisibilityIcon sx={{ mr: 1, color: '#B3E5FC' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('시정', 'Visibility', language)}: {weatherData?.current?.visibility !== null ? `${weatherData.current.visibility} km` : '--'}
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <RainIcon sx={{ mr: 1, color: '#4FC3F7' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('강수량', 'Precip.', language)}: {weatherData?.current?.precipitation !== null ? `${weatherData.current.precipitation} mm` : '0 mm'}
                </Typography>
              </Box>
            </Grid>
          </Grid>
        </>
      )}
    </Paper>
  );
};

export default WeatherWidget; 