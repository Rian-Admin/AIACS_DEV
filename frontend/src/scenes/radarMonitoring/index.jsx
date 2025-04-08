import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper, Grid } from '@mui/material';
import { translate } from '../../utils/i18n';
import RadarControl from '../../components/RadarControl';
import { radarEmulatorAPI } from '../../api';
import CombinedRadarDisplay from '../../components/RadarControl/CombinedRadarDisplay';

const RadarMonitoring = ({ language }) => {
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [isConnected, setIsConnected] = useState(false);
  const [operationMode, setOperationMode] = useState('');

  // API 응답 구조를 확인하기 위한 로깅 추가
  useEffect(() => {
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
  }, []);

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