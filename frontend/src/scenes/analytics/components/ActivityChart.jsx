import React, { useEffect, useState, useRef } from 'react';
import { Paper, Typography, Box, Divider, CircularProgress, Button, ButtonGroup } from '@mui/material';
import ZoomInIcon from '@mui/icons-material/ZoomIn';
import ZoomOutIcon from '@mui/icons-material/ZoomOut';
import RestartAltIcon from '@mui/icons-material/RestartAlt';
import axios from 'axios';
import { Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
} from 'chart.js';
import zoomPlugin from 'chartjs-plugin-zoom';

// Chart.js 컴포넌트 등록
ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler,
  zoomPlugin // 확대/축소 플러그인 등록
);

// 네온 글로우 효과 플러그인 추가
const neonGlowPlugin = {
  id: 'neonGlow',
  beforeDatasetsDraw(chart, args, pluginOptions) {
    const { ctx } = chart;
    ctx.save();
    ctx.shadowColor = 'rgba(0, 255, 255, 0.8)';
    ctx.shadowBlur = 15;
    ctx.shadowOffsetX = 0;
    ctx.shadowOffsetY = 0;
  },
  afterDatasetsDraw(chart, args, pluginOptions) {
    chart.ctx.restore();
  }
};

// Chart.js 컴포넌트에 플러그인 등록
ChartJS.register(neonGlowPlugin);

// 파일 상단에 추가
const API_BASE_URL = process.env.REACT_APP_API_URL || '';

/**
 * 사이버펑크 스타일 꺾은선 그래프 - 확대/축소 가능
 * @returns {JSX.Element}
 */
const ActivityChart = () => {
  const [activityData, setActivityData] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const chartRef = useRef(null);
  
  // 컴포넌트 마운트 시 확대/축소 플러그인 활성화 확인
  useEffect(() => {
    console.log("Chart.js plugins:", ChartJS.registry.plugins);
    console.log("Zoom plugin registered:", ChartJS.registry.plugins.get('zoom') !== undefined);
  }, []);
  
  useEffect(() => {
    const fetchActivityData = async () => {
      try {
        setLoading(true);
        
        // 백엔드 API에서 감지 데이터 가져오기
        const response = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
          params: {
            // 최근 3일 데이터만 조회
            date_from: formatDate(getDateDaysAgo(3)),
            date_to: formatDate(new Date()),
            sort_by: 'date_desc',
            per_page: 1000 // 충분한 데이터 수집을 위해
          }
        });
        
        if (response.data && response.data.detections && response.data.detections.length > 0) {
          // 1분 단위로 데이터 가공
          const detailedData = process1MinuteData(response.data.detections);
          // 유효한 데이터만 필터링 (time 속성이 있는지 확인)
          const validData = detailedData.filter(item => item && item.time);
          setActivityData(validData);
          console.log('1분 단위 활동 데이터 처리 완료:', validData.slice(0, 5));
        } else {
          console.warn('API에서 감지 데이터를 찾을 수 없습니다.');
          const sampleData = generateSample1MinuteData();
          setActivityData(sampleData);
        }
      } catch (err) {
        console.error('활동 데이터를 가져오는 중 오류 발생:', err);
        const sampleData = generateSample1MinuteData();
        setActivityData(sampleData);
      } finally {
        setLoading(false);
      }
    };

    fetchActivityData();
  }, []);

  // 1분 단위 데이터 가공 함수
  const process1MinuteData = (detections) => {
    // 시간 슬롯 맵 초기화
    const timeMap = new Map();
    
    // 24시간 * 60 (1분 간격) = 1440개의 시간 슬롯 생성
    for (let hour = 0; hour < 24; hour++) {
      for (let minute = 0; minute < 60; minute++) {
        const timeKey = `${hour.toString().padStart(2, '0')}:${minute.toString().padStart(2, '0')}`;
        timeMap.set(timeKey, { time: timeKey, activity: 0 });
      }
    }
    
    // 감지 데이터를 1분 단위로 집계
    if (Array.isArray(detections)) {
      detections.forEach(detection => {
        if (detection && detection.detection_time) {
          try {
            const detectionTime = new Date(detection.detection_time);
            const hour = detectionTime.getHours().toString().padStart(2, '0');
            const minute = detectionTime.getMinutes().toString().padStart(2, '0');
            const timeKey = `${hour}:${minute}`;
            
            if (timeMap.has(timeKey)) {
              const timeData = timeMap.get(timeKey);
              const count = detection.bb_count || 1;
              timeData.activity += count;
            }
          } catch (error) {
            console.error('시간 데이터 처리 오류:', error, detection);
          }
        }
      });
    }
    
    // 배열로 변환하여 시간 순으로 정렬
    return Array.from(timeMap.values())
      .sort((a, b) => {
        // null 체크 추가
        if (!a.time || !b.time) return 0;
        
        const [aHour, aMinute] = a.time.split(':').map(Number);
        const [bHour, bMinute] = b.time.split(':').map(Number);
        
        if (aHour !== bHour) {
          return aHour - bHour;
        }
        return aMinute - bMinute;
      });
  };

  // 날짜 포맷 헬퍼 함수
  const formatDate = (date) => {
    if (!date) return '';
    const year = date.getFullYear();
    const month = (date.getMonth() + 1).toString().padStart(2, '0');
    const day = date.getDate().toString().padStart(2, '0');
    return `${year}-${month}-${day}`;
  };

  // n일 전 날짜 구하기
  const getDateDaysAgo = (days) => {
    const date = new Date();
    date.setDate(date.getDate() - days);
    return date;
  };
  
  // 1분 단위 샘플 데이터 생성 함수 (API 실패 시 사용)
  const generateSample1MinuteData = () => {
    const sampleData = [];
    
    // 24시간 * 60 (1분 간격) = 1440개의 샘플 데이터 포인트 생성
    for (let hour = 0; hour < 24; hour++) {
      for (let minute = 0; minute < 60; minute++) {
        // 시간대별 활동 패턴 시뮬레이션
        let baseActivity;
        
        if (hour >= 0 && hour < 6) {
          // 심야 (활동 적음)
          baseActivity = 10 + Math.random() * 20;
        } else if (hour >= 6 && hour < 10) {
          // 아침 (활동 증가)
          baseActivity = 30 + Math.random() * 40;
        } else if (hour >= 10 && hour < 15) {
          // 낮 (활동 많음)
          baseActivity = 60 + Math.random() * 30;
        } else if (hour >= 15 && hour < 19) {
          // 저녁 (활동 매우 많음)
          baseActivity = 70 + Math.random() * 30;
        } else {
          // 밤 (활동 감소)
          baseActivity = 40 + Math.random() * 30;
        }
        
        // 더 세밀한 변동성 추가 (1분 단위)
        const noise = Math.sin(minute / 60 * Math.PI * 2) * 8 + (Math.random() - 0.5) * 15;
        
        const timeKey = `${hour.toString().padStart(2, '0')}:${minute.toString().padStart(2, '0')}`;
        sampleData.push({
          time: timeKey,
          activity: Math.max(0, Math.round(baseActivity + noise))
        });
      }
    }
    
    return sampleData;
  };

  // 라벨 포맷 함수 (시간 표시 가독성 향상)
  const formatTimeLabel = (time) => {
    // null, undefined 체크 추가
    if (!time) return '';
    
    const parts = time.split(':');
    if (!parts || parts.length < 2) return time;
    
    const [hour, minute] = parts;
    
    // 표시 기준에 따른 라벨 형식 변경
    if (minute === '00') {
      return `${hour}시`;
    } else if (minute === '30') {
      return `${hour}:30`;
    } else if (parseInt(minute) % 10 === 0) {
      // 10분 단위로만 숫자 표시 (확대 시)
      return `${hour}:${minute}`;
    } else {
      // 1분 단위는 빈 문자열 반환 (기본 상태에서는 표시 안 함)
      return '';
    }
  };

  // 확대/축소 조작 함수
  const handleZoomIn = () => {
    if (chartRef && chartRef.current) {
      const chart = chartRef.current;
      // Chart.js 3.x 버전 이상에서의 올바른 확대 방법
      chart.zoom(1.2); // 20% 확대
    }
  };
  
  const handleZoomOut = () => {
    if (chartRef && chartRef.current) {
      const chart = chartRef.current;
      chart.zoom(0.8); // 20% 축소
    }
  };
  
  const handleResetZoom = () => {
    if (chartRef && chartRef.current) {
      const chart = chartRef.current;
      chart.resetZoom();
    }
  };

  // 차트 그라디언트 생성 함수
  const createNeonGradient = (ctx) => {
    // 캔버스 크기에 맞게 그라디언트 생성
    const gradient = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);
    // 사이버펑크 스타일 그라디언트 색상 (약간 더 부드럽게 조정)
    gradient.addColorStop(0, 'rgba(60, 180, 255, 0.8)');    // 하늘색 (상단)
    gradient.addColorStop(0.5, 'rgba(180, 100, 255, 0.8)');  // 보라색 (중간)
    gradient.addColorStop(1, 'rgba(60, 180, 255, 0.8)');    // 하늘색 (하단)
    return gradient;
  };
  
  // 백그라운드 그라디언트 생성 함수
  const createBgGradient = (ctx) => {
    const gradient = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);
    gradient.addColorStop(0, 'rgba(60, 180, 255, 0.1)');
    gradient.addColorStop(1, 'rgba(180, 100, 255, 0.02)');
    return gradient;
  };

  // Chart.js 데이터 포맷으로 변환
  const chartData = {
    labels: activityData.map(item => formatTimeLabel(item && item.time ? item.time : '')),
    datasets: [
      {
        label: '활동량',
        data: activityData.map(item => item ? item.activity : 0),
        // 네온 효과를 위한 스타일링
        borderColor: (context) => {
          const ctx = context.chart.ctx;
          return createNeonGradient(ctx);
        },
        backgroundColor: (context) => {
          const ctx = context.chart.ctx;
          return createBgGradient(ctx);
        },
        tension: 0.2, // 더 날카로운 곡선으로 변경
        fill: true,
        pointRadius: 0, // 데이터 포인트가 너무 많아 기본적으로 숨김
        pointHoverRadius: 5, // 호버 시 표시
        pointHoverBackgroundColor: 'rgba(255, 0, 255, 1)',
        pointHoverBorderColor: 'rgba(255, 255, 255, 1)',
        pointHoverBorderWidth: 2,
        borderWidth: 2
      }
    ]
  };

  // 차트 옵션
  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    animation: {
      duration: 0 // 확대/축소가 더 부드럽게 작동하도록 애니메이션 비활성화
    },
    interaction: {
      mode: 'index',
      intersect: false,
    },
    plugins: {
      legend: {
        display: false
      },
      tooltip: {
        backgroundColor: 'rgba(0, 0, 20, 0.9)',
        titleColor: 'rgba(0, 255, 255, 1)',
        bodyColor: 'rgba(255, 255, 255, 0.9)',
        borderColor: 'rgba(0, 255, 255, 0.5)',
        borderWidth: 1,
        padding: 12,
        cornerRadius: 6,
        displayColors: false,
        titleFont: {
          size: 14,
          weight: 'bold'
        },
        bodyFont: {
          size: 13
        },
        callbacks: {
          title: (tooltipItem) => {
            if (!tooltipItem || !tooltipItem[0]) return '시간 정보';
            
            const index = tooltipItem[0].dataIndex;
            // 원본 시간 데이터 가져오기
            if (index >= 0 && index < activityData.length && activityData[index].time) {
              const timeStr = activityData[index].time;
              const [hour, minute] = timeStr.split(':');
              return `${hour}시 ${minute}분`;
            }
            
            return '시간 정보';
          },
          label: (context) => {
            return `활동량: ${context.raw || 0}`;
          }
        }
      },
      // 확대/축소 플러그인 설정
      zoom: {
        limits: {
          x: {min: 'original', max: 'original', minRange: 10},
          y: {min: 'original', max: 'original'}
        },
        pan: {
          enabled: true,
          mode: 'x',
          threshold: 5,
          // 드래그로 이동 활성화
          modifierKey: null
        },
        zoom: {
          wheel: {
            enabled: true, // 마우스 휠로 확대/축소 활성화
            speed: 0.1,
          },
          pinch: {
            enabled: true // 핀치 제스처로 확대/축소 활성화
          },
          mode: 'x', // x축만 확대/축소 가능
          // 드래그로 영역 확대 비활성화
          drag: {
            enabled: false
          },
          // 더블 클릭으로 확대 기능 비활성화
          doubleClick: {
            enabled: false
          }
        }
      }
    },
    scales: {
      x: {
        grid: {
          display: true,
          color: 'rgba(0, 255, 255, 0.1)',
          drawBorder: true,
          drawOnChartArea: true
        },
        ticks: {
          maxRotation: 45, // 레이블 회전 허용하여 겹침 방지
          minRotation: 0,
          color: 'rgba(0, 255, 255, 0.6)',
          font: {
            size: 10
          },
          autoSkip: true,
          maxTicksLimit: 12, // 기본 상태에서 표시할 최대 레이블 수 감소
          callback: function(value, index, values) {
            // 라벨 표시 로직
            const label = this.getLabelForValue(value);
            
            // 값이 없다면 원본 데이터에서 시간 찾기
            if (!label && activityData[value]) {
              const timeStr = activityData[value].time;
              if (timeStr) {
                const [hour, minute] = timeStr.split(':');
                // 정각이나 30분일 때만 표시
                if (minute === '00') {
                  return `${hour}시`;
                } else if (minute === '30') {
                  return `${hour}:30`;
                }
              }
            }
            
            return label;
          }
        }
      },
      y: {
        beginAtZero: true,
        grid: {
          display: true,
          color: 'rgba(255, 0, 255, 0.1)',
          drawBorder: true
        },
        ticks: {
          color: 'rgba(255, 255, 255, 0.6)',
          font: {
            size: 11
          },
          stepSize: 20
        }
      }
    },
    elements: {
      line: {
        borderJoinStyle: 'round'
      }
    }
  };

  return (
    <Paper sx={{ 
      p: 2.5, 
      bgcolor: '#050620', // 짙은 네이비/블랙 배경
      border: '1px solid rgba(0, 255, 255, 0.2)', 
      height: '100%',
      display: 'flex',
      flexDirection: 'column',
      borderRadius: 2,
      boxShadow: '0 0 15px rgba(0, 100, 255, 0.15), inset 0 0 15px rgba(0, 100, 255, 0.1)',
      overflow: 'hidden'
    }}>
      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 1 }}>
        <Typography 
          variant="subtitle1" 
          sx={{ 
            fontWeight: 600, 
            color: '#ffffff',
            textShadow: '0 0 3px rgba(120, 180, 255, 0.6)'
          }}
        >
          시간대별 조류 활동 패턴 (확대/축소 가능)
        </Typography>
        <ButtonGroup size="small" variant="outlined" sx={{ 
          '& .MuiButton-root': { 
            borderColor: 'rgba(100, 180, 255, 0.5)',
            color: '#ffffff',
            '&:hover': {
              borderColor: 'rgba(100, 180, 255, 0.8)',
              backgroundColor: 'rgba(100, 180, 255, 0.05)'
            }
          } 
        }}>
          <Button onClick={handleZoomIn} title="확대">
            <ZoomInIcon sx={{ fontSize: 16 }} />
          </Button>
          <Button onClick={handleZoomOut} title="축소">
            <ZoomOutIcon sx={{ fontSize: 16 }} />
          </Button>
          <Button onClick={handleResetZoom} title="리셋">
            <RestartAltIcon sx={{ fontSize: 16 }} />
          </Button>
        </ButtonGroup>
      </Box>
      <Typography variant="caption" sx={{ 
        color: 'rgba(255, 255, 255, 0.6)',
        mb: 1,
        fontSize: '0.7rem'
      }}>
        <span style={{ color: 'rgba(120, 180, 255, 0.9)' }}>마우스 휠로 확대/축소</span> • 드래그로 이동 가능
      </Typography>
      <Divider sx={{ mb: 2, borderColor: 'rgba(100, 180, 255, 0.3)' }} />
      <Box sx={{ 
        height: 340, 
        flex: 1,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        position: 'relative',
        px: 1,
        '& canvas': {
          cursor: 'grab' // 캔버스 커서를 그랩(손) 모양으로 변경하여 드래그로 이동 가능함을 표시
        }
      }}>
        {loading ? (
          <CircularProgress size={40} sx={{ color: 'rgba(0, 255, 255, 0.8)', opacity: 0.7 }} />
        ) : error ? (
          <Box textAlign="center">
            <Typography sx={{ mb: 1, color: 'rgba(255, 50, 50, 0.9)' }}>{error}</Typography>
          </Box>
        ) : (
          <Line 
            ref={chartRef} 
            data={chartData} 
            options={chartOptions} 
            plugins={[{
              id: 'customCanvasBackgroundColor',
              beforeDraw: (chart) => {
                // 차트 영역이 로드되면 커서 그랩(손) 모양으로 설정
                if (chart.canvas) {
                  chart.canvas.style.cursor = 'grab';
                }
              }
            }]}
          />
        )}
      </Box>
    </Paper>
  );
};

export default ActivityChart; 