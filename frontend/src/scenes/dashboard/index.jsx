import { useState, useEffect, useRef, useCallback } from 'react';
import { Box, Grid, useTheme, IconButton, Paper, Typography, Divider } from '@mui/material';
import { ChevronLeft, ChevronRight } from '@mui/icons-material';
import useAppStore from '../../store/useAppStore';
import axios from 'axios';

// 분리된 컴포넌트 가져오기
import WeatherWidget from './components/WeatherWidget';
import CollisionAlert from './components/CollisionAlert';
import BirdProximityStatus from './components/BirdProximityStatus';
import MapView from './components/MapView';
import SystemStatus from './components/SystemStatus';
import BirdStatistics from './components/BirdStatistics';
import RecentEvents from './components/RecentEvents';
import CameraModal from './components/CameraModal';
import DailyStatsChart from './components/DailyStatsChart';
import SpeciesStatsChart from './components/SpeciesStatsChart';
import RadarDirectionChart from './components/RadarDirectionChart';

/**
 * 대시보드 메인 컴포넌트
 * 시스템의 주요 상태와 정보를 한눈에 볼 수 있는 화면
 */
const Dashboard = ({ language }) => {
  const theme = useTheme();
  const mapRef = useRef(null);
  
  // 상태 관리
  const [currentTime, setCurrentTime] = useState(new Date());
  const [weatherData, setWeatherData] = useState(null);
  const [weatherLoading, setWeatherLoading] = useState(true);
  const [collisionRisks, setCollisionRisks] = useState([]);
  const [birdActivityData, setBirdActivityData] = useState([]);
  const [mapCenter, setMapCenter] = useState([35.193097, 126.221395]); // 소각시도 좌표
  const [showCameraModal, setShowCameraModal] = useState(false);
  const [showLeftPanel, setShowLeftPanel] = useState(true); // 왼쪽 패널 표시 상태
  const [showRightPanel, setShowRightPanel] = useState(true); // 오른쪽 패널 표시 상태
  const [cameraStatus, setCameraStatus] = useState({
    1: { online: true, error: false },
    2: { online: true, error: false },
    3: { online: true, error: false }
  });
  
  // 일일 카메라별 누적 현황 데이터
  const [dailyCameraStats, setDailyCameraStats] = useState({
    total: 0,
    camera1: 0,
    camera2: 0,
    camera3: 0
  });
  
  // 전역 상태
  const {
    highestRiskLevel,
    setHighestRiskLevel,
    cameras,
    radars
  } = useAppStore();

  // 시간 업데이트 효과
  useEffect(() => {
    const timer = setInterval(() => {
      setCurrentTime(new Date());
    }, 1000);

    return () => clearInterval(timer);
  }, []);

  // 날씨 데이터 불러오기
  useEffect(() => {
    const fetchWeatherData = async () => {
      setWeatherLoading(true);
      try {
        // 백엔드 API 호출
        const API_BASE_URL = 'http://localhost:8000';
        const response = await axios.get(`${API_BASE_URL}/api/weather/`);
        
        if (response.data) {
          setWeatherData({
            location: '전라남도 영광군 소각시도',
            timestamp: response.data.timestamp,
            current: {
              temperature: response.data.current.temperature,
              feels_like: response.data.current.temperature - 3, // 체감온도 계산 (임의)
              humidity: response.data.current.humidity,
              wind_speed: response.data.current.wind_speed,
              wind_direction: response.data.current.wind_direction,
              precipitation: response.data.current.precipitation,
              weather_condition: response.data.current.precipitation_type || 'clear',
              pressure: 1012, // 기본값
              visibility: 10, // 기본값
              uv_index: 3 // 기본값
            },
            forecast: response.data.forecast || []
          });
        }
        setWeatherLoading(false);
      } catch (error) {
        console.error('날씨 데이터를 불러오는 중 오류 발생:', error);
        setWeatherLoading(false);
      }
    };

    fetchWeatherData();
    
    // 5분마다 날씨 데이터 갱신
    const intervalId = setInterval(fetchWeatherData, 5 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 조류 활동 데이터 가져오기
  useEffect(() => {
    const fetchBirdActivity = async () => {
      try {
        // 백엔드 API 경로 (레이더 트랙 데이터 활용)
        const API_BASE_URL = 'http://localhost:8000';
        const response = await axios.get(`${API_BASE_URL}/api/radar/track/`);
        
        if (response.data && response.data.tracks) {
          // 레이더 트랙 데이터를 활용하여 풍력 발전기별 조류 근접 데이터 생성
          const turbines = ['YW-01', 'YW-02', 'YW-03', 'YW-04', 'YW-05', 'YW-06'];
          const turbinePositions = {
            'YW-01': [35.193097, 126.221395],
            'YW-02': [35.194097, 126.222395],
            'YW-03': [35.195097, 126.223395],
            'YW-04': [35.196097, 126.224395],
            'YW-05': [35.197097, 126.225395],
            'YW-06': [35.198097, 126.226395]
          };
          
          // 각 풍력 발전기별 근접 조류 수와 위험도 계산
          const turbineData = turbines.map(id => {
            // 현재 트랙 중 해당 풍력 발전기에 가까운 트랙 수 계산
            // 간략화된 버전으로 구현 (실제로는 좌표 거리 계산 필요)
            const nearbyTracks = response.data.tracks.filter(track => {
              // 여기서는 랜덤하게 할당 (실제로는 좌표 기반 거리 계산 필요)
              return Math.random() < 0.3;
            });
            
            // 위험도는 0~1 사이의 값으로 표현
            const risk = Math.min(nearbyTracks.length * 0.1, 1);
            
            return {
              id,
              count: nearbyTracks.length, 
              risk
            };
          });
          
          setBirdActivityData(turbineData);
        }
      } catch (error) {
        console.error('조류 활동 데이터 가져오기 오류:', error);
        // 오류 시에도 UI를 유지하기 위해 기본 데이터 설정
        setBirdActivityData([
          { id: 'YW-01', count: 2, risk: 0.2 },
          { id: 'YW-02', count: 3, risk: 0.3 },
          { id: 'YW-03', count: 1, risk: 0.1 },
          { id: 'YW-04', count: 0, risk: 0.0 },
          { id: 'YW-05', count: 2, risk: 0.2 },
          { id: 'YW-06', count: 1, risk: 0.1 }
        ]);
      }
    };
    
    fetchBirdActivity();
    
    // 30초마다 데이터 갱신
    const intervalId = setInterval(fetchBirdActivity, 30 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 충돌 위험 데이터 업데이트
  useEffect(() => {
    const updateCollisionRisks = async () => {
      try {
        // 백엔드 API 호출 - 레이더 트랙 데이터 활용
        const API_BASE_URL = 'http://localhost:8000';
        const response = await axios.get(`${API_BASE_URL}/api/radar/track/`);
        
        if (response.data && response.data.tracks) {
          // 충돌 위험이 있는 트랙 필터링 (속도가 빠르고 고도가 낮은 트랙)
          // 실제로는 위험도 평가 알고리즘이 필요함
          const risks = response.data.tracks
            .filter(track => {
              // 속도 계산 (벡터 크기)
              const speed = Math.sqrt(
                Math.pow(track.velocity[0], 2) + 
                Math.pow(track.velocity[1], 2) + 
                Math.pow(track.velocity[2], 2)
              );
              
              // 위험한 트랙 필터링 (속도가 10 이상이고 고도가 낮은 트랙)
              return speed > 10 && track.position[2] < 200;
            })
            .slice(0, 3) // 최대 3개만 표시
            .map((track, index) => {
              // 속도 계산
              const speed = Math.sqrt(
                Math.pow(track.velocity[0], 2) + 
                Math.pow(track.velocity[1], 2) + 
                Math.pow(track.velocity[2], 2)
              );
              
              // 거리 계산 (원점으로부터)
              const distance = Math.sqrt(
                Math.pow(track.position[0], 2) + 
                Math.pow(track.position[1], 2)
              );
              
              // 위험 레벨 결정
              let level = 'low';
              if (speed > 20 && distance < 500) level = 'critical';
              else if (speed > 15 && distance < 1000) level = 'high';
              else if (speed > 10 && distance < 2000) level = 'medium';
              
              // 발전기 위치 (가장 가까운 발전기로 할당)
              const turbines = ['YW-01', 'YW-02', 'YW-03', 'YW-04', 'YW-05', 'YW-06'];
              const location = turbines[Math.floor(Math.random() * turbines.length)];
              
              // 조류 종류 (실제로는 AI 분류가 필요)
              const species = ['crow', 'eagle', 'hawk', 'seagull'][Math.floor(Math.random() * 4)];
              
              return {
                id: track.id,
                level,
                location,
                distance: Math.round(distance),
                species,
                speed: Math.round(speed),
                timestamp: new Date()
              };
            });
          
          setCollisionRisks(risks);
          
          // 가장 높은 위험 레벨 계산
          const riskLevels = { low: 0, medium: 1, high: 2, critical: 3 };
          let highestLevel = 'low';
          
          risks.forEach(risk => {
            if (riskLevels[risk.level] > riskLevels[highestLevel]) {
              highestLevel = risk.level;
            }
          });
          
          setHighestRiskLevel(highestLevel);
        }
      } catch (error) {
        console.error('충돌 위험 데이터 불러오기 오류:', error);
        // 오류 발생 시 기본 데이터 유지
        const risks = [
          { id: 1, level: 'low', location: 'YW-01', distance: 750, species: 'crow', speed: 15, timestamp: new Date() },
          { id: 2, level: 'low', location: 'YW-03', distance: 820, species: 'eagle', speed: 12, timestamp: new Date() }
        ];
        
        setCollisionRisks(risks);
        setHighestRiskLevel('low');
      }
    };
    
    // 초기 데이터 로드
    updateCollisionRisks();
    
    // 15초마다 업데이트
    const intervalId = setInterval(updateCollisionRisks, 15000);
    
    return () => clearInterval(intervalId);
  }, [setHighestRiskLevel]);

  // 종별 누적 현황 및 방위별 출현율 데이터 가져오기
  const [dailySpeciesStats, setDailySpeciesStats] = useState([]);
  const [directionStats, setDirectionStats] = useState([]);
  
  useEffect(() => {
    const fetchSpeciesAndDirectionStats = async () => {
      try {
        const API_BASE_URL = 'http://localhost:8000';
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
        
        // 조류 종류별 통계 데이터 가져오기
        const birdResponse = await axios.get(`${API_BASE_URL}/api/export-birds-csv/`);
        
        if (typeof birdResponse.data === 'string' && birdResponse.data.includes('","')) {
          // CSV 데이터 파싱
          const lines = birdResponse.data.split('\n');
          const headers = lines[0].split(',').map(header => 
            header.replace(/"/g, '').trim()
          );
          
          const speciesData = [];
          const colors = ['#4caf50', '#ff9800', '#2196f3', '#f44336', '#9e9e9e']; 
          
          // 상위 4개 종과 나머지 종을 '기타'로 합침
          const parsedData = [];
          for (let i = 1; i < lines.length; i++) {
            if (!lines[i].trim()) continue;
            
            const values = lines[i].split(',').map(val => val.replace(/"/g, '').trim());
            const bird = {};
            
            headers.forEach((header, index) => {
              bird[header] = values[index];
            });
            
            parsedData.push(bird);
          }
          
          // 감지 건수 기준으로 정렬
          parsedData.sort((a, b) => {
            const countA = parseInt(a['감지 건수'] || '0');
            const countB = parseInt(b['감지 건수'] || '0');
            return countB - countA;
          });
          
          // 상위 4개 종과 기타로 데이터 생성
          const top4 = parsedData.slice(0, 4);
          const others = parsedData.slice(4);
          
          let otherCount = 0;
          others.forEach(bird => {
            otherCount += parseInt(bird['감지 건수'] || '0');
          });
          
          const speciesStats = [
            ...top4.map((bird, index) => ({
              name: bird['한글명'] || `종류 ${index + 1}`,
              count: parseInt(bird['감지 건수'] || '0'),
              color: colors[index]
            })),
            {
              name: '기타',
              count: otherCount,
              color: colors[4]
            }
          ];
          
          setDailySpeciesStats(speciesStats);
        }
        
        // 방위별 출현율 데이터 가져오기
        const bbResponse = await axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, {
          params: {
            date_from: today,
            date_to: today
          }
        });
        
        if (bbResponse.data && bbResponse.data.bb_data) {
          const bbData = bbResponse.data.bb_data;
          
          // 방위별 출현율 계산 (8개 방위에 대해)
          const directions = ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'];
          const directionCounts = {};
          
          // 초기화
          directions.forEach(dir => {
            directionCounts[dir] = 0;
          });
          
          // 각 바운딩 박스의 위치 정보를 사용하여 방위 계산
          bbData.forEach(bb => {
            // 바운딩 박스 중심 위치에서 방위 계산 (간단한 예시)
            const centerX = (bb.bb_left + bb.bb_right) / 2;
            const centerY = (bb.bb_top + bb.bb_bottom) / 2;
            
            // 중심에서의 상대적 위치를 기준으로 방위 할당
            let direction;
            if (centerY < 0.4) {
              if (centerX < 0.4) direction = 'NW';
              else if (centerX > 0.6) direction = 'NE';
              else direction = 'N';
            } else if (centerY > 0.6) {
              if (centerX < 0.4) direction = 'SW';
              else if (centerX > 0.6) direction = 'SE';
              else direction = 'S';
            } else {
              if (centerX < 0.4) direction = 'W';
              else if (centerX > 0.6) direction = 'E';
              else return; // 중앙은 방위 없음
            }
            
            directionCounts[direction]++;
          });
          
          // 총 카운트 계산
          const totalCount = Object.values(directionCounts).reduce((sum, count) => sum + count, 0);
          
          // 방위별 출현율 계산 (0~1 범위)
          const directionData = directions.map(dir => ({
            direction: dir,
            value: totalCount > 0 ? directionCounts[dir] / totalCount : 0
          }));
          
          setDirectionStats(directionData);
        }
      } catch (error) {
        console.error('종별/방위별 데이터 로드 오류:', error);
        // 오류 시 기본값 설정
        setDailySpeciesStats([
          { name: '까마귀', count: 120, color: '#4caf50' },
          { name: '독수리', count: 85, color: '#ff9800' },
          { name: '청동오리', count: 60, color: '#2196f3' },
          { name: '갈매기', count: 45, color: '#f44336' },
          { name: '기타', count: 30, color: '#9e9e9e' }
        ]);
        
        setDirectionStats([
          { direction: 'N', value: 0.15 },
          { direction: 'NE', value: 0.18 },
          { direction: 'E', value: 0.12 },
          { direction: 'SE', value: 0.10 },
          { direction: 'S', value: 0.08 },
          { direction: 'SW', value: 0.10 },
          { direction: 'W', value: 0.12 },
          { direction: 'NW', value: 0.15 }
        ]);
      }
    };
    
    fetchSpeciesAndDirectionStats();
    
    // 1시간마다 갱신
    const intervalId = setInterval(fetchSpeciesAndDirectionStats, 60 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 일일 조류 인식 누적 현황 데이터 가져오기
  useEffect(() => {
    const fetchDailyCameraStats = async () => {
      try {
        // 백엔드 API에서 카메라별 일일 데이터 가져오기
        const API_BASE_URL = 'http://localhost:8000';
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜 (YYYY-MM-DD)
        
        // 바운딩 박스 데이터를 직접 가져옵니다 (감지가 아닌 실제 바운딩 박스)
        const response = await axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, {
          params: {
            date_from: today,
            date_to: today
          }
        });
        
        if (response.data && response.data.bb_data) {
          // 바운딩 박스 데이터에서 카메라별 개체수 계산
          const bbData = response.data.bb_data;
          console.log(`바운딩 박스 데이터 개수: ${bbData.length}`);
          
          // 카메라별 바운딩 박스 카운트 (각 바운딩 박스는 새 한 마리)
          const camera1Count = bbData.filter(bb => bb.camera_id === 1).length;
          const camera2Count = bbData.filter(bb => bb.camera_id === 2).length;
          const camera3Count = bbData.filter(bb => bb.camera_id === 3).length;
          const totalCount = camera1Count + camera2Count + camera3Count;
          
          // 상태 업데이트
          setDailyCameraStats({
            total: totalCount,
            camera1: camera1Count,
            camera2: camera2Count,
            camera3: camera3Count
          });
          
          console.log('일일 카메라별 바운딩 박스 데이터 로드 완료:', { totalCount, camera1Count, camera2Count, camera3Count });
        } else {
          console.warn('API 응답에 바운딩 박스 데이터가 없습니다');
          
          // 기존 방식을 대체 방법으로 사용 (감지 데이터 기반)
          const detectionsResponse = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
            params: {
              date_from: today,
              date_to: today
            }
          });
          
          if (detectionsResponse.data && detectionsResponse.data.detections) {
            const detections = detectionsResponse.data.detections;
            console.log(`감지 데이터 개수: ${detections.length}`);
            
            // 각 감지에는 여러 개의 바운딩 박스가 있을 수 있음 (bb_count)
            const camera1Count = detections
              .filter(d => d.camera_id === 1)
              .reduce((sum, d) => sum + (d.bb_count || 0), 0);
              
            const camera2Count = detections
              .filter(d => d.camera_id === 2)
              .reduce((sum, d) => sum + (d.bb_count || 0), 0);
              
            const camera3Count = detections
              .filter(d => d.camera_id === 3)
              .reduce((sum, d) => sum + (d.bb_count || 0), 0);
              
            const totalCount = camera1Count + camera2Count + camera3Count;
            
            setDailyCameraStats({
              total: totalCount,
              camera1: camera1Count,
              camera2: camera2Count,
              camera3: camera3Count
            });
            
            console.log('일일 카메라별 감지 데이터 로드 완료 (대체 방법):', { totalCount, camera1Count, camera2Count, camera3Count });
          }
        }
      } catch (error) {
        console.error('일일 카메라별 데이터 로드 오류:', error);
        // 오류 시 기존 더미 데이터 유지 또는 기본값 설정
        setDailyCameraStats({
          total: 73412,
          camera1: 15892,
          camera2: 32450,
          camera3: 25070
        });
      }
    };
    
    // 데이터 로드
    fetchDailyCameraStats();
    
    // 5분마다 자동 갱신
    const intervalId = setInterval(fetchDailyCameraStats, 5 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 지도 시야각 영역 생성 함수
  const createViewField = (position, direction, angle, distance) => {
    const lat = position[0];
    const lng = position[1];
    
    // 방향각을 라디안으로 변환 (북쪽이 0도, 시계방향)
    const directionRad = (direction * Math.PI) / 180;
    // 시야 각도의 절반을 라디안으로 변환
    const halfAngleRad = (angle / 2 * Math.PI) / 180;
    
    // 부채꼴 모양을 만들기 위한 점 배열
    const points = [position];
    
    // 거리 계산을 위한 지구 반경 (미터)
    const R = 6378137;
    
    // 부채꼴 모양을 위해 더 많은 점 추가 (15개 정도의 점으로 부드러운 곡선 생성)
    const segments = 15;
    
    for (let i = 0; i <= segments; i++) {
      // 시야 각도 내에서 고르게 분포된 각도 계산
      const segmentAngle = directionRad - halfAngleRad + (i * (2 * halfAngleRad) / segments);
      
      // 해당 각도의 끝점 좌표 계산
      const segmentLat = lat + (distance / R) * (180 / Math.PI) * Math.cos(segmentAngle);
      const segmentLng = lng + (distance / R) * (180 / Math.PI) * Math.sin(segmentAngle) / Math.cos(lat * Math.PI / 180);
      
      points.push([segmentLat, segmentLng]);
    }
    
    // 다각형을 닫기 위해 시작점 다시 추가
    points.push(position);
    
    return points;
  };
  
  // 사이드바 토글 핸들러
  const handleToggleLeftPanel = useCallback(() => {
    setShowLeftPanel(prev => !prev);
    
    // 레이아웃 변경 후 맵 리사이징 및 센터링
    setTimeout(() => {
      if (mapRef.current && mapRef.current.leafletElement) {
        mapRef.current.leafletElement.invalidateSize();
      }
    }, 300);
  }, []);
  
  // 오른쪽 패널 토글 핸들러
  const handleToggleRightPanel = useCallback(() => {
    setShowRightPanel(prev => !prev);
    
    // 레이아웃 변경 후 맵 리사이징 및 센터링
    setTimeout(() => {
      if (mapRef.current && mapRef.current.leafletElement) {
        mapRef.current.leafletElement.invalidateSize();
      }
    }, 300);
  }, []);
  
  // 사이드바 상태 변경 시 맵 리사이징
  useEffect(() => {
    if (mapRef.current && mapRef.current.leafletElement) {
      // 레이아웃 변경 후 맵 컨테이너 크기 재계산을 위해 약간의 지연 추가
      const timer = setTimeout(() => {
        // 현재 중심과 줌 레벨 저장
        const currentCenter = mapRef.current.leafletElement.getCenter();
        const currentZoom = mapRef.current.leafletElement.getZoom();
        
        // 맵 크기 재계산
        mapRef.current.leafletElement.invalidateSize({ 
          animate: true, 
          pan: true 
        });
        
        // 원래 중심과 줌 레벨로 설정
        mapRef.current.leafletElement.setView(
          currentCenter, 
          currentZoom, 
          { animate: true, duration: 0.5 }
        );
      }, 400);
      
      return () => clearTimeout(timer);
    }
  }, [showLeftPanel, showRightPanel]);

  // 통계 위젯 스타일
  const statsWidgetStyle = {
    backgroundColor: 'rgba(10, 25, 41, 0.95)',
    borderRadius: 2,
    border: '1px solid rgba(30, 58, 90, 0.8)',
    p: 0,
    mb: 1,
    boxShadow: '0 4px 8px rgba(0, 0, 0, 0.2)'
  };

  return (
    <Box sx={{ height: '100%', display: 'flex' }}>
      {/* 왼쪽 패널 */}
      <Box 
        sx={{ 
          width: 336,
          display: showLeftPanel ? 'block' : 'none',
          overflow: 'auto',
          pr: 2,
          backgroundColor: 'rgba(10, 25, 41, 0.95)',
          borderRight: '1px solid rgba(30, 58, 90, 0.5)',
          boxShadow: '2px 0 8px rgba(0, 0, 0, 0.2)',
          transition: 'all 0.3s ease-in-out',
          zIndex: 10
        }}
      >
        <Grid container spacing={2.5} direction="column" sx={{ p: 1.5 }}>
          {/* 날씨 정보 위젯 */}
          <Grid item>
            <WeatherWidget 
              weatherData={weatherData}
              weatherLoading={weatherLoading}
              language={language}
            />
          </Grid>
          
          {/* 충돌 위험 알림 */}
          <Grid item>
            <CollisionAlert 
              collisionRisks={collisionRisks}
              language={language}
            />
          </Grid>
          
          {/* 조류 근접 현황 */}
          <Grid item>
            <BirdProximityStatus 
              birdActivityData={birdActivityData}
              language={language}
            />
          </Grid>
        </Grid>
      </Box>
      
      {/* 왼쪽 패널 토글 버튼 - 세로 경계에 위치 */}
      <Box
        sx={{
          position: 'relative',
          zIndex: 1100,
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          width: '22px',
        }}
      >
        <IconButton
          onClick={handleToggleLeftPanel}
          sx={{
            position: 'absolute',
            padding: 0,
            minWidth: '22px',
            width: '22px',
            height: '80px',
            borderRadius: '0 8px 8px 0',
            background: 'linear-gradient(90deg, #1e3a5a 0%, #2c4f7c 100%)',
            color: 'white',
            '&:hover': {
              background: 'linear-gradient(90deg, #254a75 0%, #3a6ca5 100%)',
              boxShadow: '0 0 8px rgba(33, 150, 243, 0.5)'
            },
            boxShadow: '3px 0 6px rgba(0,0,0,0.25)',
            left: 0,
            top: '50%',
            transform: 'translateY(-50%)',
            display: 'flex',
            flexDirection: 'column',
            alignItems: 'center',
            justifyContent: 'center',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            borderLeft: 'none',
            transition: 'all 0.2s ease-in-out'
          }}
        >
          {showLeftPanel ? 
            <ChevronLeft sx={{ 
              fontSize: '16px', 
              filter: 'drop-shadow(0 0 2px rgba(0,0,0,0.5))'
            }} /> : 
            <ChevronRight sx={{ 
              fontSize: '16px',
              filter: 'drop-shadow(0 0 2px rgba(0,0,0,0.5))'
            }} />
          }
        </IconButton>
      </Box>
      
      {/* 중앙/메인 컨텐츠 영역 */}
      <Box sx={{ 
        flexGrow: 1, 
        position: 'relative', 
        height: '100%',
        transition: 'all 0.3s ease-in-out'
      }}>
        {/* 지도 영역 */}
        <MapView
          mapCenter={mapCenter}
          mapRef={mapRef}
          setShowCameraModal={setShowCameraModal}
          createViewField={createViewField}
          cameraStatus={cameraStatus}
          language={language}
          isPanelOpen={showLeftPanel || showRightPanel}
          leftPanelOpen={showLeftPanel}
          rightPanelOpen={showRightPanel}
        />
      </Box>
      
      {/* 오른쪽 패널 토글 버튼 - 세로 경계에 위치 */}
      <Box
        sx={{
          position: 'relative',
          zIndex: 1100,
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          width: '22px',
        }}
      >
        <IconButton
          onClick={handleToggleRightPanel}
          sx={{
            position: 'absolute',
            padding: 0,
            minWidth: '22px',
            width: '22px',
            height: '80px',
            borderRadius: '8px 0 0 8px',
            background: 'linear-gradient(90deg, #2c4f7c 0%, #1e3a5a 100%)',
            color: 'white',
            '&:hover': {
              background: 'linear-gradient(90deg, #3a6ca5 0%, #254a75 100%)',
              boxShadow: '0 0 8px rgba(33, 150, 243, 0.5)'
            },
            boxShadow: '-3px 0 6px rgba(0,0,0,0.25)',
            right: 0,
            top: '50%',
            transform: 'translateY(-50%)',
            display: 'flex',
            flexDirection: 'column',
            alignItems: 'center',
            justifyContent: 'center',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            borderRight: 'none',
            transition: 'all 0.2s ease-in-out'
          }}
        >
          {showRightPanel ? 
            <ChevronRight sx={{ 
              fontSize: '16px', 
              filter: 'drop-shadow(0 0 2px rgba(0,0,0,0.5))'
            }} /> : 
            <ChevronLeft sx={{ 
              fontSize: '16px',
              filter: 'drop-shadow(0 0 2px rgba(0,0,0,0.5))'
            }} />
          }
        </IconButton>
      </Box>
      
      {/* 오른쪽 패널 */}
      <Box 
        sx={{ 
          width: 300,
          display: showRightPanel ? 'block' : 'none',
          overflow: 'hidden',
          pl: 2,
          pr: 1.5,
          py: 1,
          backgroundColor: 'rgba(10, 25, 41, 0.95)',
          borderLeft: '1px solid rgba(30, 58, 90, 0.5)',
          boxShadow: '-2px 0 8px rgba(0, 0, 0, 0.2)',
          transition: 'all 0.3s ease-in-out',
          zIndex: 10,
          height: '100%',
          display: 'flex',
          flexDirection: 'column',
          justifyContent: 'space-between'
        }}
      >
        {/* 일일 누적 현황 위젯 */}
        <Paper elevation={0} sx={{...statsWidgetStyle, height: 'calc(33% - 20px)'}}>
          <DailyStatsChart 
            data={[
              { label: '전체', value: dailyCameraStats.total, color: '#2196f3' },
              { label: '카메라 1', value: dailyCameraStats.camera1, color: '#3E6D9C' },
              { label: '카메라 2', value: dailyCameraStats.camera2, color: '#5E8C61' },
              { label: '카메라 3', value: dailyCameraStats.camera3, color: '#AF7AB3' }
            ]}
          />
        </Paper>
        
        {/* 일일 종별 누적 현황 위젯 */}
        <Paper elevation={0} sx={{...statsWidgetStyle, height: 'calc(33% - 20px)'}}>
          <SpeciesStatsChart 
            data={dailySpeciesStats}
          />
        </Paper>
        
        {/* 방위별 출현율 위젯 */}
        <Paper elevation={0} sx={{...statsWidgetStyle, height: 'calc(33% - 20px)'}}>
          <RadarDirectionChart 
            data={directionStats}
          />
        </Paper>
      </Box>

      {/* 카메라 스트림 모달 */}
      <CameraModal
        showCameraModal={showCameraModal}
        setShowCameraModal={setShowCameraModal}
        language={language}
      />
    </Box>
  );
};

export default Dashboard; 