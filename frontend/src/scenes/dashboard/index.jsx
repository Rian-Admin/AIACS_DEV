import { useState, useEffect, useRef } from 'react';
import { Box, Grid, useTheme } from '@mui/material';
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
  const [mapCenter, setMapCenter] = useState([35.192892, 126.221627]); // 소각시도 좌표
  const [showCameraModal, setShowCameraModal] = useState(false);
  const [cameraStatus, setCameraStatus] = useState({
    1: { online: true, error: false },
    2: { online: true, error: false },
    3: { online: true, error: false }
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

  // 날씨 데이터 불러오기 (예시 데이터)
  useEffect(() => {
    const fetchWeatherData = async () => {
      setWeatherLoading(true);
      try {
        // 실제로는 API 호출로 대체
        setTimeout(() => {
          setWeatherData({
            location: '전남 영광군 소각시도',
            timestamp: new Date().toISOString(),
            current: {
              temperature: 12.5,
              feels_like: 10.2,
              humidity: 65,
              wind_speed: 4.2,
              wind_direction: 'NE',
              precipitation: 0,
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

  return (
    <Box sx={{ height: '100%' }}>
      <Grid container spacing={2} sx={{ height: '100%' }}>
        {/* 왼쪽 패널 */}
        <Grid item xs={12} md={4} lg={3} xl={2.5}>
          <Grid container spacing={2} direction="column">
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
        </Grid>
        
        {/* 중앙 패널 */}
        <Grid item xs={12} md={8} lg={6} xl={7}>
          <MapView
            mapCenter={mapCenter}
            mapRef={mapRef}
            setShowCameraModal={setShowCameraModal}
            createViewField={createViewField}
            cameraStatus={cameraStatus}
            language={language}
          />
        </Grid>
        
        {/* 오른쪽 패널 */}
        <Grid item xs={12} lg={3} xl={2.5} sx={{ display: { xs: 'none', lg: 'block' } }}>
          <Grid container spacing={2} direction="column">
            {/* 시스템 상태 */}
            <Grid item>
              <SystemStatus language={language} />
            </Grid>
            
            {/* 조류 감지 통계 */}
            <Grid item>
              <BirdStatistics language={language} />
            </Grid>
            
            {/* 이벤트 기록 */}
            <Grid item>
              <RecentEvents language={language} />
            </Grid>
          </Grid>
        </Grid>
      </Grid>

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