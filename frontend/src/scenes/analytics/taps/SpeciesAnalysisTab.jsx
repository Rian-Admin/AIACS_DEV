import React, { useState, useEffect } from 'react';
import { 
  Box, 
  Typography, 
  Paper,
  ToggleButtonGroup,
  ToggleButton,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  TextField,
  Button,
  CircularProgress,
  Divider
} from '@mui/material';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFns';
import { LocalizationProvider } from '@mui/x-date-pickers/LocalizationProvider';
import { DatePicker } from '@mui/x-date-pickers/DatePicker';
import { translate } from '../../../utils/i18n';

// 컴포넌트 및 유틸리티 불러오기
import BarChartD3 from '../../../components/charts/BarChartD3';
import PieChartD3 from '../../../components/charts/PieChartD3';
import { 
  prepareSpeciesDistributionData, 
  translateSpecies, 
  formatDate, 
  prepareHourlyData, 
  prepareDailyData, 
  prepareWeeklyData, 
  prepareMonthlyData 
} from '../utils/formatHelpers';

/**
 * 종별 분석 탭 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드
 * @param {Object} props.speciesData 종 데이터
 * @param {Function} props.loadData 데이터 로딩 함수
 * @param {boolean} props.loading 로딩 중 여부
 * @returns {JSX.Element} 종별 분석 탭 컴포넌트
 */
const SpeciesAnalysisTab = ({ language, speciesData, loadData, loading }) => {
  const [timeRange, setTimeRange] = useState('week');
  const [startDate, setStartDate] = useState(new Date(Date.now() - 7 * 24 * 60 * 60 * 1000));
  const [endDate, setEndDate] = useState(new Date());
  const [species, setSpecies] = useState('all');
  const [chartType, setChartType] = useState('bar');
  const [weekOption, setWeekOption] = useState('thisWeek');
  const [monthOption, setMonthOption] = useState('thisMonth');
  const [autoRefresh, setAutoRefresh] = useState(false); // 자동 새로고침 비활성화

  // 기간 변경 처리
  const handleTimeRangeChange = (event, newValue) => {
    if (newValue) {
      setTimeRange(newValue);
    }
  };

  // 차트 타입 변경 처리
  const handleChartTypeChange = (event, newValue) => {
    if (newValue) {
      setChartType(newValue);
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
      } else if (weekOption === 'twoWeeksAgo') {
        // 2주 전
        const day = now.getDay(); // 0: 일요일, 1: 월요일, ...
        const diff = now.getDate() - day - 14;
        actualStartDate = new Date(now.setDate(diff));
        actualEndDate = new Date(new Date(actualStartDate).setDate(actualStartDate.getDate() + 6));
      }
    } else if (timeRange === 'month' && monthOption) {
      const now = new Date();
      const currentYear = now.getFullYear();
      const currentMonth = now.getMonth();
      
      if (monthOption === 'thisMonth') {
        // 이번 달 
        actualStartDate = new Date(currentYear, currentMonth, 1);
        actualEndDate = new Date(currentYear, currentMonth + 1, 0);
      } else if (monthOption === 'lastMonth') {
        // 지난 달
        actualStartDate = new Date(currentYear, currentMonth - 1, 1);
        actualEndDate = new Date(currentYear, currentMonth, 0);
      } else if (monthOption === 'twoMonthsAgo') {
        // 2달 전
        actualStartDate = new Date(currentYear, currentMonth - 2, 1);
        actualEndDate = new Date(currentYear, currentMonth - 1, 0);
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
  }, [timeRange, weekOption, monthOption, species]);
  
  // 차트 타이틀 생성
  const getChartTitle = () => {
    switch(timeRange) {
      case 'hour':
        return translate('시간별 종 분포', 'Hourly Species Distribution', 'Distribusi Spesies per Jam', language);
      case 'day':
        return translate('일별 종 분포', 'Daily Species Distribution', 'Distribusi Spesies Harian', language);
      case 'week':
        return translate('주간별 종 분포', 'Weekly Species Distribution', 'Distribusi Spesies Mingguan', language);
      case 'month':
        return translate('월별 종 분포', 'Monthly Species Distribution', 'Distribusi Spesies Bulanan', language);
      default:
        return translate('기간별 종 분포', 'Species Distribution by Period', 'Distribusi Spesies per Periode', language);
    }
  };
  
  return (
    <Box sx={{ p: 3 }}>
      {/* 기간별 필터링 UI */}
      <Paper sx={{ p: 2, mb: 3, bgcolor: '#0c1e36', border: '1px solid #1e3a5a' }}>
        <Typography variant="h6" sx={{ mb: 2 }}>
          {translate('종별 감지 통계 설정', 'Species Detection Statistics Settings', 'Pengaturan Statistik Deteksi Spesies', language)}
        </Typography>
        
        <Box sx={{ mb: 3 }}>
          <Typography variant="subtitle2" sx={{ mb: 1 }}>
            {translate('조류 종류', 'Bird Species', 'Spesies Burung', language)}
          </Typography>
          <FormControl fullWidth size="small">
            <Select
              value={species}
              onChange={(e) => setSpecies(e.target.value)}
              displayEmpty
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
        </Box>
        
        <Typography variant="subtitle2" sx={{ mb: 1 }}>
          {translate('기간 선택', 'Select Period', 'Pilih Periode', language)}
        </Typography>
        <ToggleButtonGroup
          value={timeRange}
          exclusive
          onChange={handleTimeRangeChange}
          aria-label="time range"
          fullWidth
          size="small"
          sx={{ 
            mb: 2,
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
        
        <Box sx={{ mb: 2 }}>
          <Typography variant="subtitle2" sx={{ mb: 1 }}>
            {translate('차트 타입', 'Chart Type', 'Jenis Grafik', language)}
          </Typography>
          <ToggleButtonGroup
            value={chartType}
            exclusive
            onChange={handleChartTypeChange}
            aria-label="chart type"
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
            <ToggleButton value="bar" aria-label="bar">
              {translate('막대 차트', 'Bar Chart', 'Grafik Batang', language)}
            </ToggleButton>
            <ToggleButton value="pie" aria-label="pie">
              {translate('파이 차트', 'Pie Chart', 'Grafik Lingkaran', language)}
            </ToggleButton>
          </ToggleButtonGroup>
        </Box>
        
        <Divider sx={{ my: 2 }} />
        
        <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2, alignItems: 'flex-end' }}>
          {timeRange === 'week' && (
            <FormControl sx={{ minWidth: 200 }} size="small">
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
          )}
          
          {timeRange === 'month' && (
            <FormControl sx={{ minWidth: 200 }} size="small">
              <InputLabel id="month-option-label">
                {translate('월 옵션', 'Month Option', 'Opsi Bulan', language)}
              </InputLabel>
              <Select
                labelId="month-option-label"
                value={monthOption}
                label={translate('월 옵션', 'Month Option', 'Opsi Bulan', language)}
                onChange={(e) => setMonthOption(e.target.value)}
              >
                <MenuItem value="thisMonth">{translate('이번 달', 'This Month', 'Bulan Ini', language)}</MenuItem>
                <MenuItem value="lastMonth">{translate('지난 달', 'Last Month', 'Bulan Lalu', language)}</MenuItem>
                <MenuItem value="twoMonthsAgo">{translate('2달 전', '2 Months Ago', '2 Bulan Lalu', language)}</MenuItem>
              </Select>
            </FormControl>
          )}
          
          {timeRange === 'custom' && (
            <>
              <LocalizationProvider dateAdapter={AdapterDateFns}>
                <DatePicker
                  label={translate('시작일', 'Start Date', 'Tanggal Mulai', language)}
                  value={startDate}
                  onChange={(newValue) => setStartDate(newValue)}
                  renderInput={(params) => <TextField {...params} size="small" sx={{ minWidth: 150 }} />}
                />
              </LocalizationProvider>
              
              <LocalizationProvider dateAdapter={AdapterDateFns}>
                <DatePicker
                  label={translate('종료일', 'End Date', 'Tanggal Selesai', language)}
                  value={endDate}
                  onChange={(newValue) => setEndDate(newValue)}
                  renderInput={(params) => <TextField {...params} size="small" sx={{ minWidth: 150 }} />}
                />
              </LocalizationProvider>
            </>
          )}
          
          <Button 
            variant="contained" 
            color="primary" 
            onClick={applyFilters}
            disabled={loading}
            startIcon={loading ? <CircularProgress size={20} color="inherit" /> : null}
          >
            {loading
              ? translate('로딩 중...', 'Loading...', 'Memuat...', language)
              : translate('적용', 'Apply', 'Terapkan', language)
            }
          </Button>
        </Box>
      </Paper>

      <Typography variant="h6" sx={{ mb: 2 }}>
        {translate(
          `${getChartTitle()} 종별 감지 분포 상세`, 
          `${getChartTitle()} Detailed Species Detection Distribution`, 
          `${getChartTitle()} Distribusi Deteksi Spesies Terperinci`, 
          language
        )}
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
          <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: 400 }}>
            <CircularProgress />
          </Box>
        ) : chartType === 'bar' ? (
          <BarChartD3
            data={
              timeRange === 'hour' ? 
                speciesData.hourlyDistribution?.map(item => ({
                  hour: item.hour.toString().padStart(2, '0') + ':00',
                  value: item.count,
                  species: translateSpecies(item.species, language)
                })) :
              timeRange === 'day' ? 
                speciesData.dailyDistribution?.map(item => {
                  const date = new Date(item.date);
                  const days = ['일', '월', '화', '수', '목', '금', '토'];
                  return {
                    name: days[date.getDay()],
                    value: item.count,
                    species: translateSpecies(item.species, language)
                  };
                }) :
              timeRange === 'week' ? 
                speciesData.weeklyDistribution?.map(item => {
                  const date = new Date(item.date);
                  const month = date.getMonth() + 1;
                  const weekOfMonth = Math.ceil(date.getDate() / 7);
                  return {
                    name: `${month}월 ${weekOfMonth}주차`,
                    value: item.count,
                    species: translateSpecies(item.species, language)
                  };
                }) :
                prepareSpeciesDistributionData(speciesData.distribution, language)
            }
            xKey={
              timeRange === 'hour' ? 'hour' : 
              'name'
            }
            yKey="value"
            xLabel={
              timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
              timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
              timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
              translate('종', 'Species', 'Spesies', language)
            }
            yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
            color="#0088FE"
            tooltipLabel={translate('건', ' detections', ' deteksi', language)}
            height={400}
            isDateTick={timeRange !== 'hour' && timeRange !== 'month'}
            periodType={timeRange}
          />
        ) : (
          <PieChartD3
            data={prepareSpeciesDistributionData(speciesData.distribution, language)}
            nameKey="name"
            valueKey="value"
            colors={['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d']}
            height={400}
          />
        )}
      </Paper>

      <Typography variant="h6" sx={{ mb: 2 }}>
        {translate('위험도별 종 분포', 'Species Distribution by Risk Level', 'Distribusi Spesies berdasarkan Tingkat Risiko', language)}
      </Typography>
      {speciesData.distribution && speciesData.distribution.length > 0 ? (
        <Paper 
          sx={{ 
            p: 2, 
            bgcolor: '#0c1e36', 
            border: '1px solid #1e3a5a',
            mb: 3
          }}
        >
          <BarChartD3
            data={speciesData.distribution.map(item => ({
              name: translateSpecies(item.species, language),
              value: item.count,
              risk: item.riskLevel
            }))}
            xKey="name"
            yKey="value"
            xLabel={translate('종', 'Species', 'Spesies', language)}
            yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
            color="#FFBB28"
            tooltipLabel={translate('건', ' detections', ' deteksi', language)}
            height={400}
          />
        </Paper>
      ) : (
        <Typography variant="body2" color="text.secondary" sx={{ mb: 2 }}>
          {translate('이 차트는 추후 구현될 예정입니다.', 'This chart will be implemented in the future.', 'Grafik ini akan diimplementasikan di masa mendatang.', language)}
        </Typography>
      )}
    </Box>
  );
};

export default SpeciesAnalysisTab; 