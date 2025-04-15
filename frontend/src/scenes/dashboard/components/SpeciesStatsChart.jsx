import React from 'react';
import { Box, Typography, Tooltip } from '@mui/material';

/**
 * 조류 종별 누적 현황을 표시하는 차트 컴포넌트
 * @param {object} props
 * @param {Array} props.data - 데이터 배열 [{name, count, color}]
 */
const SpeciesStatsChart = ({ data }) => {
  // 데이터 정렬 (큰 값 -> 작은 값)
  const sortedData = [...data].sort((a, b) => b.count - a.count);
  
  // 전체 합계 계산
  const total = data.reduce((sum, item) => sum + item.count, 0);
  
  return (
    <Box sx={{ width: '100%' }}>
      {sortedData.map((item, index) => {
        // 비율 계산
        const ratio = item.count / total;
        const percent = (ratio * 100).toFixed(1);
        
        return (
          <Tooltip
            key={index}
            title={`${item.name}: ${item.count.toLocaleString()} 건 (${percent}%)`}
            arrow
            placement="right"
          >
            <Box sx={{ mb: 1.5, cursor: 'pointer' }}>
              <Box sx={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', mb: 0.5 }}>
                <Typography 
                  variant="caption" 
                  sx={{ 
                    color: 'white',
                    display: 'flex',
                    alignItems: 'center'
                  }}
                >
                  <Box
                    component="span"
                    sx={{
                      width: 10,
                      height: 10,
                      backgroundColor: item.color,
                      borderRadius: '50%',
                      display: 'inline-block',
                      mr: 1
                    }}
                  />
                  {item.name}
                </Typography>
                <Typography 
                  variant="caption" 
                  sx={{ 
                    color: 'white',
                    fontSize: '0.75rem'
                  }}
                >
                  {item.count.toLocaleString()}
                </Typography>
              </Box>
              <Box 
                sx={{ 
                  width: '100%', 
                  height: 6, 
                  backgroundColor: 'rgba(255,255,255,0.1)',
                  borderRadius: 1,
                  overflow: 'hidden'
                }}
              >
                <Box 
                  sx={{ 
                    width: `${percent}%`, 
                    height: '100%', 
                    backgroundColor: item.color,
                    borderRadius: 1,
                    transition: 'width 1s ease-in-out'
                  }}
                />
              </Box>
            </Box>
          </Tooltip>
        );
      })}
      
      {/* 요약 정보 */}
      <Box 
        sx={{ 
          mt: 2, 
          pt: 1, 
          borderTop: '1px solid rgba(255,255,255,0.1)',
          display: 'flex',
          justifyContent: 'space-between'
        }}
      >
        <Typography variant="caption" sx={{ color: 'rgba(255,255,255,0.7)' }}>
          전체 종 수: {data.length}종
        </Typography>
        <Typography variant="caption" sx={{ color: 'rgba(255,255,255,0.7)' }}>
          총 인식 수: {total.toLocaleString()}건
        </Typography>
      </Box>
    </Box>
  );
};

export default SpeciesStatsChart; 