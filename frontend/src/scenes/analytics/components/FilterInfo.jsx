import React from 'react';
import { Box, Typography, Paper, CircularProgress } from '@mui/material';

// 종류 번역 함수
const translateSpecies = (species) => {
  const speciesMap = {
    'all': '모든 종류',
    'eagle': '독수리',
    'hawk': '매',
    'falcon': '팔콘',
    'seagull': '갈매기',
    'crow': '까마귀',
    'sparrow': '참새'
  };
  return speciesMap[species] || species;
};

/**
 * 필터 정보 표시 컴포넌트
 * @param {Object} props
 * @param {Object} props.filters - 필터 상태 객체
 * @param {boolean} props.isLoading - 로딩 상태
 * @param {Function} props.formatDate - 날짜 포맷 함수
 * @returns {JSX.Element}
 */
const FilterInfo = ({ filters, isLoading, formatDate }) => {
  const { timeRange, species, startDate, endDate } = filters;

  return (
    <Paper sx={{ p: 1.5, mb: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a' }}>
      <Box sx={{ 
        p: 1, 
        backgroundColor: 'rgba(0, 30, 60, 0.3)', 
        borderRadius: 1,
        border: '1px solid rgba(30, 136, 229, 0.2)',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'space-between'
      }}>
        <Typography variant="body2" color="text.secondary">
          적용된 필터: 
          <Box component="span" sx={{ ml: 1, color: 'primary.main', fontWeight: 'medium' }}>
            {timeRange === 'day' && '오늘'}
            {timeRange === 'week' && '이번 주'}
            {timeRange === 'month' && '이번 달'}
            {timeRange === 'year' && '올해'}
            {timeRange === 'custom' && `${formatDate(startDate)} - ${formatDate(endDate)}`}
          </Box>
          {species !== 'all' && (
            <Box component="span" sx={{ ml: 2, color: 'info.main', fontWeight: 'medium' }}>
              종류: {translateSpecies(species)}
            </Box>
          )}
        </Typography>
        {isLoading && <CircularProgress size={20} />}
      </Box>
    </Paper>
  );
};

export default FilterInfo; 