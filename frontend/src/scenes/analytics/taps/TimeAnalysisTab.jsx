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
import LineChartD3 from '../../../components/charts/LineChartD3';
import { 
  prepareMonthlyData, 
  prepareHourlyData, 
  prepareDailyData,
  prepareWeeklyData,
  formatDate 
} from '../utils/formatHelpers';

/**
 * 시간별 분석 탭 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드
 * @param {Object} props.timeData 시간 데이터
 * @param {Function} props.loadData 데이터 로딩 함수
 * @param {boolean} props.loading 로딩 중 여부
 * @returns {JSX.Element} 시간별 분석 탭 컴포넌트
 */
const TimeAnalysisTab = ({ language, timeData, loadData, loading }) => {
  const [timeRange, setTimeRange] = useState('week');
  const [startDate, setStartDate] = useState(new Date(Date.now() - 7 * 24 * 60 * 60 * 1000));
  const [endDate, setEndDate] = useState(new Date());
  const [weekOption, setWeekOption] = useState('thisWeek');
  const [monthOption, setMonthOption] = useState('thisMonth');
  const [chartType, setChartType] = useState('line');

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
    
    await loadData(timeRange, actualStartDate, actualEndDate);
  };
  
  // 초기 필터 적용
  useEffect(() => {
    applyFilters();
  }, [timeRange]); // 기간이 변경될 때만 자동으로 적용
  
  // 차트 타이틀 생성
  const getChartTitle = () => {
    switch(timeRange) {
      case 'hour':
        return translate('시간별 감지 추이', 'Hourly Detection Trend', 'Tren Deteksi per Jam', language);
      case 'day':
        return translate('일별 감지 추이', 'Daily Detection Trend', 'Tren Deteksi Harian', language);
      case 'week':
        return translate('주간별 감지 추이', 'Weekly Detection Trend', 'Tren Deteksi Mingguan', language);
      case 'month':
        return translate('월별 감지 추이', 'Monthly Detection Trend', 'Tren Deteksi Bulanan', language);
      default:
        return translate('기간별 감지 추이', 'Detection Trend by Period', 'Tren Deteksi per Periode', language);
    }
  };
  
  return (
    <Box sx={{ p: 3 }}>
      {/* 기간별 필터링 UI */}
      <Paper sx={{ p: 2, mb: 3, bgcolor: '#0c1e36', border: '1px solid #1e3a5a' }}>
        <Typography variant="h6" sx={{ mb: 2 }}>
          {translate('기간별 감지 통계', 'Detection Statistics by Period', 'Statistik Deteksi per Periode', language)}
        </Typography>
        
        <Box sx={{ mb: 3 }}>
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
        </Box>
        
        <Box sx={{ mb: 3 }}>
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
            <ToggleButton value="line" aria-label="line">
              {translate('선 차트', 'Line Chart', 'Grafik Garis', language)}
            </ToggleButton>
            <ToggleButton value="bar" aria-label="bar">
              {translate('막대 차트', 'Bar Chart', 'Grafik Batang', language)}
            </ToggleButton>
          </ToggleButtonGroup>
        </Box>
        
        <Divider sx={{ my: 2 }} />
        
        <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2, mb: 2 }}>
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
            sx={{ alignSelf: 'flex-end' }}
          >
            {loading
              ? translate('로딩 중...', 'Loading...', 'Memuat...', language)
              : translate('적용', 'Apply', 'Terapkan', language)
            }
          </Button>
        </Box>
      </Paper>

      <Typography variant="h6" sx={{ mb: 2 }}>
        {getChartTitle()}
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
          chartType === 'line' ? (
            <LineChartD3
              data={
                timeRange === 'hour' ? prepareHourlyData(timeData.hourly) :
                timeRange === 'day' ? prepareDailyData(timeData.daily, language) :
                timeRange === 'week' ? prepareWeeklyData(timeData.daily, language) :
                monthOption === 'thisMonth' ? 
                  (() => {
                    // 이번 달 1일부터 말일까지 표시
                    const now = new Date();
                    const currentMonth = now.getMonth();
                    const currentYear = now.getFullYear();
                    const daysInMonth = new Date(currentYear, currentMonth + 1, 0).getDate();
                    
                    return Array.from({length: daysInMonth}, (_, i) => {
                      const day = i + 1;
                      const dateStr = `${currentYear}-${(currentMonth + 1).toString().padStart(2, '0')}-${day.toString().padStart(2, '0')}`;
                      
                      // 해당 날짜의 데이터 찾기
                      const dayData = timeData.daily && timeData.daily.find(item => {
                        const itemDate = new Date(item.date);
                        return itemDate.getDate() === day && itemDate.getMonth() === currentMonth;
                      });
                      
                      return {
                        name: `${currentMonth + 1}/${day}`,
                        count: dayData ? dayData.count : 0
                      };
                    });
                  })() : 
                prepareMonthlyData(timeData.monthly, language)
              }
              xKey={
                timeRange === 'hour' ? 'hour' : 
                timeRange === 'month' && monthOption !== 'thisMonth' ? 'month' : 
                'name'
              }
              yKeys={['count']}
              colors={['#00C49F']}
              labels={[translate('감지 수', 'Detections', 'Deteksi', language)]}
              xLabel={
                timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
                timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
                timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
                monthOption === 'thisMonth' ? translate('일', 'Day', 'Hari', language) :
                translate('월', 'Month', 'Bulan', language)
              }
              yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
              height={300}
              isDateTick={false}
              periodType={timeRange}
            />
          ) : (
            <BarChartD3
              data={
                timeRange === 'hour' ? prepareHourlyData(timeData.hourly) :
                timeRange === 'day' ? prepareDailyData(timeData.daily, language) :
                timeRange === 'week' ? prepareWeeklyData(timeData.daily, language) :
                monthOption === 'thisMonth' ? 
                  (() => {
                    // 이번 달 1일부터 말일까지 표시
                    const now = new Date();
                    const currentMonth = now.getMonth();
                    const currentYear = now.getFullYear();
                    const daysInMonth = new Date(currentYear, currentMonth + 1, 0).getDate();
                    
                    return Array.from({length: daysInMonth}, (_, i) => {
                      const day = i + 1;
                      const dateStr = `${currentYear}-${(currentMonth + 1).toString().padStart(2, '0')}-${day.toString().padStart(2, '0')}`;
                      
                      // 해당 날짜의 데이터 찾기
                      const dayData = timeData.daily && timeData.daily.find(item => {
                        const itemDate = new Date(item.date);
                        return itemDate.getDate() === day && itemDate.getMonth() === currentMonth;
                      });
                      
                      return {
                        name: `${currentMonth + 1}/${day}`,
                        count: dayData ? dayData.count : 0
                      };
                    });
                  })() : 
                prepareMonthlyData(timeData.monthly, language)
              }
              xKey={
                timeRange === 'hour' ? 'hour' : 
                timeRange === 'month' && monthOption !== 'thisMonth' ? 'month' : 
                'name'
              }
              yKey="count"
              xLabel={
                timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
                timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
                timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
                monthOption === 'thisMonth' ? translate('일', 'Day', 'Hari', language) :
                translate('월', 'Month', 'Bulan', language)
              }
              yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
              color="#00C49F"
              tooltipLabel={translate('건', ' detections', ' deteksi', language)}
              height={300}
              isDateTick={false}
              periodType={timeRange}
            />
          )
        )}
      </Paper>

      <Typography variant="h6" sx={{ mb: 2 }}>
        {translate('시간별 감지 활동 상세', 'Detailed Hourly Detection Activity', 'Aktivitas Deteksi per Jam Terperinci', language)}
      </Typography>
      <Paper 
        sx={{ 
          p: 2, 
          bgcolor: '#0c1e36', 
          border: '1px solid #1e3a5a',
          height: 300
        }}
      >
        <BarChartD3
          data={prepareHourlyData(timeData.hourly)}
          xKey="hour"
          yKey="count"
          xLabel={translate('시간', 'Hour', 'Jam', language)}
          yLabel={translate('감지 수', 'Detections', 'Deteksi', language)}
          color="#8884d8"
          tooltipLabel={translate('건', ' detections', ' deteksi', language)}
          height={300}
        />
      </Paper>
    </Box>
  );
};

export default TimeAnalysisTab; 