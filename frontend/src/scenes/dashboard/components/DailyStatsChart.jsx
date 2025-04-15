import React from 'react';
import { Box, Typography, Tooltip } from '@mui/material';

/**
 * 일일 누적 현황을 표시하는 차트 컴포넌트
 * @param {object} props
 * @param {Array} props.data - 데이터 배열 [{label, value, color}]
 */
const DailyStatsChart = ({ data }) => {
  // 최대값 계산 (그래프 스케일링용)
  const maxValue = Math.max(...data.map(item => item.value));
  
  return (
    <Box sx={{ width: '100%' }}>
      {/* 바 차트 */}
      <Box sx={{ display: 'flex', height: 180, alignItems: 'flex-end', mb: 1 }}>
        {data.map((item, index) => (
          <Tooltip
            key={index}
            title={`${item.label}: ${item.value.toLocaleString()} 건`}
            arrow
            placement="top"
          >
            <Box
              sx={{
                flex: 1,
                height: `${(item.value / maxValue) * 100}%`,
                backgroundColor: item.color,
                mx: 0.5,
                borderRadius: '4px 4px 0 0',
                position: 'relative',
                transition: 'all 0.3s ease',
                '&:hover': {
                  filter: 'brightness(1.2)',
                  transform: 'translateY(-5px)'
                },
                display: 'flex',
                flexDirection: 'column',
                justifyContent: 'flex-end',
                alignItems: 'center',
                cursor: 'pointer'
              }}
            >
              <Typography
                variant="caption"
                sx={{
                  color: 'white',
                  fontSize: '0.65rem',
                  fontWeight: 'bold',
                  textShadow: '0 0 2px rgba(0,0,0,0.7)',
                  mb: 0.5,
                  px: 0.5,
                  textAlign: 'center',
                  width: '100%',
                  overflow: 'hidden',
                  textOverflow: 'ellipsis',
                  whiteSpace: 'nowrap'
                }}
              >
                {item.value.toLocaleString()}
              </Typography>
            </Box>
          </Tooltip>
        ))}
      </Box>
      
      {/* X축 레이블 */}
      <Box sx={{ display: 'flex', mt: 1 }}>
        {data.map((item, index) => (
          <Box
            key={index}
            sx={{
              flex: 1,
              textAlign: 'center',
              px: 0.5
            }}
          >
            <Typography
              variant="caption"
              sx={{
                color: 'rgba(255, 255, 255, 0.7)',
                fontSize: '0.7rem',
                display: 'block',
                overflow: 'hidden',
                textOverflow: 'ellipsis',
                whiteSpace: 'nowrap'
              }}
            >
              {item.label}
            </Typography>
          </Box>
        ))}
      </Box>
    </Box>
  );
};

export default DailyStatsChart; 