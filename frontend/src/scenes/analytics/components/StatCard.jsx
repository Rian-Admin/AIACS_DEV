import React from 'react';
import { Box, Card, CardContent, Typography } from '@mui/material';

/**
 * 통계 카드 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.title 카드 제목
 * @param {string|number} props.value 표시할 값
 * @param {string} props.subtitle 부제목 (선택사항)
 * @param {string} props.color 색상 (선택사항)
 * @param {React.ReactNode} props.icon 아이콘 (선택사항)
 * @returns {JSX.Element} 통계 카드 컴포넌트
 */
const StatCard = ({ title, value, subtitle, color, icon }) => (
  <Card sx={{ bgcolor: '#0a1929', border: '1px solid #1e3a5a', height: '100%' }}>
    <CardContent>
      <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start' }}>
        <Box>
          <Typography variant="h6" color="text.secondary" gutterBottom>
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

export default StatCard; 