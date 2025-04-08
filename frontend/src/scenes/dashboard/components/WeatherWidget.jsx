import { Box, Grid, Typography, useTheme, Paper, CircularProgress, Divider } from '@mui/material';
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
  Water as RainIcon
} from '@mui/icons-material';

const WeatherWidget = ({ weatherData, weatherLoading, language }) => {
  // 날씨 상태에 따른 아이콘 반환
  const getWeatherIcon = (condition) => {
    switch (condition) {
      case 'clear':
        return <SunnyIcon sx={{ color: '#FFD54F' }} />;
      case 'night':
        return <NightIcon sx={{ color: '#90CAF9' }} />;
      case 'cloudy':
        return <CloudIcon sx={{ color: '#B0BEC5' }} />;
      case 'storm':
        return <StormIcon sx={{ color: '#7E57C2' }} />;
      case 'snow':
        return <SnowIcon sx={{ color: '#E1F5FE' }} />;
      case 'rain':
        return <RainIcon sx={{ color: '#4FC3F7' }} />;
      default:
        return <SunnyIcon sx={{ color: '#FFD54F' }} />;
    }
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
          {weatherData?.timestamp ? new Date(weatherData.timestamp).toLocaleTimeString() : '--:--'}
        </Typography>
      </Box>
      
      {/* 위치 정보 */}
      <Typography variant="body2" color="#aaa" sx={{ mb: 2 }}>
        {weatherData?.location || translate('데이터 로드 중...', 'Loading data...', language)}
      </Typography>
      
      {weatherLoading ? (
        <Box display="flex" justifyContent="center" alignItems="center" py={2}>
          <CircularProgress size={30} />
        </Box>
      ) : (
        <>
          <Box display="flex" justifyContent="space-between" alignItems="center">
            <Box>
              <Typography variant="h3">
                {weatherData?.current?.temperature ? `${weatherData.current.temperature.toFixed(1)}°` : '--°'}
              </Typography>
              <Typography variant="body2" color="#aaa">
                {translate('체감온도', 'Feels like', language)}: {weatherData?.current?.feels_like ? `${weatherData.current.feels_like.toFixed(1)}°` : '--°'}
              </Typography>
            </Box>
            <Box sx={{ fontSize: '50px' }}>
              {getWeatherIcon(weatherData?.current?.weather_condition)}
            </Box>
          </Box>
          
          <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
          
          <Grid container spacing={1}>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <OpacityIcon sx={{ mr: 1, color: '#64B5F6' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('습도', 'Humidity', language)}: {weatherData?.current?.humidity || '--'}%
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <AirIcon sx={{ mr: 1, color: '#81D4FA' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('풍속', 'Wind', language)}: {weatherData?.current?.wind_speed || '--'} m/s
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <VisibilityIcon sx={{ mr: 1, color: '#B3E5FC' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('시정', 'Visibility', language)}: {weatherData?.current?.visibility || '--'} km
                </Typography>
              </Box>
            </Grid>
            <Grid item xs={6}>
              <Box display="flex" alignItems="center">
                <RainIcon sx={{ mr: 1, color: '#4FC3F7' }} fontSize="small" />
                <Typography variant="body2">
                  {translate('강수량', 'Precip.', language)}: {weatherData?.current?.precipitation || '0'} mm
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