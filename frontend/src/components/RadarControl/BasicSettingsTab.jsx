import React from 'react';
import {
  Box,
  Grid,
  Paper,
  Typography,
  TextField,
  Button,
  Divider,
  CircularProgress
} from '@mui/material';

/**
 * 레이더 기본설정 탭 컴포넌트
 */
const BasicSettingsTab = ({ 
  language, 
  translate,
  operationParams,
  setOperationParams,
  loading,
  requestOperationParams,
  updateOperationParams
}) => {
  return (
    <Paper sx={{ p: 3, backgroundColor: '#132f4c', border: '1px solid #1e3a5a' }}>
      <Typography variant="h6" sx={{ mb: 3, color: 'white' }}>
        {translate("운용 파라미터 설정", "Operation Parameters", language)}
      </Typography>
      
      {/* 레이더 위치 설정 섹션 */}
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate("레이더 위치 설정", "Radar Location Settings", language)}
      </Typography>
      
      <Grid container spacing={2} sx={{ mb: 3 }}>
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("경도", "Longitude", language)}
            type="number"
            value={operationParams.rdp.longitude}
            onChange={(e) => setOperationParams({
              ...operationParams,
              rdp: { ...operationParams.rdp, longitude: parseFloat(e.target.value) }
            })}
            InputProps={{ endAdornment: '°' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("위도", "Latitude", language)}
            type="number"
            value={operationParams.rdp.latitude}
            onChange={(e) => setOperationParams({
              ...operationParams,
              rdp: { ...operationParams.rdp, latitude: parseFloat(e.target.value) }
            })}
            InputProps={{ endAdornment: '°' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("고도", "Altitude", language)}
            type="number"
            value={operationParams.rdp.altitude}
            onChange={(e) => setOperationParams({
              ...operationParams,
              rdp: { ...operationParams.rdp, altitude: parseFloat(e.target.value) }
            })}
            InputProps={{ endAdornment: 'm' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
      </Grid>
      
      <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />
      
      {/* 레이더 패널 설정 섹션 */}
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate("레이더 패널 설정", "Radar Panel Settings", language)}
      </Typography>
      
      <Grid container spacing={2} sx={{ mb: 3 }}>
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("기울기", "Tilt", language)}
            type="number"
            value={operationParams.rsp[0].tilt}
            onChange={(e) => {
              const newRsp = [...operationParams.rsp];
              newRsp[0] = { ...newRsp[0], tilt: parseFloat(e.target.value) };
              setOperationParams({ ...operationParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: '°' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("슬랜트", "Slant", language)}
            type="number"
            value={operationParams.rsp[0].slant}
            onChange={(e) => {
              const newRsp = [...operationParams.rsp];
              newRsp[0] = { ...newRsp[0], slant: parseFloat(e.target.value) };
              setOperationParams({ ...operationParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: '°' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={4}>
          <TextField
            fullWidth
            label={translate("헤딩", "Heading", language)}
            type="number"
            value={operationParams.rsp[0].heading}
            onChange={(e) => {
              const newRsp = [...operationParams.rsp];
              newRsp[0] = { ...newRsp[0], heading: parseFloat(e.target.value) };
              setOperationParams({ ...operationParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: '°' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
      </Grid>
      
      {/* 설정 버튼 */}
      <Box sx={{ display: 'flex', justifyContent: 'flex-end', mt: 2 }}>
        <Button
          variant="contained"
          color="primary"
          onClick={() => updateOperationParams(operationParams)}
          disabled={loading}
          sx={{ mr: 1 }}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate("설정 적용", "Apply Settings", language)
          )}
        </Button>
        
        <Button
          variant="outlined"
          onClick={requestOperationParams}
          disabled={loading}
          sx={{
            borderColor: '#1e3a5a',
            color: '#90caf9',
            '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
          }}
        >
          {translate("기본값 불러오기", "Load Defaults", language)}
        </Button>
      </Box>
    </Paper>
  );
};

export default BasicSettingsTab; 