import React from 'react';
import { Paper, Typography, Box, Divider } from '@mui/material';

// 색상 배열
const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d'];

/**
 * 종류별 분포 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.distribution - 종류별 분포 데이터
 * @returns {JSX.Element}
 */
const DistributionChart = ({ distribution }) => {
  // 데이터 최대값 계산
  const maxCount = Math.max(...distribution.map(s => s.count));

  return (
    <Paper sx={{ p: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a', flexGrow: 1, minWidth: 300, minHeight: 250 }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        종류별 탐지 분포
      </Typography>
      <Divider sx={{ mb: 2 }} />
      <Box sx={{ display: 'flex', flexDirection: 'column', gap: 1 }}>
        {distribution.map((species, index) => (
          <Box key={species.name} sx={{ display: 'flex', alignItems: 'center', mb: 1 }}>
            <Box 
              sx={{ 
                width: 12, 
                height: 12, 
                borderRadius: '50%', 
                bgcolor: COLORS[index % COLORS.length],
                mr: 1 
              }} 
            />
            <Typography variant="body2" sx={{ flex: 1 }}>{species.name}</Typography>
            <Typography variant="body2" color="text.secondary">{species.count}회</Typography>
            <Box 
              sx={{ 
                ml: 2, 
                height: 8, 
                width: `${species.count / maxCount * 100}%`, 
                maxWidth: '200px',
                bgcolor: COLORS[index % COLORS.length],
                borderRadius: 4 
              }} 
            />
          </Box>
        ))}
      </Box>
    </Paper>
  );
};

export default DistributionChart; 