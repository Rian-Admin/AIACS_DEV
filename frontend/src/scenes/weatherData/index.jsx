import React from 'react';
import { Box, Typography, Paper, Grid, Alert, Button } from '@mui/material';
import { translate } from '../../utils/i18n';
import useAppStore from '../../store/useAppStore';
import { useNavigate } from 'react-router-dom';

const WeatherData = ({ language }) => {
  const navigate = useNavigate();
  const { weatherEnabled } = useAppStore();
  
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
  
  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h4" sx={{ mb: 3 }}>
        {translate('기상 정보', 'Weather Data', language)}
      </Typography>
      
      <Paper
        sx={{
          p: 3,
          backgroundColor: '#0a1929',
          border: '1px solid #1e3a5a',
          borderRadius: 2
        }}
      >
        <Typography variant="body1">
          {translate('기상 정보 컴포넌트가 여기에 구현될 예정입니다.', 
                   'Weather data component will be implemented here.', 
                   language)}
        </Typography>
      </Paper>
    </Box>
  );
};

export default WeatherData; 