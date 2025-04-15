import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper, Grid, Alert, Button } from '@mui/material';
import { translate } from '../../utils/i18n';
import RadarControl from '../../components/RadarControl';
import { radarEmulatorAPI } from '../../api';
import CombinedRadarDisplay from '../../components/RadarControl/CombinedRadarDisplay';
import useAppStore from '../../store/useAppStore';
import { useNavigate } from 'react-router-dom';

const RadarMonitoring = ({ language }) => {
  const navigate = useNavigate();
  const { radarEnabled } = useAppStore();
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [isConnected, setIsConnected] = useState(false);
  const [operationMode, setOperationMode] = useState('');

  // 레이더 기능이 비활성화된 경우 데이터 폴링 중지
  useEffect(() => {
    if (!radarEnabled) {
      setPlotData([]);
      setTrackData([]);
      setIsConnected(false);
      setOperationMode('');
    }
  }, [radarEnabled]);

  // API 응답 구조를 확인하기 위한 로깅 추가
  useEffect(() => {
    // 레이더 기능이 비활성화된 경우 API 호출 중지
    if (!radarEnabled) return;
    
    const fetchRadarData = async () => {
      try {
        // 시뮬레이션/실제 레이더 데이터 가져오기
        const response = await radarEmulatorAPI.getSimulationStatus();
        console.log('레이더 API 응답:', response.data);
        
        // 연결 상태 및 운용 모드 업데이트
        if (response.data.connected !== undefined) {
          setIsConnected(response.data.connected);
        }
        
        if (response.data.operation_mode) {
          setOperationMode(response.data.operation_mode);
        }
        
        // API 응답 구조에 맞게 데이터 처리
        if (response.data.plots && response.data.plots.length > 0) {
          // Plot 데이터 직접 사용
          setPlotData(response.data.plots);
          console.log(`플롯 데이터 업데이트: ${response.data.plots.length}개`);
        } else if (response.data.Plot && response.data.Plot.length > 0) {
          // Plot 키로 데이터가 오는 경우
          setPlotData(response.data.Plot);
          console.log(`플롯 데이터 업데이트: ${response.data.Plot.length}개`);
        } else if (response.data.nPlot && response.data.Plot && response.data.Plot.length > 0) {
          // nPlot과 Plot 배열 형태로 오는 경우
          setPlotData(response.data.Plot);
          console.log(`플롯 데이터 업데이트: ${response.data.Plot.length}개`);
        } else {
          // 플롯 데이터가 없는 경우 빈 배열로 설정
          setPlotData([]);
        }
        
        // 트랙 데이터 처리
        if (response.data.tracks && response.data.tracks.length > 0) {
          setTrackData(response.data.tracks);
          console.log(`트랙 데이터 업데이트: ${response.data.tracks.length}개`);
        } else if (response.data.Track && response.data.Track.length > 0) {
          setTrackData(response.data.Track);
          console.log(`트랙 데이터 업데이트: ${response.data.Track.length}개`);
        } else if (response.data.nTrack && response.data.Track && response.data.Track.length > 0) {
          setTrackData(response.data.Track);
          console.log(`트랙 데이터 업데이트: ${response.data.Track.length}개`);
        } else {
          // 트랙 데이터가 없는 경우 빈 배열로 설정
          setTrackData([]);
        }
      } catch (error) {
        console.error('레이더 데이터 가져오기 오류:', error);
        // 오류 발생 시 데이터 초기화
        setPlotData([]);
        setTrackData([]);
        setIsConnected(false);
      }
    };

    fetchRadarData();
    // API 호출 주기
    const interval = setInterval(fetchRadarData, 1000);
    
    return () => clearInterval(interval);
  }, [radarEnabled]);

  // 레이더 기능이 비활성화된 경우 접근 불가 메시지 표시
  if (!radarEnabled) {
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
            {translate('레이더 기능이 비활성화되었습니다', 'Radar Features are Disabled', language)}
          </Typography>
          <Typography variant="body1" sx={{ mb: 2 }}>
            {translate(
              '이 기능을 사용하려면 설정에서 레이더 기능을 활성화해야 합니다.',
              'To use this feature, you need to enable radar functionality in the settings.',
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
    <Box p={2}>
      <Typography variant="h5" color="white" mb={2}>
        {translate('레이더 모니터링', 'Radar Monitoring', language)}
        <span style={{ 
          color: isConnected ? 'lightgreen' : 'orange', 
          fontSize: '0.8em', 
          marginLeft: '10px' 
        }}>
          {isConnected ? 
            `(${operationMode || '연결됨'})` : 
            '(연결 안됨)'
          }
        </span>
      </Typography>
      
      <Grid container spacing={2}>
        <Grid item xs={12} md={8}>
          <Paper elevation={0} sx={{ p: 2, borderRadius: 2, backgroundColor: '#0a1929', border: '1px solid #1e3a5a', height: '100%' }}>
            <Typography variant="h6" mb={2}>
              {translate('통합 레이더 데이터', 'Combined Radar Data', language)}
              <span style={{ color: '#4fc3f7', marginLeft: '10px', fontSize: '0.8em' }}>플롯: {plotData.length}개</span>
              <span style={{ color: '#ff9800', marginLeft: '10px', fontSize: '0.8em' }}>트랙: {trackData.length}개</span>
            </Typography>
            <Box sx={{ height: 'auto', width: '100%', position: 'relative' }}>
              <CombinedRadarDisplay plotData={plotData} trackData={trackData} language={language} translate={translate} />
            </Box>
          </Paper>
        </Grid>
        <Grid item xs={12} md={4}>
          <Paper elevation={0} sx={{ p: 2, borderRadius: 2, backgroundColor: '#0a1929', border: '1px solid #1e3a5a', height: '100%' }}>
            <Typography variant="h6" mb={2}>
              {translate('레이더 컨트롤', 'Radar Control', language)}
            </Typography>
            <RadarControl language={language} />
          </Paper>
        </Grid>
      </Grid>
    </Box>
  );
};

export default RadarMonitoring; 