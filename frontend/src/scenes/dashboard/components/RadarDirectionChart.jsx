import React from 'react';
import { Box, Typography } from '@mui/material';

/**
 * 방위별 출현율을 표시하는 레이더 차트 컴포넌트
 * @param {object} props
 * @param {Array} props.data - 데이터 배열 [{direction, value}]
 */
const RadarDirectionChart = ({ data }) => {
  // 차트 크기
  const size = 200;
  const centerX = size / 2;
  const centerY = size / 2;
  const radius = size * 0.4;
  
  // 8개 방위 (N, NE, E, SE, S, SW, W, NW)
  const directions = ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'];
  
  // 각 방위별 각도 (북쪽이 0도, 시계방향)
  const angles = {
    'N': 0,
    'NE': 45,
    'E': 90,
    'SE': 135,
    'S': 180,
    'SW': 225,
    'W': 270,
    'NW': 315
  };
  
  // 방위별 데이터 맵 생성
  const dataMap = data.reduce((acc, item) => {
    acc[item.direction] = item.value;
    return acc;
  }, {});
  
  // SVG 좌표 계산 함수 (각도와 값에 따라)
  const getCoordinates = (direction, value) => {
    const angle = angles[direction] * (Math.PI / 180);
    const distance = radius * value;
    const x = centerX + distance * Math.sin(angle);
    const y = centerY - distance * Math.cos(angle);
    return [x, y];
  };
  
  // 다각형 좌표 생성
  const polygonPoints = directions
    .map(direction => {
      const value = dataMap[direction] || 0;
      return getCoordinates(direction, value);
    })
    .map(([x, y]) => `${x},${y}`)
    .join(' ');
  
  return (
    <Box sx={{ width: '100%', display: 'flex', justifyContent: 'center', my: 1 }}>
      <Box sx={{ position: 'relative', width: size, height: size }}>
        {/* 레이더 차트 배경 */}
        <svg width={size} height={size} viewBox={`0 0 ${size} ${size}`}>
          {/* 방위 텍스트 */}
          {directions.map(direction => {
            const angle = angles[direction] * (Math.PI / 180);
            const distance = radius * 1.15;
            const x = centerX + distance * Math.sin(angle);
            const y = centerY - distance * Math.cos(angle);
            
            return (
              <text
                key={`text-${direction}`}
                x={x}
                y={y}
                textAnchor="middle"
                dominantBaseline="middle"
                fill="rgba(255, 255, 255, 0.7)"
                fontSize="10"
              >
                {direction}
              </text>
            );
          })}
          
          {/* 데이터 다각형 */}
          <polygon
            points={polygonPoints}
            fill="rgba(33, 150, 243, 0.5)"
            stroke="#2196f3"
            strokeWidth="2"
          />
          
          {/* 데이터 포인트 */}
          {directions.map(direction => {
            const value = dataMap[direction] || 0;
            const [x, y] = getCoordinates(direction, value);
            
            return (
              <circle
                key={`point-${direction}`}
                cx={x}
                cy={y}
                r="4"
                fill="#ff9800"
                stroke="#ffffff"
                strokeWidth="1"
              />
            );
          })}
        </svg>
        
        {/* 중앙 레이블 */}
        <Box
          sx={{
            position: 'absolute',
            top: '50%',
            left: '50%',
            transform: 'translate(-50%, -50%)',
            display: 'flex',
            flexDirection: 'column',
            alignItems: 'center'
          }}
        >
          <Box
            sx={{
              width: 12,
              height: 12,
              backgroundColor: 'rgba(33, 150, 243, 0.7)',
              borderRadius: '50%',
              mb: 0.5
            }}
          />
          <Typography
            variant="caption"
            sx={{
              color: 'white',
              fontSize: '0.7rem',
              whiteSpace: 'nowrap'
            }}
          >
            출현율
          </Typography>
        </Box>
      </Box>
    </Box>
  );
};

export default RadarDirectionChart; 