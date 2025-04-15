import { useState, useEffect, useRef, useCallback } from 'react';
import { Box, Grid, useTheme, IconButton, Paper, Typography, Divider } from '@mui/material';
import { ChevronLeft, ChevronRight } from '@mui/icons-material';
import useAppStore from '../../store/useAppStore';

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
  
  // 더미 데이터 - 일일 카메라별 누적 현황
  const [dailyCameraStats] = useState({
    total: 73412,
    camera1: 15892,
    camera2: 32450,
    camera3: 25070
  });
  
  // 더미 데이터 - 일일 종별 누적 현황
  const [dailySpeciesStats] = useState([
    { name: '까마귀', count: 42150, color: '#4caf50' },
    { name: '독수리', count: 15230, color: '#ff9800' },
    { name: '청동오리', count: 8732, color: '#2196f3' },
    { name: '갈매기', count: 5120, color: '#f44336' },
    { name: '기타', count: 2180, color: '#9e9e9e' }
  ]);
  
  // 더미 데이터 - 방위별 출현율
  const [directionStats] = useState([
    { direction: 'N', value: 0.72 },
    { direction: 'NE', value: 0.85 },
    { direction: 'E', value: 0.53 },
    { direction: 'SE', value: 0.62 },
    { direction: 'S', value: 0.38 },
    { direction: 'SW', value: 0.42 },
    { direction: 'W', value: 0.28 },
    { direction: 'NW', value: 0.65 }
  ]);
  
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

  // 날씨 데이터 불러오기 (예시 데이터)
  useEffect(() => {
    const fetchWeatherData = async () => {
      setWeatherLoading(true);
      try {
        // 실제로는 API 호출로 대체
        setTimeout(() => {
          setWeatherData({
            location: '전라남도 영광군 소각시도',
            timestamp: new Date().toISOString(),
            current: {
              temperature: 8.1,
              feels_like: 5.2,
              humidity: 65,
              wind_speed: 4.2,
              wind_direction: 'NE',
              precipitation: 1,
              weather_condition: 'clear',
              pressure: 1012,
              visibility: 10,
              uv_index: 3
            },
            forecast: [
              // 일기 예보 데이터
            ]
          });
          setWeatherLoading(false);
        }, 1000);
      } catch (error) {
        console.error('날씨 데이터를 불러오는 중 오류 발생:', error);
        setWeatherLoading(false);
      }
    };

    fetchWeatherData();
  }, []);

  // 조류 활동 데이터 생성 (예시 데이터)
  useEffect(() => {
    // 풍력 발전기별 조류 근접 데이터
    const turbineData = [
      { id: 'YW-01', count: 8, risk: 0.4 },
      { id: 'YW-02', count: 10, risk: 0.5 },
      { id: 'YW-03', count: 7, risk: 0.7 },
      { id: 'YW-04', count: 5, risk: 0.3 },
      { id: 'YW-05', count: 4, risk: 0.2 },
      { id: 'YW-06', count: 3, risk: 0.15 }
    ];
    
    setBirdActivityData(turbineData);
  }, []);

  // 충돌 위험 데이터 업데이트 (예시 데이터)
  useEffect(() => {
    const updateCollisionRisks = () => {
      // 실제로는 API 호출로 데이터를 가져와야 함
      const risks = [
        { id: 1, level: 'low', location: 'YW-01', distance: 750, species: 'crow', speed: 15, timestamp: new Date() },
        { id: 2, level: 'medium', location: 'YW-03', distance: 320, species: 'eagle', speed: 22, timestamp: new Date() },
        { id: 3, level: 'high', location: 'YW-05', distance: 180, species: 'hawk', speed: 30, timestamp: new Date() }
      ];
      
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
    };
    
    // 초기 데이터 로드
    updateCollisionRisks();
    
    // 15초마다 업데이트
    const intervalId = setInterval(updateCollisionRisks, 15000);
    
    return () => clearInterval(intervalId);
  }, [setHighestRiskLevel]);

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
    p: 2,
    mb: 2.5,
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
          overflow: 'auto',
          pl: 2,
          pr: 1.5,
          py: 1.5,
          backgroundColor: 'rgba(10, 25, 41, 0.95)',
          borderLeft: '1px solid rgba(30, 58, 90, 0.5)',
          boxShadow: '-2px 0 8px rgba(0, 0, 0, 0.2)',
          transition: 'all 0.3s ease-in-out',
          zIndex: 10
        }}
      >
        {/* 일일 누적 현황 위젯 */}
        <Paper elevation={0} sx={statsWidgetStyle}>
          <Typography variant="subtitle1" color="lightblue" gutterBottom fontWeight="bold">
            조류 인식 일일 누적 현황
          </Typography>
          <Divider sx={{ borderColor: 'rgba(150, 200, 255, 0.15)', mb: 2 }} />
          
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
        <Paper elevation={0} sx={statsWidgetStyle}>
          <Typography variant="subtitle1" color="lightblue" gutterBottom fontWeight="bold">
            조류 종별 일일 누적 현황
          </Typography>
          <Divider sx={{ borderColor: 'rgba(150, 200, 255, 0.15)', mb: 2 }} />
          
          <SpeciesStatsChart 
            data={dailySpeciesStats}
          />
        </Paper>
        
        {/* 방위별 출현율 위젯 */}
        <Paper elevation={0} sx={statsWidgetStyle}>
          <Typography variant="subtitle1" color="lightblue" gutterBottom fontWeight="bold">
            방위별 출현율
          </Typography>
          <Divider sx={{ borderColor: 'rgba(150, 200, 255, 0.15)', mb: 2 }} />
          
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