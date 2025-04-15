import React from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Grid, 
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  TextField,
  Button,
  CircularProgress
} from '@mui/material';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFns';
import { LocalizationProvider } from '@mui/x-date-pickers/LocalizationProvider';
import { DatePicker } from '@mui/x-date-pickers/DatePicker';
import FileDownloadIcon from '@mui/icons-material/FileDownload';

/**
 * 필터 섹션 컴포넌트
 * @param {Object} props
 * @param {Object} props.filters - 필터 상태 객체
 * @param {Function} props.onFilterChange - 필터 변경 핸들러
 * @param {Function} props.onApplyFilters - 필터 적용 핸들러
 * @param {Function} props.onCsvDownload - CSV 다운로드 핸들러
 * @param {boolean} props.isLoading - 로딩 상태
 * @param {boolean} props.hasData - 데이터 존재 여부
 * @param {Array} props.birdClasses - 조류 클래스 목록
 * @param {Array} props.cameras - 카메라 목록
 * @returns {JSX.Element}
 */
const FilterSection = ({ 
  filters, 
  onFilterChange, 
  onApplyFilters, 
  onCsvDownload, 
  isLoading, 
  hasData,
  birdClasses = [],
  cameras = []
}) => {
  const { timeRange, startDate, endDate, birdClass, cameraId } = filters;

  // 데이터가 배열인지 확인
  const cameraArray = Array.isArray(cameras) ? cameras : [];
  const birdClassArray = Array.isArray(birdClasses) ? birdClasses : [];

  return (
    <Paper sx={{ p: 2, mb: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a' }}>
      <Box>
        <Typography variant="h6" sx={{ mb: 1 }}>
          기간 별 검색
        </Typography>
        <Typography variant="body2" sx={{ mb: 2, color: 'text.secondary' }}>
          아래 필터를 적용하여 데이터를 확인할 수 있습니다.
        </Typography>
        
        <Grid container spacing={2}>
          <Grid item xs={12}>
            <Grid container spacing={2} alignItems="flex-end">
              <Grid item xs={12} sm={4} md={2}>
                <Typography variant="subtitle2" sx={{ mb: 0.5 }}>기간</Typography>
                <FormControl fullWidth size="small">
                  <InputLabel id="time-range-label">기간</InputLabel>
                  <Select
                    labelId="time-range-label"
                    value={timeRange}
                    label="기간"
                    onChange={e => onFilterChange('timeRange', e.target.value)}
                  >
                    <MenuItem value="day">오늘</MenuItem>
                    <MenuItem value="week">이번 주</MenuItem>
                    <MenuItem value="month">이번 달</MenuItem>
                    <MenuItem value="year">올해</MenuItem>
                    <MenuItem value="custom">사용자 지정</MenuItem>
                  </Select>
                </FormControl>
              </Grid>
              
              <Grid item xs={12} sm={4} md={2}>
                <Typography variant="subtitle2" sx={{ mb: 0.5 }}>조류 종류</Typography>
                <FormControl fullWidth size="small">
                  <InputLabel id="species-label">조류 종류</InputLabel>
                  <Select
                    labelId="species-label"
                    value={birdClass}
                    label="조류 종류"
                    onChange={(e) => onFilterChange('birdClass', e.target.value)}
                  >
                    <MenuItem value="">전체</MenuItem>
                    {birdClassArray.map((bird) => (
                      <MenuItem key={bird.class_id} value={bird.class_id}>
                        {bird.bird_name_ko}
                      </MenuItem>
                    ))}
                  </Select>
                </FormControl>
              </Grid>
              
              <Grid item xs={12} sm={4} md={2}>
                <Typography variant="subtitle2" sx={{ mb: 0.5 }}>카메라</Typography>
                <FormControl fullWidth size="small">
                  <InputLabel id="camera-label">카메라</InputLabel>
                  <Select
                    labelId="camera-label"
                    value={cameraId}
                    label="카메라"
                    onChange={(e) => onFilterChange('cameraId', e.target.value)}
                  >
                    <MenuItem value="">전체</MenuItem>
                    {cameraArray.map((camera) => (
                      <MenuItem key={camera.camera_id} value={camera.camera_id}>
                        카메라 {camera.camera_id}
                      </MenuItem>
                    ))}
                  </Select>
                </FormControl>
              </Grid>
              
              {timeRange === 'custom' && (
                <>
                  <Grid item xs={12} sm={4} md={2}>
                    <Typography variant="subtitle2" sx={{ mb: 0.5 }}>시작일</Typography>
                    <LocalizationProvider dateAdapter={AdapterDateFns}>
                      <DatePicker
                        value={startDate}
                        onChange={(newValue) => onFilterChange('startDate', newValue)}
                        renderInput={(params) => <TextField {...params} fullWidth size="small" />}
                      />
                    </LocalizationProvider>
                  </Grid>
                  
                  <Grid item xs={12} sm={4} md={2}>
                    <Typography variant="subtitle2" sx={{ mb: 0.5 }}>종료일</Typography>
                    <LocalizationProvider dateAdapter={AdapterDateFns}>
                      <DatePicker
                        value={endDate}
                        onChange={(newValue) => onFilterChange('endDate', newValue)}
                        renderInput={(params) => <TextField {...params} fullWidth size="small" />}
                      />
                    </LocalizationProvider>
                  </Grid>
                </>
              )}
              
              <Grid item xs={6} sm={3} md={1}>
                <Button 
                  variant="outlined" 
                  color="primary" 
                  onClick={onCsvDownload}
                  disabled={isLoading || !hasData}
                  fullWidth
                  startIcon={<FileDownloadIcon />}
                  sx={{ height: 40 }}
                >
                  CSV
                </Button>
              </Grid>
              
              <Grid item xs={6} sm={3} md={1} sx={{ ml: { xs: 0, md: 'auto' } }}>
                <Button 
                  variant="contained" 
                  color="primary" 
                  onClick={onApplyFilters}
                  disabled={isLoading}
                  fullWidth
                  sx={{ height: 40 }}
                >
                  {isLoading
                    ? <><CircularProgress size={16} sx={{ mr: 1, color: 'white' }} />로딩</>
                    : '적용'
                  }
                </Button>
              </Grid>
            </Grid>
          </Grid>
        </Grid>
      </Box>
    </Paper>
  );
};

export default FilterSection; 