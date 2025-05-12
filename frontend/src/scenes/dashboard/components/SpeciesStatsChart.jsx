import React from 'react';
import { Box, Typography, Tooltip } from '@mui/material';

/**
 * 조류 종별 누적 현황을 표시하는 차트 컴포넌트 - 네온 스타일
 * @param {object} props
 * @param {Array} props.data - 데이터 배열 [{name, count, color}]
 */
const SpeciesStatsChart = ({ data }) => {
  // 데이터가 없거나 빈 배열인 경우 체크
  if (!data || data.length === 0 || data.every(item => item.count === 0)) {
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
        <Typography variant="subtitle2" sx={{ 
          color: 'white', 
          mb: 1, 
          fontWeight: 'bold',
          textShadow: '0 0 5px rgba(255,255,255,0.5)'
        }}>
          조류 종별 일일 누적 현황
        </Typography>
        
        <Box sx={{ 
          flex: 1, 
          display: 'flex', 
          alignItems: 'center', 
          justifyContent: 'center' 
        }}>
          <Typography 
            variant="body2" 
            sx={{ 
              color: 'rgba(169,234,255,0.7)',
              textShadow: '0 0 3px rgba(169,234,255,0.4)',
              fontSize: '0.8rem'
            }}
          >
            데이터가 없습니다
          </Typography>
        </Box>
      </Box>
    );
  }

  // 데이터 정렬 (큰 값 -> 작은 값)
  const sortedData = [...data].sort((a, b) => b.count - a.count);
  
  // 전체 합계 계산
  const total = data.reduce((sum, item) => sum + item.count, 0);
  
  // 최대값 계산 (그래프 스케일링용)
  const maxValue = Math.max(...data.map(item => item.count));
  
  // 상위 5개만 표시하고 나머지는 '기타'로 묶기
  let displayData = sortedData;
  if (sortedData.length > 5) {
    const top5 = sortedData.slice(0, 5);
    const others = sortedData.slice(5);
    const othersCount = others.reduce((sum, item) => sum + item.count, 0);
    
    displayData = [
      ...top5,
      {
        name: '기타',
        count: othersCount,
        color: '#7986CB' // 보라색 계열
      }
    ];
  }
  
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
      <Typography variant="subtitle2" sx={{ 
        color: 'white', 
        mb: 1, 
        fontWeight: 'bold',
        textShadow: '0 0 5px rgba(255,255,255,0.5)'
      }}>
        조류 종별 일일 누적 현황
      </Typography>
      
      <Box sx={{ flex: 1, overflow: 'hidden', display: 'flex', flexDirection: 'column' }}>
        {displayData.map((item, index) => {
          // 비율 계산
          const ratio = item.count / maxValue;
          
          return (
            <Box key={index} sx={{ mb: 1, position: 'relative' }}>
              <Box sx={{ 
                display: 'flex', 
                alignItems: 'center', 
                justifyContent: 'space-between', 
                mb: 0.3,
                position: 'relative',
                zIndex: 2
              }}>
                <Box sx={{
                  display: 'flex',
                  alignItems: 'center'
                }}>
                  <Box
                    component="span"
                    sx={{
                      width: 8,
                      height: 8,
                      backgroundColor: item.color,
                      borderRadius: '50%',
                      display: 'inline-block',
                      mr: 0.7,
                      boxShadow: `0 0 4px ${item.color}, 0 0 6px ${item.color}`
                    }}
                  />
                  <Typography 
                    variant="caption" 
                    sx={{ 
                      color: 'white',
                      fontWeight: 500,
                      textShadow: '0 0 5px rgba(255, 255, 255, 0.5)',
                      fontSize: '0.7rem'
                    }}
                  >
                    {item.name}
                  </Typography>
                </Box>
                <Typography 
                  variant="caption" 
                  sx={{ 
                    color: 'white',
                    fontWeight: 600,
                    textShadow: `0 0 5px ${item.color}`,
                    fontSize: '0.7rem'
                  }}
                >
                  {item.count.toLocaleString()}
                </Typography>
              </Box>
              
              {/* 네온 막대 */}
              <Box 
                sx={{ 
                  width: '100%', 
                  height: 6, 
                  backgroundColor: 'rgba(15, 25, 40, 0.6)',
                  borderRadius: 10,
                  overflow: 'hidden',
                  position: 'relative'
                }}
              >
                <Box 
                  sx={{ 
                    width: `${Math.max(ratio * 100, 5)}%`, 
                    height: '100%', 
                    borderRadius: 10,
                    background: `linear-gradient(90deg, ${item.color} 0%, ${item.color}99 80%, ${item.color}66 100%)`,
                    boxShadow: `0 0 6px ${item.color}, 0 0 8px ${item.color}99, inset 0 0 4px rgba(255, 255, 255, 0.8)`,
                    transition: 'width 1s ease-in-out',
                    position: 'relative',
                    '&::after': {
                      content: '""',
                      position: 'absolute',
                      top: 0,
                      left: 0,
                      width: '100%',
                      height: '50%',
                      background: 'linear-gradient(to bottom, rgba(255,255,255,0.3), transparent)',
                      borderRadius: '10px 10px 0 0'
                    }
                  }}
                />
              </Box>
            </Box>
          );
        })}
      </Box>
      
      {/* 요약 정보 */}
      <Box 
        sx={{ 
          mt: 0.7, 
          pt: 0.7, 
          borderTop: '1px solid rgba(100, 180, 255, 0.2)',
          display: 'flex',
          justifyContent: 'space-between'
        }}
      >
        <Typography sx={{ 
          color: 'rgba(180, 210, 255, 0.9)',
          textShadow: '0 0 5px rgba(100, 180, 255, 0.6)',
          fontSize: '0.65rem'
        }}>
          전체 종 수: {data.length}종
        </Typography>
        <Typography sx={{ 
          color: 'rgba(180, 210, 255, 0.9)',
          textShadow: '0 0 5px rgba(100, 180, 255, 0.6)',
          fontWeight: 'bold',
          fontSize: '0.65rem'
        }}>
          총 인식 수: {total.toLocaleString()}건
        </Typography>
      </Box>
    </Box>
  );
};

export default SpeciesStatsChart; 