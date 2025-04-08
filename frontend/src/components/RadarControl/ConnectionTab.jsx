import React, { useState } from 'react';
import {
  Box,
  Grid,
  Paper,
  Typography,
  TextField,
  Button,
  Divider,
  CircularProgress,
  Collapse,
  IconButton
} from '@mui/material';
import PowerSettingsNewIcon from '@mui/icons-material/PowerSettingsNew';
import AccessTimeIcon from '@mui/icons-material/AccessTime';
import MemoryIcon from '@mui/icons-material/Memory';
import DataUsageIcon from '@mui/icons-material/DataUsage';
import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
import ExpandLessIcon from '@mui/icons-material/ExpandLess';

/**
 * 레이더 연결 탭 컴포넌트
 */
const ConnectionTab = ({ 
  language, 
  translate,
  ip, 
  setIp, 
  port, 
  setPort, 
  connected, 
  loading, 
  toggleConnection, 
  sendTimeSync, 
  requestOperationParams,
  requestPerformanceParams,
  requestBeamSchedule,
  requestAreaSettings,
  message4001,
  message4003,
  message4005,
  message4034
}) => {
  // 초기값: true로 설정하여 처음부터 결과 영역 표시
  const [showOperationParams, setShowOperationParams] = useState(true);
  const [showPerformanceParams, setShowPerformanceParams] = useState(true);
  const [showAreaSettings, setShowAreaSettings] = useState(true);
  
  // 운용파라미터 요청 및 결과 표시 함수
  const handleRequestOperationParams = async () => {
    await requestOperationParams();
    setShowOperationParams(true); // 요청 후 결과 표시 영역 자동 펼치기
  };
  
  // 시스템파라미터 요청 및 결과 표시 함수
  const handleRequestPerformanceParams = async () => {
    await requestPerformanceParams();
    setShowPerformanceParams(true); // 요청 후 결과 표시 영역 자동 펼치기
  };
  
  // 영역설정정보 요청 및 결과 표시 함수
  const handleRequestAreaSettings = async () => {
    await requestAreaSettings();
    setShowAreaSettings(true); // 요청 후 결과 표시 영역 자동 펼치기
  };
  
  return (
    <Paper sx={{ p: 3, backgroundColor: '#132f4c', border: '1px solid #1e3a5a' }}>
      <Typography variant="h6" sx={{ mb: 3, color: 'white' }}>
        {translate("레이더 연결", "Radar Connection", language)}
      </Typography>
      
      <Grid container spacing={2} alignItems="center">
        <Grid item xs={12} sm={4}>
          <TextField
            label={translate("IP 주소", "IP Address", language)}
            value={ip}
            onChange={(e) => setIp(e.target.value)}
            disabled={connected || loading}
            fullWidth
            variant="outlined"
            sx={{
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' }
              },
              '& .MuiInputLabel-root': { color: '#90caf9' },
              '& .MuiInputBase-input': { color: 'white' }
            }}
          />
        </Grid>
        <Grid item xs={12} sm={4}>
          <TextField
            label={translate("포트", "Port", language)}
            value={port}
            onChange={(e) => setPort(e.target.value)}
            disabled={connected || loading}
            fullWidth
            variant="outlined"
            sx={{
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' }
              },
              '& .MuiInputLabel-root': { color: '#90caf9' },
              '& .MuiInputBase-input': { color: 'white' }
            }}
          />
        </Grid>
        <Grid item xs={12} sm={4}>
          <Button
            fullWidth
            variant="contained"
            color={connected ? "error" : "primary"}
            startIcon={<PowerSettingsNewIcon />}
            onClick={toggleConnection}
            disabled={loading}
            sx={{
              backgroundColor: connected ? '#d32f2f' : '#1976d2',
              '&:hover': { backgroundColor: connected ? '#b71c1c' : '#1565c0' }
            }}
          >
            {loading ? (
              <CircularProgress size={24} color="inherit" />
            ) : (
              translate(connected ? "연결 해제" : "연결", connected ? "Disconnect" : "Connect", language)
            )}
          </Button>
        </Grid>
      </Grid>
      
      {connected && (
        <Box mt={3}>
          <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />
          <Typography variant="subtitle1" sx={{ mb: 2, color: 'white' }}>
            {translate("초기 설정", "Initial Setup", language)}
          </Typography>
          
          <Grid container spacing={2}>
            <Grid item xs={12} sm={4}>
              <Button
                fullWidth
                variant="outlined"
                startIcon={<AccessTimeIcon />}
                onClick={sendTimeSync}
                disabled={loading}
                sx={{
                  borderColor: '#1e3a5a',
                  color: '#90caf9',
                  '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
                }}
              >
                {translate("시간 동기화", "Time Synchronization", language)}
              </Button>
            </Grid>
            
            <Grid item xs={12} sm={4}>
              <Box>
                <Button
                  fullWidth
                  variant="outlined"
                  startIcon={<DataUsageIcon />}
                  onClick={handleRequestOperationParams}
                  disabled={loading}
                  sx={{
                    borderColor: '#1e3a5a',
                    color: '#90caf9',
                    '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
                  }}
                >
                  {translate("운용파라미터 요청", "Request Operation Parameters", language)}
                </Button>
                
                {/* 결과 보기/숨기기 토글 버튼 - 항상 표시 */}
                <Box sx={{ mt: 1, display: 'flex', justifyContent: 'center' }}>
                  <IconButton 
                    size="small" 
                    onClick={() => setShowOperationParams(!showOperationParams)}
                    sx={{ color: '#90caf9' }}
                  >
                    {showOperationParams ? <ExpandLessIcon /> : <ExpandMoreIcon />}
                    <Typography variant="caption" sx={{ ml: 0.5, color: '#90caf9' }}>
                      {showOperationParams ? 
                        translate("접기", "Hide", language) : 
                        translate("결과 보기", "Show Results", language)
                      }
                    </Typography>
                  </IconButton>
                </Box>
                
                {/* 운용파라미터 결과 영역 - 항상 영역은 존재하지만 내용이 조건부 렌더링 */}
                <Collapse in={showOperationParams}>
                  <Paper elevation={3} sx={{ p: 2, mt: 1, backgroundColor: '#102a43', overflow: 'auto', maxHeight: '300px' }}>
                    <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                      {translate("운용파라미터 (4003)", "Operation Parameters (4003)", language)}
                    </Typography>
                    
                    {message4003 ? (
                      <pre style={{ color: '#4fc3f7', fontSize: '0.7rem', whiteSpace: 'pre-wrap', overflowX: 'auto' }}>
                        {JSON.stringify(message4003, null, 2)}
                      </pre>
                    ) : (
                      <Typography variant="body2" sx={{ color: '#ffcc80', fontStyle: 'italic', textAlign: 'center', py: 2 }}>
                        {translate("데이터가 없습니다. 운용파라미터 요청 버튼을 눌러 데이터를 가져오세요.", 
                                "No data available. Click the Request Operation Parameters button to fetch data.", 
                                language)}
                      </Typography>
                    )}
                  </Paper>
                </Collapse>
              </Box>
            </Grid>
            
            <Grid item xs={12} sm={4}>
              <Box>
                <Button
                  fullWidth
                  variant="outlined"
                  startIcon={<MemoryIcon />}
                  onClick={handleRequestPerformanceParams}
                  disabled={loading}
                  sx={{
                    borderColor: '#1e3a5a',
                    color: '#90caf9',
                    '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
                  }}
                >
                  {translate("성능파라미터 요청", "Request System Parameters", language)}
                </Button>
                
                {/* 결과 보기/숨기기 토글 버튼 - 항상 표시 */}
                <Box sx={{ mt: 1, display: 'flex', justifyContent: 'center' }}>
                  <IconButton 
                    size="small" 
                    onClick={() => setShowPerformanceParams(!showPerformanceParams)}
                    sx={{ color: '#90caf9' }}
                  >
                    {showPerformanceParams ? <ExpandLessIcon /> : <ExpandMoreIcon />}
                    <Typography variant="caption" sx={{ ml: 0.5, color: '#90caf9' }}>
                      {showPerformanceParams ? 
                        translate("접기", "Hide", language) : 
                        translate("결과 보기", "Show Results", language)
                      }
                    </Typography>
                  </IconButton>
                </Box>
                
                {/* 시스템파라미터 결과 영역 - 항상 영역은 존재하지만 내용이 조건부 렌더링 */}
                <Collapse in={showPerformanceParams}>
                  <Paper elevation={3} sx={{ p: 2, mt: 1, backgroundColor: '#102a43', overflow: 'auto', maxHeight: '300px' }}>
                    <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                      {translate("성능파라미터 (4005)", "System Parameters (4005)", language)}
                    </Typography>
                    
                    {message4005 ? (
                      <pre style={{ color: '#4fc3f7', fontSize: '0.7rem', whiteSpace: 'pre-wrap', overflowX: 'auto' }}>
                        {JSON.stringify(message4005, null, 2)}
                      </pre>
                    ) : (
                      <Typography variant="body2" sx={{ color: '#ffcc80', fontStyle: 'italic', textAlign: 'center', py: 2 }}>
                        {translate("데이터가 없습니다. 시스템파라미터 요청 버튼을 눌러 데이터를 가져오세요.", 
                                "No data available. Click the Request System Parameters button to fetch data.", 
                                language)}
                      </Typography>
                    )}
                  </Paper>
                </Collapse>
              </Box>
            </Grid>
            
            {/* 빔스케줄 정보 요청 버튼 추가 */}
            <Grid item xs={12} sm={6}>
              <Button
                fullWidth
                variant="outlined"
                startIcon={<DataUsageIcon />}
                onClick={requestBeamSchedule}
                disabled={loading}
                sx={{
                  mt: 2,
                  borderColor: '#1e3a5a',
                  color: '#90caf9',
                  '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
                }}
              >
                {translate("빔스케줄 정보 요청 (3031)", "Request Beam Schedule (3031)", language)}
              </Button>
            </Grid>
            
            {/* 영역설정 정보 요청 버튼 추가 */}
            <Grid item xs={12} sm={6}>
              <Button
                fullWidth
                variant="outlined"
                startIcon={<DataUsageIcon />}
                onClick={handleRequestAreaSettings}
                disabled={loading}
                sx={{
                  mt: 2,
                  borderColor: '#1e3a5a',
                  color: '#90caf9',
                  '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
                }}
              >
                {translate("영역설정 정보 요청 (3034)", "Request Area Settings (3034)", language)}
              </Button>
              
              {/* 결과 보기/숨기기 토글 버튼 */}
              <Box sx={{ mt: 1, display: 'flex', justifyContent: 'center' }}>
                <IconButton 
                  size="small" 
                  onClick={() => setShowAreaSettings(!showAreaSettings)}
                  sx={{ color: '#90caf9' }}
                >
                  {showAreaSettings ? <ExpandLessIcon /> : <ExpandMoreIcon />}
                  <Typography variant="caption" sx={{ ml: 0.5, color: '#90caf9' }}>
                    {showAreaSettings ? 
                      translate("접기", "Hide", language) : 
                      translate("결과 보기", "Show Results", language)
                    }
                  </Typography>
                </IconButton>
              </Box>
              
              {/* 영역설정 결과 영역 */}
              <Collapse in={showAreaSettings}>
                <Paper elevation={3} sx={{ p: 2, mt: 1, backgroundColor: '#102a43', overflow: 'auto', maxHeight: '300px' }}>
                  <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
                    {translate("영역설정 정보 (4034)", "Area Settings (4034)", language)}
                  </Typography>
                  
                  {message4034 ? (
                    <pre style={{ color: '#4fc3f7', fontSize: '0.7rem', whiteSpace: 'pre-wrap', overflowX: 'auto' }}>
                      {JSON.stringify(message4034, null, 2)}
                    </pre>
                  ) : (
                    <Typography variant="body2" sx={{ color: '#ffcc80', fontStyle: 'italic', textAlign: 'center', py: 2 }}>
                      {translate("데이터가 없습니다. 영역설정 정보 요청 버튼을 눌러 데이터를 가져오세요.", 
                              "No data available. Click the Request Area Settings button to fetch data.", 
                              language)}
                    </Typography>
                  )}
                </Paper>
              </Collapse>
            </Grid>
          </Grid>
        </Box>
      )}
    </Paper>
  );
};

export default ConnectionTab; 