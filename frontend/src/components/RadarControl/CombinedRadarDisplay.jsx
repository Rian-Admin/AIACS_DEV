import React, { useEffect, useRef, useState } from 'react';
import { Box, Typography, Divider, Paper, IconButton, Tooltip, Slider, FormControlLabel, Switch, TextField, Button } from '@mui/material';
import DeleteIcon from '@mui/icons-material/Delete';
import { max, scaleLinear } from 'd3';

const CombinedRadarDisplay = ({ 
  plotData, 
  trackData, 
  language, 
  translate, 
  onDeleteTrack,
  defaultAlarmDistance = 2000, // 기본 경보 거리를 props로 받음 (기본값 2km)
  saveSettings = true, // 설정 저장 여부 (기본값 true)
  hideAlertSettings = false // 경보 설정 UI 표시 여부
}) => {
  // 마운트 상태 추적을 위한 ref 추가
  const isMountedRef = useRef(true);

  // 컴포넌트 마운트/언마운트 감지
  useEffect(() => {
    isMountedRef.current = true;
    
    return () => {
      isMountedRef.current = false;
    };
  }, []);

  // localStorage에서 저장된 값을 가져오거나 props의 기본값 사용
  const getSavedAlarmDistance = () => {
    if (!saveSettings) return defaultAlarmDistance;
    const saved = localStorage.getItem('radarAlarmDistance');
    return saved ? parseInt(saved, 10) : defaultAlarmDistance;
  };
  
  const getSavedAlarmEnabled = () => {
    if (!saveSettings) return false;
    const saved = localStorage.getItem('radarAlarmEnabled');
    return saved ? saved === 'true' : false;
  };
  
  const [lastUpdate, setLastUpdate] = useState(new Date());
  const [dataChangeCount, setDataChangeCount] = useState(0);
  const [trackHistory, setTrackHistory] = useState({});  // 트랙 위치 이력 저장
  const [clickableAreas, setClickableAreas] = useState([]);
  const [alarmDistance, setAlarmDistance] = useState(getSavedAlarmDistance()); // localStorage 값 사용
  const [isAlarmEnabled, setIsAlarmEnabled] = useState(getSavedAlarmEnabled()); // localStorage 값 사용
  const [isAlarmTriggered, setIsAlarmTriggered] = useState(false); // 경보 발생 상태
  const [customAlarmDistance, setCustomAlarmDistance] = useState(''); // 사용자 입력 임시 저장
  
  const canvasRef = useRef(null);
  const legendCanvasRef = useRef(null);
  const clickableAreasRef = useRef([]);
  const alarmSoundRef = useRef(null);
  
  const prevPlotDataRef = useRef([]); // 이전 플롯 데이터 저장용 ref
  const prevTrackDataRef = useRef([]); // 이전 트랙 데이터 저장용 ref
  
  // 트랙 색상 지정
  const trackColors = [
    '#FF5252', // 빨강
    '#FF9800', // 주황
    '#FFEB3B', // 노랑
    '#4CAF50', // 초록
    '#2196F3', // 파랑
    '#9C27B0', // 보라
    '#E91E63', // 분홍
    '#00BCD4'  // 청록
  ];
  
  // 트랙 색상 결정 함수 - ID를 기준으로 일관된 색상 사용
  const getTrackColor = (trackId) => {
    // trackId를 숫자로 변환 (문자열인 경우 대비)
    const idNum = parseInt(trackId, 10) || trackId;
    // ID에 따라 고정된 색상 인덱스 사용 (ID가 바뀌지 않는 한 항상 같은 색상)
    const colorIndex = Math.abs(idNum) % trackColors.length;
    return trackColors[colorIndex];
  };
  
  // 데이터 변경 감지를 위한 로직
  useEffect(() => {
    // 데이터 변경 확인 함수
    const isDataChanged = (newData, prevData, dataType) => {
      if (!newData || !prevData) return true;
      
      // 길이가 다르면 변경된 것으로 간주
      if (newData.length !== prevData.length) {
        console.log(`${dataType} 데이터 길이 변경: ${prevData.length} → ${newData.length}`);
        return true;
      }
      
      // 실제 데이터 내용 비교
      for (let i = 0; i < newData.length; i++) {
        const newItem = newData[i];
        const oldItem = prevData[i];
        
        // id가 다르면 변경된 것으로 간주
        if (newItem.id !== oldItem.id && newItem.plotId !== oldItem.plotId && newItem.trackId !== oldItem.trackId) {
          console.log(`${dataType} 데이터 ID 변경: ${oldItem.id || oldItem.plotId || oldItem.trackId} → ${newItem.id || newItem.plotId || newItem.trackId}`);
          return true;
        }
        
        // 위치 데이터가 있고 변경되었으면 변경된 것으로 간주
        if (newItem.position && oldItem.position) {
          // 위치 배열이 비어있지 않은 경우만 비교
          if (newItem.position.length > 0 && oldItem.position.length > 0) {
            if (
              newItem.position[0] !== oldItem.position[0] ||
              newItem.position[1] !== oldItem.position[1]
            ) {
              console.log(`${dataType} 데이터 위치 변경: ${oldItem.id || oldItem.plotId || i}`);
              return true;
            }
          }
        }
        
        // 시간 정보가 있고 변경되었으면 변경된 것으로 간주
        if (newItem.time !== oldItem.time && newItem.time && oldItem.time) {
          console.log(`${dataType} 데이터 시간 변경: ${oldItem.time} → ${newItem.time}`);
          return true;
        }
      }
      
      return false;
    };
    
    // 플롯 또는 트랙 데이터 변경 감지
    let plotChanged = isDataChanged(plotData, prevPlotDataRef.current, 'Plot');
    let trackChanged = isDataChanged(trackData, prevTrackDataRef.current, 'Track');
    
    if (plotChanged || trackChanged) {
      console.log(`데이터 변경 감지: 플롯(${plotData?.length || 0}개), 트랙(${trackData?.length || 0}개)`);
      setLastUpdate(new Date());
      setDataChangeCount(prev => prev + 1);
      
      // 현재 데이터를 이전 데이터로 저장 (깊은 복사)
      if (plotData) prevPlotDataRef.current = JSON.parse(JSON.stringify(plotData));
      if (trackData) prevTrackDataRef.current = JSON.parse(JSON.stringify(trackData));
    }
  }, [plotData, trackData]);
  
  // 캔버스에 극좌표계 시각화
  useEffect(() => {
    console.log(`CombinedRadarDisplay - 데이터 업데이트: 플롯(${plotData?.length || 0}개), 트랙(${trackData?.length || 0}개)`);
    
    if (!isMountedRef.current || !canvasRef.current || !legendCanvasRef.current) {
      console.log('캔버스 요소가 없거나 컴포넌트가 언마운트됨');
      return;
    }
    
    try {
      // 캔버스 가져오기
      const canvas = canvasRef.current;
      const ctx = canvas.getContext('2d');
      const legendCanvas = legendCanvasRef.current;
      const legendCtx = legendCanvas.getContext('2d');
      
      // 캔버스 클리어
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      legendCtx.clearRect(0, 0, legendCanvas.width, legendCanvas.height);
      
      // 캔버스 중앙점 및 크기 설정
      const width = canvas.width;
      const height = canvas.height;
      const centerX = width / 2;
      const centerY = height / 2;
      
      console.log(`캔버스 크기: ${width}x${height}, 중앙점: (${centerX}, ${centerY})`);
      
      // 배경색 설정
      ctx.fillStyle = '#0a1929';
      ctx.fillRect(0, 0, width, height);
      
      // 최대 거리 계산 (고정값으로 설정)
      const maxRange = 4000; // 4km로 고정
      console.log(`최대 거리: ${maxRange}m (고정값)`);
      
      // 거리 스케일 설정
      const radiusScale = scaleLinear()
        .domain([0, maxRange])
        .range([0, Math.min(centerX, centerY) - 30]);
      
      // 경보 거리 표시
      if (isAlarmEnabled) {
        ctx.beginPath();
        ctx.arc(centerX, centerY, radiusScale(alarmDistance), 0, 2 * Math.PI);
        ctx.strokeStyle = isAlarmTriggered ? 'rgba(255, 0, 0, 0.7)' : 'rgba(255, 255, 0, 0.7)';
        ctx.lineWidth = 3;
        ctx.stroke();
        
        // 경보 거리 레이블
        ctx.fillStyle = isAlarmTriggered ? 'rgba(255, 0, 0, 1)' : 'rgba(255, 255, 0, 1)';
        ctx.font = 'bold 12px Arial';
        ctx.textAlign = 'center';
        ctx.fillText(
          `경보 거리: ${(alarmDistance / 1000).toFixed(1)}km`,
          centerX,
          centerY - radiusScale(alarmDistance) - 15
        );
      }
      
      // 그리드 및 눈금 그리기
      ctx.strokeStyle = '#1e3a5a';
      ctx.fillStyle = '#90caf9';
      ctx.lineWidth = 1;
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.font = '10px Arial';
      
      // 동심원 그리기
      for (let r = 1000; r <= maxRange; r += 1000) {
        ctx.beginPath();
        ctx.arc(centerX, centerY, radiusScale(r), 0, 2 * Math.PI);
        ctx.stroke();
        
        // 거리 레이블
        ctx.fillText(
          `${(r / 1000).toFixed(0)}km`, 
          centerX, 
          centerY - radiusScale(r) - 10
        );
      }
      
      // 방향 표시 업데이트: 북쪽(0도), 동쪽(90도), 남쪽(180도), 서쪽(270도)
      // 북쪽이 위쪽, 동쪽이 오른쪽이 되도록 각도 표시
      for (let angle = 0; angle < 360; angle += 45) {
        // 컴퓨터 좌표계에서 각도 변환 (북쪽이 -90도)
        const radians = ((90 - angle) % 360) * Math.PI / 180;
        ctx.beginPath();
        ctx.moveTo(centerX, centerY);
        ctx.lineTo(
          centerX + Math.cos(radians) * radiusScale(maxRange),
          centerY + Math.sin(radians) * radiusScale(maxRange)
        );
        ctx.stroke();
        
        // 각도 레이블
        const labelRadius = radiusScale(maxRange) + 15;
        ctx.fillText(
          `${angle}°`,
          centerX + Math.cos(radians) * labelRadius,
          centerY + Math.sin(radians) * labelRadius
        );
      }
      
      // 원점 표시
      ctx.beginPath();
      ctx.arc(centerX, centerY, 3, 0, 2 * Math.PI);
      ctx.fillStyle = 'white';
      ctx.fill();
      
      // 극좌표를 캔버스 좌표로 변환하는 유틸리티 함수
      const polarToCanvas = (range, azimuth) => {
        // 범위 체크
        if (range <= 0 || range > maxRange) {
          return null;
        }
        
        // 거리 계산
        const distance = radiusScale(range);
        
        // 방위각 라디안 변환 및 컴퓨터 좌표계에 맞게 각도 변환
        const angleRad = ((90 - azimuth) % 360) * Math.PI / 180;
        
        // 캔버스 좌표 계산
        const canvasX = centerX + Math.cos(angleRad) * distance;
        const canvasY = centerY + Math.sin(angleRad) * distance;
        
        if (isNaN(canvasX) || isNaN(canvasY)) {
          return null;
        }
        
        return { x: canvasX, y: canvasY };
      };
      
      // 직교 좌표를 극좌표로 변환하는 함수
      const cartesianToPolar = (x, y) => {
        const range = Math.sqrt(x*x + y*y);
        let azimuth = Math.atan2(x, y) * 180 / Math.PI;
        if (azimuth < 0) {
          azimuth += 360; // 음수 각도를 0-360 범위로 변환
        }
        return { range, azimuth };
      };
      
      // 트랙과 플롯의 관계 매핑 함수 개선
      const mapTracksToPlots = (trackData, plotData) => {
        const trackPlotMap = new Map();
        
        if (!trackData || !plotData || !Array.isArray(trackData) || !Array.isArray(plotData)) {
          return trackPlotMap;
        }

        trackData.forEach(track => {
          const trackId = track.trackId || track.id;
          if (!trackId || !track.position || !Array.isArray(track.position) || track.position.length < 2) return;
          
          const trackColor = getTrackColor(trackId);
          const [tx, ty, tz] = track.position;
          
          // 관련 플롯 찾기 - 거리 임계값 증가 (300m -> 800m)
          const relatedPlots = plotData.filter(plot => {
            if (!plot.position || !Array.isArray(plot.position) || plot.position.length < 2) return false;
            
            const [px, py, pz] = plot.position;
            
            // 위치 차이 계산 - 3D 거리 사용
            const dx = px - tx;
            const dy = py - ty;
            const dz = (pz - tz) || 0; // z값이 없으면 0 사용
            const distance = Math.sqrt(dx*dx + dy*dy + dz*dz);
            
            // 더 넓은 거리 범위 허용 (800m까지)
            return distance < 800;
          });
          
          if (relatedPlots.length > 0) {
            trackPlotMap.set(trackId, {
              color: trackColor,
              plots: relatedPlots.map(p => p.plotId || p.id)
            });
          }
        });
        
        return trackPlotMap;
      };
      
      // 트랙과 플롯의 관계 매핑
      const trackPlotMap = mapTracksToPlots(trackData, plotData);
      
      // 트랙 데이터 처리 및 관계 매핑
      const tracksWithColors = [];
      
      // 각 트랙에 색상 할당 및 관련 플롯 매핑
      if (trackData && trackData.length > 0) {
        trackData.forEach((track, index) => {
          const trackId = track.trackId || track.id || index;
          const trackColor = getTrackColor(trackId);
          
          tracksWithColors.push({
            ...track,
            color: trackColor,
            indexInArray: index
          });
        });
      }
      
      // 아이템 그리기 함수
      const drawItem = (item, type, index) => {
        try {
          // 변수를 모두 let으로 선언하여 재할당 가능하게 함
          let x, y, z;
          let range = 0, azimuth = 0;
          let hasValidPosition = false;
          
          // position 배열이 있고 유효한 값이 있는 경우
          if (item.position && Array.isArray(item.position) && item.position.length >= 2) {
            // 배열 구조 분해 할당으로 x, y, z 값 추출
            [x, y, z = 0] = item.position;
            
            // 0, 0, 0 위치인 경우 (무효한 데이터로 간주)
            if (x === 0 && y === 0 && z === 0) {
              console.log(`무효한 위치 데이터 (0,0,0) 감지: ${type} ID=${item.id || item.trackId || index}`);
              // 0,0,0 위치는 무시하고 그리지 않음
              return false;
            }
            
            // 직교 좌표를 극좌표로 변환
            range = Math.sqrt(x * x + y * y);
            azimuth = Math.atan2(x, y) * 180 / Math.PI; // x, y 순서 주의 (동쪽이 x+, 북쪽이 y+)
            if (azimuth < 0) {
              azimuth += 360;
            }
            hasValidPosition = true;
            
            // 위치 정보 로그 (디버깅용)
            if (type === 'Track') {
              console.log(`트랙 위치 처리: ID=${item.id || item.trackId}, 위치=[${x}, ${y}, ${z}], 범위=${range}, 방위각=${azimuth}`);
            }
          }
          // Range, Azimuth 형식의 위치가 있는 경우 (극좌표)
          else if (item.Range !== undefined && item.Azimuth !== undefined) {
            range = parseFloat(item.Range);
            azimuth = parseFloat(item.Azimuth);
            
            // 극좌표를 직교좌표로 변환
            const azimuthRad = azimuth * Math.PI / 180;
            x = range * Math.sin(azimuthRad);
            y = range * Math.cos(azimuthRad);
            z = item.Elevation || 0;
            
            hasValidPosition = true;
          }
          // range, azimuth 형식의 위치가 있는 경우 (극좌표 - 소문자)
          else if (item.range !== undefined && item.azimuth !== undefined) {
            range = parseFloat(item.range);
            azimuth = parseFloat(item.azimuth);
            
            // 극좌표를 직교좌표로 변환
            const azimuthRad = azimuth * Math.PI / 180;
            x = range * Math.sin(azimuthRad);
            y = range * Math.cos(azimuthRad);
            z = item.elevation || 0;
            
            hasValidPosition = true;
          }
          // x, y 직교좌표가 있는 경우
          else if (item.x !== undefined && item.y !== undefined) {
            x = parseFloat(item.x);
            y = parseFloat(item.y);
            z = parseFloat(item.z || 0);
            
            range = Math.sqrt(x * x + y * y);
            azimuth = Math.atan2(x, y) * 180 / Math.PI;
            if (azimuth < 0) {
              azimuth += 360;
            }
            hasValidPosition = true;
          }
          
          // 유효한 위치 데이터가 있을 때만 그리기
          if (!hasValidPosition || range <= 0) {
            if (type === 'Track') {
              console.log(`유효하지 않은 위치 데이터: ${type} ID=${item.id || item.trackId || index}, 범위=${range}`);
            }
            return false; // 그리기 스킵
          }
          
          // 캔버스 좌표 계산
          const canvasCoords = polarToCanvas(range, azimuth);
          if (!canvasCoords) {
            return false;
          }
          
          const { x: canvasX, y: canvasY } = canvasCoords;
          
          // 아이템 ID 가져오기
          const id = item.id || item.trackId || item.plotId || index;
          
          // 색상 결정 (트랙과 연관된 플롯은 같은 색상)
          let color = type === 'Plot' ? '#4fc3f7' : '#ff9800';
          
          // 플롯인 경우 관련 트랙 색상 확인
          if (type === 'Plot') {
            const plotId = item.plotId || item.id || index;
            
            // 모든 트랙-플롯 매핑을 확인하여 현재 플롯이 어떤 트랙에 속하는지 확인
            for (const [trackId, trackInfo] of trackPlotMap.entries()) {
              if (trackInfo.plots.includes(plotId)) {
                color = trackInfo.color;
                break;
              }
            }
          } else if (type === 'Track') {
            // 트랙인 경우 할당된 색상 사용
            const trackWithColor = tracksWithColors.find(t => 
              (t.trackId || t.id) === (item.trackId || item.id)
            );
            if (trackWithColor) {
              color = trackWithColor.color;
            }
          }

          // 아이템 그리기
          const size = type === 'Plot' ? 4 : 6;
          
          ctx.fillStyle = color;
          ctx.beginPath();
          
          if (type === 'Plot') {
            // 플롯은 원으로 표시
            ctx.arc(canvasX, canvasY, size, 0, 2 * Math.PI);
            ctx.fill();
          } else {
            // 트랙도 원으로 표시 (다이아몬드 대신)
            ctx.arc(canvasX, canvasY, size, 0, 2 * Math.PI);
            ctx.fill();
            
            // 트랙인 경우 삭제 버튼 추가
            if (onDeleteTrack) {
              drawTrackDeleteButton(ctx, canvasX, canvasY, item.trackId || item.id || index);
            }
          }
          
          // ID 표시
          if (id !== undefined) {
            ctx.fillStyle = 'white';
            ctx.font = '12px Arial';
            ctx.fillText(id.toString(), canvasX, canvasY - size - 5);
          }
          
          return true;
        } catch (error) {
          console.error('아이템 그리기 오류:', error, item);
          return false;
        }
      };
      
      // 플롯과 트랙 데이터 그리기
      let itemsDrawn = 0;
      let itemsSkipped = 0;
      let objectsInsideAlarmRadius = 0;
      
      // 먼저 플롯 데이터 그리기
      if (plotData && plotData.length > 0) {
        plotData.forEach((item, index) => {
          if (drawItem(item, 'Plot', index)) {
            itemsDrawn++;
            
            // 경보 거리 체크 (플롯)
            if (isAlarmEnabled && item.position && Array.isArray(item.position) && item.position.length >= 2) {
              const [x, y, z = 0] = item.position;
              const range = Math.sqrt(x * x + y * y);
              
              if (range <= alarmDistance) {
                objectsInsideAlarmRadius++;
              }
            }
          } else {
            itemsSkipped++;
          }
        });
      }
      
      // 그 위에 트랙 데이터 그리기
      if (trackData && trackData.length > 0) {
        // 트랙 위치 이력 업데이트
        const newTrackHistory = { ...trackHistory };
        const newClickableAreas = [];
        
        // 각 트랙에 대해 이력 업데이트
        trackData.forEach(item => {
          const id = item.id || item.trackId;
          if (!id) return; // ID가 없으면 처리하지 않음
          
          // 유효한 위치 데이터 확인
          if (!item.position || !Array.isArray(item.position) || item.position.length < 2) return;
          
          const [x, y, z] = item.position;
          
          // 직교 좌표를 극좌표로 변환
          const { range, azimuth } = cartesianToPolar(x, y);
          
          // 극좌표를 캔버스 좌표로 변환
          const canvasCoords = polarToCanvas(range, azimuth);
          if (!canvasCoords) return;
          
          const { x: canvasX, y: canvasY } = canvasCoords;
          
          // 이 트랙의 이력이 없으면 초기화
          if (!newTrackHistory[id]) {
            newTrackHistory[id] = [];
          }
          
          // 새 위치를 이력에 추가 (최대 10개 위치만 저장)
          // 이전과 현재 위치가 동일하면 추가하지 않음 (중복 방지)
          const lastPos = newTrackHistory[id].length > 0 ? 
                         newTrackHistory[id][newTrackHistory[id].length - 1] : null;
          
          if (!lastPos || (lastPos[0] !== canvasX || lastPos[1] !== canvasY)) {
            // unshift 대신 push 사용하여 시간순으로 데이터 추가 (오래된 데이터 -> 최신 데이터)
            newTrackHistory[id].push([canvasX, canvasY, Date.now()]); // 타임스탬프도 함께 저장
            
            // 최대 10개 위치만 유지 (오래된 데이터 제거)
            if (newTrackHistory[id].length > 10) {
              newTrackHistory[id] = newTrackHistory[id].slice(-10);
            }
          }
          
          // 클릭 가능한 영역 추가
          newClickableAreas.push({
            type: 'deleteTrack',
            x: canvasX,
            y: canvasY,
            radius: 10,
            trackId: id
          });
        });
        
        // 트랙 이력 상태 업데이트
        setTrackHistory(newTrackHistory);
        
        // 트랙 경로 그리기 함수 개선
        trackData.forEach(item => {
          const id = item.id || item.trackId;
          if (!id) return;
          
          // 유효한 위치 데이터 확인
          if (!item.position || !Array.isArray(item.position) || item.position.length < 2) return;
          
          const [x, y, z] = item.position;
          
          // 직교 좌표를 극좌표로 변환
          const { range, azimuth } = cartesianToPolar(x, y);
          
          // 극좌표를 캔버스 좌표로 변환
          const { canvasX, canvasY } = polarToCanvas(range, azimuth);
          
          // 트랙 경로 그리기
          drawTrackPath(ctx, id, [canvasX, canvasY], getTrackColor(id));
          
          // 이동 방향 화살표 그리기 (속도 벡터가 있는 경우)
          if (item.velocity && Array.isArray(item.velocity) && item.velocity.length >= 2) {
            const [vx, vy] = item.velocity;
            
            // 속도 스케일 조정 (화살표 길이)
            const arrowLength = 20;
            const speed = Math.sqrt(vx*vx + vy*vy);
            
            if (speed > 0) {
              // 정규화된 방향 벡터에 화살표 길이 적용
              const dirX = (vx / speed) * arrowLength;
              const dirY = (vy / speed) * arrowLength;
              
              // 트랙 위치에서 방향 벡터 방향으로 화살표 그리기
              drawArrow(
                ctx, 
                canvasX, 
                canvasY, 
                canvasX + dirX, 
                canvasY - dirY, // Y축은 반대 방향
                getTrackColor(id)
              );
            }
          }
        });
        
        // 트랙 심볼 그리기
        trackData.forEach((item, index) => {
          if (drawItem(item, 'Track', index)) {
            itemsDrawn++;
            
            // 경보 거리 체크 (트랙)
            if (isAlarmEnabled && item.position && Array.isArray(item.position) && item.position.length >= 2) {
              const [x, y, z = 0] = item.position;
              const range = Math.sqrt(x * x + y * y);
              
              if (range <= alarmDistance) {
                objectsInsideAlarmRadius++;
              }
            }
          } else {
            itemsSkipped++;
          }
        });
        
        // 클릭 가능한 영역 업데이트
        clickableAreasRef.current = newClickableAreas;
      } else {
        // 트랙 데이터가 없으면 이력 초기화
        setTrackHistory({});
      }
      
      // 경보 처리
      if (isAlarmEnabled) {
        if (objectsInsideAlarmRadius > 0 && !isAlarmTriggered) {
          setIsAlarmTriggered(true);
          // 경보음 재생
          if (alarmSoundRef.current) {
            alarmSoundRef.current.play().catch(err => console.error('경보음 재생 실패:', err));
          }
        } else if (objectsInsideAlarmRadius === 0 && isAlarmTriggered) {
          setIsAlarmTriggered(false);
          // 경보음 중지
          if (alarmSoundRef.current) {
            alarmSoundRef.current.pause();
            alarmSoundRef.current.currentTime = 0;
          }
        }
      }
      
      // 데이터가 없는 경우 안내 메시지 표시
      if ((!plotData || plotData.length === 0) && (!trackData || trackData.length === 0)) {
        ctx.fillStyle = 'white';
        ctx.font = '14px Arial';
        ctx.textAlign = 'center';
        ctx.fillText('표시할 레이더 데이터가 없습니다', centerX, centerY - 50);
      }
      
      console.log(`총 ${itemsDrawn}개 아이템 그려짐, ${itemsSkipped}개 건너뜀`);
      
      // 범례 그리기
      legendCtx.fillStyle = '#1e3a5a';
      legendCtx.fillRect(0, 0, legendCanvas.width, legendCanvas.height);
      
      const legendItems = [
        { label: '플롯', color: '#4fc3f7', isPlot: true },
        { label: '트랙', color: '#ff9800', isPlot: false },
        { label: '트랙 경로', color: '#ff9800', isTrace: true },
        { label: '이동 방향', color: '#ff9800', isArrow: true },
        { label: '방위각', color: 'white' }
      ];
      
      legendItems.forEach((item, index) => {
        const x = 20;
        const y = 15 + index * 20;
        
        legendCtx.fillStyle = item.color;
        
        if (item.isPlot) {
          // 플롯 범례 - 원
          legendCtx.beginPath();
          legendCtx.arc(x, y, 5, 0, 2 * Math.PI);
          legendCtx.fill();
        } else if (item.isTrace) {
          // 트랙 경로 범례 - 점선
          legendCtx.strokeStyle = item.color;
          legendCtx.lineWidth = 1.5;
          legendCtx.setLineDash([5, 3]);
          legendCtx.beginPath();
          legendCtx.moveTo(x - 5, y);
          legendCtx.lineTo(x + 5, y);
          legendCtx.stroke();
          legendCtx.setLineDash([]);
          
          // 작은 점 표시
          legendCtx.beginPath();
          legendCtx.arc(x + 5, y, 2, 0, 2 * Math.PI);
          legendCtx.fill();
        } else if (item.isArrow) {
          // 화살표 범례
          legendCtx.strokeStyle = item.color;
          legendCtx.fillStyle = item.color;
          legendCtx.lineWidth = 1.5;
          
          // 화살표 선
          legendCtx.beginPath();
          legendCtx.moveTo(x - 5, y);
          legendCtx.lineTo(x + 5, y);
          legendCtx.stroke();
          
          // 화살표 머리
          legendCtx.beginPath();
          legendCtx.moveTo(x + 5, y);
          legendCtx.lineTo(x + 2, y - 3);
          legendCtx.lineTo(x + 2, y + 3);
          legendCtx.closePath();
          legendCtx.fill();
        } else if (!item.isTrace && !item.isPlot && !item.isArrow) {
          // 트랙 범례 - 다이아몬드
          legendCtx.beginPath();
          legendCtx.moveTo(x, y - 5);
          legendCtx.lineTo(x + 5, y);
          legendCtx.lineTo(x, y + 5);
          legendCtx.lineTo(x - 5, y);
          legendCtx.closePath();
          legendCtx.fill();
        }
        
        legendCtx.fillStyle = 'white';
        legendCtx.font = '12px Arial';
        legendCtx.textAlign = 'left';
        legendCtx.textBaseline = 'middle';
        legendCtx.fillText(item.label, x + 15, y);
      });
      
      // 마지막 업데이트 시간 표시
      ctx.fillStyle = 'white';
      ctx.font = '12px Arial';
      ctx.textAlign = 'right';
      ctx.fillText(lastUpdate.toLocaleTimeString(), width - 10, height - 10);
      
      // 데이터 업데이트 횟수 표시
      ctx.textAlign = 'left';
      ctx.fillText(`업데이트: ${dataChangeCount}회`, 10, height - 10);
      
    } catch (error) {
      console.error('레이더 시각화 오류:', error);
    }
  }, [plotData, trackData, lastUpdate, dataChangeCount, onDeleteTrack, alarmDistance, isAlarmEnabled, isAlarmTriggered]);
  
  // 경보음 초기화
  useEffect(() => {
    // 경보음 생성
    const alarmSound = new Audio();
    alarmSound.src = "data:audio/wav;base64,//uQRAAAAWMSLwUIYAAsYkXgoQwAEaYLWfkWgAI0wWs/ItAAAGDgYtAgAyN+QWaAAihwMWm4G8QQRDiMcCBcH3Cc+CDv/7xA4Tvh9Rz/y8QADBwMWgQAZG/ILNAARQ4GLTcDeIIIhxGOBAuD7hOfBB3/94gcJ3w+o5/5eIAIAAAVwWgQAVQ2ORaIQwEMAJiDg95G4nQL7mQVWI6GwRcfsZAcsKkJvxgxEjzFUgfHoSQ9Qq7KNwqHwuB13MA4a1q/DmBrHgPcmjiGoh//EwC5nGPEmS4RcfkVKOhJf+WOgoxJclFz3kgn//dBA+ya1GhurNn8zb//9NNutNuhz31f////9vt///z+IdAEAAAK4LQIAKobHItEIYCGAExBwe8jcToF9zIKrEdDYIuP2MgOWFSE34wYiR5iqQPj0JIeoVdlG4VD4XA67mAcNa1fhzA1jwHuTRxDUQ//iYBczjHiTJcIuPyKlHQkv/LHQUYkuSi57yQT//uggfZNajQ3Vmz+Zt//+mm3Wm3Q576v////+32///5/EOgAAADVghQAAAAA//uQZAUAB1WI0PZugAAAAAoQwAAAEk3nRd2qAAAAACiDgAAAAAAABCqEEQRLCgwpBGMlJkIz8jKhGvj4k6jzRnqasNKIeoh5gI7BJaC1A1AoNBjJgbyApVS4IDlZgDU5WUAxEKDNmmALHzZp0Fkz1FMTmGFl1FMEyodIavcCAUHDWrKAIA4aa2ooFollowEAAAAAAADpm1QAEByZDgABAAbQZvANoACJAA+gNwBbQDKQFmA88BMAJkAPwB2wGLAEeAPcAb0B7QGDgBDgO+AUEA9YBoQFPAK2AFUAH4ArgB5gFpgPTA9YB4YEDAVuA94D8wHxAPKAgMB8YAEwEpgP4gf8A+sCAoH4QQhBCYEJQP+ghGCEYIRghGCEgH/AQlBCUD9YITAf8BCcEHAQfBB4D/gIOg/7B+kD6YH5wPkgfKA+0B/wEBgP8gfgA/ECAoH+wP3g/gCB4IJgf+BCcEJgP8AAAAAAAAAA"; // 간단한 "삐" 소리
    alarmSound.loop = true;
    alarmSoundRef.current = alarmSound;
    
    return () => {
      if (alarmSoundRef.current) {
        alarmSoundRef.current.pause();
        alarmSoundRef.current = null;
      }
    };
  }, []);
  
  // 캔버스 설정
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    
    // 캔버스 크기 설정 - 더 큰 크기로 변경 (500x500 -> 800x800)
    canvas.width = 1000;
    canvas.height = 1000;
    
    // 캔버스 스타일 설정
    canvas.style.width = '1000px';
    canvas.style.height = '1000px';
    
    // 디스플레이 크기 로깅
    console.log(`캔버스 크기: ${canvas.width}x${canvas.height}, 중앙점: (${canvas.width/2}, ${canvas.height/2})`);
    
    // 레전드 캔버스 설정 (옵션)
    const legendCanvas = legendCanvasRef.current;
    if (legendCanvas) {
      legendCanvas.width = 200;
      legendCanvas.height = 120;
    }
  }, []);
  
  // 데이터 테이블에 표시할 유효한 데이터 준비
  const getValidItems = (dataArray, type) => {
    if (!dataArray || dataArray.length === 0) return [];
    
    return dataArray
      .filter(item => {
        // 유효한 위치 데이터 확인
        if (item.position && Array.isArray(item.position) && item.position.length >= 2) {
          return true;
        }
        if (item.Range !== undefined && item.Azimuth !== undefined) {
          return true;
        }
        if (item.range !== undefined && item.azimuth !== undefined) {
          return true;
        }
        if (item.x !== undefined && item.y !== undefined) {
          return true;
        }
        return false;
      })
      .map(item => ({
        ...item,
        _type: type // 내부적으로 타입 표시를 위한 프로퍼티 추가
      }));
  };
  
  // 통합 데이터 테이블 렌더링
  const renderDataTable = () => {
    // 유효한 플롯 및 트랙 데이터 준비
    const validPlotData = getValidItems(plotData, 'Plot');
    const validTrackData = getValidItems(trackData, 'Track');
    
    // 모든 데이터 합치기
    const allValidData = [...validPlotData, ...validTrackData];
    
    // 상위 10개만 선택 (필요시 조정)
    const displayData = allValidData.slice(0, 10);
    
    return (
      <Box sx={{ mt: 2 }}>
        <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 1 }}>
          최근 레이더 데이터 (플롯: {validPlotData.length}/{plotData?.length || 0}개, 트랙: {validTrackData.length}/{trackData?.length || 0}개)
        </Typography>
        <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', overflowX: 'auto' }}>
          <Box sx={{ minWidth: 500 }}>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr style={{ borderBottom: '2px solid #90caf9', color: 'white' }}>
                  <th style={{ padding: '8px', textAlign: 'left' }}>유형</th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>ID</th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>방위각</th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>거리</th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>고도</th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>
                    RCS/속도
                  </th>
                  <th style={{ padding: '8px', textAlign: 'left' }}>
                    시간
                  </th>
                  <th style={{ padding: '8px', textAlign: 'center' }}>
                    작업
                  </th>
                </tr>
              </thead>
              <tbody>
                {displayData.length > 0 ? (
                  displayData.map((item, index) => {
                    // 데이터 항목에서 필요한 값 추출
                    let azimuth = 0, range = 0, altitude = 0, velocity = 0, rcs = 0, time = '';
                    const type = item._type; // 내부 타입 사용
                    
                    try {
                      // position 배열이 있고 비어있지 않은 경우
                      if (item.position && Array.isArray(item.position) && item.position.length >= 2) {
                        let [x, y, z] = item.position;
                        range = Math.sqrt(x * x + y * y);
                        
                        // API 응답과 일치하도록 각도 계산
                        if (item.angle !== undefined) {
                          azimuth = item.angle;
                        } else {
                          // 각도 계산: x가 동쪽(+), y가 북쪽(+) 기준으로 통일
                          azimuth = Math.atan2(x, y) * 180 / Math.PI;
                          if (azimuth < 0) {
                            azimuth += 360; // 음수 각도를 0-360 범위로 변환
                          }
                        }
                        
                        // API 응답에 range 값이 있으면 그 값을 사용
                        if (item.range !== undefined) {
                          range = item.range;
                        }
                        
                        altitude = z || 0;
                      } 
                      // 직접적인 값이 있는 경우
                      else {
                        azimuth = item.azimuth || item.Azimuth || item.angle || 0;
                        // 방위각 표시 형식 통일 (0-360도)
                        if (azimuth < 0) {
                          azimuth += 360;
                        }
                        range = item.range || item.Range || 0;
                        altitude = item.altitude || item.elevation || item.Elevation || 0;
                      }
                      
                      // 시간 정보 처리
                      if (item.tv_sec) {
                        time = new Date(item.tv_sec * 1000).toLocaleTimeString();
                      } else if (item.time) {
                        if (item.time > 1000000000) { // 유닉스 타임스탬프로 가정
                          time = new Date(item.time * 1000).toLocaleTimeString();
                        } else {
                          time = item.time.toString();
                        }
                      }
                      
                      // 속도 또는 RCS
                      if (type === 'Plot') {
                        rcs = item.rcs || item.RCS || 0;
                      } else {
                        if (item.velocity && Array.isArray(item.velocity) && item.velocity.length >= 2) {
                          let [vx, vy, vz] = item.velocity;
                          velocity = Math.sqrt(vx*vx + vy*vy + (vz || 0)*(vz || 0));
                        } else {
                          velocity = item.speed || 0;
                        }
                      }
                    } catch (error) {
                      console.error('데이터 테이블 처리 오류:', error, item);
                    }
                    
                    // 트랙 ID 가져오기
                    const id = item.id || item.trackId || item.plotId || index;
                    
                    return (
                      <tr key={index} style={{ color: 'white' }}>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {type === 'Plot' ? 
                            <span style={{ color: '#4fc3f7' }}>플롯</span> : 
                            <span style={{ color: '#ff9800' }}>트랙</span>}
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {id}
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {azimuth.toFixed(1)}°
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {range.toFixed(0)} m
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {altitude.toFixed(0)} m
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {type === 'Plot' 
                            ? `${rcs.toFixed(1)} dBsm` 
                            : `${velocity ? velocity.toFixed(1) : 'N/A'} m/s`}
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a' }}>
                          {time || 'N/A'}
                        </td>
                        <td style={{ padding: '8px', borderBottom: '1px solid #1e3a5a', textAlign: 'center' }}>
                          {type === 'Track' && onDeleteTrack && (
                            <IconButton 
                              size="small" 
                              onClick={() => onDeleteTrack(id)}
                              sx={{ 
                                color: '#ff5252',
                                '&:hover': { backgroundColor: 'rgba(255, 82, 82, 0.1)' }
                              }}
                              title="트랙 종료"
                            >
                              <DeleteIcon fontSize="small" />
                            </IconButton>
                          )}
                        </td>
                      </tr>
                    );
                  })
                ) : (
                  <tr>
                    <td colSpan={8} style={{ textAlign: 'center', padding: '16px', color: 'white' }}>
                      유효한 위치 데이터가 없습니다
                    </td>
                  </tr>
                )}
              </tbody>
            </table>
          </Box>
        </Paper>
      </Box>
    );
  };
  
  // 캔버스에 트랙 종료 버튼 추가하는 함수
  const drawTrackDeleteButton = (ctx, x, y, trackId) => {
    if (!onDeleteTrack) return; // 콜백이 없으면 그리지 않음
    
    const buttonSize = 12;
    const buttonX = x + 15;
    const buttonY = y - 15;
    
    // 삭제 버튼 배경 (빨간색 원)
    ctx.fillStyle = '#ff5252';
    ctx.beginPath();
    ctx.arc(buttonX, buttonY, buttonSize / 2, 0, 2 * Math.PI);
    ctx.fill();
    
    // X 표시 (흰색)
    ctx.strokeStyle = 'white';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(buttonX - buttonSize / 4, buttonY - buttonSize / 4);
    ctx.lineTo(buttonX + buttonSize / 4, buttonY + buttonSize / 4);
    ctx.moveTo(buttonX + buttonSize / 4, buttonY - buttonSize / 4);
    ctx.lineTo(buttonX - buttonSize / 4, buttonY + buttonSize / 4);
    ctx.stroke();
    
    // 클릭 영역 저장 (캔버스 클릭 이벤트에서 사용)
    if (!canvasRef.current.clickableAreas) {
      canvasRef.current.clickableAreas = [];
    }
    
    canvasRef.current.clickableAreas.push({
      type: 'deleteTrack',
      x: buttonX - buttonSize / 2,
      y: buttonY - buttonSize / 2,
      width: buttonSize,
      height: buttonSize,
      trackId: trackId
    });
  };
  
  // 캔버스 클릭 이벤트 핸들러
  const handleCanvasClick = (event) => {
    if (!canvasRef.current || !clickableAreasRef.current || !onDeleteTrack) return;
    
    const canvas = canvasRef.current;
    const rect = canvas.getBoundingClientRect();
    
    // 클릭 위치 계산
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;
    
    // 클릭 가능한 영역 확인
    const clickedArea = clickableAreasRef.current.find(area => {
      return (
        x >= area.x - area.radius &&
        x <= area.x + area.radius &&
        y >= area.y - area.radius && 
        y <= area.y + area.radius &&
        area.type === 'deleteTrack'
      );
    });
    
    if (clickedArea) {
      onDeleteTrack(clickedArea.trackId);
    }
  };
  
  // 캔버스 이벤트 리스너 설정
  useEffect(() => {
    const canvas = canvasRef.current;
    if (canvas && onDeleteTrack) {
      canvas.addEventListener('click', handleCanvasClick);
      
      return () => {
        canvas.removeEventListener('click', handleCanvasClick);
      };
    }
  }, [onDeleteTrack]);
  
  // 트랙 경로 그리기 함수 개선
  const drawTrackPath = (ctx, trackId, currentPos, color) => {
    if (!trackHistory[trackId] || trackHistory[trackId].length < 2) return;
    
    const positions = trackHistory[trackId];
    
    // 경로 선 스타일 설정 - 더 잘 보이게 설정
    ctx.strokeStyle = color || getTrackColor(trackId);
    ctx.lineWidth = 2.0; // 선 두께 증가
    ctx.setLineDash([6, 4]); // 선명한 점선 패턴
    
    // 경로 그리기
    ctx.beginPath();
    
    // 시간순으로 정렬된 데이터의 첫 위치(가장 오래된 데이터)에서 시작
    const [startX, startY] = positions[0];
    ctx.moveTo(startX, startY);
    
    // 순차적으로 경로 포인트로 선 그리기 (오래된 데이터 -> 최신 데이터)
    for (let i = 1; i < positions.length; i++) {
      const [x, y] = positions[i];
      // 좌표 유효성 검사
      if (x !== undefined && y !== undefined && !isNaN(x) && !isNaN(y)) {
        ctx.lineTo(x, y);
      }
    }
    
    ctx.stroke();
    ctx.setLineDash([]); // 점선 스타일 초기화
    
    // 경로 포인트에 작은 점 표시 (시간에 따른 위치 표시)
    for (let i = 0; i < positions.length; i++) {
      const [x, y] = positions[i];
      // 좌표 유효성 검사
      if (x !== undefined && y !== undefined && !isNaN(x) && !isNaN(y)) {
        ctx.fillStyle = color || getTrackColor(trackId);
        ctx.beginPath();
        ctx.arc(x, y, 2, 0, Math.PI * 2);
        ctx.fill();
      }
    }
  };
  
  // 이동 방향 화살표 그리기 함수
  const drawArrow = (ctx, fromX, fromY, toX, toY, color) => {
    // 유효하지 않은 좌표 검사
    if (fromX === undefined || fromY === undefined || isNaN(fromX) || isNaN(fromY) ||
        toX === undefined || toY === undefined || isNaN(toX) || isNaN(toY)) {
      console.log(`유효하지 않은 화살표 좌표: (${fromX}, ${fromY}) -> (${toX}, ${toY})`);
      return;
    }
    
    const headLength = 12; // 화살표 머리 길이
    const angle = Math.atan2(toY - fromY, toX - fromX);
    
    // 화살표 선 스타일 설정
    ctx.strokeStyle = color;
    ctx.fillStyle = color;
    ctx.lineWidth = 2.5; // 선 두께 증가
    
    // 화살표 선 그리기
    ctx.beginPath();
    ctx.moveTo(fromX, fromY);
    ctx.lineTo(toX, toY);
    ctx.stroke();
    
    // 화살표 머리 그리기 - 삼각형 형태
    ctx.beginPath();
    ctx.moveTo(toX, toY);
    ctx.lineTo(
      toX - headLength * Math.cos(angle - Math.PI / 6),
      toY - headLength * Math.sin(angle - Math.PI / 6)
    );
    ctx.lineTo(
      toX - headLength * Math.cos(angle + Math.PI / 6),
      toY - headLength * Math.sin(angle + Math.PI / 6)
    );
    ctx.closePath();
    ctx.fill(); // 채워진 삼각형으로 표시
    
    // 콘솔 로그로 디버깅
    console.log(`화살표 그리기: (${fromX.toFixed(1)}, ${fromY.toFixed(1)}) -> (${toX.toFixed(1)}, ${toY.toFixed(1)}), 색상: ${color}`);
  };
  
  // 알람 거리 변경 시 localStorage에 저장
  useEffect(() => {
    if (saveSettings) {
      localStorage.setItem('radarAlarmDistance', alarmDistance.toString());
    }
  }, [alarmDistance, saveSettings]);
  
  // 알람 활성화 상태 변경 시 localStorage에 저장
  useEffect(() => {
    if (saveSettings) {
      localStorage.setItem('radarAlarmEnabled', isAlarmEnabled.toString());
    }
  }, [isAlarmEnabled, saveSettings]);
  
  // 사용자 정의 거리 입력 핸들러
  const handleCustomDistanceChange = (e) => {
    setCustomAlarmDistance(e.target.value);
  };
  
  // 사용자 정의 거리 적용
  const applyCustomDistance = () => {
    const value = parseInt(customAlarmDistance, 10);
    if (!isNaN(value) && value >= 100 && value <= 5000) {
      setAlarmDistance(value);
      setCustomAlarmDistance('');
    }
  };
  
  return (
    <Box sx={{ position: 'relative', width: '100%', height: '100%' }}>
      {/* 레이더 디스플레이 */}
      <Box sx={{ width: '100%', display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
        {/* 극좌표계 시각화 */}
        <Box sx={{ position: 'relative', width: '100%', aspectRatio: '1', maxWidth: '1000px' }}>
          <canvas 
            ref={canvasRef} 
            width={1000} 
            height={1000}
            onClick={handleCanvasClick}
            style={{ width: '100%', height: '100%', display: 'block' }}
          />
        </Box>
        
        {/* 범례 캔버스 */}
        <Box sx={{ mt: 2, width: '100%', maxWidth: '200px', height: '120px' }}>
          <canvas ref={legendCanvasRef} width={200} height={120} />
        </Box>
        
        {/* 데이터 테이블 */}
        {renderDataTable()}
      </Box>

      {/* 경보설정 UI - hideAlertSettings가 false일 때만 표시 */}
      {!hideAlertSettings && (
        <Box sx={{ width: '90%', mb: 2, p: 2, bgcolor: '#1e3a5a', borderRadius: 1 }}>
          <Typography variant="subtitle1" sx={{ color: 'white', mb: 1 }}>
            경보 설정
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'center', mb: 1 }}>
            <FormControlLabel
              control={
                <Switch
                  checked={isAlarmEnabled}
                  onChange={(e) => setIsAlarmEnabled(e.target.checked)}
                  color="warning"
                />
              }
              label={<Typography sx={{ color: 'white' }}>경보 활성화</Typography>}
            />
            {isAlarmTriggered && (
              <Typography sx={{ ml: 2, color: 'red', fontWeight: 'bold' }}>
                ⚠️ 경보 발생!
              </Typography>
            )}
          </Box>
          <Box sx={{ display: 'flex', alignItems: 'center', mb: 2 }}>
            <Typography sx={{ width: '80px', color: 'white' }}>거리: </Typography>
            <Slider
              value={alarmDistance}
              onChange={(e, newValue) => setAlarmDistance(newValue)}
              min={500}
              max={4000}
              step={100}
              disabled={!isAlarmEnabled}
              valueLabelDisplay="auto"
              valueLabelFormat={(value) => `${(value/1000).toFixed(1)}km`}
              sx={{ 
                color: isAlarmTriggered ? 'red' : 'yellow',
                '& .MuiSlider-thumb': {
                  backgroundColor: isAlarmTriggered ? 'red' : 'yellow',
                }
              }}
            />
            <Typography sx={{ width: '70px', color: 'white', ml: 2 }}>
              {(alarmDistance/1000).toFixed(1)}km
            </Typography>
          </Box>
          
          {/* 사용자 정의 거리 입력 */}
          <Box sx={{ display: 'flex', alignItems: 'center' }}>
            <Typography sx={{ width: '80px', color: 'white' }}>사용자 설정: </Typography>
            <TextField
              size="small"
              value={customAlarmDistance}
              onChange={handleCustomDistanceChange}
              disabled={!isAlarmEnabled}
              placeholder="100-5000m"
              inputProps={{ inputMode: 'numeric', pattern: '[0-9]*' }}
              sx={{ 
                width: '120px',
                '& .MuiOutlinedInput-root': {
                  color: 'white',
                  '& fieldset': {
                    borderColor: 'rgba(255, 255, 255, 0.3)',
                  },
                  '&:hover fieldset': {
                    borderColor: 'rgba(255, 255, 255, 0.5)',
                  },
                  '&.Mui-focused fieldset': {
                    borderColor: isAlarmTriggered ? 'red' : 'yellow',
                  },
                },
                '& .MuiInputBase-input': {
                  color: 'white',
                },
              }}
            />
            <Button
              variant="outlined"
              size="small"
              onClick={applyCustomDistance}
              disabled={!isAlarmEnabled || !customAlarmDistance}
              sx={{ 
                ml: 1,
                color: 'white',
                borderColor: 'rgba(255, 255, 255, 0.3)',
                '&:hover': {
                  borderColor: 'rgba(255, 255, 255, 0.8)',
                  backgroundColor: 'rgba(255, 255, 255, 0.1)',
                },
                '&.Mui-disabled': {
                  color: 'rgba(255, 255, 255, 0.3)',
                }
              }}
            >
              적용
            </Button>
            <Button
              variant="outlined"
              size="small"
              onClick={() => setAlarmDistance(defaultAlarmDistance)}
              disabled={!isAlarmEnabled}
              sx={{ 
                ml: 1,
                color: 'white',
                borderColor: 'rgba(255, 255, 255, 0.3)',
                '&:hover': {
                  borderColor: 'rgba(255, 255, 255, 0.8)',
                  backgroundColor: 'rgba(255, 255, 255, 0.1)',
                },
                '&.Mui-disabled': {
                  color: 'rgba(255, 255, 255, 0.3)',
                }
              }}
            >
              기본값
            </Button>
          </Box>
        </Box>
      )}
    </Box>
  );
};

export default CombinedRadarDisplay; 