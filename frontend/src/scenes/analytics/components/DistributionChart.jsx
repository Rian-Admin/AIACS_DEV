import React from 'react';
import { Paper, Typography, Box, Divider, CircularProgress } from '@mui/material';
import { PieChart, Pie, Cell, ResponsiveContainer, Tooltip, Legend } from 'recharts';

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
        <Divider sx={{ mb: 4 }} />
        <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: 200 }}>
          <Typography variant="body2" color="text.secondary">데이터가 없습니다</Typography>
        </Box>
      </Paper>
    );
  }
  
  // 도넛 차트를 위한 데이터 처리
  const totalCount = distributionArray.reduce((sum, item) => sum + (item.count || 0), 0);
  
  const significantData = [];
  let othersCount = 0;
  
  // 5% 이상인 항목만 남기고 나머지는 '기타'로 묶음
  distributionArray.forEach(item => {
    const percent = (item.count || 0) / totalCount;
    if (percent > 0.05) {
      significantData.push(item);
    } else {
      othersCount += (item.count || 0);
    }
  });
  
  // '기타' 항목이 있으면 추가
  if (othersCount > 0) {
    significantData.push({
      name: '기타',
      count: othersCount
    });
  }
  
  // 데이터 정렬 (내림차순)
  significantData.sort((a, b) => b.count - a.count);
  
  // 차트용 데이터 포맷 변환
  const chartData = significantData.map(item => ({
    name: item.name || '미분류',
    value: item.count || 0
  }));

  // 커스텀 툴팁
  const CustomTooltip = ({ active, payload }) => {
    if (active && payload && payload.length) {
      const percent = totalCount > 0 
        ? ((payload[0].value / totalCount) * 100).toFixed(1) 
        : 0;
      
      return (
        <Box sx={{ bgcolor: 'background.paper', p: 1, border: '1px solid grey', borderRadius: 1 }}>
          <Typography variant="body2" color="textPrimary">
            {payload[0].name}: {payload[0].value}회 ({percent}%)
          </Typography>
        </Box>
      );
    }
    return null;
  };

  return (
    <Paper sx={{ 
      p: 2, 
      bgcolor: '#0a1929', 
      border: '1px solid #1e3a5a', 
      height: '100%',
      display: 'flex',
      flexDirection: 'column'
    }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        종별 탐지 분포
      </Typography>
      <Divider sx={{ mb: 1 }} />
      
      <ResponsiveContainer width="100%" height={180} style={{ flex: '0 0 auto', marginBottom: 15 }}>
        <PieChart>
          <Pie
            data={chartData}
            cx="50%"
            cy="50%"
            labelLine={false}
            outerRadius={70}
            innerRadius={30}
            fill="#8884d8"
            dataKey="value"
            nameKey="name"
            label={({ name, percent }) => {
              // 이름이 너무 길면 축약
              const displayName = name.length > 8 
                ? `${name.substring(0, 6)}...` 
                : name;
              return `${displayName} ${(percent * 100).toFixed(0)}%`;
            }}
          >
            {chartData.map((entry, index) => (
              <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
            ))}
          </Pie>
          <Tooltip content={<CustomTooltip />} />
        </PieChart>
      </ResponsiveContainer>
      
      {/* 커스텀 범례 추가 */}
      <Box sx={{ 
        display: 'flex', 
        flexWrap: 'wrap',
        justifyContent: 'center', 
        gap: 2,
        mt: 2,
        mb: 2
      }}>
        {significantData.map((item, index) => (
          <Box key={item.name} sx={{ display: 'flex', alignItems: 'center' }}>
            <Box 
              sx={{ 
                width: 12, 
                height: 12, 
                borderRadius: '50%', 
                bgcolor: COLORS[index % COLORS.length],
                mr: 0.5 
              }} 
            />
            <Typography variant="caption">{item.name}</Typography>
          </Box>
        ))}
      </Box>
      
      <Divider sx={{ mb: 2 }} />
      
      {/* 상세 수치 표시 */}
      <Box sx={{ 
        display: 'flex', 
        flexDirection: 'column', 
        gap: 0.7, 
        mt: 1,
        flex: '1 1 auto',
        overflow: 'auto'
      }}>
        {significantData.map((species, index) => {
          const percent = totalCount > 0 ? ((species.count || 0) / totalCount * 100).toFixed(1) : 0;
          return (
            <Box key={species.name || `unknown-${index}`} sx={{ display: 'flex', alignItems: 'center' }}>
              <Box 
                sx={{ 
                  width: 10, 
                  height: 10, 
                  borderRadius: '50%', 
                  bgcolor: COLORS[index % COLORS.length],
                  mr: 1 
                }} 
              />
              <Typography variant="body2" sx={{ flex: 1 }}>{species.name || '미분류'}</Typography>
              <Typography variant="body2" color="text.secondary">
                {species.count || 0}회 ({percent}%)
              </Typography>
            </Box>
          );
        })}
      </Box>
    </Paper>
  );
};

export default DistributionChart; 