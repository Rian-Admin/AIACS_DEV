import React from 'react';
import {
  Box,
  Grid,
  Paper,
  Typography,
  Button,
  Divider,
  CircularProgress,
  FormControlLabel,
  Switch
} from '@mui/material';
import PowerSettingsNewIcon from '@mui/icons-material/PowerSettingsNew';
import PlayArrowIcon from '@mui/icons-material/PlayArrow';
import StopIcon from '@mui/icons-material/Stop';
import CombinedRadarDisplay from './CombinedRadarDisplay';

/**
 * 레이더 운용 탭 컴포넌트
 */
const OperationTab = ({ 
  language, 
  translate,
  loading,
  operationMode,
  startOperation,
  stopOperation,
  message4001,
  plotData,
  trackData,
  showRawData,
  handleRawDataToggle,
  pollingInterval,
  handlePollingIntervalChange,
  isOperating,
  onDeleteTrack
}) => {
  return (
    <Paper sx={{ p: 3, backgroundColor: '#132f4c', border: '1px solid #1e3a5a' }}>
      <Typography variant="h6" sx={{ mb: 3, color: 'white' }}>
        {translate("레이더 운용", "Radar Operation", language)}
      </Typography>
      
      <Box sx={{ mb: 3, display: 'flex', justifyContent: 'center' }}>
        <Button
          variant="contained"
          color={operationMode === 2 ? "error" : "primary"}
          size="large"
          startIcon={operationMode === 2 ? <StopIcon /> : <PlayArrowIcon />}
          onClick={operationMode === 2 ? stopOperation : startOperation}
          disabled={loading}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : operationMode === 2 ? (
            translate("운용 종료", "Stop Operation", language)
          ) : (
            translate("운용 시작", "Start Operation", language)
          )}
        </Button>
      </Box>
      
      <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />

    </Paper>
  );
};

export default OperationTab; 