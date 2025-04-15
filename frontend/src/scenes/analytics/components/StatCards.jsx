import React from 'react';
import { Grid, Box, Typography, Card, CardContent } from '@mui/material';
import BarChartIcon from '@mui/icons-material/BarChart';
import WarningIcon from '@mui/icons-material/Warning';
import BirdIcon from '@mui/icons-material/Pets';
import TrendingUpIcon from '@mui/icons-material/TrendingUp';

/**
 * 통계 카드 컴포넌트
 * @param {Object} props
 * @param {string} props.title - 카드 제목
 * @param {string|number} props.value - 카드 값
 * @param {string} props.subtitle - 카드 부제목
 * @param {string} props.color - 카드 색상
 * @param {React.ReactNode} props.icon - 카드 아이콘
 * @returns {JSX.Element}
 */
const StatCard = ({ title, value, subtitle, color, icon }) => (
  <Card sx={{ bgcolor: '#0a1929', border: '1px solid #1e3a5a', height: '100%' }}>
    <CardContent>
      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start' }}>
        <Box>
          <Typography variant="subtitle2" color="text.secondary" gutterBottom>
            {title}
          </Typography>
          <Typography variant="h4" component="div" color={color || 'primary.main'}>
            {value}
          </Typography>
          {subtitle && (
            <Typography variant="body2" color="text.secondary" sx={{ mt: 1 }}>
              {subtitle}
            </Typography>
          )}
        </Box>
        {icon && (
          <Box sx={{ color: color || 'primary.main', fontSize: 40 }}>
            {icon}
          </Box>
        )}
      </Box>
    </CardContent>
  </Card>
);

/**
 * 통계 카드 섹션 컴포넌트
 * @param {Object} props
 * @param {Object} props.data - 통계 데이터
 * @returns {JSX.Element}
 */
const StatCards = ({ data }) => {
  return (
    <Grid container spacing={2} sx={{ mb: 2 }}>
      <Grid item xs={12} sm={6} md={3}>
        <StatCard 
          title="총 탐지 수"
          value={data.totalDetections.toLocaleString()}
          subtitle="전체 조류 탐지 횟수"
          color="#2196f3"
          icon={<BarChartIcon />}
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <StatCard 
          title="금일 탐지 수"
          value={data.todayDetections.toLocaleString()}
          subtitle="오늘 발생한 조류 탐지"
          color="#4caf50"
          icon={<BirdIcon />}
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <StatCard 
          title="고위험 탐지"
          value={data.highRiskDetections.toLocaleString()}
          subtitle={`전체의 ${Math.round(data.highRiskDetections / data.totalDetections * 100)}%`}
          color="#f44336"
          icon={<WarningIcon />}
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <StatCard 
          title="탐지 증가율"
          value="+12.5%"
          subtitle="지난 주 대비"
          color="#ff9800"
          icon={<TrendingUpIcon />}
        />
      </Grid>
    </Grid>
  );
};

export default StatCards; 