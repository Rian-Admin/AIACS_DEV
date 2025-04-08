import React, { useState, useEffect } from 'react';
import {
  Box,
  Typography,
  Button,
  Paper,
  Alert,
  CircularProgress,
  Divider
} from '@mui/material';
import PlayArrowIcon from '@mui/icons-material/PlayArrow';
import StopIcon from '@mui/icons-material/Stop';
import { radarEmulatorAPI } from '../../api';

const ScenarioTab = ({ translate, language }) => {
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [success, setSuccess] = useState('');
  const [isPlaying, setIsPlaying] = useState(false);
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [statusInterval, setStatusInterval] = useState(null);

  // 플롯/트랙 데이터 주기적으로 가져오기
  useEffect(() => {
    if (isPlaying) {
      // 상태 주기적으로 확인
      const intervalId = setInterval(async () => {
        try {
          const statusResponse = await radarEmulatorAPI.getSimulationStatus();
          console.log('시뮬레이션 상태 확인:', statusResponse.data);
          
          if (statusResponse.data.plots) {
            setPlotData(statusResponse.data.plots);
          }
          
          if (statusResponse.data.tracks) {
            setTrackData(statusResponse.data.tracks);
          }
        } catch (err) {
          console.error('상태 확인 중 오류:', err);
        }
      }, 1000);
      
      setStatusInterval(intervalId);
      
      return () => {
        clearInterval(intervalId);
      };
    } else if (statusInterval) {
      clearInterval(statusInterval);
    }
  }, [isPlaying]);

  // 시뮬레이션 시작
  const handleStartSimulation = async () => {
    setLoading(true);
    setError('');
    try {
      // 운용 모드 설정
      const startResponse = await radarEmulatorAPI.startOperation();
      
      if (startResponse.data && startResponse.data.status === 'success') {
        setIsPlaying(true);
        setSuccess('레이더 운용이 시작되었습니다. 플롯/트랙 데이터를 수신합니다.');
        
        // 플롯/트랙 활성화
        await radarEmulatorAPI.activatePlotTrack();
      } else {
        setError(`레이더 운용 시작 실패: ${startResponse.data?.message || '알 수 없는 오류'}`);
      }
    } catch (err) {
      console.error('운용 시작 오류:', err);
      setError(`레이더 운용 시작 실패: ${err.response?.data?.message || err.message}`);
    } finally {
      setLoading(false);
    }
  };

  // 시뮬레이션 중지
  const handleStopSimulation = async () => {
    setLoading(true);
    setError('');
    try {
      const response = await radarEmulatorAPI.stopSimulation();
      
      if (response.data && response.data.status === 'success') {
        setIsPlaying(false);
        setSuccess('레이더 운용이 중지되었습니다.');
        setTimeout(() => setSuccess(''), 3000);
      } else {
        setError(`운용 중지 실패: ${response.data?.message || '알 수 없는 오류'}`);
      }
    } catch (err) {
      console.error('운용 중지 오류:', err);
      setError(`운용 중지 실패: ${err.response?.data?.message || err.message}`);
    } finally {
      setLoading(false);
    }
  };

  // 결과 메시지 표시 컴포넌트
  const renderResultMessage = () => {
    if (error) {
      return (
        <Alert severity="error" sx={{ mt: 2 }}>
          {error}
        </Alert>
      );
    }
    
    if (success) {
      return (
        <Alert severity="success" sx={{ mt: 2 }}>
          {success}
        </Alert>
      );
    }
    
    return null;
  };

  // 플롯/트랙 데이터 표시
  const renderDataDisplay = () => {
    return (
      <Paper sx={{ p: 2, mt: 2, maxHeight: '400px', overflow: 'auto' }}>
        <Typography variant="subtitle1" gutterBottom>
          {translate('수신 데이터', 'Received Data', language)}
        </Typography>
        
        <Typography variant="subtitle2" gutterBottom>
          {translate('트랙 데이터', 'Track Data', language)} ({trackData.length})
        </Typography>
        {trackData.length > 0 ? (
          <Box component="ul" sx={{ pl: 2 }}>
            {trackData.map((track, idx) => (
              <Typography component="li" key={idx} variant="body2">
                트랙 #{track.id} - 
                위치: [{track.position.join(', ')}], 
                속도: [{track.velocity?.join(', ') || 'N/A'}], 
                유형: {track.type}
              </Typography>
            ))}
          </Box>
        ) : (
          <Typography variant="body2" color="text.secondary">
            {translate('수신된 트랙 데이터가 없습니다', 'No track data received', language)}
          </Typography>
        )}
        
        <Divider sx={{ my: 2 }} />
        
        <Typography variant="subtitle2" gutterBottom>
          {translate('플롯 데이터', 'Plot Data', language)} ({plotData.length})
        </Typography>
        {plotData.length > 0 ? (
          <Box component="ul" sx={{ pl: 2 }}>
            {plotData.map((plot, idx) => (
              <Typography component="li" key={idx} variant="body2">
                플롯 #{plot.id} - 
                위치: [{plot.position.join(', ')}], 
                RCS: {plot.rcs}
              </Typography>
            ))}
          </Box>
        ) : (
          <Typography variant="body2" color="text.secondary">
            {translate('수신된 플롯 데이터가 없습니다', 'No plot data received', language)}
          </Typography>
        )}
      </Paper>
    );
  };

  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h6" gutterBottom>
        {translate('레이더 운용', 'Radar Operation', language)}
      </Typography>
      
      <Paper sx={{ p: 2, mb: 2, backgroundColor: 'rgba(253, 237, 200, 0.2)', border: '1px solid #FFA726' }}>
        <Typography variant="subtitle2" color="warning.dark" gutterBottom>
          <strong>{translate('중요 안내사항', 'Important Instructions', language)}</strong>
        </Typography>
        <Box component="ul" sx={{ pl: 2, m: 0 }}>
          <Typography component="li" variant="body2">
            {translate(
              '에뮬레이터는 시작 시 T_Scenario.json 파일의 시나리오를 자동으로 로드합니다.',
              'The emulator automatically loads scenarios from T_Scenario.json file at startup.',
              language
            )}
          </Typography>
          <Typography component="li" variant="body2">
            {translate(
              '"운용 시작" 버튼을 클릭하면 레이더 운용이 시작되고 플롯/트랙 데이터가 표시됩니다.',
              'Click "Start Operation" to begin radar operation and display plot/track data.',
              language
            )}
          </Typography>
        </Box>
      </Paper>

      <Box sx={{ display: 'flex', gap: 2 }}>
        <Button
          variant="contained"
          color="primary"
          startIcon={<PlayArrowIcon />}
          disabled={loading || isPlaying}
          onClick={handleStartSimulation}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate('운용 시작', 'Start Operation', language)
          )}
        </Button>
        
        <Button
          variant="contained"
          color="error"
          startIcon={<StopIcon />}
          disabled={!isPlaying || loading}
          onClick={handleStopSimulation}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate('운용 중지', 'Stop Operation', language)
          )}
        </Button>
      </Box>
      
      {renderResultMessage()}
      {renderDataDisplay()}
    </Box>
  );
};

export default ScenarioTab; 