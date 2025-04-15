import React from 'react';
import { Paper, Typography, Box, Divider, CircularProgress } from '@mui/material';

// 색상 배열
const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d'];

/**
 * 종류별 분포 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.distribution - 종류별 분포 데이터
 * @returns {JSX.Element}
 */
const DistributionChart = ({ distribution = [] }) => {
  // 배열 확인
  const distributionArray = Array.isArray(distribution) ? distribution : [];
  
  // 데이터가 없는 경우
  if (distributionArray.length === 0) {
    return (
      <Paper sx={{ p: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a', flexGrow: 1, minWidth: 300, minHeight: 250 }}>
        <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
          종류별 탐지 분포
        </Typography>
        <Divider sx={{ mb: 2 }} />
        <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: 200 }}>
          <Typography variant="body2" color="text.secondary">데이터가 없습니다</Typography>
        </Box>
      </Paper>
    );
  }
  
  // 데이터 최대값 계산
  const maxCount = Math.max(...distributionArray.map(s => s.count || 0));

  return (
    <Paper sx={{ p: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a', flexGrow: 1, minWidth: 300, minHeight: 250 }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        종류별 탐지 분포
      </Typography>
      <Divider sx={{ mb: 2 }} />
      <Box sx={{ display: 'flex', flexDirection: 'column', gap: 1 }}>
        {distributionArray.map((species, index) => (
          <Box key={species.name || `unknown-${index}`} sx={{ display: 'flex', alignItems: 'center', mb: 1 }}>
            <Box 
              sx={{ 
                width: 12, 
                height: 12, 
                borderRadius: '50%', 
                bgcolor: COLORS[index % COLORS.length],
                mr: 1 
              }} 
            />
            <Typography variant="body2" sx={{ flex: 1 }}>{species.name || '미분류'}</Typography>
            <Typography variant="body2" color="text.secondary">{species.count || 0}회</Typography>
            <Box 
              sx={{ 
                ml: 2, 
                height: 8, 
                width: `${(species.count || 0) / (maxCount || 1) * 100}%`, 
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