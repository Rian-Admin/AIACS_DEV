import React from 'react';
import { 
  Grid, 
  Paper, 
  Typography, 
  Box, 
  Divider,
} from '@mui/material';
import BarChartIcon from '@mui/icons-material/BarChart';
import ViewComfyIcon from '@mui/icons-material/ViewComfy';

/**
 * 숫자 포맷팅 헬퍼 함수
 * @param {number} num - 포맷팅할 숫자
 * @param {number} decimals - 소수점 자릿수
 * @returns {string} 포맷팅된 숫자 문자열
 */
const formatNumber = (num, decimals = 0) => {
  if (num === undefined || num === null) return '0';
  
  return Number(num).toLocaleString('ko-KR', {
    minimumFractionDigits: decimals,
    maximumFractionDigits: decimals
  });
};

/**
 * 통계 카드 컴포넌트
 * @param {Object} props
 * @param {Object} props.data - 통계 데이터
 * @returns {JSX.Element}
 */
const StatCards = ({ data }) => {
  const { totalDetections, statistics } = data;
  
  return (
    <Grid container spacing={2} sx={{ mb: 3 }}>
      {/* 조류 개체 수 카드 */}
      <Grid item xs={12} sm={6} md={6}>
        <Paper 
          sx={{ 
            p: 2, 
            bgcolor: '#0a1929',
            border: '1px solid #1e3a5a',
            height: '100%' 
          }}
        >
          <Box sx={{ display: 'flex', justifyContent: 'space-between' }}>
            <Box>
              <Typography variant="overline" color="primary">
                조류 개체 수
              </Typography>
              <Typography variant="h4" sx={{ mt: 1, mb: 0.5, fontWeight: 'bold' }}>
                {formatNumber(statistics?.count || 0)}
              </Typography>
              <Typography variant="body2" color="text.secondary">
                분석 기간 내 총 바운딩 박스 수
              </Typography>
            </Box>
            <BarChartIcon color="primary" sx={{ fontSize: 48, opacity: 0.7 }} />
          </Box>
        </Paper>
      </Grid>
      
      {/* 총 탐지 기록 카드 */}
      <Grid item xs={12} sm={6} md={6}>
        <Paper 
          sx={{ 
            p: 2, 
            bgcolor: '#0a1929',
            border: '1px solid #1e3a5a',
            height: '100%' 
          }}
        >
          <Box sx={{ display: 'flex', justifyContent: 'space-between' }}>
            <Box>
              <Typography variant="overline" color="info.main">
                총 탐지 횟수
              </Typography>
              <Typography variant="h4" sx={{ mt: 1, mb: 0.5, fontWeight: 'bold' }}>
                {console.log("Total Detections value:", totalDetections)}
                {formatNumber(totalDetections || 0)}
              </Typography>
              <Typography variant="body2" color="text.secondary">
                필터링된 모든 탐지 기록
              </Typography>
            </Box>
            <ViewComfyIcon color="info" sx={{ fontSize: 48, opacity: 0.7 }} />
          </Box>
        </Paper>
      </Grid>
    </Grid>
  );
};

export default StatCards; 