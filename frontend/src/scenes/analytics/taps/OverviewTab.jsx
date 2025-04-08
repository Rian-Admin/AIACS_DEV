import React, { useState, useEffect } from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Grid,
  ToggleButtonGroup,
  ToggleButton,
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
import { translate } from '../../../utils/i18n';
import { 
  BarChart as BarChartIcon, 
  Warning as WarningIcon, 
  Shield as ShieldIcon, 
  Speed as SpeedIcon 
} from '@mui/icons-material';

// 컴포넌트 및 유틸리티 불러오기
import StatCard from '../components/StatCard';
import BarChartD3 from '../../../components/charts/BarChartD3';
import LineChartD3 from '../../../components/charts/LineChartD3';
import PieChartD3 from '../../../components/charts/PieChartD3';
import { 
  prepareWeeklyData, 
  prepareSpeciesDistributionData, 
  prepareHourlyData,
  prepareDailyData,
  prepareMonthlyData,
  formatDate
} from '../utils/formatHelpers';

// 차트 색상
const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d'];

/**
 * 개요 탭 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드
 * @param {Object} props.summaryData 요약 데이터
 * @param {Object} props.timeData 시간 데이터
 * @param {Object} props.speciesData 종 데이터
 * @param {Function} props.loadData 데이터 로딩 함수
 * @param {boolean} props.loading 로딩 중 여부
 * @returns {JSX.Element} 개요 탭 컴포넌트
 */
const OverviewTab = ({ language, summaryData, timeData, speciesData, loadData, loading }) => {
  const [timeRange, setTimeRange] = useState('week');
  const [startDate, setStartDate] = useState(new Date(Date.now() - 7 * 24 * 60 * 60 * 1000));
  const [endDate, setEndDate] = useState(new Date());
  const [species, setSpecies] = useState('all');
  const [weekOption, setWeekOption] = useState('thisWeek');
  const [autoRefresh, setAutoRefresh] = useState(false); // 자동 새로고침 비활성화

  // 기간 변경 처리
  const handleTimeRangeChange = (event, newValue) => {
    if (newValue) {
      setTimeRange(newValue);
    }
  };

  // 필터 적용
  const applyFilters = async () => {
    // 자동 갱신이 비활성화된 경우 리턴
    if (!autoRefresh && !document.getElementById('manualRefresh')) {
      return;
    }
    
    // 주간 옵션 처리
    let actualStartDate = startDate;
    let actualEndDate = endDate;
    
    if (timeRange === 'week' && weekOption) {
      const now = new Date();
      if (weekOption === 'thisWeek') {
        // 이번 주 (일요일부터 토요일까지)
        const day = now.getDay(); // 0: 일요일, 1: 월요일, ...
        const diff = now.getDate() - day;
        actualStartDate = new Date(now.setDate(diff));
        actualEndDate = new Date(new Date(actualStartDate).setDate(actualStartDate.getDate() + 6));
      } else if (weekOption === 'lastWeek') {
        // 지난 주
        const day = now.getDay(); // 0: 일요일, 1: 월요일, ...
        const diff = now.getDate() - day - 7;
        actualStartDate = new Date(now.setDate(diff));
        actualEndDate = new Date(new Date(actualStartDate).setDate(actualStartDate.getDate() + 6));
      }
    }
    
    await loadData(timeRange, actualStartDate, actualEndDate, species);
  };
  
  // 초기 필터 적용 - 컴포넌트 마운트 시 한 번만 실행
  useEffect(() => {
    // 숨겨진 요소 추가
    const hiddenInput = document.createElement('input');
    hiddenInput.type = 'hidden';
    hiddenInput.id = 'manualRefresh';
    hiddenInput.value = true;
    document.body.appendChild(hiddenInput);
    
    applyFilters();
    
    // 정리 함수
    return () => {
      if (document.getElementById('manualRefresh')) {
        document.getElementById('manualRefresh').remove();
      }
    };
  }, []); // 빈 배열로 처음 한 번만 실행
  
  // 기간 변경 시 필터 적용 (수동으로 변경했을 때만)
  useEffect(() => {
    if (document.getElementById('manualRefresh')) {
      document.getElementById('manualRefresh').value = true;
      applyFilters();
      document.getElementById('manualRefresh').value = false;
    }
  }, [timeRange, weekOption]);
  
  return (
    <Box sx={{ p: 3 }}>
      {/* 기간별 필터링 UI */}
      <Paper sx={{ p: 2, mb: 3, bgcolor: '#0c1e36', border: '1px solid #1e3a5a' }}>
        <Typography variant="h6" sx={{ mb: 2 }}>
          {translate('기간별 감지 통계', 'Detection Statistics by Period', 'Statistik Deteksi per Periode', language)}
        </Typography>
        
        <Grid container spacing={2} alignItems="center">
          <Grid item xs={12} md={6}>
            <ToggleButtonGroup
              value={timeRange}
              exclusive
              onChange={handleTimeRangeChange}
              aria-label="time range"
              fullWidth
              size="small"
              sx={{ 
                '& .MuiToggleButton-root': {
                  color: 'text.secondary',
                  '&.Mui-selected': {
                    bgcolor: 'primary.dark',
                    color: 'primary.contrastText',
                    '&:hover': {
                      bgcolor: 'primary.main',
                    }
                  }
                }
              }}
            >
              <ToggleButton value="hour" aria-label="hour">
                {translate('시간별', 'Hourly', 'Per Jam', language)}
              </ToggleButton>
              <ToggleButton value="day" aria-label="day">
                {translate('일별', 'Daily', 'Harian', language)}
              </ToggleButton>
              <ToggleButton value="week" aria-label="week">
                {translate('주간별', 'Weekly', 'Mingguan', language)}
              </ToggleButton>
              <ToggleButton value="month" aria-label="month">
                {translate('월별', 'Monthly', 'Bulanan', language)}
              </ToggleButton>
              <ToggleButton value="custom" aria-label="custom">
                {translate('사용자 지정', 'Custom', 'Kustom', language)}
              </ToggleButton>
            </ToggleButtonGroup>
          </Grid>
          
          {timeRange === 'week' && (
            <Grid item xs={12} md={3}>
              <FormControl fullWidth size="small">
                <InputLabel id="week-option-label">
                  {translate('주간 옵션', 'Week Option', 'Opsi Minggu', language)}
                </InputLabel>
                <Select
                  labelId="week-option-label"
                  value={weekOption}
                  label={translate('주간 옵션', 'Week Option', 'Opsi Minggu', language)}
                  onChange={(e) => setWeekOption(e.target.value)}
                >
                  <MenuItem value="thisWeek">{translate('이번 주', 'This Week', 'Minggu Ini', language)}</MenuItem>
                  <MenuItem value="lastWeek">{translate('지난 주', 'Last Week', 'Minggu Lalu', language)}</MenuItem>
                  <MenuItem value="twoWeeksAgo">{translate('2주 전', '2 Weeks Ago', '2 Minggu Lalu', language)}</MenuItem>
                </Select>
              </FormControl>
            </Grid>
          )}
          
          <Grid item xs={12} md={3}>
            <FormControl fullWidth size="small">
              <InputLabel id="species-label">
                {translate('조류 종류', 'Bird Species', 'Spesies Burung', language)}
              </InputLabel>
              <Select
                labelId="species-label"
                value={species}
                label={translate('조류 종류', 'Bird Species', 'Spesies Burung', language)}
                onChange={(e) => setSpecies(e.target.value)}
              >
                <MenuItem value="all">{translate('모든 종류', 'All Species', 'Semua Spesies', language)}</MenuItem>
                <MenuItem value="eagle">{translate('독수리', 'Eagle', 'Elang', language)}</MenuItem>
                <MenuItem value="hawk">{translate('매', 'Hawk', 'Elang', language)}</MenuItem>
                <MenuItem value="falcon">{translate('팔콘', 'Falcon', 'Falcon', language)}</MenuItem>
                <MenuItem value="seagull">{translate('갈매기', 'Seagull', 'Camar', language)}</MenuItem>
                <MenuItem value="crow">{translate('까마귀', 'Crow', 'Gagak', language)}</MenuItem>
                <MenuItem value="sparrow">{translate('참새', 'Sparrow', 'Burung gereja', language)}</MenuItem>
              </Select>
            </FormControl>
          </Grid>
          
          {timeRange === 'custom' && (
            <>
              <Grid item xs={12} md={3}>
                <LocalizationProvider dateAdapter={AdapterDateFns}>
                  <DatePicker
                    label={translate('시작일', 'Start Date', 'Tanggal Mulai', language)}
                    value={startDate}
                    onChange={(newValue) => setStartDate(newValue)}
                    renderInput={(params) => <TextField {...params} fullWidth size="small" />}
                  />
                </LocalizationProvider>
              </Grid>
              <Grid item xs={12} md={3}>
                <LocalizationProvider dateAdapter={AdapterDateFns}>
                  <DatePicker
                    label={translate('종료일', 'End Date', 'Tanggal Selesai', language)}
                    value={endDate}
                    onChange={(newValue) => setEndDate(newValue)}
                    renderInput={(params) => <TextField {...params} fullWidth size="small" />}
                  />
                </LocalizationProvider>
              </Grid>
            </>
          )}
          
          <Grid item xs={12} sx={{ display: 'flex', justifyContent: 'flex-end', mt: 2 }}>
            <Button 
              variant="contained" 
              color="primary" 
              onClick={applyFilters}
              disabled={loading}
              startIcon={loading ? <CircularProgress size={20} color="inherit" /> : null}
            >
              {loading
                ? translate('로딩 중...', 'Loading...', 'Memuat...', language)
                : translate('필터 적용', 'Apply Filters', 'Terapkan Filter', language)
              }
            </Button>
          </Grid>
        </Grid>
      </Paper>
      
      {/* 주요 통계 카드 */}
      <Grid container spacing={3} sx={{ mb: 3 }}>
        <Grid item xs={12} sm={6} md={3}>
          <StatCard 
            title={translate('총 조류 감지 수', 'Total Bird Detections', 'Total Deteksi Burung', language)}
            value={summaryData.totalDetections?.toLocaleString() || '0'}
            subtitle={
              summaryData.weeklyChange > 0 
                ? translate(`지난 주 대비 ${summaryData.weeklyChange}% 증가`, `${summaryData.weeklyChange}% increase vs last week`, `Kenaikan ${summaryData.weeklyChange}% dari minggu lalu`, language)
                : translate(`지난 주 대비 ${Math.abs(summaryData.weeklyChange)}% 감소`, `${Math.abs(summaryData.weeklyChange)}% decrease vs last week`, `Penurunan ${Math.abs(summaryData.weeklyChange)}% dari minggu lalu`, language)
            }
            color="#2196f3"
            icon={<BarChartIcon />}
          />
        </Grid>
        <Grid item xs={12} sm={6} md={3}>
          <StatCard 
            title={translate('고위험 탐지', 'High Risk Detections', 'Deteksi Risiko Tinggi', language)}
            value={summaryData.highRiskDetections?.toLocaleString() || '0'}
            subtitle={
              summaryData.totalDetections 
                ? translate(`전체의 ${Math.round(summaryData.highRiskDetections / summaryData.totalDetections * 100)}%`, `${Math.round(summaryData.highRiskDetections / summaryData.totalDetections * 100)}% of total`, `${Math.round(summaryData.highRiskDetections / summaryData.totalDetections * 100)}% dari total`, language)
                : '0%'
            }
            color="#f44336"
            icon={<WarningIcon />}
          />
        </Grid>
        <Grid item xs={12} sm={6} md={3}>
          <StatCard 
            title={translate('퇴치 시스템 활성화', 'Deterrent Activations', 'Aktivasi Pengendalian', language)}
            value={summaryData.defenseActivations?.toLocaleString() || '0'}
            subtitle={translate(
              `성공률: ${summaryData.successRate || '0.0'}%`, 
              `Success rate: ${summaryData.successRate || '0.0'}%`, 
              `Tingkat keberhasilan: ${summaryData.successRate || '0.0'}%`, 
              language
            )}
            color="#4caf50"
            icon={<ShieldIcon />}
          />
        </Grid>
        <Grid item xs={12} sm={6} md={3}>
          <StatCard 
            title={translate('평균 대응 시간', 'Avg. Response Time', 'Waktu Respons Rata-rata', language)}
            value={`${summaryData.avgResponseTime || 0}s`}
            subtitle={
              summaryData.monthlyChange > 0 
                ? translate(`지난 달 대비 ${summaryData.monthlyChange}초 향상`, `${summaryData.monthlyChange}s improvement vs last month`, `Peningkatan ${summaryData.monthlyChange} detik dari bulan lalu`, language)
                : translate(`지난 달 대비 ${Math.abs(summaryData.monthlyChange)}초 감소`, `${Math.abs(summaryData.monthlyChange)}s decrease vs last month`, `Penurunan ${Math.abs(summaryData.monthlyChange)} detik dari bulan lalu`, language)
            }
            color="#ff9800"
            icon={<SpeedIcon />}
          />
        </Grid>
      </Grid>

      {/* 주간 탐지 차트 */}
      <Typography variant="h6" sx={{ mb: 2 }}>
        {translate('기간별 감지 현황', 'Detection Activity by Period', 'Aktivitas Deteksi per Periode', language)}
      </Typography>
      <Paper 
        sx={{ 
          p: 2, 
          bgcolor: '#0c1e36', 
          border: '1px solid #1e3a5a',
          mb: 3
        }}
      >
        {loading ? (
          <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: 300 }}>
            <CircularProgress />
          </Box>
        ) : (
          <BarChartD3
            data={
              timeRange === 'hour' ? prepareHourlyData(timeData.hourly) :
              timeRange === 'day' ? prepareDailyData(timeData.daily, language) :
              timeRange === 'week' ? prepareWeeklyData(timeData.daily, language) :
              timeRange === 'month' ? 
                (() => {
                  // 월별 데이터를 가져와서 날짜순으로 정렬(과거→현재)
                  const monthlyData = prepareMonthlyData(timeData.monthly, language);
                  return [...monthlyData].reverse(); // 역순으로 정렬하여 과거→현재 순서로 표시
                })() :
              prepareWeeklyData(timeData.daily, language) // 기본적으로 주간 데이터 사용
            }
            xKey={
              timeRange === 'hour' ? 'hour' : 
              timeRange === 'month' ? 'month' : 
              'name'
            }
            yKey="count"
            xLabel={
              timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
              timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
              timeRange === 'month' ? translate('월', 'Month', 'Bulan', language) :
              translate('날짜', 'Date', 'Tanggal', language)
            }
            yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
            color="#0088FE"
            tooltipLabel={translate('건', ' detections', ' deteksi', language)}
            height={300}
            isDateTick={false}
            periodType={timeRange}
          />
        )}
      </Paper>

      {/* 종 분포 및 시간별 활동 차트 - 2열 레이아웃 */}
      <Grid container spacing={3}>
        <Grid item xs={12} md={6}>
          <Typography variant="h6" sx={{ mb: 2 }}>
            {translate('종별 분포', 'Species Distribution', 'Distribusi Spesies', language)}
          </Typography>
          <Paper 
            sx={{ 
              p: 2, 
              bgcolor: '#0c1e36', 
              border: '1px solid #1e3a5a',
              height: 300
            }}
          >
            <PieChartD3
              data={prepareSpeciesDistributionData(speciesData.distribution, language)}
              nameKey="name"
              valueKey="value"
              colors={COLORS}
              height={300}
            />
          </Paper>
        </Grid>
        <Grid item xs={12} md={6}>
          <Typography variant="h6" sx={{ mb: 2 }}>
            {translate('시간별 활동', 'Hourly Activity', 'Aktivitas per Jam', language)}
          </Typography>
          <Paper 
            sx={{ 
              p: 2, 
              bgcolor: '#0c1e36', 
              border: '1px solid #1e3a5a',
              height: 300
            }}
          >
            <LineChartD3
              data={prepareHourlyData(timeData.hourly)}
              xKey="hour"
              yKeys={['count']}
              colors={['#00C49F']}
              labels={[translate('시간별 활동', 'Hourly Activity', 'Aktivitas per Jam', language)]}
              xLabel={translate('시간', 'Hour', 'Jam', language)}
              yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
              height={300}
              periodType="hour"
            />
          </Paper>
        </Grid>
      </Grid>
    </Box>
  );
};

export default OverviewTab; 