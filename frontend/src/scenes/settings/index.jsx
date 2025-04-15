import React from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Grid, 
  FormControlLabel, 
  Switch,
  Divider,
  Card,
  CardContent,
  CardHeader,
  Alert
} from '@mui/material';
import { translate } from '../../utils/i18n';
import useAppStore from '../../store/useAppStore';

const Settings = ({ language }) => {
  // 전역 상태 가져오기
  const { 
    radarEnabled, 
    setRadarEnabled,
    weatherEnabled,
    setWeatherEnabled,
    alertSoundEnabled,
    setAlertSoundEnabled
  } = useAppStore();

  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h4" sx={{ mb: 3 }}>
        {translate('설정', 'Settings', language)}
      </Typography>
      
      <Grid container spacing={3}>
        {/* 시스템 설정 카드 */}
        <Grid item xs={12} md={6}>
          <Card sx={{ 
            backgroundColor: '#0a1929', 
            border: '1px solid #1e3a5a',
            borderRadius: 2
          }}>
            <CardHeader 
              title={translate('시스템 설정', 'System Settings', language)}
              sx={{ borderBottom: '1px solid #1e3a5a', p: 2 }}
            />
            <CardContent sx={{ p: 3 }}>
              {/* 레이더 활성화 토글 */}
              <Box sx={{ mb: 3 }}>
                <FormControlLabel
                  control={
                    <Switch
                      checked={radarEnabled}
                      onChange={(e) => setRadarEnabled(e.target.checked)}
                      color="primary"
                    />
                  }
                  label={translate('레이더 기능 사용', 'Enable Radar Features', language)}
                />
                <Typography variant="caption" sx={{ display: 'block', color: '#aaa', mt: 0.5, ml: 2 }}>
                  {translate(
                    '레이더 관련 기능을 모두 활성화/비활성화합니다', 
                    'Enable or disable all radar-related features', 
                    language
                  )}
                </Typography>
                
                {!radarEnabled && (
                  <Alert severity="info" sx={{ mt: 1, mb: 1 }}>
                    {translate(
                      '레이더 기능이 비활성화되었습니다. 레이더 관련 탭과 기능이 표시되지 않습니다.',
                      'Radar features are disabled. Radar-related tabs and functions will not be displayed.',
                      language
                    )}
                  </Alert>
                )}
              </Box>
              
              {/* 기상정보 활성화 토글 */}
              <Box sx={{ mb: 3 }}>
                <FormControlLabel
                  control={
                    <Switch
                      checked={weatherEnabled}
                      onChange={(e) => setWeatherEnabled(e.target.checked)}
                      color="primary"
                    />
                  }
                  label={translate('기상정보 기능 사용', 'Enable Weather Features', language)}
                />
                <Typography variant="caption" sx={{ display: 'block', color: '#aaa', mt: 0.5, ml: 2 }}>
                  {translate(
                    '기상정보 관련 기능을 모두 활성화/비활성화합니다', 
                    'Enable or disable all weather-related features', 
                    language
                  )}
                </Typography>
                
                {!weatherEnabled && (
                  <Alert severity="info" sx={{ mt: 1, mb: 1 }}>
                    {translate(
                      '기상정보 기능이 비활성화되었습니다. 기상정보 관련 탭과 기능이 표시되지 않습니다.',
                      'Weather features are disabled. Weather-related tabs and functions will not be displayed.',
                      language
                    )}
                  </Alert>
                )}
              </Box>
              
              <Divider sx={{ my: 2 }} />
              
              {/* 알림 소리 토글 */}
              <Box sx={{ mb: 1 }}>
                <FormControlLabel
                  control={
                    <Switch
                      checked={alertSoundEnabled}
                      onChange={(e) => setAlertSoundEnabled(e.target.checked)}
                      color="primary"
                    />
                  }
                  label={translate('알림 소리 사용', 'Enable Alert Sounds', language)}
                />
              </Box>
            </CardContent>
          </Card>
        </Grid>
        
        {/* 추가 설정 카드 (필요시) */}
        <Grid item xs={12} md={6}>
          <Card sx={{ 
            backgroundColor: '#0a1929', 
            border: '1px solid #1e3a5a',
            borderRadius: 2 
          }}>
            <CardHeader 
              title={translate('기타 설정', 'Other Settings', language)}
              sx={{ borderBottom: '1px solid #1e3a5a', p: 2 }}
            />
            <CardContent sx={{ p: 3 }}>
              <Typography variant="body1">
                {translate(
                  '추가 설정 옵션이 여기에 표시됩니다.', 
                  'Additional setting options will be displayed here.', 
                  language
                )}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>
    </Box>
  );
};

export default Settings; 