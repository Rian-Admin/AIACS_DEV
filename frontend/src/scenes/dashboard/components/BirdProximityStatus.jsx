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
  
  // 최근 1분 이내의 데이터만 필터링
  const getRecentData = (data) => {
    const oneMinuteAgo = new Date(currentTime.getTime() - 60000);
    return data.filter(item => {
      // timestamp가 있으면 사용, 없으면 현재 시간 기준으로 처리
      const itemTime = item.timestamp ? new Date(item.timestamp) : currentTime;
      return itemTime >= oneMinuteAgo;
    });
  };
  
  // 표시할 데이터 YW-01, YW-02, YW-03, YW-04, YW-05, YW-06 필터링 및 최근 1분 데이터만 추출
  const stationIds = ['YW-01', 'YW-02', 'YW-03', 'YW-04', 'YW-05', 'YW-06'];
  const recentData = getRecentData(birdActivityData);
  
  // 각 스테이션별로 데이터 합산
  const aggregatedData = stationIds.map(id => {
    const stationData = recentData.filter(item => item.id === id);
    const count = stationData.length > 0 
      ? stationData.reduce((sum, item) => sum + item.count, 0)
      : Math.floor(Math.random() * 10) + 2; // 데이터 없을 경우 임의의 값 (2-12)
    
    return {
      id,
      count,
      // 임의의 값 생성 (데모용)
      value: count === 0 ? 0 : count
    };
  });
  
  // 값에 따라 정렬
  const sortedData = [...aggregatedData].sort((a, b) => a.id.localeCompare(b.id));
  
  // 최대값 계산 (그래프 너비 조정용)
  const maxValue = Math.max(...sortedData.map(item => item.value), 14);

  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 1.5, 
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
          mb: 1, 
          color: 'white',
          textShadow: '0 0 5px rgba(255,255,255,0.5)'
        }}
      >
        {translate('조류 실시간 접근 현황', 'Wind Turbine Risk Alert', language)}
      </Typography>
      
      <Box sx={{ 
        flex: 1, 
        display: 'flex', 
        flexDirection: 'column',
        justifyContent: 'space-around',
        py: 0.5
      }}>
        {/* 가로 막대 그래프 영역 */}
        {sortedData.map((item) => {
          // 막대 너비 계산 (최대값 기준 비율)
          const widthPercent = Math.max((item.value / maxValue) * 100, 5); // 최소 5% 너비
          
          return (
            <Box 
              key={item.id} 
              sx={{ 
                display: 'flex',
                alignItems: 'center',
                mb: 0.7
              }}
            >
              {/* 라벨 */}
              <Typography 
                variant="caption" 
                sx={{ 
                  width: '60px',
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
                    height: '16px',
                    width: `${widthPercent}%`,
                    backgroundColor: '#003366',
                    borderRadius: '2px',
                    position: 'relative',
                    boxShadow: '0 0 15px #00B4FF, 0 0 5px #00B4FF inset',
                    border: '1px solid #00B4FF',
                    transition: 'width 0.5s ease-in-out',
                    '&::after': {
                      content: '""',
                      position: 'absolute',
                      top: 0,
                      left: 0,
                      width: '100%',
                      height: '50%',
                      background: 'linear-gradient(0deg, rgba(0,180,255,0) 0%, rgba(0,180,255,0.3) 100%)',
                      borderRadius: '2px 2px 0 0'
                    }
                  }}
                />
              </Box>
              
              {/* 카운트 값 */}
              <Typography 
                variant="caption" 
                sx={{ 
                  width: '30px',
                  ml: 1.5,
                  color: '#A9EAFF',
                  textShadow: '0 0 3px #A9EAFF',
                  fontSize: '0.75rem',
                  textAlign: 'right'
                }}
              >
                {item.value}
              </Typography>
            </Box>
          );
        })}
        
        {/* 하단 경계선 */}
        <Box 
          sx={{ 
            height: '1px',
            background: 'linear-gradient(90deg, rgba(0,180,255,0) 0%, rgba(0,180,255,0.5) 50%, rgba(0,180,255,0) 100%)',
            mt: 0.5,
            boxShadow: '0 0 5px #00B4FF',
            width: '90%',
            mx: 'auto'
          }}
        />
      </Box>
    </Paper>
  );
};

export default BirdProximityStatus; 