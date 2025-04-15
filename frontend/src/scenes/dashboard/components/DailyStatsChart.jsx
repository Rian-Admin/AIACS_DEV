import React from 'react';
import { Box, Typography, Tooltip } from '@mui/material';

/**
 * 일일 누적 현황을 표시하는 차트 컴포넌트 - 네온 스타일
 * @param {object} props
 * @param {Array} props.data - 데이터 배열 [{label, value, color}]
 */
const DailyStatsChart = ({ data }) => {
  // 최대값 계산 (그래프 스케일링용)
  const maxValue = Math.max(...data.map(item => item.value));
  
  return (
    <Box sx={{ 
      width: '100%',
      height: '100%',
      backgroundColor: '#050A18',
      borderRadius: 2,
      p: 1,
      display: 'flex',
      flexDirection: 'column'
    }}>
      {/* 제목 */}
      <Typography 
        variant="subtitle2" 
        fontWeight="bold" 
        sx={{ 
          mb: 1,
          color: 'white',
          textShadow: '0 0 5px rgba(255,255,255,0.5)'
        }}
      >
        조류 인식 일일 누적 현황
      </Typography>

      {/* 바 차트 */}
      <Box sx={{ 
        display: 'flex', 
        flex: 1,
        alignItems: 'flex-end',
        justifyContent: 'space-around',
        px: 2,
        pt: 1,
        pb: 2
      }}>
        {data.map((item, index) => (
          <Tooltip
            key={index}
            title={`${item.label}: ${item.value.toLocaleString()} 건`}
            arrow
            placement="top"
          >
            <Box
              sx={{
                width: '18%',
                height: `${(item.value / maxValue) * 100}%`,
                position: 'relative',
                transition: 'all 0.3s ease',
                cursor: 'pointer',
                '&:hover': {
                  transform: 'translateY(-5px) scale(1.02)',
                },
              }}
            >
              {/* 네온 글로우 효과와 함께 막대 */}
              <Box
                sx={{
                  width: '100%',
                  height: '100%',
                  borderRadius: 3,
                  background: `linear-gradient(180deg, ${item.color}CC 0%, ${item.color} 100%)`,
                  boxShadow: `
                    0 0 5px ${item.color}99,
                    0 0 10px ${item.color}99,
                    0 0 20px ${item.color}66,
                    0 0 40px ${item.color}33,
                    inset 0 0 15px rgba(255, 255, 255, 0.3)
                  `,
                  position: 'relative',
                  zIndex: 1,
                  display: 'flex',
                  flexDirection: 'column',
                  justifyContent: 'flex-end',
                  alignItems: 'center',
                  pb: 2
                }}
              >
                {/* 아이템 값 표시 */}
                <Typography
                  variant="body2"
                  sx={{
                    color: 'white',
                    fontWeight: 'bold',
                    textShadow: `
                      0 0 5px ${item.color}, 
                      0 0 10px ${item.color}99
                    `,
                    textAlign: 'center',
                    fontSize: '0.75rem'
                  }}
                >
                  {item.value.toLocaleString()}
                </Typography>
              </Box>
              
              {/* 바닥 반사 효과 */}
              <Box
                sx={{
                  width: '80%',
                  height: '8px',
                  background: `radial-gradient(ellipse at center, ${item.color}99 0%, transparent 70%)`,
                  position: 'absolute',
                  bottom: '-10px',
                  left: '10%',
                  filter: 'blur(3px)',
                  opacity: 0.7,
                  zIndex: 0
                }}
              />
              
              {/* 레이블 */}
              <Typography
                variant="caption"
                sx={{
                  color: item.color,
                  fontWeight: 'bold',
                  textShadow: `0 0 5px ${item.color}, 0 0 10px ${item.color}`,
                  position: 'absolute',
                  bottom: '-25px',
                  left: '50%',
                  transform: 'translateX(-50%)',
                  width: '100%',
                  textAlign: 'center',
                  fontSize: '0.7rem'
                }}
              >
                {item.label}
              </Typography>
            </Box>
          </Tooltip>
        ))}
      </Box>
    </Box>
  );
};

export default DailyStatsChart; 