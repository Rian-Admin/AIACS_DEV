import React from 'react';
import { Box, Typography } from '@mui/material';

/**
 * 방위별 조류 출현율을 표시하는 레이더 차트 컴포넌트
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
  
  // 육각형 꼭지점 계산 함수
  const getHexagonPoints = (centerX, centerY, radius) => {
    const points = [];
    for (let i = 0; i < 6; i++) {
      const angle = (Math.PI / 3) * i - Math.PI / 2; // 북쪽부터 시작하도록 -90도 회전
      const x = centerX + radius * Math.cos(angle);
      const y = centerY + radius * Math.sin(angle);
      points.push([x, y]);
    }
    return points;
  };
  
  // 육각형 그리드 생성
  const hexGridLevels = [0.33, 0.66, 1.0];
  const hexGrids = hexGridLevels.map(level => {
    const hexPoints = getHexagonPoints(centerX, centerY, radius * level);
    return hexPoints.map(([x, y]) => `${x},${y}`).join(' ');
  });
  
  // 다각형 좌표 생성
  const polygonPoints = directions
    .map(direction => {
      const value = dataMap[direction] || 0;
      return getCoordinates(direction, value);
    })
    .map(([x, y]) => `${x},${y}`)
    .join(' ');
  
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
        방위별 조류 출현율
      </Typography>
      
      <Box sx={{ 
        flex: 1, 
        display: 'flex', 
        justifyContent: 'center', 
        alignItems: 'center',
        position: 'relative',
        mt: -1,
        mb: -1
      }}>
        <Box sx={{ width: `${size}px`, height: `${size}px`, margin: 'auto' }}>
          {/* 레이더 차트 배경 */}
          <svg width="100%" height="100%" viewBox={`0 0 ${size} ${size}`} preserveAspectRatio="xMidYMid meet">
            {/* 배경 그리드 */}
            <circle cx={centerX} cy={centerY} r={radius * 0.33} fill="transparent" stroke="rgba(0, 180, 255, 0.1)" strokeWidth="1" />
            <circle cx={centerX} cy={centerY} r={radius * 0.66} fill="transparent" stroke="rgba(0, 180, 255, 0.1)" strokeWidth="1" />
            <circle cx={centerX} cy={centerY} r={radius} fill="transparent" stroke="rgba(0, 180, 255, 0.2)" strokeWidth="1" />
            
            {/* 방위 라인 */}
            {directions.map(direction => {
              const angle = angles[direction] * (Math.PI / 180);
              const x = centerX + radius * Math.sin(angle);
              const y = centerY - radius * Math.cos(angle);
              
              return (
                <line
                  key={`line-${direction}`}
                  x1={centerX}
                  y1={centerY}
                  x2={x}
                  y2={y}
                  stroke="rgba(0, 180, 255, 0.1)"
                  strokeWidth="1"
                />
              );
            })}
            
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
                  fill="#A9EAFF"
                  fontSize="8"
                  fontWeight="bold"
                  style={{
                    textShadow: '0 0 3px #00B4FF'
                  }}
                >
                  {direction}
                </text>
              );
            })}
            
            {/* 데이터 다각형 */}
            <polygon
              points={polygonPoints}
              fill="rgba(0, 180, 255, 0.15)"
              stroke="#00B4FF"
              strokeWidth="1.5"
              style={{
                filter: 'drop-shadow(0 0 5px rgba(0, 180, 255, 0.7))'
              }}
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
                  r="2.5"
                  fill="#FFA500"
                  style={{
                    filter: 'drop-shadow(0 0 3px rgba(255, 165, 0, 0.8))'
                  }}
                />
              );
            })}
          </svg>
        </Box>
      </Box>
    </Box>
  );
};

export default RadarDirectionChart; 