import React, { useState, useEffect } from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Grid, 
  Button,
  CircularProgress,
  Table,
  TableHead,
  TableRow,
  TableCell,
  TableBody,
  Tooltip,
  ToggleButtonGroup,
  ToggleButton,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  TextField,
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
  formatKoreanDate, 
  formatDate, 
  prepareHourlyData, 
  prepareDailyData, 
  prepareWeeklyData, 
  prepareMonthlyData 
} from '../utils/formatHelpers';
import { getDefenseData } from '../../../services/analyticsService';

/**
 * 퇴치 시스템 탭 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드
 * @param {Object} props.defenseData 퇴치 시스템 데이터
 * @param {Function} props.loadData 데이터 로딩 함수
 * @param {boolean} props.loading 로딩 중 여부
 * @param {number} props.tabValue 현재 탭 값
 * @returns {JSX.Element} 퇴치 시스템 탭 컴포넌트
 */
const DefenseSystemTab = ({ language, defenseData, loadData, loading, tabValue }) => {
  const [timeRange, setTimeRange] = useState('week');
  const [startDate, setStartDate] = useState(new Date(Date.now() - 7 * 24 * 60 * 60 * 1000));
  const [endDate, setEndDate] = useState(new Date());
  const [weekOption, setWeekOption] = useState('thisWeek');
  const [monthOption, setMonthOption] = useState('thisMonth');
  const [chartType, setChartType] = useState('line');
  const [autoRefresh, setAutoRefresh] = useState(false); // 자동 새로고침 비활성화

  // null 체크 추가
  useEffect(() => {
    // defenseData가 null인 경우 빈 객체로 초기화
    if (!defenseData) {
      console.error("defenseData가 null입니다. 상위 컴포넌트에서 올바르게 전달되었는지 확인하세요.");
    }
  }, [defenseData]);

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
        // 이번 달 (1일부터 마지막 일까지)
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
  
  // 효율 데이터 새로고침
  const refreshEffectivenessData = async () => {
    try {
      console.log('효율 데이터 수동 새로고침 시작');
      document.getElementById('manualRefresh').value = true;
      const actualStartDate = startDate;
      const actualEndDate = endDate;
      const refreshedData = await getDefenseData(timeRange, actualStartDate, actualEndDate);
      if (refreshedData && !refreshedData.error) {
        // 상위 컴포넌트의 loadData를 호출하여 전체 데이터 갱신
        await loadData(timeRange, actualStartDate, actualEndDate);
        console.log('효율 데이터 수동 새로고침 완료');
      }
      document.getElementById('manualRefresh').value = false;
    } catch (error) {
      console.error('효율 데이터 수동 갱신 오류:', error);
    }
  };
  
  // 퇴치 기록 데이터 새로고침
  const refreshDetentionRecords = async () => {
    try {
      console.log('퇴치 기록 데이터 수동 새로고침 시작');
      const actualStartDate = startDate;
      const actualEndDate = endDate;
      const refreshedData = await getDefenseData(timeRange, actualStartDate, actualEndDate);
      if (refreshedData && !refreshedData.error) {
        // 상위 컴포넌트의 loadData를 호출하여 전체 데이터 갱신
        await loadData(timeRange, actualStartDate, actualEndDate);
        console.log('퇴치 기록 데이터 수동 새로고침 완료');
      }
    } catch (error) {
      console.error('퇴치 기록 데이터 수동 갱신 오류:', error);
    }
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
    
    // 추가 - 콘솔 로그로 마운트 시점 확인
    console.log('DefenseSystemTab 마운트 시점 데이터 로드: ', new Date().toISOString());
    
    // 정리 함수
    return () => {
      if (document.getElementById('manualRefresh')) {
        document.getElementById('manualRefresh').remove();
      }
    };
  }, []); // 빈 배열로 처음 한 번만 실행

  // defenseData 변경 시 디버깅 정보 출력
  useEffect(() => {
    if (defenseData && defenseData.activations) {
      console.log('퇴치 시스템 활성화 데이터:', defenseData.activations);
      
      // 데이터 구조 확인
      const hasSuccess = defenseData.activations.some(item => item.success > 0);
      const hasFailure = defenseData.activations.some(item => item.failure > 0);
      
      console.log('성공 데이터 포함:', hasSuccess);
      console.log('실패 데이터 포함:', hasFailure);
      
      if (!hasSuccess && !hasFailure) {
        console.warn('성공/실패 데이터가 없습니다. 데이터 구조를 확인하세요.');
      }
    }
  }, [defenseData]);
  
  // 기간 변경 시 필터 적용 (수동으로 변경했을 때만)
  useEffect(() => {
    // 탭 변경 시에는 데이터 다시 로드하지 않음
    if (document.getElementById('manualRefresh') && document.getElementById('manualRefresh').value === 'true') {
      console.log('기간 변경으로 인한 데이터 로드: ', timeRange, weekOption, monthOption);
      document.getElementById('manualRefresh').value = 'pending';
      applyFilters().then(() => {
        document.getElementById('manualRefresh').value = 'false';
      });
    }
  }, [timeRange, weekOption, monthOption]);

  // 퇴치 시스템 탭이 활성화될 때만 데이터 로드
  useEffect(() => {
    if (tabValue === 3 && document.getElementById('manualRefresh')) {
      document.getElementById('manualRefresh').value = 'true';
    }
  }, [tabValue]);
  
  return (
    <Box sx={{ p: 3 }}>
      {/* 기간별 필터링 UI */}
      <Paper sx={{ p: 2, mb: 3, bgcolor: '#0c1e36', border: '1px solid #1e3a5a' }}>
        <Typography variant="h6" sx={{ mb: 2 }}>
          {translate('퇴치 시스템 활성화 통계 설정', 'Deterrent System Activation Statistics Settings', 'Pengaturan Statistik Aktivasi Sistem Pertahanan', language)}
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
            <ToggleButton value="line" aria-label="line">
              {translate('선 차트', 'Line Chart', 'Grafik Garis', language)}
            </ToggleButton>
            <ToggleButton value="bar" aria-label="bar">
              {translate('막대 차트', 'Bar Chart', 'Grafik Batang', language)}
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
        {translate('퇴치 시스템 활성화 통계', 'Deterrent System Activation Statistics', 'Statistik Aktivasi Sistem Pengendalian', language)}
      </Typography>
      {defenseData && defenseData.activations && defenseData.activations.length > 0 ? (
        <>
          <Paper 
            sx={{ 
              p: 2, 
              bgcolor: '#0c1e36', 
              border: '1px solid #1e3a5a',
              mb: 3,
              position: 'relative'
            }}
          >
            {/* 성공/실패 범례 */}
            <Box
              sx={{
                position: 'absolute',
                top: 15,
                right: 20,
                display: 'flex',
                gap: 2,
                zIndex: 10
              }}
            >
              <Box sx={{ display: 'flex', alignItems: 'center', gap: 0.5 }}>
                <Box sx={{ width: 12, height: 12, bgcolor: '#4caf50', borderRadius: '2px' }} />
                <Typography variant="caption" sx={{ color: '#fff' }}>
                  {translate('성공', 'Success', 'Sukses', language)}
                </Typography>
              </Box>
              <Box sx={{ display: 'flex', alignItems: 'center', gap: 0.5 }}>
                <Box sx={{ width: 12, height: 12, bgcolor: '#f44336', borderRadius: '2px' }} />
                <Typography variant="caption" sx={{ color: '#fff' }}>
                  {translate('실패', 'Failure', 'Gagal', language)}
                </Typography>
              </Box>
            </Box>
            
            {chartType === 'line' ? (
              <LineChartD3
                data={
                  timeRange === 'hour' ? 
                    // 시간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 시간별 데이터 추출
                    Array.from({length: 24}, (_, i) => {
                      const hour = i;
                      const hourStr = `${hour.toString().padStart(2, '0')}:00`;
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 시간대 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 시간대 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            const itemHour = itemDate.getHours();
                            return itemHour === hour;
                          } catch(e) {
                            console.error(`날짜 변환 오류: ${item.start_time}`, e);
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          console.log(`시간 ${hourStr}에 매칭된 실제 퇴치 기록 수: ${matchingItems.length}`);
                          
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                          
                          if (activationData.success > 0 || activationData.failure > 0) {
                            console.log(`${hourStr} 실제 데이터: 성공=${activationData.success}, 실패=${activationData.failure}`);
                          }
                        }
                      }
                      
                      return {
                        hour: hourStr,
                        success: activationData.success,
                        failure: activationData.failure
                      };
                    }) :
                  timeRange === 'day' ? 
                    // 일별 데이터: 실제 퇴치 기록(rawDeterrents)에서 일별 데이터 추출
                    Array.from({length: 7}, (_, i) => {
                      const days = ['일', '월', '화', '수', '목', '금', '토'];
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 요일 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 요일 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            return itemDate.getDay() === i;
                          } catch(e) {
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                        }
                      }
                      
                      return {
                        name: days[i],
                        success: activationData.success,
                        failure: activationData.failure
                      };
                    }) :
                  timeRange === 'week' ? 
                    // 주간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 주간별 데이터 추출
                    Array.from({length: 4}, (_, i) => {
                      const weekNum = 4 - i; // 최근 주부터 역순으로
                      const now = new Date();
                      const month = now.getMonth() + 1;
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 주차 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 주차 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            const dateWeekOfMonth = Math.ceil(itemDate.getDate() / 7);
                            return dateWeekOfMonth === weekNum && itemDate.getMonth() + 1 === month;
                          } catch(e) {
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                        }
                      }
                      
                      return {
                        name: `${month}월 ${weekNum}주차`,
                        success: activationData.success,
                        failure: activationData.failure
                      };
                    }) :
                    // 월별 데이터: 이번달 1일부터 31일까지 표시
                    monthOption === 'thisMonth' ? 
                      (() => {
                        const now = new Date();
                        const currentMonth = now.getMonth();
                        const currentYear = now.getFullYear();
                        const daysInMonth = new Date(currentYear, currentMonth + 1, 0).getDate();
                        
                        return Array.from({length: daysInMonth}, (_, i) => {
                          const day = i + 1;
                          
                          // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                          let activationData = { success: 0, failure: 0 };
                          
                          // rawDeterrents 데이터가 있는 경우에만 처리
                          if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                            // 실제 퇴치 기록에서 해당 일자 데이터 찾기
                            const matchingItems = defenseData.rawDeterrents.filter(item => {
                              try {
                                if (!item.start_time) return false;
                                
                                // 시작 시간을 기준으로 일자 매칭
                                const itemDate = new Date(item.start_time);
                                if (isNaN(itemDate.getTime())) return false;
                                
                                return itemDate.getDate() === day && 
                                       itemDate.getMonth() === currentMonth &&
                                       itemDate.getFullYear() === currentYear;
                              } catch(e) {
                                return false;
                              }
                            });
                            
                            // 성공/실패 데이터 집계
                            if (matchingItems.length > 0) {
                              matchingItems.forEach(item => {
                                // is_success 필드로 성공/실패 구분
                                if (item.is_success) {
                                  activationData.success += 1;
                                } else {
                                  activationData.failure += 1;
                                }
                              });
                            }
                          }
                          
                          return {
                            name: `${currentMonth + 1}/${day}`,
                            success: activationData.success,
                            failure: activationData.failure
                          };
                        });
                      })() :
                      // 그 외 월별 데이터: 최근 6개월 포함
                      Array.from({length: 6}, (_, i) => {
                        const now = new Date();
                        const targetDate = new Date(now.getFullYear(), now.getMonth() - i, 1);
                        const month = targetDate.getMonth() + 1;
                        const year = targetDate.getFullYear();
                        
                        // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                        let activationData = { success: 0, failure: 0 };
                        
                        // rawDeterrents 데이터가 있는 경우에만 처리
                        if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                          // 실제 퇴치 기록에서 해당 월 데이터 찾기
                          const matchingItems = defenseData.rawDeterrents.filter(item => {
                            try {
                              if (!item.start_time) return false;
                              
                              // 시작 시간을 기준으로 월 매칭
                              const itemDate = new Date(item.start_time);
                              if (isNaN(itemDate.getTime())) return false;
                              
                              return itemDate.getMonth() + 1 === month && 
                                     itemDate.getFullYear() === year;
                            } catch(e) {
                              return false;
                            }
                          });
                          
                          // 성공/실패 데이터 집계
                          if (matchingItems.length > 0) {
                            matchingItems.forEach(item => {
                              // is_success 필드로 성공/실패 구분
                              if (item.is_success) {
                                activationData.success += 1;
                              } else {
                                activationData.failure += 1;
                              }
                            });
                          }
                        }
                        
                        return {
                          name: `${month}월`,
                          success: activationData.success,
                          failure: activationData.failure
                        };
                      }).reverse() // 오래된 월부터 최신 월 순으로 정렬
                }
                xKey={
                  timeRange === 'hour' ? 'hour' : 'name'
                }
                yKeys={['success', 'failure']}
                colors={['#4caf50', '#f44336']}
                labels={[
                  translate('성공', 'Success', 'Sukses', language),
                  translate('실패', 'Failure', 'Gagal', language)
                ]}
                xLabel={
                  timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
                  timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
                  timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
                  monthOption === 'thisMonth' ? translate('일', 'Day', 'Hari', language) :
                  translate('월', 'Month', 'Bulan', language)
                }
                yLabel={translate('활성화 수', 'Activations', 'Aktivasi', language)}
                height={300}
                isDateTick={false}
                periodType={timeRange}
                hourlyTicks={timeRange === 'hour'}
                displayLegend={false} // 커스텀 범례를 사용하므로 기본 범례 숨김
              />
            ) : (
              <BarChartD3
                data={
                  timeRange === 'hour' ? 
                    // 시간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 시간별 데이터 추출
                    Array.from({length: 24}, (_, i) => {
                      const hour = i;
                      const hourStr = `${hour.toString().padStart(2, '0')}:00`;
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 시간대 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 시간대 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            const itemHour = itemDate.getHours();
                            return itemHour === hour;
                          } catch(e) {
                            console.error(`날짜 변환 오류: ${item.start_time}`, e);
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          console.log(`시간 ${hourStr}에 매칭된 실제 퇴치 기록 수: ${matchingItems.length}`);
                          
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                          
                          if (activationData.success > 0 || activationData.failure > 0) {
                            console.log(`${hourStr} 실제 데이터: 성공=${activationData.success}, 실패=${activationData.failure}`);
                          }
                        }
                      }
                      
                      return {
                        hour: hourStr,
                        value: activationData.success + activationData.failure,
                        success: activationData.success,
                        failure: activationData.failure
                      };
                    }) :
                  timeRange === 'day' ? 
                    // 일별 데이터: 실제 퇴치 기록(rawDeterrents)에서 일별 데이터 추출
                    Array.from({length: 7}, (_, i) => {
                      const days = ['일', '월', '화', '수', '목', '금', '토'];
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 요일 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 요일 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            return itemDate.getDay() === i;
                          } catch(e) {
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                        }
                      }
                      
                      return {
                        name: days[i],
                        value: (activationData.success || 0) + (activationData.failure || 0),
                        success: activationData.success || 0,
                        failure: activationData.failure || 0
                      };
                    }) :
                  timeRange === 'week' ? 
                    // 주간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 주간별 데이터 추출
                    Array.from({length: 4}, (_, i) => {
                      const weekNum = 4 - i; // 최근 주부터 역순으로
                      const now = new Date();
                      const month = now.getMonth() + 1;
                      
                      // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                      let activationData = { success: 0, failure: 0 };
                      
                      // rawDeterrents 데이터가 있는 경우에만 처리
                      if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                        // 실제 퇴치 기록에서 해당 주차 데이터 찾기
                        const matchingItems = defenseData.rawDeterrents.filter(item => {
                          try {
                            if (!item.start_time) return false;
                            
                            // 시작 시간을 기준으로 주차 매칭
                            const itemDate = new Date(item.start_time);
                            if (isNaN(itemDate.getTime())) return false;
                            
                            const dateWeekOfMonth = Math.ceil(itemDate.getDate() / 7);
                            return dateWeekOfMonth === weekNum && itemDate.getMonth() + 1 === month;
                          } catch(e) {
                            return false;
                          }
                        });
                        
                        // 성공/실패 데이터 집계
                        if (matchingItems.length > 0) {
                          matchingItems.forEach(item => {
                            // is_success 필드로 성공/실패 구분
                            if (item.is_success) {
                              activationData.success += 1;
                            } else {
                              activationData.failure += 1;
                            }
                          });
                        }
                      }
                      
                      return {
                        name: `${month}월 ${weekNum}주차`,
                        value: (activationData.success || 0) + (activationData.failure || 0),
                        success: activationData.success || 0,
                        failure: activationData.failure || 0
                      };
                    }) :
                    // 월별 데이터: 이번달 1일부터 31일까지 표시 
                    monthOption === 'thisMonth' ? 
                      (() => {
                        const now = new Date();
                        const currentMonth = now.getMonth();
                        const currentYear = now.getFullYear();
                        const daysInMonth = new Date(currentYear, currentMonth + 1, 0).getDate();
                        
                        return Array.from({length: daysInMonth}, (_, i) => {
                          const day = i + 1;
                          const dateStr = `${currentYear}-${(currentMonth + 1).toString().padStart(2, '0')}-${day.toString().padStart(2, '0')}`;
                          const activationData = (defenseData.activations || []).find(item => {
                            const date = new Date(item.date);
                            return date.getDate() === day && date.getMonth() === currentMonth;
                          }) || { success: 0, failure: 0 };
                          
                          return {
                            name: `${currentMonth + 1}/${day}`,
                            value: (activationData.success || 0) + (activationData.failure || 0),
                            success: activationData.success || 0,
                            failure: activationData.failure || 0
                          };
                        });
                      })() :
                      // 그 외 월별 데이터: 최근 6개월 포함
                      Array.from({length: 6}, (_, i) => {
                        const now = new Date();
                        const targetDate = new Date(now.getFullYear(), now.getMonth() - i, 1);
                        const month = targetDate.getMonth() + 1;
                        const year = targetDate.getFullYear();
                        
                        // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                        let activationData = { success: 0, failure: 0 };
                        
                        // rawDeterrents 데이터가 있는 경우에만 처리
                        if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                          // 실제 퇴치 기록에서 해당 월 데이터 찾기
                          const matchingItems = defenseData.rawDeterrents.filter(item => {
                            try {
                              if (!item.start_time) return false;
                              
                              // 시작 시간을 기준으로 월 매칭
                              const itemDate = new Date(item.start_time);
                              if (isNaN(itemDate.getTime())) return false;
                              
                              return itemDate.getMonth() + 1 === month && 
                                     itemDate.getFullYear() === year;
                            } catch(e) {
                              return false;
                            }
                          });
                          
                          // 성공/실패 데이터 집계
                          if (matchingItems.length > 0) {
                            matchingItems.forEach(item => {
                              // is_success 필드로 성공/실패 구분
                              if (item.is_success) {
                                activationData.success += 1;
                              } else {
                                activationData.failure += 1;
                              }
                            });
                          }
                        }
                        
                        return {
                          name: `${month}월`,
                          value: (activationData.success || 0) + (activationData.failure || 0),
                          success: activationData.success || 0,
                          failure: activationData.failure || 0
                        };
                      }).reverse() // 오래된 월부터 최신 월 순으로 정렬
                }
                xKey={
                  timeRange === 'hour' ? 'hour' : 'name'
                }
                yKey="value"
                xLabel={
                  timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
                  timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
                  timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
                  monthOption === 'thisMonth' ? translate('일', 'Day', 'Hari', language) :
                  translate('월', 'Month', 'Bulan', language)
                }
                yLabel={translate('활성화 수', 'Activations', 'Aktivasi', language)}
                color="#4caf50"
                tooltipLabel={translate('건', ' activations', ' aktivasi', language)}
                height={300}
                periodType={timeRange}
                hourlyTicks={timeRange === 'hour'}
                displayAllTicks={timeRange === 'hour'}
              />
            )}
          </Paper>
          
          <Grid container spacing={3}>
            <Grid item xs={12} md={6}>
              <Typography variant="h6" sx={{ mb: 2 }}>
                {translate('평균 대응 시간', 'Average Response Time', 'Waktu Respons Rata-rata', language)}
              </Typography>
              <Paper 
                sx={{ 
                  p: 2, 
                  bgcolor: '#0c1e36', 
                  border: '1px solid #1e3a5a',
                  mb: 3
                }}
              >
                <LineChartD3
                  data={
                    timeRange === 'hour' ? 
                      // 시간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 시간별 데이터 추출
                      Array.from({length: 24}, (_, i) => {
                        const hour = i;
                        const hourStr = `${hour.toString().padStart(2, '0')}:00`;
                        
                        // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                        let activationData = { success: 0, failure: 0 };
                        
                        // rawDeterrents 데이터가 있는 경우에만 처리
                        if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                          // 실제 퇴치 기록에서 해당 시간대 데이터 찾기
                          const matchingItems = defenseData.rawDeterrents.filter(item => {
                            try {
                              if (!item.start_time) return false;
                              
                              // 시작 시간을 기준으로 시간대 매칭
                              const itemDate = new Date(item.start_time);
                              if (isNaN(itemDate.getTime())) return false;
                              
                              const itemHour = itemDate.getHours();
                              return itemHour === hour;
                            } catch(e) {
                              console.error(`날짜 변환 오류: ${item.start_time}`, e);
                              return false;
                            }
                          });
                          
                          // 성공/실패 데이터 집계
                          if (matchingItems.length > 0) {
                            console.log(`시간 ${hourStr}에 매칭된 실제 퇴치 기록 수: ${matchingItems.length}`);
                            
                            matchingItems.forEach(item => {
                              // is_success 필드로 성공/실패 구분
                              if (item.is_success) {
                                activationData.success += 1;
                              } else {
                                activationData.failure += 1;
                              }
                            });
                            
                            if (activationData.success > 0 || activationData.failure > 0) {
                              console.log(`${hourStr} 실제 데이터: 성공=${activationData.success}, 실패=${activationData.failure}`);
                            }
                          }
                        }
                        
                        return {
                          hour: hourStr,
                          avgTime: activationData.success > 0 || activationData.failure > 0 ? 
                            ((activationData.success + activationData.failure) / 2).toFixed(1) : 0
                        };
                      }) :
                    timeRange === 'day' ? 
                      // 일별 데이터: 실제 퇴치 기록(rawDeterrents)에서 일별 데이터 추출
                      Array.from({length: 7}, (_, i) => {
                        const days = ['일', '월', '화', '수', '목', '금', '토'];
                        
                        // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                        let activationData = { success: 0, failure: 0 };
                        
                        // rawDeterrents 데이터가 있는 경우에만 처리
                        if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                          // 실제 퇴치 기록에서 해당 요일 데이터 찾기
                          const matchingItems = defenseData.rawDeterrents.filter(item => {
                            try {
                              if (!item.start_time) return false;
                              
                              // 시작 시간을 기준으로 요일 매칭
                              const itemDate = new Date(item.start_time);
                              if (isNaN(itemDate.getTime())) return false;
                              
                              return itemDate.getDay() === i;
                            } catch(e) {
                              return false;
                            }
                          });
                          
                          // 성공/실패 데이터 집계
                          if (matchingItems.length > 0) {
                            matchingItems.forEach(item => {
                              // is_success 필드로 성공/실패 구분
                              if (item.is_success) {
                                activationData.success += 1;
                              } else {
                                activationData.failure += 1;
                              }
                            });
                          }
                        }
                        
                        return {
                          name: days[i],
                          avgTime: activationData.success > 0 || activationData.failure > 0 ? 
                            ((activationData.success + activationData.failure) / 2).toFixed(1) : 0
                        };
                      }) :
                    timeRange === 'week' ? 
                      // 주간별 데이터: 실제 퇴치 기록(rawDeterrents)에서 주간별 데이터 추출
                      Array.from({length: 4}, (_, i) => {
                        const weekNum = 4 - i; // 최근 주부터 역순으로
                        const now = new Date();
                        const month = now.getMonth() + 1;
                        
                        // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                        let activationData = { success: 0, failure: 0 };
                        
                        // rawDeterrents 데이터가 있는 경우에만 처리
                        if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                          // 실제 퇴치 기록에서 해당 주차 데이터 찾기
                          const matchingItems = defenseData.rawDeterrents.filter(item => {
                            try {
                              if (!item.start_time) return false;
                              
                              // 시작 시간을 기준으로 주차 매칭
                              const itemDate = new Date(item.start_time);
                              if (isNaN(itemDate.getTime())) return false;
                              
                              const dateWeekOfMonth = Math.ceil(itemDate.getDate() / 7);
                              return dateWeekOfMonth === weekNum && itemDate.getMonth() + 1 === month;
                            } catch(e) {
                              return false;
                            }
                          });
                          
                          // 성공/실패 데이터 집계
                          if (matchingItems.length > 0) {
                            matchingItems.forEach(item => {
                              // is_success 필드로 성공/실패 구분
                              if (item.is_success) {
                                activationData.success += 1;
                              } else {
                                activationData.failure += 1;
                              }
                            });
                          }
                        }
                        
                        return {
                          name: `${month}월 ${weekNum}주차`,
                          avgTime: activationData.success > 0 || activationData.failure > 0 ? 
                            ((activationData.success + activationData.failure) / 2).toFixed(1) : 0
                        };
                      }) :
                      // 월별 데이터: 이번달 1일부터 마지막 일까지 (monthOption이 thisMonth인 경우)
                      monthOption === 'thisMonth' ? 
                        (() => {
                          const now = new Date();
                          const currentMonth = now.getMonth();
                          const currentYear = now.getFullYear();
                          const daysInMonth = new Date(currentYear, currentMonth + 1, 0).getDate();
                          
                          return Array.from({length: daysInMonth}, (_, i) => {
                            const day = i + 1;
                            const dateStr = `${currentYear}-${(currentMonth + 1).toString().padStart(2, '0')}-${day.toString().padStart(2, '0')}`;
                            const responseData = (defenseData.responseTime || []).find(item => {
                              const date = item.date ? new Date(item.date) : null;
                              return date && date.getDate() === day && date.getMonth() === currentMonth;
                            });
                            
                        return {
                              name: `${currentMonth + 1}/${day}`,
                              avgTime: responseData ? responseData.avgTime || 0 : 0
                            };
                          });
                        })() :
                        // 최근 6개월 포함 (다른 월 옵션)
                        Array.from({length: 6}, (_, i) => {
                          const now = new Date();
                          const targetDate = new Date(now.getFullYear(), now.getMonth() - i, 1);
                          const month = targetDate.getMonth() + 1;
                          const year = targetDate.getFullYear();
                          
                          // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                          let activationData = { success: 0, failure: 0 };
                          
                          // rawDeterrents 데이터가 있는 경우에만 처리
                          if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                            // 실제 퇴치 기록에서 해당 월 데이터 찾기
                            const matchingItems = defenseData.rawDeterrents.filter(item => {
                              try {
                                if (!item.start_time) return false;
                                
                                // 시작 시간을 기준으로 월 매칭
                                const itemDate = new Date(item.start_time);
                                if (isNaN(itemDate.getTime())) return false;
                                
                                return itemDate.getMonth() + 1 === month && 
                                       itemDate.getFullYear() === year;
                              } catch(e) {
                                return false;
                              }
                            });
                            
                            // 성공/실패 데이터 집계
                            if (matchingItems.length > 0) {
                              matchingItems.forEach(item => {
                                // is_success 필드로 성공/실패 구분
                                if (item.is_success) {
                                  activationData.success += 1;
                                } else {
                                  activationData.failure += 1;
                                }
                              });
                            }
                          }
                          
                          return {
                            name: `${month}월`,
                            avgTime: activationData.success > 0 || activationData.failure > 0 ? 
                              ((activationData.success + activationData.failure) / 2).toFixed(1) : 0
                          };
                        }).reverse() // 오래된 월부터 최신 월 순으로 정렬
                  }
                  xKey={
                    timeRange === 'hour' ? 'hour' : 
                    timeRange === 'month' && monthOption !== 'thisMonth' ? 'month' : 
                    'name'
                  }
                  yKeys={['avgTime']}
                  colors={['#ff9800']}
                  labels={[translate('평균 대응 시간(초)', 'Avg. Response Time(s)', 'Waktu Respons Rata-rata(d)', language)]}
                  xLabel={
                    timeRange === 'hour' ? translate('시간', 'Hour', 'Jam', language) :
                    timeRange === 'day' ? translate('요일', 'Day', 'Hari', language) :
                    timeRange === 'week' ? translate('주차', 'Week', 'Minggu', language) :
                    monthOption === 'thisMonth' ? translate('일', 'Day', 'Hari', language) :
                    translate('월', 'Month', 'Bulan', language)
                  }
                  yLabel={translate('초', 'Seconds', 'Detik', language)}
                  height={300}
                  isDateTick={false}
                  periodType={timeRange}
                  hourlyTicks={timeRange === 'hour'}
                  displayAllTicks={timeRange === 'hour'}
                  displayLegend={false}
                />
              </Paper>
            </Grid>
          
            <Grid item xs={12} md={6}>
              <Typography variant="h6" sx={{ mb: 2 }}>
                {translate('퇴치 장치별 효율', 'Deterrent Device Effectiveness', 'Efektivitas Perangkat Pengendalian', language)}
              </Typography>
              <Paper 
                sx={{ 
                  p: 2, 
                  bgcolor: '#0c1e36', 
                  border: '1px solid #1e3a5a',
                  mb: 3
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
                    {defenseData.error && (
                      <Typography variant="caption" color="error" sx={{ mt: 1, textAlign: 'center' }}>
                        {translate(
                          '오류가 발생했습니다. 자동으로 다시 시도합니다.',
                          'An error occurred. Automatically retrying...',
                          'Terjadi kesalahan. Mencoba lagi secara otomatis...',
                          language
                        )}
                      </Typography>
                    )}
                    <Button 
                      variant="outlined" 
                      size="small" 
                      sx={{ mt: 2 }}
                      onClick={refreshEffectivenessData}
                    >
                      {translate('새로고침', 'Refresh', 'Segarkan', language)}
                    </Button>
                  </Box>
                )}
              </Paper>
            </Grid>
          </Grid>
          
          <Typography variant="h6" sx={{ mb: 2 }}>
            {translate('최근 퇴치 기록', 'Recent Deterrent Activations', 'Aktivasi Pengendalian Terbaru', language)}
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
                      
                      // 방어 대응 시간
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
                    onClick={refreshDetentionRecords}
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
                  onClick={refreshDetentionRecords}
                >
                  {translate('수동 새로고침', 'Manual Refresh', 'Penyegaran Manual', language)}
                </Button>
              </Box>
            )}
          </Paper>

          <Typography variant="h6" sx={{ mb: 2 }}>
            {translate('시간대별 퇴치 성공률', 'Hourly Deterrent Success Rate', 'Tingkat Keberhasilan Pengusiran per Jam', language)}
          </Typography>
          <Paper 
            sx={{ 
              p: 2, 
              bgcolor: '#0c1e36', 
              border: '1px solid #1e3a5a',
              mb: 3
            }}
          >
            <BarChartD3
              data={
                // 시간별 성공률 데이터: 실제 퇴치 기록(rawDeterrents)에서 시간별 데이터 추출
                Array.from({length: 24}, (_, i) => {
                  const hour = i;
                  const hourStr = `${hour.toString().padStart(2, '0')}:00`;
                  
                  // 활성화 데이터 찾기 개선 - 실제 퇴치 기록에서만 데이터 추출
                  let activationData = { success: 0, failure: 0 };
                  
                  // rawDeterrents 데이터가 있는 경우에만 처리
                  if (defenseData.rawDeterrents && defenseData.rawDeterrents.length > 0) {
                    // 실제 퇴치 기록에서 해당 시간대 데이터 찾기
                    const matchingItems = defenseData.rawDeterrents.filter(item => {
                      try {
                        if (!item.start_time) return false;
                        
                        // 시작 시간을 기준으로 시간대 매칭
                        const itemDate = new Date(item.start_time);
                        if (isNaN(itemDate.getTime())) return false;
                        
                        const itemHour = itemDate.getHours();
                        return itemHour === hour;
                      } catch(e) {
                        console.error(`날짜 변환 오류: ${item.start_time}`, e);
                        return false;
                      }
                    });
                    
                    // 성공/실패 데이터 집계
                    if (matchingItems.length > 0) {
                      matchingItems.forEach(item => {
                        // is_success 필드로 성공/실패 구분
                        if (item.is_success) {
                          activationData.success += 1;
                        } else {
                          activationData.failure += 1;
                        }
                      });
                    }
                  }
                  
                  // 총 활성화 수
                  const totalActivations = activationData.success + activationData.failure;
                  // 성공률 계산 (데이터가 있는 경우에만)
                  const successRate = totalActivations > 0 
                    ? Math.round((activationData.success / totalActivations) * 100) 
                    : 0;
                  
                  return {
                    name: hourStr,
                    value: successRate,
                    count: totalActivations
                  };
                })
              }
              xKey="name"
              yKey="value"
              xLabel={translate('시간', 'Hour', 'Jam', language)}
              yLabel={translate('성공률(%)', 'Success Rate(%)', 'Tingkat Keberhasilan(%)', language)}
              color="#8884d8"
              tooltipLabel="%"
              height={300}
              hourlyTicks={true}
              displayAllTicks={true}
              periodType="hour"
            />
          </Paper>
        </>
      ) : (
        <Box sx={{ py: 5, display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
          <CircularProgress sx={{ mb: 2 }} size={40} />
          <Typography variant="h6" sx={{ mb: 1, textAlign: 'center' }}>
            {translate('퇴치 시스템 데이터를 불러오는 중입니다...', 'Loading deterrent system data...', 'Memuat data sistem pengendalian...', language)}
          </Typography>
          <Typography variant="body2" sx={{ textAlign: 'center', color: 'text.secondary' }}>
            {translate('잠시만 기다려주세요.', 'Please wait a moment.', 'Mohon tunggu sebentar.', language)}
          </Typography>
        </Box>
      )}
    </Box>
  );
};

export default DefenseSystemTab; 