import React from 'react';
import { 
  Grid, 
  Paper, 
  Typography, 
  Box, 
  Divider,
} from '@mui/material';
import BarChartIcon from '@mui/icons-material/BarChart';
import BubbleChartIcon from '@mui/icons-material/BubbleChart';
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
      {/* 총 탐지 수 카드 */}
      <Grid item xs={12} sm={6} md={4}>
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
                총 탐지 수
              </Typography>
              <Typography variant="h4" sx={{ mt: 1, mb: 0.5, fontWeight: 'bold' }}>
                {formatNumber(totalDetections)}
              </Typography>
              <Typography variant="body2" color="text.secondary">
                필터링된 모든 탐지 기록
              </Typography>
            </Box>
            <BarChartIcon color="primary" sx={{ fontSize: 48, opacity: 0.7 }} />
          </Box>
        </Paper>
      </Grid>
      
      {/* 평균 바운딩 박스 크기 */}
      <Grid item xs={12} sm={6} md={4}>
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
              <Typography variant="overline" color="success.main">
                평균 바운딩 박스 크기
              </Typography>
              <Box sx={{ display: 'flex', alignItems: 'baseline', mt: 1, mb: 0.5 }}>
                <Typography variant="h4" sx={{ fontWeight: 'bold' }}>
                  {formatNumber(statistics?.avgSize || 0, 1)}%
                </Typography>
                <Typography variant="subtitle1" sx={{ ml: 1 }}>
                  × {formatNumber(statistics?.avgHeight || 0, 1)}%
                </Typography>
              </Box>
              <Typography variant="body2" color="text.secondary">
                너비 × 높이 (이미지 대비 %)
              </Typography>
            </Box>
            <BubbleChartIcon color="success" sx={{ fontSize: 48, opacity: 0.7 }} />
          </Box>
        </Paper>
      </Grid>
      
      {/* 바운딩 박스 데이터 수 */}
      <Grid item xs={12} sm={6} md={4}>
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
                바운딩 박스 데이터 수
              </Typography>
              <Typography variant="h4" sx={{ mt: 1, mb: 0.5, fontWeight: 'bold' }}>
                {formatNumber(statistics?.count || 0)}
              </Typography>
              <Typography variant="body2" color="text.secondary">
                분석 기간 내 총 바운딩 박스 수
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