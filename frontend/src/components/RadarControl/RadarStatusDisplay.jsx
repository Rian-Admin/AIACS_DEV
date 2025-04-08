import React from 'react';
import { Box, Paper, Typography, Grid, Chip, Divider } from '@mui/material';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';
import ErrorIcon from '@mui/icons-material/Error';
import WarningIcon from '@mui/icons-material/Warning';
import SpeedIcon from '@mui/icons-material/Speed';
import AccessTimeIcon from '@mui/icons-material/AccessTime';
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import SignalCellularAltIcon from '@mui/icons-material/SignalCellularAlt';
import ThermostatIcon from '@mui/icons-material/Thermostat';
import BatteryChargingFullIcon from '@mui/icons-material/BatteryChargingFull';
import PowerIcon from '@mui/icons-material/Power';
import PauseCircleOutlineIcon from '@mui/icons-material/PauseCircleOutline';
import SignalCellular4BarIcon from '@mui/icons-material/SignalCellular4Bar';
import WifiOffIcon from '@mui/icons-material/WifiOff';
import HelpOutlineIcon from '@mui/icons-material/HelpOutline';

// 언어 텍스트 번역 함수
const translate = (korean, english, language) => {
  return language === 'ko' ? korean : english;
};

// 운용 모드에 따른 텍스트 및 색상 반환
const getOperationModeInfo = (mode, language) => {
  switch (mode) {
    case 0:
      return {
        text: translate('대기', 'Standby', language),
        color: 'warning',
        icon: <WarningIcon />
      };
    case 1:
      return {
        text: translate('초기화 중', 'Initializing', language),
        color: 'info',
        icon: <AccessTimeIcon />
      };
    case 2:
      return {
        text: translate('운용 중', 'Operating', language),
        color: 'success',
        icon: <CheckCircleIcon />
      };
    case 3:
      return {
        text: translate('오류', 'Error', language),
        color: 'error',
        icon: <ErrorIcon />
      };
    default:
      return {
        text: translate('알 수 없음', 'Unknown', language),
        color: 'default',
        icon: <WarningIcon />
      };
  }
};

// 상태 값에 따른 텍스트 및 색상 반환
const getStatusInfo = (status, language) => {
  if (status === 0) {
    return {
      text: translate('정상', 'Normal', language),
      color: 'success',
      icon: <CheckCircleIcon />
    };
  } else if (status === 1) {
    return {
      text: translate('경고', 'Warning', language),
      color: 'warning',
      icon: <WarningIcon />
    };
  } else {
    return {
      text: translate('오류', 'Error', language),
      color: 'error',
      icon: <ErrorIcon />
    };
  }
};

// 수정: 레이더 패널 상태 코드에 따른 정보 반환 함수 추가
const getPanelStateInfo = (state, language) => {
  switch (state) {
    case 0:
      return {
        text: translate('부팅 중', 'Booting', language),
        color: 'warning',
        icon: <PowerIcon />,
        description: translate('시스템이 부팅 중입니다', 'System is booting', language)
      };
    case 1:
      return {
        text: translate('대기 중', 'Standby', language),
        color: 'info',
        icon: <PauseCircleOutlineIcon />,
        description: translate('시스템이 대기 상태입니다', 'System is on standby', language)
      };
    case 13:
      return {
        text: translate('운용 중 (송신 ON)', 'Operating (Tx ON)', language),
        color: 'success',
        icon: <SignalCellular4BarIcon />,
        description: translate('시스템이 운용 중이며 송신 기능이 활성화되어 있습니다', 'System is operating with transmission on', language)
      };
    case 14:
      return {
        text: translate('운용 중 (송신 OFF)', 'Operating (Tx OFF)', language),
        color: 'secondary',
        icon: <WifiOffIcon />,
        description: translate('시스템이 운용 중이나 송신 기능이 비활성화되어 있습니다', 'System is operating with transmission off', language)
      };
    default:
      return {
        text: translate('알 수 없음', 'Unknown', language),
        color: 'error',
        icon: <HelpOutlineIcon />,
        description: translate('알 수 없는 상태입니다', 'Unknown state', language)
      };
  }
};

const RadarStatusDisplay = ({ data, language }) => {
  if (!data || !data.rdp) return null;

  // data.rsp 확인 (null/undefined 또는 빈 배열인 경우 처리)
  const hasRspData = data.rsp && Array.isArray(data.rsp) && data.rsp.length > 0;
  
  // 운용 모드 정보
  const modeInfo = getOperationModeInfo(data.rdp.op_mode || 0, language);
  
  // 시간 정보 변환
  const timestamp = data.time_sec ? new Date(data.time_sec * 1000).toLocaleString() : '';
  
  return (
    <Box sx={{ mt: 2, mb: 3 }}>
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate('레이더 상태 정보', 'Radar Status Information', language)}
      </Typography>
      
      {/* 운용 상태 및 시간 */}
      <Grid container spacing={2} sx={{ mb: 2 }}>
        <Grid item xs={12} sm={6} md={3}>
          <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', height: '100%' }}>
            <Box sx={{ display: 'flex', flexDirection: 'column', height: '100%' }}>
              <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                {translate('운용 모드', 'Operation Mode', language)}
              </Typography>
              <Box sx={{ display: 'flex', alignItems: 'center', mt: 'auto' }}>
                <Chip
                  icon={modeInfo.icon}
                  label={modeInfo.text}
                  color={modeInfo.color}
                  sx={{ fontWeight: 'bold' }}
                />
              </Box>
            </Box>
          </Paper>
        </Grid>
        
        <Grid item xs={12} sm={6} md={3}>
          <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', height: '100%' }}>
            <Box sx={{ display: 'flex', flexDirection: 'column', height: '100%' }}>
              <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                {translate('타임스탬프', 'Timestamp', language)}
              </Typography>
              <Box sx={{ display: 'flex', alignItems: 'center', mt: 'auto' }}>
                <AccessTimeIcon sx={{ color: '#90caf9', mr: 1 }} />
                <Typography sx={{ color: 'white' }}>
                  {timestamp}
                </Typography>
              </Box>
            </Box>
          </Paper>
        </Grid>
        
        <Grid item xs={12} sm={6} md={3}>
          <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', height: '100%' }}>
            <Box sx={{ display: 'flex', flexDirection: 'column', height: '100%' }}>
              <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                {translate('트랙 개수', 'Track Count', language)}
              </Typography>
              <Box sx={{ display: 'flex', alignItems: 'center', mt: 'auto' }}>
                <DeviceHubIcon sx={{ color: '#90caf9', mr: 1 }} />
                <Typography variant="h5" sx={{ color: 'white' }}>
                  {data.rdp.track_count || 0}
                </Typography>
              </Box>
            </Box>
          </Paper>
        </Grid>
        
        <Grid item xs={12} sm={6} md={3}>
          <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', height: '100%' }}>
            <Box sx={{ display: 'flex', flexDirection: 'column', height: '100%' }}>
              <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                {translate('플롯 개수', 'Plot Count', language)}
              </Typography>
              <Box sx={{ display: 'flex', alignItems: 'center', mt: 'auto' }}>
                <SignalCellularAltIcon sx={{ color: '#90caf9', mr: 1 }} />
                <Typography variant="h5" sx={{ color: 'white' }}>
                  {data.rdp.plot_count || 0}
                </Typography>
              </Box>
            </Box>
          </Paper>
        </Grid>
      </Grid>
      
      {/* 레이더 패널 상태 - rsp 데이터가 있을 때만 표시 */}
      {hasRspData && (
        <>
          <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />
          
          <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 2 }}>
            {translate('레이더 패널 상태', 'Radar Panel Status', language)}
          </Typography>
          
          <Grid container spacing={2}>
            {data.rsp.map((panel, index) => (
              <Grid item xs={12} sm={6} md={4} key={index}>
                {/* 디버그 출력을 위한 코드 추가 */}
                {console.log(`패널 ${index} 데이터:`, panel)}
                <Paper sx={{ p: 2, backgroundColor: '#1e3a5a' }}>
                  <Typography variant="subtitle2" sx={{ color: 'white', mb: 2 }}>
                    {translate('패널', 'Panel', language)} #{index + 1}
                  </Typography>
                  
                  <Grid container spacing={1}>
                    {/* 패널 상태 */}
                    <Grid item xs={12}>
                      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 1 }}>
                        <Typography variant="body2" sx={{ color: '#90caf9' }}>
                          {translate('상태', 'Status', language)}:
                        </Typography>
                        <Chip
                          size="small"
                          icon={getPanelStateInfo(panel.State || 0, language).icon}
                          label={getPanelStateInfo(panel.State || 0, language).text}
                          color={getPanelStateInfo(panel.State || 0, language).color}
                          title={getPanelStateInfo(panel.State || 0, language).description}
                        />
                      </Box>
                    </Grid>
                    
                    {/* 온도 */}
                    <Grid item xs={12}>
                      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 1 }}>
                        <Box sx={{ display: 'flex', alignItems: 'center' }}>
                          <ThermostatIcon sx={{ color: '#90caf9', fontSize: '1rem', mr: 0.5 }} />
                          <Typography variant="body2" sx={{ color: '#90caf9' }}>
                            {translate('온도', 'Temperature', language)}:
                          </Typography>
                        </Box>
                        <Typography variant="body2" sx={{ color: 'white' }}>
                          {panel.temperature ? panel.temperature.toFixed(1) : 'N/A'}°C
                        </Typography>
                      </Box>
                    </Grid>
                    
                    {/* 전압 */}
                    <Grid item xs={12}>
                      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 1 }}>
                        <Box sx={{ display: 'flex', alignItems: 'center' }}>
                          <BatteryChargingFullIcon sx={{ color: '#90caf9', fontSize: '1rem', mr: 0.5 }} />
                          <Typography variant="body2" sx={{ color: '#90caf9' }}>
                            {translate('전압', 'Voltage', language)}:
                          </Typography>
                        </Box>
                        <Typography variant="body2" sx={{ color: 'white' }}>
                          {panel.voltage ? panel.voltage.toFixed(1) : 'N/A'}V
                        </Typography>
                      </Box>
                    </Grid>
                    
                    {/* 방향 */}
                    <Grid item xs={12}>
                      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                        <Typography variant="body2" sx={{ color: '#90caf9' }}>
                          {translate('방향', 'Heading', language)}:
                        </Typography>
                        <Typography variant="body2" sx={{ color: 'white' }}>
                          {panel.heading ? panel.heading.toFixed(1) : 'N/A'}°
                        </Typography>
                      </Box>
                    </Grid>
                  </Grid>
                </Paper>
              </Grid>
            ))}
          </Grid>
        </>
      )}
    </Box>
  );
};

export default RadarStatusDisplay; 