import React from 'react';
import { Paper, Typography, Box, Divider, Grid } from '@mui/material';

/**
 * 시간대별 활동 패턴 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.activityData - 시간대별 활동 데이터 배열 (선택 사항)
 * @returns {JSX.Element}
 */
const ActivityChart = ({ activityData = [] }) => {
  // activityData가 배열인지 확인
  const activityArray = Array.isArray(activityData) ? activityData : [];
  
  // 데이터가 없으면 기본 패턴 생성
  let timeSlots;
  if (activityArray.length === 0) {
    // 임의의 활동 데이터 생성
    const getRandomHeight = () => Math.floor(Math.random() * 80) + 20;
    timeSlots = Array(12).fill(0).map((_, i) => ({
      hour: i * 2,
      activity: getRandomHeight()
    }));
  } else {
    timeSlots = activityArray;
  }
  
  // 최대 활동량 계산 (차트 스케일링용)
  const maxActivity = Math.max(...timeSlots.map(slot => slot.activity));
  
  // 활동량을 차트 높이로 변환 (최대 120px)
  const getBarHeight = (activity) => {
    return (activity / (maxActivity || 1)) * 120;
  };

  return (
    <Paper sx={{ p: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a', flexGrow: 1, minWidth: 300, minHeight: 250 }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        시간대별 활동 패턴
      </Typography>
      <Divider sx={{ mb: 2 }} />
      <Box sx={{ display: 'flex', alignItems: 'flex-end', justifyContent: 'space-between', height: 160, mb: 1 }}>
        {timeSlots.map((slot) => (
          <Box 
            key={slot.hour} 
            sx={{ 
              display: 'flex', 
              flexDirection: 'column', 
              alignItems: 'center',
              width: '7%'
            }}
          >
            <Box 
              sx={{ 
                height: `${getBarHeight(slot.activity)}px`, 
                width: '70%', 
                bgcolor: 'primary.main',
                borderRadius: '3px 3px 0 0',
                opacity: 0.7,
                transition: 'height 0.3s ease',
                '&:hover': {
                  opacity: 1,
                  bgcolor: 'primary.light'
                }
              }} 
            />
            <Typography variant="caption" color="text.secondary" sx={{ mt: 0.5 }}>
              {`${slot.hour}시`}
            </Typography>
          </Box>
        ))}
      </Box>
    </Paper>
  );
};

export default ActivityChart; 