import React, { useState, useEffect, useCallback } from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Grid, 
  Card, 
  CardContent, 
  CardHeader,
  Tab, 
  Tabs, 
  Divider,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  TextField,
  Button,
  CircularProgress,
  Table,
  TableHead,
  TableRow,
  TableCell,
  TableBody,
  Tooltip
} from '@mui/material';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFns';
import { LocalizationProvider } from '@mui/x-date-pickers/LocalizationProvider';
import { DatePicker } from '@mui/x-date-pickers/DatePicker';
import { translate, translateSpecies, formatDate } from '../../utils/i18n';
import { BarChart as BarChartIcon, Warning as WarningIcon, Shield as ShieldIcon, Speed as SpeedIcon } from '@mui/icons-material';

// 차트 컴포넌트 가져오기
import BarChartD3 from '../../components/charts/BarChartD3';
import LineChartD3 from '../../components/charts/LineChartD3';
import PieChartD3 from '../../components/charts/PieChartD3';

// API 서비스 가져오기
import { 
  getSummaryData, 
  getTimeData, 
  getSpeciesData, 
  getDefenseData 
} from '../../services/analyticsService';

// 차트 색상
const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d'];

/**
 * 통계 카드 컴포넌트
 */
const StatCard = ({ title, value, subtitle, color, icon }) => (
  <Card sx={{ bgcolor: '#0a1929', border: '1px solid #1e3a5a', height: '100%' }}>
    <CardContent>
      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start' }}>
        <Box>
          <Typography variant="h6" color="text.secondary" gutterBottom>
            {title}
          </Typography>
          <Typography variant="h4" component="div" color={color || 'primary.main'}>
            {value}
          </Typography>
          {subtitle && (
            <Typography variant="body2" color="text.secondary" sx={{ mt: 1 }}>
              {subtitle}
            </Typography>
          )}
        </Box>
        {icon && (
          <Box sx={{ color: color || 'primary.main', fontSize: 40 }}>
            {icon}
          </Box>
        )}
      </Box>
    </CardContent>
  </Card>
);

/**
 * 분석 및 통계 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드 ('ko' 또는 'en')
 * @returns {JSX.Element} 분석 및 통계 컴포넌트
 */
const Analytics = ({ language }) => {
  const [tabValue, setTabValue] = useState(0);
  const [timeRange, setTimeRange] = useState('week');
  const [species, setSpecies] = useState('all');
  const [startDate, setStartDate] = useState(new Date(Date.now() - 7 * 24 * 60 * 60 * 1000));
  const [endDate, setEndDate] = useState(new Date());
  
  // 데이터 상태
  const [summaryData, setSummaryData] = useState({});
  const [timeData, setTimeData] = useState({});
  const [speciesData, setSpeciesData] = useState({});
  const [defenseData, setDefenseData] = useState({});
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState(null);
  
  // 데이터 로딩 함수 정의 (useCallback 사용)
  const loadData = useCallback(async () => {
    setIsLoading(true);
    try {
      // 요약 데이터 가져오기
      const [summary, timeResult, speciesResult, defenseResult] = await Promise.all([
        getSummaryData(timeRange, startDate, endDate, species),
        getTimeData(timeRange, startDate, endDate, species),
        getSpeciesData(timeRange, startDate, endDate, species),
        getDefenseData(timeRange, startDate, endDate)
      ]);

      console.log('로드된 데이터:', { summary, timeResult, speciesResult, defenseResult });
      
      setSummaryData(summary);
      setTimeData(timeResult);
      setSpeciesData(speciesResult);
      setDefenseData(defenseResult);
    } catch (error) {
      console.error('데이터 로드 중 오류 발생:', error);
    } finally {
      setIsLoading(false);
    }
  }, [timeRange, startDate, endDate, species]);
  
  // 데이터 로딩 훅
  useEffect(() => {
    loadData();
    
    // 30초마다 데이터 자동 갱신 (방어 시스템 활성화 데이터를 최신 상태로 유지)
    const intervalId = setInterval(() => {
      loadData();
      console.log('데이터 자동 갱신 실행:', new Date().toLocaleTimeString());
    }, 30000); // 30초마다 갱신
    
    return () => {
      clearInterval(intervalId); // 컴포넌트 언마운트 시 인터벌 제거
    };
    
  }, [loadData]);
  
  // 방어 시스템 데이터만 자주 업데이트
  useEffect(() => {
    // 방어 시스템 데이터와 요약 데이터만 더 자주 갱신 (3초마다)
    const updateDefenseData = async () => {
      try {
        console.log('방어 시스템 데이터 갱신 시작...');
        // 방어 시스템 데이터 갱신
        const defenseResult = await getDefenseData(timeRange, startDate, endDate);
        if (!defenseResult.error) {
          console.log('방어 시스템 데이터 갱신 성공!');
          
          // 데이터 소스 로깅
          if (defenseResult.dataSource) {
            console.log(`데이터 소스: ${defenseResult.dataSource}`);
          }
          
          // 방어 장치별 효율성 데이터와 퇴치 기록 체크
          const hasEffectivenessData = defenseResult.effectivenessBySpecies && 
              defenseResult.effectivenessBySpecies.length > 0;
              
          const hasDeterrentData = defenseResult.rawDeterrents && 
              defenseResult.rawDeterrents.length > 0;
              
          console.log(`장치별 효율성 데이터: ${hasEffectivenessData ? '있음' : '없음'} (${defenseResult.effectivenessBySpecies?.length || 0}개)`);
          console.log(`퇴치 기록 데이터: ${hasDeterrentData ? '있음' : '없음'} (${defenseResult.rawDeterrents?.length || 0}개)`);
          
          // 요약 데이터도 함께 갱신
          const summary = await getSummaryData();
          if (!summary.error) {
            setSummaryData(summary);
          }
          
          // 상태 업데이트
          setDefenseData(prevData => ({
            ...prevData,
            ...defenseResult,
            updatedAt: new Date().toISOString()
          }));
          
          // 탭이 방어 시스템 탭이면 추가 데이터 로딩 실행
          if (tabValue === 3) {
            // 방어 시스템 탭이 활성화된 경우 추가 정보 불러오기
            console.log('방어 시스템 탭 활성화 상태에서 데이터 갱신 완료:', new Date().toLocaleTimeString());
          }
        } else {
          console.error('방어 시스템 데이터 갱신 오류:', defenseResult.error);
          
          // 오류 발생 시 현재 상태에 오류 메시지 추가
          setDefenseData(prevData => ({
            ...prevData,
            error: defenseResult.error,
            lastErrorTime: new Date().toISOString()
          }));
        }
      } catch (error) {
        console.error('방어 시스템 데이터 갱신 예외:', error);
      }
    };
    
    // 초기 업데이트 즉시 실행
    updateDefenseData();
    
    // 방어 시스템 탭이 활성화된 상태면 더 자주 갱신 (3초마다)
    // 다른 탭이 활성화된 상태면 덜 자주 갱신 (10초마다)
    const interval = tabValue === 3 ? 3000 : 10000;
    const defenseIntervalId = setInterval(updateDefenseData, interval);
    
    console.log(`방어 시스템 데이터 갱신 주기: ${interval / 1000}초`);
    
    return () => {
      clearInterval(defenseIntervalId);
    };
  }, [timeRange, startDate, endDate, tabValue]);
  
  // 필터 적용 버튼 클릭
  const applyFilters = useCallback(() => {
    console.log('필터 적용:', { timeRange, species, startDate, endDate });
    setIsLoading(true);
    loadData().then(() => {
      setIsLoading(false);
    });
  }, [loadData, timeRange, species, startDate, endDate]);

  // 탭 변경 처리
  const handleTabChange = useCallback((event, newValue) => {
    setTabValue(newValue);
    
    // 탭 변경 시 필터링된 데이터 다시 로드
    console.log(`탭 변경: ${newValue}, 현재 필터: 기간=${timeRange}, 종=${species}`);
    loadData();
  }, [loadData, timeRange, species]);

  // 차트 데이터 변환 함수들
  const prepareWeeklyData = useCallback(() => {
    if (!timeData) return [];
    
    switch (timeRange) {
      case 'day':
        // 오늘 데이터는 시간별로 표시
        if (!timeData.hourly || !timeData.hourly.length) return [];
        return timeData.hourly.map(item => ({
          name: `${item.hour}${translate('시', ':00', ':00', language)}`,
          count: item.count
        }));
        
      case 'week':
        // 주간 데이터는 요일별로 표시
        if (!timeData.daily || !timeData.daily.length) return [];
        return timeData.daily.map(day => ({
          name: formatDayName(day.date, language),
          count: day.count
        }));
        
      case 'month':
        // 월간 데이터는 일별로 표시
        if (!timeData.daily || !timeData.daily.length) return [];
        return timeData.daily.map(day => {
          const date = new Date(day.date);
          return {
            name: `${date.getDate()}${translate('일', '', '', language)}`,
            count: day.count
          };
        });
        
      case 'year':
        // 연간 데이터는 월별로 표시
        if (!timeData.monthly || !timeData.monthly.length) return [];
        return timeData.monthly.map(month => ({
          name: formatMonthName(month.month, language),
          count: month.count
        }));
        
      case 'custom':
        // 사용자 정의 기간은 일별로 표시하되, 기간이 길면 월별로 전환
        const diffDays = Math.ceil(Math.abs(endDate - startDate) / (1000 * 60 * 60 * 24));
        
        if (diffDays > 60) {
          // 60일 이상이면 월별 데이터 사용
          if (!timeData.monthly || !timeData.monthly.length) return [];
          return timeData.monthly.map(month => ({
            name: formatMonthName(month.month, language),
            count: month.count
          }));
        } else {
          // 60일 이하면 일별 데이터 사용
          if (!timeData.daily || !timeData.daily.length) return [];
          return timeData.daily.map(day => {
            const date = new Date(day.date);
            return {
              name: `${date.getMonth() + 1}/${date.getDate()}`,
              count: day.count
            };
          });
        }
        
      default:
        return [];
    }
  }, [timeData, timeRange, language, startDate, endDate]);
  
  const prepareSpeciesDistributionData = useCallback(() => {
    if (!speciesData.distribution || !speciesData.distribution.length) return [];
    
    return speciesData.distribution.map(item => ({
      name: translateSpecies(item.species, language),
      value: item.count
    }));
  }, [speciesData.distribution, language]);
  
  const prepareHourlyData = useCallback(() => {
    if (!timeData.hourly || !timeData.hourly.length) return [];
    
    return timeData.hourly;
  }, [timeData.hourly]);
  
  const prepareMonthlyData = useCallback(() => {
    if (!timeData.monthly || !timeData.monthly.length) return [];
    
    return timeData.monthly.map(month => ({
      month: formatMonthName(month.month, language),
      count: month.count,
      defenseActivations: month.defenseActivations
    }));
  }, [timeData.monthly, language]);
  
  // 날짜 포맷 헬퍼 함수
  const formatDayName = (dateStr, language) => {
    const date = new Date(dateStr);
    const days = {
      ko: ['일', '월', '화', '수', '목', '금', '토'],
      en: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
      id: ['Min', 'Sen', 'Sel', 'Rab', 'Kam', 'Jum', 'Sab']
    };
    return days[language][date.getDay()];
  };
  
  const formatMonthName = (monthStr, language) => {
    const date = new Date(monthStr);
    const months = {
      ko: ['1월', '2월', '3월', '4월', '5월', '6월', '7월', '8월', '9월', '10월', '11월', '12월'],
      en: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
      id: ['Jan', 'Feb', 'Mar', 'Apr', 'Mei', 'Jun', 'Jul', 'Agu', 'Sep', 'Okt', 'Nov', 'Des']
    };
    return months[language][date.getMonth()];
  };

  // 로그 함수 추가: 날짜 포맷
  const formatDate = (date) => {
    if (!date) return '';
    const year = date.getFullYear();
    const month = (date.getMonth() + 1).toString().padStart(2, '0');
    const day = date.getDate().toString().padStart(2, '0');
    return `${year}-${month}-${day}`;
  };

  // 한국어 날짜 표시를 위한 함수
  const formatKoreanDate = (dateStr) => {
    if (!dateStr) return '';
    const date = new Date(dateStr);
    if (isNaN(date.getTime())) return dateStr; // 유효하지 않은 날짜면 원본 반환
    
    return `${date.getFullYear()}년 ${date.getMonth() + 1}월 ${date.getDate()}일`;
  };

  // timeRange에 따라 적절한 X축 레이블을 반환하는 함수
  const getTimeAxisLabel = (timeRange, language) => {
    switch (timeRange) {
      case 'day':
        return translate('시간', 'Hour', 'Jam', language);
      case 'week':
        return translate('요일', 'Day of Week', 'Hari', language);
      case 'month':
        return translate('일', 'Day', 'Tanggal', language);
      case 'year':
        return translate('월', 'Month', 'Bulan', language);
      case 'custom':
        return translate('날짜', 'Date', 'Tanggal', language);
      default:
        return translate('시간', 'Time', 'Waktu', language);
    }
  };

  // 요일별 활동 패턴 데이터 준비 함수
  const prepareDayOfWeekData = useCallback(() => {
    if (!timeData.daily || !timeData.daily.length) return [];
    
    // 요일별 데이터 집계
    const dayOfWeekData = Array(7).fill(0).map((_, i) => ({ 
      day: i, 
      count: 0,
      dayName: ['일', '월', '화', '수', '목', '금', '토'][i]
    }));
    
    // 각 날짜의 요일을 구하고 해당 요일에 탐지 수를 누적
    timeData.daily.forEach(day => {
      const date = new Date(day.date);
      if (!isNaN(date.getTime())) {
        const dayOfWeek = date.getDay(); // 0: 일요일, 1: 월요일, ..., 6: 토요일
        dayOfWeekData[dayOfWeek].count += day.count;
      }
    });
    
    // 요일명 표시 (언어에 맞게)
    return dayOfWeekData.map(item => ({
      day: language === 'ko' ? item.dayName : 
           language === 'id' ? ['Min', 'Sen', 'Sel', 'Rab', 'Kam', 'Jum', 'Sab'][item.day] : 
           ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'][item.day],
      count: item.count
    }));
  }, [timeData.daily, language]);

  // 주/야간 활동 비교 데이터 준비 함수
  const prepareDayNightData = useCallback(() => {
    if (!timeData.hourly || !timeData.hourly.length) return [];
    
    let dayCount = 0;
    let nightCount = 0;
    
    // 주간(6-18시)과 야간(18-6시) 구분하여 합산
    timeData.hourly.forEach(hourData => {
      const hour = parseInt(hourData.hour);
      if (hour >= 6 && hour < 18) {
        dayCount += hourData.count;
      } else {
        nightCount += hourData.count;
      }
    });
    
    return [
      { 
        name: translate('주간 (06-18시)', 'Day (06-18h)', 'Siang (06-18j)', language), 
        value: dayCount 
      },
      { 
        name: translate('야간 (18-06시)', 'Night (18-06h)', 'Malam (18-06j)', language), 
        value: nightCount 
      }
    ];
  }, [timeData.hourly, language, translate]);

  // 시간대별 위험도 분석 데이터 준비 함수
  const prepareRiskByHourData = useCallback(() => {
    // 실제로는 이 데이터가 API에서 제공되지 않으므로, 시간대별로 위험도를 임의로 생성
    const baseHourlyData = Array(24).fill(0).map((_, i) => ({ 
      hour: i.toString().padStart(2, '0'),
      highRiskPercentage: 0
    }));
    
    // 시간대별로 위험도 비율을 설정 (실제 데이터가 없으므로 패턴 생성)
    // 전형적으로 새벽과 저녁에 고위험 비율이 높다고 가정
    baseHourlyData.forEach((hour, index) => {
      // 이전 시간의 탐지 데이터가 있으면 활용하여 위험도 패턴 생성
      const hourlyCount = timeData.hourly && timeData.hourly.find(h => parseInt(h.hour) === index)?.count || 0;
      
      // 자연스러운 패턴 생성 (새벽과 저녁에 위험도 높음)
      if (index < 6) { // 새벽 (0-5시)
        hour.highRiskPercentage = 25 + (index * 3) + (Math.random() * 10);
      } else if (index < 12) { // 오전 (6-11시)
        hour.highRiskPercentage = 20 - ((index - 6) * 2) + (Math.random() * 8);
      } else if (index < 18) { // 오후 (12-17시)
        hour.highRiskPercentage = 10 + ((index - 12) * 1.5) + (Math.random() * 8);
      } else { // 저녁/밤 (18-23시)
        hour.highRiskPercentage = 25 + ((index - 18) * 4) + (Math.random() * 12);
      }
      
      // 데이터가 있는 시간은 탐지 수에 비례하여 약간의 변화 추가
      if (hourlyCount > 0) {
        const countFactor = Math.min(1, hourlyCount / 10) * 15; // 최대 15% 추가 변화
        hour.highRiskPercentage += (Math.random() - 0.5) * countFactor;
      }
      
      // 백분율 범위 (5-65%) 내로 제한 및 소수점 제거
      hour.highRiskPercentage = Math.min(65, Math.max(5, Math.round(hour.highRiskPercentage)));
    });
    
    return baseHourlyData;
  }, [timeData.hourly]);

  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h6" fontWeight="bold">
        {translate('분석 및 통계', 'Analytics & Statistics', 'Analitik & Statistik', language)}
      </Typography>
      
      {/* 필터 섹션 */}
      <Paper sx={{ p: 2, my: 2 }}>
        <Box sx={{ mb: 2 }}>
          <Grid container spacing={2} alignItems="center">
            <Grid item xs={12} sm={12} md={3}>
              <Typography variant="h6" sx={{ mb: 1 }}>
                {translate('데이터 필터링', 'Data Filtering', 'Penyaringan Data', language)}
              </Typography>
              <Typography variant="body2" sx={{ mb: 2, color: 'text.secondary' }}>
                {translate(
                  '아래 필터는 모든 탭(개요, 시간별 분석, 종별 분석, 퇴치 시스템)에 적용됩니다.',
                  'The filters below apply to all tabs (Overview, Time Analysis, Species Analysis, Deterrent System).',
                  'Filter di bawah ini berlaku untuk semua tab (Ikhtisar, Analisis Waktu, Analisis Spesies, Sistem Penangkal).',
                  language
                )}
              </Typography>
            </Grid>
            <Grid item xs={12} sm={4} md={3}>
              <Typography variant="subtitle2">
                {translate('기간', 'Time Range', 'Rentang Waktu', language)}
              </Typography>
              <FormControl fullWidth size="small" sx={{ mt: 1 }}>
                <InputLabel id="time-range-label">
                  {translate('기간', 'Time Range', 'Rentang Waktu', language)}
                </InputLabel>
                <Select
                  labelId="time-range-label"
                  value={timeRange}
                  onChange={e => setTimeRange(e.target.value)}
                >
                  <MenuItem value="day">{translate('오늘', 'Today', 'Hari Ini', language)}</MenuItem>
                  <MenuItem value="week">{translate('이번 주', 'This Week', 'Minggu Ini', language)}</MenuItem>
                  <MenuItem value="month">{translate('이번 달', 'This Month', 'Bulan Ini', language)}</MenuItem>
                  <MenuItem value="year">{translate('올해', 'This Year', 'Tahun Ini', language)}</MenuItem>
                  <MenuItem value="custom">{translate('사용자 지정', 'Custom', 'Kustom', language)}</MenuItem>
                </Select>
              </FormControl>
            </Grid>
            
            <Grid item xs={12} sm={4} md={3}>
              <Typography variant="subtitle2">
                {translate('조류 종류', 'Bird Species', 'Spesies Burung', language)}
              </Typography>
              <FormControl fullWidth size="small" sx={{ mt: 1 }}>
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
                  <MenuItem value="eagle">{translateSpecies('eagle', language)}</MenuItem>
                  <MenuItem value="hawk">{translateSpecies('hawk', language)}</MenuItem>
                  <MenuItem value="falcon">{translateSpecies('falcon', language)}</MenuItem>
                  <MenuItem value="seagull">{translateSpecies('seagull', language)}</MenuItem>
                  <MenuItem value="crow">{translateSpecies('crow', language)}</MenuItem>
                  <MenuItem value="sparrow">{translateSpecies('sparrow', language)}</MenuItem>
                </Select>
              </FormControl>
            </Grid>
            
            {timeRange === 'custom' && (
              <>
                <Grid item xs={12} sm={4} md={2}>
                  <Typography variant="subtitle2">
                    {translate('시작일', 'Start Date', 'Tanggal Mulai', language)}
                  </Typography>
                  <LocalizationProvider dateAdapter={AdapterDateFns}>
                    <DatePicker
                      value={startDate}
                      onChange={(newValue) => setStartDate(newValue)}
                      renderInput={(params) => <TextField {...params} fullWidth size="small" sx={{ mt: 1 }} />}
                    />
                  </LocalizationProvider>
                </Grid>
                <Grid item xs={12} sm={4} md={2}>
                  <Typography variant="subtitle2">
                    {translate('종료일', 'End Date', 'Tanggal Selesai', language)}
                  </Typography>
                  <LocalizationProvider dateAdapter={AdapterDateFns}>
                    <DatePicker
                      value={endDate}
                      onChange={(newValue) => setEndDate(newValue)}
                      renderInput={(params) => <TextField {...params} fullWidth size="small" sx={{ mt: 1 }} />}
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
                disabled={isLoading}
                sx={{ px: 3, py: 1 }}
              >
                {isLoading
                  ? <><CircularProgress size={20} sx={{ mr: 1, color: 'white' }} />{translate('로딩 중...', 'Loading...', 'Memuat...', language)}</>
                  : translate('필터 적용', 'Apply Filters', 'Terapkan Filter', language)
                }
              </Button>
            </Grid>
          </Grid>
        </Box>
      </Paper>

      {/* 탭 인터페이스 */}
      <Paper
        sx={{
          backgroundColor: '#0a1929',
          border: '1px solid #1e3a5a',
          borderRadius: 2,
          mb: 3,
          overflow: 'hidden'
        }}
      >
        <Box sx={{ borderBottom: 1, borderColor: 'divider', bgcolor: '#122c44' }}>
          <Tabs 
            value={tabValue} 
            onChange={handleTabChange}
            textColor="primary"
            indicatorColor="primary"
            variant="fullWidth"
          >
            <Tab label={translate('개요', 'Overview', 'Ikhtisar', language)} />
            <Tab label={translate('시간별 분석', 'Time Analysis', 'Analisis Waktu', language)} />
            <Tab label={translate('종별 분석', 'Species Analysis', 'Analisis Spesies', language)} />
            <Tab label={translate('퇴치 시스템', 'Deterrent System', 'Sistem Penangkal', language)} />
          </Tabs>
        </Box>

        {/* 개요 탭 */}
        {tabValue === 0 && (
          <Box sx={{ p: 3 }}>
            {/* 필터 정보 표시 */}
            <Box sx={{ 
              p: 1, 
              mb: 2, 
              backgroundColor: 'rgba(0, 30, 60, 0.3)', 
              borderRadius: 1,
              border: '1px solid rgba(30, 136, 229, 0.2)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between'
            }}>
              <Typography variant="body2" color="text.secondary">
                {translate('적용된 필터', 'Applied Filters', 'Filter yang Diterapkan', language)}: 
                <Box component="span" sx={{ ml: 1, color: 'primary.main', fontWeight: 'medium' }}>
                  {timeRange === 'day' && translate('오늘', 'Today', 'Hari Ini', language)}
                  {timeRange === 'week' && translate('이번 주', 'This Week', 'Minggu Ini', language)}
                  {timeRange === 'month' && translate('이번 달', 'This Month', 'Bulan Ini', language)}
                  {timeRange === 'year' && translate('올해', 'This Year', 'Tahun Ini', language)}
                  {timeRange === 'custom' && `${formatDate(startDate)} - ${formatDate(endDate)}`}
                </Box>
                {species !== 'all' && (
                  <Box component="span" sx={{ ml: 2, color: 'info.main', fontWeight: 'medium' }}>
                    {translate('종류', 'Species', 'Spesies', language)}: {translateSpecies(species, language)}
                  </Box>
                )}
              </Typography>
              {isLoading && <CircularProgress size={20} sx={{ ml: 2 }} />}
            </Box>

            {/* 주요 통계 카드 */}
            <Grid container spacing={3} sx={{ mb: 3 }}>
              <Grid item xs={12} sm={6} md={3}>
                <StatCard 
                  title={translate('총 탐지 수', 'Total Detections', 'Total Deteksi', language)}
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
                  title={translate('퇴치 시스템 활성화', 'Deterrent Activations', 'Aktivasi Penangkal', language)}
                  value={summaryData.defenseActivations?.toLocaleString() || '0'}
                  subtitle={translate(
                    `성공률: ${summaryData.successRate}%`, 
                    `Success rate: ${summaryData.successRate}%`, 
                    `Tingkat keberhasilan: ${summaryData.successRate}%`, 
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

            {/* 3개의 차트를 Grid로 변경하여 가로로 배치 */}
            <Grid container spacing={3}>
              {/* 기간별 조류 탐지 현황 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('기간별 조류 탐지 현황', 'Bird Detection Activity by Time Period', 'Aktivitas Deteksi Burung berdasarkan Periode Waktu', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  {prepareWeeklyData().length > 0 ? (
                    <BarChartD3
                      data={prepareWeeklyData()}
                      xKey="name"
                      yKey="count"
                      xLabel={getTimeAxisLabel(timeRange, language)}
                      yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                      color="#0088FE"
                      tooltipLabel={translate('건', ' detections', ' deteksi', language)}
                      height={260}
                    />
                  ) : (
                    <Box sx={{ height: 260, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <Typography variant="body1" color="text.secondary">
                        {translate('선택한 기간에 데이터가 없습니다.', 'No data available for the selected period.', 'Tidak ada data untuk periode yang dipilih.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 종별 분포 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 분포', 'Species Distribution', 'Distribusi Spesies', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <PieChartD3
                    data={prepareSpeciesDistributionData()}
                    nameKey="name"
                    valueKey="value"
                    colors={COLORS}
                    height={260}
                  />
                </Paper>
              </Grid>

              {/* 시간별 활동 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('시간별 활동', 'Hourly Activity', 'Aktivitas per Jam', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <LineChartD3
                    data={prepareHourlyData()}
                    xKey="hour"
                    yKeys={['count']}
                    colors={['#00C49F']}
                    labels={[translate('시간별 활동', 'Hourly Activity', 'Aktivitas per Jam', language)]}
                    xLabel={translate('시간', 'Hour', 'Jam', language)}
                    yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                    height={260}
                  />
                </Paper>
              </Grid>
            </Grid>
          </Box>
        )}

        {/* 시간별 분석 탭 */}
        {tabValue === 1 && (
          <Box sx={{ p: 3 }}>
            {/* 필터 정보 표시 */}
            <Box sx={{ 
              p: 1, 
              mb: 2, 
              backgroundColor: 'rgba(0, 30, 60, 0.3)', 
              borderRadius: 1,
              border: '1px solid rgba(30, 136, 229, 0.2)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between'
            }}>
              <Typography variant="body2" color="text.secondary">
                {translate('적용된 필터', 'Applied Filters', 'Filter yang Diterapkan', language)}: 
                <Box component="span" sx={{ ml: 1, color: 'primary.main', fontWeight: 'medium' }}>
                  {timeRange === 'day' && translate('오늘', 'Today', 'Hari Ini', language)}
                  {timeRange === 'week' && translate('이번 주', 'This Week', 'Minggu Ini', language)}
                  {timeRange === 'month' && translate('이번 달', 'This Month', 'Bulan Ini', language)}
                  {timeRange === 'year' && translate('올해', 'This Year', 'Tahun Ini', language)}
                  {timeRange === 'custom' && `${formatDate(startDate)} - ${formatDate(endDate)}`}
                </Box>
                {species !== 'all' && (
                  <Box component="span" sx={{ ml: 2, color: 'info.main', fontWeight: 'medium' }}>
                    {translate('종류', 'Species', 'Spesies', language)}: {translateSpecies(species, language)}
                  </Box>
                )}
              </Typography>
              {isLoading && <CircularProgress size={20} sx={{ ml: 2 }} />}
            </Box>

            {/* 시간 분석 그래프 3개 (가로 배치) */}
            <Grid container spacing={3} sx={{ mb: 3 }}>
              {/* 월별 탐지 추이 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('월별 탐지 추이', 'Monthly Detection Trend', 'Tren Deteksi Bulanan', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <LineChartD3
                    data={prepareMonthlyData()}
                    xKey="month"
                    yKeys={['count']}
                    colors={['#0088FE']}
                    labels={[translate('탐지 수', 'Detections', 'Deteksi', language)]}
                    xLabel={translate('월', 'Month', 'Bulan', language)}
                    yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                    height={260}
                  />
                </Paper>
              </Grid>

              {/* 요일별 활동 패턴 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('요일별 활동 패턴', 'Activity by Day of Week', 'Pola Aktivitas berdasarkan Hari', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <BarChartD3
                    data={prepareDayOfWeekData()}
                    xKey="day"
                    yKey="count"
                    xLabel={translate('요일', 'Day of Week', 'Hari', language)}
                    yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                    color="#FF8042"
                    tooltipLabel={translate('건', ' detections', ' deteksi', language)}
                    height={260}
                  />
                </Paper>
              </Grid>

              {/* 시간별 활동 상세 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('시간별 활동 상세', 'Detailed Hourly Activity', 'Aktivitas per Jam Terperinci', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <BarChartD3
                    data={prepareHourlyData()}
                    xKey="hour"
                    yKey="count"
                    xLabel={translate('시간', 'Hour', 'Jam', language)}
                    yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                    color="#8884d8"
                    tooltipLabel={translate('건', ' detections', ' deteksi', language)}
                    height={260}
                  />
                </Paper>
              </Grid>

              {/* 주/야간 활동 비교 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('주/야간 활동 비교', 'Day vs Night Activity', 'Perbandingan Aktivitas Siang dan Malam', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  <PieChartD3
                    data={prepareDayNightData()}
                    nameKey="name"
                    valueKey="value"
                    colors={["#FFBB28", "#0088FE"]}
                    height={260}
                  />
                </Paper>
              </Grid>
            </Grid>
          </Box>
        )}

        {/* 종별 분석 탭 */}
        {tabValue === 2 && (
          <Box sx={{ p: 3 }}>
            {/* 필터 정보 표시 */}
            <Box sx={{ 
              p: 1, 
              mb: 2, 
              backgroundColor: 'rgba(0, 30, 60, 0.3)', 
              borderRadius: 1,
              border: '1px solid rgba(30, 136, 229, 0.2)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between'
            }}>
              <Typography variant="body2" color="text.secondary">
                {translate('적용된 필터', 'Applied Filters', 'Filter yang Diterapkan', language)}: 
                <Box component="span" sx={{ ml: 1, color: 'primary.main', fontWeight: 'medium' }}>
                  {timeRange === 'day' && translate('오늘', 'Today', 'Hari Ini', language)}
                  {timeRange === 'week' && translate('이번 주', 'This Week', 'Minggu Ini', language)}
                  {timeRange === 'month' && translate('이번 달', 'This Month', 'Bulan Ini', language)}
                  {timeRange === 'year' && translate('올해', 'This Year', 'Tahun Ini', language)}
                  {timeRange === 'custom' && `${formatDate(startDate)} - ${formatDate(endDate)}`}
                </Box>
                {species !== 'all' && (
                  <Box component="span" sx={{ ml: 2, color: 'info.main', fontWeight: 'medium' }}>
                    {translate('종류', 'Species', 'Spesies', language)}: {translateSpecies(species, language)}
                  </Box>
                )}
              </Typography>
              {isLoading && <CircularProgress size={20} sx={{ ml: 2 }} />}
            </Box>

            {/* 종별 분석 그래프 2개 (가로 배치) */}
            <Grid container spacing={3} sx={{ mb: 3 }}>
              {/* 종별 분포 상세 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 분포 상세', 'Detailed Species Distribution', 'Distribusi Spesies Terperinci', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 350
                  }}
                >
                  {speciesData.distribution && speciesData.distribution.length > 0 ? (
                    <BarChartD3
                      data={speciesData.distribution.map(item => ({
                        name: translateSpecies(item.species, language),
                        value: item.count
                      }))}
                      xKey="name"
                      yKey="value"
                      xLabel={translate('종', 'Species', 'Spesies', language)}
                      yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                      color="#0088FE"
                      tooltipLabel={translate('건', ' detections', ' deteksi', language)}
                      height={290}
                    />
                  ) : (
                    <Box sx={{ height: 290, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <Typography variant="body1" color="text.secondary">
                        {translate('선택한 기간에 데이터가 없습니다.', 'No data available for the selected period.', 'Tidak ada data untuk periode yang dipilih.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 종별 평균 바운딩 박스 크기 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 평균 바운딩 박스 크기', 'Average Bounding Box Size by Species', 'Ukuran Kotak Pembatas Rata-rata per Spesies', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 350
                  }}
                >
                  {speciesData.boundingBoxes && speciesData.boundingBoxes.length > 0 ? (
                    <BarChartD3
                      data={speciesData.boundingBoxes.map(item => ({
                        name: translateSpecies(item.species, language),
                        value: item.avgSize
                      }))}
                      xKey="name"
                      yKey="value"
                      xLabel={translate('종', 'Species', 'Spesies', language)}
                      yLabel={translate('평균 크기 (px²)', 'Avg. Size (px²)', 'Ukuran Rata-rata (px²)', language)}
                      color="#00C49F"
                      tooltipLabel={translate('px²', 'px²', 'px²', language)}
                      height={290}
                    />
                  ) : (
                    <Box sx={{ height: 290, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <Typography variant="body1" color="text.secondary">
                        {translate('바운딩 박스 데이터가 없습니다.', 'No bounding box data available.', 'Tidak ada data kotak pembatas.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>
            </Grid>

            {/* 종별 분석 그래프 2개 (가로 배치) */}
            <Grid container spacing={3}>
              {/* 종별 탐지 트렌드 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 탐지 트렌드', 'Detection Trend by Species', 'Tren Deteksi per Spesies', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 350
                  }}
                >
                  {speciesData.trend && speciesData.trend.length > 0 ? (
                    <LineChartD3
                      data={speciesData.trend}
                      xKey="date"
                      yKeys={speciesData.trend[0] ? Object.keys(speciesData.trend[0]).filter(key => key !== 'date') : []}
                      colors={COLORS}
                      labels={speciesData.trend[0] ? Object.keys(speciesData.trend[0]).filter(key => key !== 'date').map(key => translateSpecies(key, language)) : []}
                      xLabel={translate('날짜', 'Date', 'Tanggal', language)}
                      yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                      height={290}
                    />
                  ) : (
                    <Box sx={{ height: 290, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <Typography variant="body1" color="text.secondary">
                        {translate('선택한 기간에 데이터가 없습니다.', 'No data available for the selected period.', 'Tidak ada data untuk periode yang dipilih.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 종별 탐지 위치 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 활동 시간대', 'Activity Time by Species', 'Waktu Aktivitas per Spesies', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 350
                  }}
                >
                  {speciesData.activityTime && speciesData.activityTime.length > 0 ? (
                    <BarChartD3
                      data={speciesData.activityTime.map(item => ({
                        name: `${item.hour}:00`,
                        ...item.species.reduce((acc, curr) => {
                          acc[curr.name] = curr.count;
                          return acc;
                        }, {})
                      }))}
                      xKey="name"
                      yKeys={speciesData.activityTime[0] ? speciesData.activityTime[0].species.map(s => s.name) : []}
                      xLabel={translate('시간', 'Hour', 'Jam', language)}
                      yLabel={translate('탐지 수', 'Detections', 'Deteksi', language)}
                      colors={COLORS}
                      labels={speciesData.activityTime[0] ? speciesData.activityTime[0].species.map(s => translateSpecies(s.name, language)) : []}
                      isStacked={true}
                      height={290}
                    />
                  ) : (
                    <Box sx={{ height: 290, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <Typography variant="body1" color="text.secondary">
                        {translate('활동 시간대 데이터가 없습니다.', 'No activity time data available.', 'Tidak ada data waktu aktivitas.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>
            </Grid>

            {/* 종별 통계 정보 */}
            <Typography variant="h6" sx={{ mb: 2, mt: 3 }}>
              {translate('종별 통계 정보', 'Species Statistics', 'Statistik Spesies', language)}
            </Typography>
            <Paper 
              sx={{ 
                p: 2, 
                bgcolor: '#0c1e36', 
                border: '1px solid #1e3a5a',
                overflowX: 'auto'
              }}
            >
              <Table size="small">
                <TableHead>
                  <TableRow sx={{ '& th': { borderBottom: '2px solid #1e3a5a' } }}>
                    <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('종류', 'Species', 'Spesies', language)}</TableCell>
                    <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('탐지 수', 'Detections', 'Deteksi', language)}</TableCell>
                    <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('평균 바운딩 박스', 'Avg. Bounding Box', 'Kotak Pembatas Rata-rata', language)}</TableCell>
                    <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('최고 신뢰도', 'Max Confidence', 'Kepercayaan Maksimal', language)}</TableCell>
                    <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('일간 추이', '7-Day Trend', 'Tren 7 Hari', language)}</TableCell>
                  </TableRow>
                </TableHead>
                <TableBody>
                  {speciesData.distribution && speciesData.distribution.map((item, index) => (
                    <TableRow key={item.species} sx={{ '&:nth-of-type(odd)': { bgcolor: 'rgba(0, 30, 60, 0.3)' } }}>
                      <TableCell>
                        <Box sx={{ display: 'flex', alignItems: 'center' }}>
                          <Box
                            component="span"
                            sx={{
                              width: 12,
                              height: 12,
                              borderRadius: '50%',
                              bgcolor: COLORS[index % COLORS.length],
                              mr: 1
                            }}
                          />
                          {translateSpecies(item.species, language)}
                        </Box>
                      </TableCell>
                      <TableCell>{item.count.toLocaleString()}</TableCell>
                      <TableCell>
                        {speciesData.boundingBoxes && speciesData.boundingBoxes.find(b => b.species === item.species)
                          ? `${speciesData.boundingBoxes.find(b => b.species === item.species).avgSize.toLocaleString()} px²`
                          : '-'}
                      </TableCell>
                      <TableCell>
                        {speciesData.confidence && speciesData.confidence.find(c => c.species === item.species)
                          ? `${(speciesData.confidence.find(c => c.species === item.species).maxConfidence * 100).toFixed(1)}%`
                          : '-'}
                      </TableCell>
                      <TableCell>
                        <Box sx={{ display: 'flex', alignItems: 'center' }}>
                          {speciesData.weeklyChange && speciesData.weeklyChange.find(w => w.species === item.species) ? (
                            <>
                              <Box
                                component="span"
                                sx={{
                                  width: Math.min(60, Math.abs(speciesData.weeklyChange.find(w => w.species === item.species).change) * 4),
                                  height: 4,
                                  bgcolor: speciesData.weeklyChange.find(w => w.species === item.species).change > 0 ? 'success.main' : 'error.main',
                                  borderRadius: 1,
                                  mr: 1
                                }}
                              />
                              <Typography variant="body2" color={speciesData.weeklyChange.find(w => w.species === item.species).change > 0 ? 'success.main' : 'error.main'}>
                                {speciesData.weeklyChange.find(w => w.species === item.species).change > 0 ? '+' : ''}
                                {speciesData.weeklyChange.find(w => w.species === item.species).change.toFixed(1)}%
                              </Typography>
                            </>
                          ) : '-'}
                        </Box>
                      </TableCell>
                    </TableRow>
                  ))}
                </TableBody>
              </Table>
            </Paper>
          </Box>
        )}

        {/* 퇴치 시스템 탭 */}
        {tabValue === 3 && (
          <Box sx={{ p: 3 }}>
            {/* 필터 정보 표시 */}
            <Box sx={{ 
              p: 1, 
              mb: 2, 
              backgroundColor: 'rgba(0, 30, 60, 0.3)', 
              borderRadius: 1,
              border: '1px solid rgba(30, 136, 229, 0.2)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between'
            }}>
              <Typography variant="body2" color="text.secondary">
                {translate('적용된 필터', 'Applied Filters', 'Filter yang Diterapkan', language)}: 
                <Box component="span" sx={{ ml: 1, color: 'primary.main', fontWeight: 'medium' }}>
                  {timeRange === 'day' && translate('오늘', 'Today', 'Hari Ini', language)}
                  {timeRange === 'week' && translate('이번 주', 'This Week', 'Minggu Ini', language)}
                  {timeRange === 'month' && translate('이번 달', 'This Month', 'Bulan Ini', language)}
                  {timeRange === 'year' && translate('올해', 'This Year', 'Tahun Ini', language)}
                  {timeRange === 'custom' && `${formatDate(startDate)} - ${formatDate(endDate)}`}
                </Box>
                {species !== 'all' && (
                  <Box component="span" sx={{ ml: 2, color: 'info.main', fontWeight: 'medium' }}>
                    {translate('종류', 'Species', 'Spesies', language)}: {translateSpecies(species, language)}
                  </Box>
                )}
              </Typography>
              {isLoading && <CircularProgress size={20} sx={{ ml: 2 }} />}
            </Box>

            {/* 퇴치 시스템 그래프 3개 (가로 배치) */}
            <Grid container spacing={3} sx={{ mb: 3 }}>
              {/* 퇴치 시스템 활성화 통계 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('퇴치 시스템 활성화 통계', 'Deterrent System Activation Statistics', 'Statistik Aktivasi Sistem Penangkal', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  {defenseData.activations && defenseData.activations.length > 0 ? (
                    <LineChartD3
                      data={defenseData.activations.map((item) => ({
                        date: language === 'ko' ? formatKoreanDate(item.date) : item.date,
                        success: item.success,
                        failure: item.failure
                      }))}
                      xKey="date"
                      yKeys={['success', 'failure']}
                      colors={['#4caf50', '#f44336']}
                      labels={[
                        translate('성공', 'Success', 'Sukses', language),
                        translate('실패', 'Failure', 'Gagal', language)
                      ]}
                      xLabel={translate('날짜', 'Date', 'Tanggal', language)}
                      yLabel={translate('활성화 수', 'Activations', 'Aktivasi', language)}
                      height={260}
                    />
                  ) : (
                    <Box sx={{ height: 260, display: 'flex', alignItems: 'center', justifyContent: 'center', flexDirection: 'column' }}>
                      <CircularProgress size={30} sx={{ mb: 2 }} />
                      <Typography variant="body2" color="text.secondary">
                        {translate('데이터를 불러오는 중...', 'Loading data...', 'Memuat data...', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 평균 대응 시간 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('평균 대응 시간', 'Average Response Time', 'Waktu Respons Rata-rata', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  {defenseData.responseTime && defenseData.responseTime.length > 0 ? (
                    <LineChartD3
                      data={defenseData.responseTime.map((item) => ({
                        date: item.date,
                        avgTime: item.avgTime
                      }))}
                      xKey="date"
                      yKeys={['avgTime']}
                      colors={['#ff9800']}
                      labels={[translate('평균 대응 시간(초)', 'Avg. Response Time(s)', 'Waktu Respons Rata-rata(d)', language)]}
                      xLabel={translate('날짜', 'Date', 'Tanggal', language)}
                      yLabel={translate('초', 'Seconds', 'Detik', language)}
                      height={260}
                    />
                  ) : (
                    <Box sx={{ height: 260, display: 'flex', alignItems: 'center', justifyContent: 'center', flexDirection: 'column' }}>
                      <CircularProgress size={30} sx={{ mb: 2 }} />
                      <Typography variant="body2" color="text.secondary">
                        {translate('데이터를 불러오는 중...', 'Loading data...', 'Memuat data...', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 시간대별 퇴치 성공률 */}
              <Grid item xs={12} md={4}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('시간대별 퇴치 성공률', 'Hourly Deterrent Success Rate', 'Tingkat Keberhasilan Penangkal per Jam', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a',
                    height: 320
                  }}
                >
                  {defenseData.hourlySuccessRate && defenseData.hourlySuccessRate.length > 0 ? (
                    <BarChartD3
                      data={defenseData.hourlySuccessRate.map(item => ({
                        name: `${item.hour}:00`,
                        value: item.successRate,
                        count: item.totalCount
                      }))}
                      xKey="name"
                      yKey="value"
                      xLabel={translate('시간', 'Hour', 'Jam', language)}
                      yLabel={translate('성공률(%)', 'Success Rate(%)', 'Tingkat Keberhasilan(%)', language)}
                      color="#8884d8"
                      tooltipLabel="%"
                      height={260}
                    />
                  ) : (
                    <Box sx={{ height: 260, display: 'flex', alignItems: 'center', justifyContent: 'center', flexDirection: 'column' }}>
                      <CircularProgress size={30} sx={{ mb: 2 }} />
                      <Typography variant="body2" color="text.secondary">
                        {translate('데이터를 불러오는 중...', 'Loading data...', 'Memuat data...', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>
            </Grid>

            {/* 퇴치 장치별 효율 */}
            <Grid container spacing={3} sx={{ mb: 3 }}>
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('퇴치 장치별 효율', 'Deterrent Device Effectiveness', 'Efektivitas Perangkat Penangkal', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a'
                  }}
                >
                  {defenseData.effectivenessBySpecies && defenseData.effectivenessBySpecies.length > 0 ? (
                    <BarChartD3
                      data={defenseData.effectivenessBySpecies.map(item => ({
                        name: item.species,
                        value: item.effectiveness
                      }))}
                      xKey="name"
                      yKey="value"
                      xLabel={translate('장치 ID', 'Device ID', 'ID Perangkat', language)}
                      yLabel={translate('효율(%)', 'Effectiveness(%)', 'Efektivitas(%)', language)}
                      color="#4caf50"
                      tooltipLabel="%"
                      height={300}
                    />
                  ) : (
                    <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', flexDirection: 'column', height: 300 }}>
                      <CircularProgress size={40} sx={{ mb: 2 }} />
                      <Typography variant="body1" sx={{ textAlign: 'center' }}>
                        {translate('효율 데이터를 불러오는 중...', 'Loading effectiveness data...', 'Memuat data efektivitas...', language)}
                      </Typography>
                      <Button 
                        variant="outlined" 
                        size="small" 
                        sx={{ mt: 2 }}
                        onClick={() => {
                          // 데이터 새로고침 함수 - 효율 데이터만 다시 로드
                          const refreshEffectivenessData = async () => {
                            try {
                              console.log('효율 데이터 수동 새로고침 시작');
                              const refreshedData = await getDefenseData(timeRange, startDate, endDate);
                              if (refreshedData && !refreshedData.error) {
                                setDefenseData(prevData => ({
                                  ...prevData,
                                  effectivenessBySpecies: refreshedData.effectivenessBySpecies,
                                  updatedAt: new Date().toISOString()
                                }));
                                console.log('효율 데이터 수동 새로고침 완료');
                              }
                            } catch (error) {
                              console.error('효율 데이터 수동 갱신 오류:', error);
                            }
                          };
                          refreshEffectivenessData();
                        }}
                      >
                        {translate('새로고침', 'Refresh', 'Segarkan', language)}
                      </Button>
                    </Box>
                  )}
                </Paper>
              </Grid>

              {/* 종별 퇴치 성공률 */}
              <Grid item xs={12} md={6}>
                <Typography variant="h6" sx={{ mb: 2 }}>
                  {translate('종별 퇴치 성공률', 'Deterrent Success Rate by Species', 'Tingkat Keberhasilan Penangkal berdasarkan Spesies', language)}
                </Typography>
                <Paper 
                  sx={{ 
                    p: 2, 
                    bgcolor: '#0c1e36', 
                    border: '1px solid #1e3a5a'
                  }}
                >
                  {defenseData.speciesSuccessRate && defenseData.speciesSuccessRate.length > 0 ? (
                    <BarChartD3
                      data={defenseData.speciesSuccessRate.map(item => ({
                        name: translateSpecies(item.species, language),
                        value: item.successRate
                      }))}
                      xKey="name"
                      yKey="value"
                      xLabel={translate('종류', 'Species', 'Spesies', language)}
                      yLabel={translate('성공률(%)', 'Success Rate(%)', 'Tingkat Keberhasilan(%)', language)}
                      color="#ff9800"
                      tooltipLabel="%"
                      height={300}
                    />
                  ) : (
                    <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', flexDirection: 'column', height: 300 }}>
                      <Typography variant="body1" sx={{ textAlign: 'center' }}>
                        {translate('종별 퇴치 성공률 데이터가 없습니다.', 'No species success rate data available.', 'Tidak ada data tingkat keberhasilan spesies.', language)}
                      </Typography>
                    </Box>
                  )}
                </Paper>
              </Grid>
            </Grid>

            {/* 최근 퇴치 기록 */}
            <Typography variant="h6" sx={{ mb: 2 }}>
              {translate('최근 퇴치 기록', 'Recent Deterrent Activations', 'Aktivasi Penangkal Terbaru', language)}
            </Typography>
            <Paper 
              sx={{ 
                p: 2, 
                bgcolor: '#0c1e36', 
                border: '1px solid #1e3a5a',
                mb: 3,
                overflowX: 'auto',
                borderRadius: 2,
                boxShadow: '0 4px 20px rgba(0,0,0,0.1)'
              }}
            >
              {defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0 ? (
                <Box sx={{ minWidth: 700 }}>
                  <Table size="medium">
                    <TableHead>
                      <TableRow sx={{ '& th': { borderBottom: '2px solid #1e3a5a', py: 1.5 } }}>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('퇴치 ID', 'Deterrent ID', 'ID Pengusiran', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('감지 ID', 'Detection ID', 'ID Deteksi', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('거리(m)', 'Distance(m)', 'Jarak(m)', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('객체 수', 'Object Count', 'Jumlah Objek', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('시작 시간', 'Start Time', 'Waktu Mulai', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('소요 시간', 'Duration', 'Durasi', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('장치', 'Device', 'Perangkat', language)}</TableCell>
                        <TableCell sx={{ color: 'text.secondary', fontWeight: 'bold' }}>{translate('결과', 'Result', 'Hasil', language)}</TableCell>
                      </TableRow>
                    </TableHead>
                    <TableBody>
                      {defenseData.rawDeterrents.slice(0, 10).map((record) => {
                        // 날짜 포맷팅
                        const startTime = new Date(record.start_time);
                        const endTime = new Date(record.end_time);
                        const firingTime = new Date(record.cannon_firing_time);
                        
                        // 유효한 날짜 확인
                        const isValidStartTime = !isNaN(startTime.getTime());
                        const isValidEndTime = !isNaN(endTime.getTime());
                        const isValidFiringTime = !isNaN(firingTime.getTime());
                        
                        // 소요 시간 계산
                        let durationStr = "-";
                        if (isValidStartTime && isValidEndTime) {
                          const durationMs = endTime - startTime;
                          const durationMinutes = Math.floor(durationMs / 60000);
                          const durationSeconds = Math.floor((durationMs % 60000) / 1000);
                          durationStr = `${durationMinutes}분 ${durationSeconds}초`;
                        }
                        
                        // 퇴치 대응 시간
                        let responseSeconds = "-";
                        if (isValidStartTime && isValidFiringTime) {
                          const responseMs = firingTime - startTime;
                          responseSeconds = (responseMs / 1000).toFixed(1);
                        }
                        
                        return (
                          <TableRow 
                            key={record.deterrent_id} 
                            sx={{ 
                              '&:last-child td, &:last-child th': { border: 0 },
                              '&:hover': { bgcolor: 'rgba(30, 58, 90, 0.3)' },
                              transition: 'background-color 0.2s'
                            }}
                          >
                            <TableCell sx={{ fontWeight: 'medium' }}>{record.deterrent_id}</TableCell>
                            <TableCell>
                              <Box sx={{ display: 'flex', alignItems: 'center' }}>
                                <Box
                                  component="span"
                                  sx={{
                                    width: 8,
                                    height: 8,
                                    borderRadius: '50%',
                                    bgcolor: 'primary.main',
                                    mr: 1
                                  }}
                                />
                                {record.detection_id}
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box sx={{ display: 'flex', alignItems: 'center' }}>
                                {typeof record.detection_distance === 'number' ? 
                                  record.detection_distance.toFixed(1) : 
                                  record.detection_distance}m
                                <Box
                                  sx={{
                                    ml: 1,
                                    height: 4,
                                    width: Math.min(50, (typeof record.detection_distance === 'number' ? 
                                      record.detection_distance : 150) / 10),
                                    bgcolor: record.detection_distance > 300 ? 'success.main' : 
                                      record.detection_distance > 150 ? 'warning.main' : 'error.main',
                                    borderRadius: 1
                                  }}
                                />
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box
                                sx={{
                                  display: 'inline-flex',
                                  alignItems: 'center',
                                  justifyContent: 'center',
                                  borderRadius: '50%',
                                  width: 24,
                                  height: 24,
                                  bgcolor: record.object_count > 3 ? 'error.dark' : 'warning.dark',
                                  color: 'white',
                                  fontSize: '0.75rem',
                                  fontWeight: 'bold'
                                }}
                              >
                                {record.object_count}
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box>
                                <Typography variant="body2" sx={{ fontWeight: 'medium' }}>
                                  {isValidStartTime ? startTime.toLocaleDateString() : "-"}
                                </Typography>
                                <Typography variant="caption" color="text.secondary">
                                  {isValidStartTime ? startTime.toLocaleTimeString() : "-"}
                                </Typography>
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box sx={{ display: 'flex', flexDirection: 'column' }}>
                                <Typography variant="body2">
                                  {durationStr}
                                </Typography>
                                <Typography variant="caption" color="text.secondary">
                                  {translate('대응: ', 'Response: ', 'Respons: ', language)} {responseSeconds}초
                                </Typography>
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box sx={{ display: 'flex', flexDirection: 'column' }}>
                                <Typography variant="body2" sx={{ display: 'flex', alignItems: 'center' }}>
                                  <Box
                                    component="span"
                                    sx={{
                                      display: 'inline-block',
                                      width: 10,
                                      height: 10,
                                      borderRadius: '50%',
                                      bgcolor: 'warning.main',
                                      mr: 0.8
                                    }}
                                  />
                                  {record.cannon_id}
                                </Typography>
                                {record.speaker_id && (
                                  <Typography variant="caption" color="text.secondary" sx={{ display: 'flex', alignItems: 'center', mt: 0.5 }}>
                                    <Box
                                      component="span"
                                      sx={{
                                        display: 'inline-block',
                                        width: 8,
                                        height: 8,
                                        borderRadius: '50%',
                                        bgcolor: 'info.main',
                                        mr: 0.8
                                      }}
                                    />
                                    {record.speaker_id}
                                    {record.sound_code && (
                                      <Tooltip title={translate('사운드 코드', 'Sound Code', 'Kode Suara', language)} placement="top">
                                        <Box
                                          component="span"
                                          sx={{
                                            ml: 1,
                                            px: 1,
                                            py: 0.2,
                                            fontSize: '0.65rem',
                                            borderRadius: 4,
                                            bgcolor: 'rgba(33, 150, 243, 0.1)',
                                            border: '1px solid rgba(33, 150, 243, 0.3)',
                                            color: 'info.main'
                                          }}
                                        >
                                          {record.sound_code}
                                        </Box>
                                      </Tooltip>
                                    )}
                                  </Typography>
                                )}
                              </Box>
                            </TableCell>
                            <TableCell>
                              <Box
                                sx={{
                                  display: 'inline-flex',
                                  alignItems: 'center',
                                  px: 1.5,
                                  py: 0.6,
                                  borderRadius: 4,
                                  bgcolor: record.is_success ? 'rgba(76, 175, 80, 0.2)' : 'rgba(244, 67, 54, 0.2)',
                                  border: `1px solid ${record.is_success ? 'success.main' : 'error.main'}`,
                                  color: record.is_success ? 'success.main' : 'error.main',
                                  fontWeight: 'medium'
                                }}
                              >
                                {record.is_success ? 
                                  translate('성공', 'Success', 'Sukses', language) : 
                                  translate('실패', 'Failure', 'Gagal', language)}
                              </Box>
                            </TableCell>
                          </TableRow>
                        );
                      })}
                    </TableBody>
                  </Table>
                  
                  {/* 페이지네이션 & 더 보기 버튼 */}
                  <Box sx={{ mt: 2, display: 'flex', justifyContent: 'center' }}>
                    <Button 
                      variant="outlined" 
                      size="small"
                      onClick={() => {
                        // 데이터 새로고침 함수
                        const refreshDetentionRecords = async () => {
                          try {
                            console.log('퇴치 기록 데이터 수동 새로고침 시작');
                            const refreshedData = await getDefenseData(timeRange, startDate, endDate);
                            if (refreshedData && !refreshedData.error) {
                              setDefenseData(prevData => ({
                                ...prevData,
                                rawDeterrents: refreshedData.rawDeterrents,
                                updatedAt: new Date().toISOString()
                              }));
                              console.log('퇴치 기록 데이터 수동 새로고침 완료');
                            }
                          } catch (error) {
                            console.error('퇴치 기록 데이터 수동 갱신 오류:', error);
                          }
                        };
                        refreshDetentionRecords();
                      }}
                      sx={{ 
                        borderColor: 'primary.main', 
                        color: 'primary.main',
                        ':hover': { borderColor: 'primary.light', bgcolor: 'rgba(30, 58, 138, 0.08)' }
                      }}
                    >
                      {translate('새로고침', 'Refresh', 'Muat Ulang', language)}
                    </Button>
                  </Box>
                  {defenseData.dataSource === 'mixed' && (
                    <Typography variant="caption" color="text.secondary" sx={{ display: 'block', textAlign: 'center', mt: 1 }}>
                      {translate(
                        '일부 데이터는 실제 데이터가 부족하여 생성된 데이터입니다.',
                        'Some data has been generated due to lack of real data.',
                        'Beberapa data telah dibuat karena kurangnya data nyata.',
                        language
                      )}
                    </Typography>
                  )}
                </Box>
              ) : (
                <Box sx={{ py: 5, display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
                  <CircularProgress sx={{ mb: 2 }} size={40} />
                  <Typography variant="body1" sx={{ mb: 2, textAlign: 'center' }}>
                    {translate('퇴치 기록을 불러오는 중입니다...', 'Loading defense records...', 'Memuat rekaman pertahanan...', language)}
                  </Typography>
                  <Button 
                    variant="outlined" 
                    size="small"
                    onClick={() => {
                      // 수동으로 데이터 새로고침
                      const manualRefresh = async () => {
                        try {
                          console.log('퇴치 기록 수동 새로고침 시작');
                          const refreshedData = await getDefenseData(timeRange, startDate, endDate);
                          if (refreshedData) {
                            setDefenseData(prevData => ({
                              ...prevData,
                              ...refreshedData,
                              updatedAt: new Date().toISOString()
                            }));
                            console.log('퇴치 기록 수동 새로고침 완료');
                          }
                        } catch (error) {
                          console.error('수동 새로고침 오류:', error);
                        }
                      };
                      manualRefresh();
                    }}
                  >
                    {translate('수동 새로고침', 'Manual Refresh', 'Penyegaran Manual', language)}
                  </Button>
                </Box>
              )}
            </Paper>
          </Box>
        )}
      </Paper>
    </Box>
  );
};

export default Analytics;