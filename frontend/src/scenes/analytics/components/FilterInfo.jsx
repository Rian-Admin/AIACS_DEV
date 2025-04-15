import React from 'react';
import { Box, Typography, Paper, CircularProgress, Chip, Stack } from '@mui/material';
import FilterAltIcon from '@mui/icons-material/FilterAlt';

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
 * @param {Array} props.birdClasses - 조류 클래스 목록
 * @returns {JSX.Element}
 */
const FilterInfo = ({ filters, isLoading, formatDate, birdClasses = [] }) => {
  const { timeRange, startDate, endDate, birdClass, cameraId } = filters;
  
  // 조류 클래스 정보가 배열인지 확인
  const birdClassArray = Array.isArray(birdClasses) ? birdClasses : [];
  
  // 선택된 조류 클래스의 이름 찾기
  const getSelectedBirdClassName = () => {
    if (!birdClass) return '';
    const selectedBird = birdClassArray.find(bird => bird.class_id == birdClass);
    return selectedBird ? selectedBird.bird_name_ko : birdClass;
  };

  return (
    <Paper sx={{ p: 1.5, mb: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a' }}>
      <Box sx={{ 
        p: 1, 
        backgroundColor: 'rgba(0, 30, 60, 0.3)', 
        borderRadius: 1,
        border: '1px solid rgba(30, 136, 229, 0.2)',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'space-between',
        flexWrap: 'wrap',
        gap: 1
      }}>
        <Box sx={{ display: 'flex', alignItems: 'center', gap: 1 }}>
          <FilterAltIcon fontSize="small" color="primary" />
          <Typography variant="body2" color="text.secondary">적용된 필터:</Typography>
          
          <Stack direction="row" spacing={1} sx={{ flexWrap: 'wrap' }}>
            <Chip
              label={
                timeRange === 'day' ? '오늘' :
                timeRange === 'week' ? '이번 주' :
                timeRange === 'month' ? '이번 달' :
                timeRange === 'year' ? '올해' :
                timeRange === 'custom' ? `${formatDate(startDate)} - ${formatDate(endDate)}` :
                '전체 기간'
              }
              size="small"
              color="primary"
              variant="outlined"
            />
            
            {birdClass && (
              <Chip
                label={`조류: ${getSelectedBirdClassName()}`}
                size="small"
                color="info"
                variant="outlined"
              />
            )}
            
            {cameraId && (
              <Chip
                label={`카메라: ${cameraId}`}
                size="small"
                color="success"
                variant="outlined"
              />
            )}
          </Stack>
        </Box>
        
        {isLoading && <CircularProgress size={20} />}
      </Box>
    </Paper>
  );
};

export default FilterInfo; 