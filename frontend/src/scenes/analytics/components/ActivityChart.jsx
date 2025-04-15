import React from 'react';
import { Paper, Typography, Box, Divider, Grid } from '@mui/material';

/**
 * 시간대별 활동 패턴 차트 컴포넌트
 * @returns {JSX.Element}
 */
const ActivityChart = () => {
  // 임의의 활동 데이터 생성
  const getRandomHeight = () => Math.random() * 100 + 20;
  const timeSlots = Array(12).fill(0).map((_, i) => ({
    hour: i * 2,
    activity: getRandomHeight()
  }));

  return (
    <Paper sx={{ p: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a', flexGrow: 1, minWidth: 300, minHeight: 250 }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        시간대별 활동 패턴
      </Typography>
      <Divider sx={{ mb: 2 }} />
      <Typography variant="body2" color="text.secondary" sx={{ mb: 2 }}>
        시간대별 활동 패턴이 여기에 표시됩니다.
      </Typography>
      <Grid container spacing={1}>
        {timeSlots.map((slot) => (
          <Grid item xs={1} key={slot.hour}>
            <Box sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
              <Box 
                sx={{ 
                  height: `${slot.activity}px`, 
                  width: '80%', 
                  bgcolor: 'primary.main',
                  borderRadius: '3px 3px 0 0',
                  opacity: 0.7
                }} 
              />
              <Typography variant="caption" color="text.secondary">{`${slot.hour}`}</Typography>
            </Box>
          </Grid>
        ))}
      </Grid>
    </Paper>
  );
};

export default ActivityChart; 