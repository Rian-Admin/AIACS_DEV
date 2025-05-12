import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper } from '@mui/material';
import { translate } from '../../../utils/i18n';

const BirdProximityStatus = ({ birdActivityData, language }) => {
  const [currentTime, setCurrentTime] = useState(new Date());
  
  // 1분마다 현재 시간 업데이트
  useEffect(() => {
    const timer = setInterval(() => {
      setCurrentTime(new Date());
    }, 60000); // 1분 간격
    
    return () => clearInterval(timer);
  }, []);
  
  // 최근 5분 이내의 데이터만 필터링 (1분에서 5분으로 변경)
  const getRecentData = (data) => {
    const fiveMinutesAgo = new Date(currentTime.getTime() - 5 * 60000);
    return data.filter(item => {
      // timestamp가 있으면 사용, 없으면 현재 시간 기준으로 처리
      const itemTime = item.timestamp ? new Date(item.timestamp) : currentTime;
      return itemTime >= fiveMinutesAgo;
    });
  };
  
  // 표시할 데이터 SG-01, SG-02, SG-03로 변경
  const stationIds = ['SG-01', 'SG-02', 'SG-03'];
  const recentData = getRecentData(birdActivityData);
  
  // 각 카메라별로 데이터 합산
  const aggregatedData = stationIds.map(id => {
    // 카메라 ID 번호 (SG-01 -> 1, SG-02 -> 2, SG-03 -> 3)
    const cameraNumber = parseInt(id.split('-')[1]);
    
    // 이 카메라 ID에 해당하는 데이터 필터링
    const stationData = recentData.filter(item => {
      // 새로운 SG 형식과 기존 YW 형식 모두 지원
      return item.id === id || 
             (cameraNumber === 1 && item.id === 'YW-01') ||
             (cameraNumber === 2 && item.id === 'YW-02') ||
             (cameraNumber === 3 && item.id === 'YW-03');
    });
    
    // 해당 카메라의 바운딩 박스 개수 계산
    const count = stationData.length > 0 
      ? stationData.reduce((sum, item) => sum + (item.count || 0), 0)
      : 0; // 데이터 없을 경우 0으로 설정 (랜덤 제거)
    
    return {
      id,
      count,
      // 값은 항상 실제 count 값 사용
      value: count
    };
  });
  
  // 값에 따라 정렬
  const sortedData = [...aggregatedData].sort((a, b) => a.id.localeCompare(b.id));
  
  // 최대값 계산 (그래프 너비 조정용)
  const maxValue = Math.max(...sortedData.map(item => item.value), 10);
  
  // 모든 데이터가 0인지 확인
  const allZero = sortedData.every(item => item.value === 0);

  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 1, 
        borderRadius: 2, 
        backgroundColor: '#050A18',
        border: '1px solid rgba(30, 58, 90, 0.3)',
        height: '100%',
        display: 'flex',
        flexDirection: 'column'
      }}
    >
      <Typography 
        variant="subtitle2" 
        fontWeight="bold" 
        sx={{ 
          mb: 0.5,
          color: 'white',
          textShadow: '0 0 5px rgba(255,255,255,0.5)',
          fontSize: '0.85rem'
        }}
      >
        {translate('조류 실시간 접근 현황', 'Real-time Bird Detection', language)}
      </Typography>
      
      <Box sx={{ 
        flex: 1, 
        display: 'flex', 
        flexDirection: 'column',
        justifyContent: 'space-around',
        py: 0.5
      }}>
        {/* 모든 값이 0인 경우 메시지 표시 */}
        {allZero ? (
          <Box 
            sx={{ 
              display: 'flex',
              justifyContent: 'center',
              alignItems: 'center',
              height: '100%'
            }}
          >
            <Typography 
              variant="caption" 
              sx={{ 
                color: 'rgba(169,234,255,0.6)',
                textAlign: 'center',
                fontSize: '0.75rem'
              }}
            >
              {translate('데이터가 없습니다', 'No data available', language)}
            </Typography>
          </Box>
        ) : (
          /* 가로 막대 그래프 영역 */
          sortedData.map((item) => {
            // 막대 너비 계산 (최대값 기준 비율)
            const widthPercent = item.value === 0 ? 0 : Math.max((item.value / maxValue) * 100, 5); // 최소 5% 너비
            
            return (
              <Box 
                key={item.id} 
                sx={{ 
                  display: 'flex',
                  alignItems: 'center',
                  mb: 0.5
                }}
              >
                {/* 라벨 */}
                <Typography 
                  variant="caption" 
                  sx={{ 
                    width: '50px',
                    color: '#00B4FF',
                    fontWeight: 'bold',
                    textShadow: '0 0 5px #00B4FF',
                    fontSize: '0.75rem',
                    pr: 1
                  }}
                >
                  {item.id}
                </Typography>
                
                {/* 가로 막대 */}
                <Box sx={{ flex: 1, position: 'relative' }}>
                  <Box 
                    sx={{ 
                      height: '14px',
                      width: `${widthPercent}%`,
                      backgroundColor: '#003366',
                      borderRadius: '2px',
                      position: 'relative',
                      boxShadow: item.value > 0 ? '0 0 15px #00B4FF, 0 0 5px #00B4FF inset' : 'none',
                      border: item.value > 0 ? '1px solid #00B4FF' : '1px dashed rgba(0, 180, 255, 0.3)',
                      transition: 'width 0.5s ease-in-out',
                      '&::after': item.value > 0 ? {
                        content: '""',
                        position: 'absolute',
                        top: 0,
                        left: 0,
                        width: '100%',
                        height: '50%',
                        background: 'linear-gradient(0deg, rgba(0,180,255,0) 0%, rgba(0,180,255,0.3) 100%)',
                        borderRadius: '2px 2px 0 0'
                      } : {}
                    }}
                  />
                </Box>
                
                {/* 카운트 값 */}
                <Typography 
                  variant="caption" 
                  sx={{ 
                    width: '25px',
                    ml: 1,
                    color: item.value > 0 ? '#A9EAFF' : 'rgba(169,234,255,0.5)',
                    textShadow: item.value > 0 ? '0 0 3px #A9EAFF' : 'none',
                    fontSize: '0.75rem',
                    textAlign: 'right'
                  }}
                >
                  {item.value}
                </Typography>
              </Box>
            );
          })
        )}
        
        {/* 하단 시간 표시 */}
        <Typography 
          variant="caption" 
          sx={{ 
            color: 'rgba(169,234,255,0.6)',
            textAlign: 'center',
            fontSize: '0.65rem',
            mt: 0.5
          }}
        >
          {currentTime.toLocaleTimeString(undefined, {
            hour: '2-digit',
            minute: '2-digit',
            hour12: false
          })} {translate('기준', 'updated', language)}
        </Typography>
      </Box>
    </Paper>
  );
};

export default BirdProximityStatus; 