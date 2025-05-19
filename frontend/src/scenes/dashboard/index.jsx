import { useState, useEffect, useRef, useCallback } from 'react';
import { Box, Grid, useTheme, IconButton, Paper, Typography, Divider, Tooltip } from '@mui/material';
import { ChevronLeft, ChevronRight, Refresh, CachedOutlined } from '@mui/icons-material';
import useAppStore from '../../store/useAppStore';
import axios from 'axios';
import { translate } from '../../utils/i18n';

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

// API_BASE_URL을 파일 상단에 한 번만 정의하고 모든 곳에서 동일하게 사용하도록 변경
// 하드코딩된 localhost:8000 URL을 상대 경로로 변경
const API_BASE_URL = process.env.REACT_APP_API_URL || ''; // 빈 문자열은 상대 경로를 의미합니다

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
  const [collisionRisksLoading, setCollisionRisksLoading] = useState(false);
  const [birdActivityData, setBirdActivityData] = useState([]);
  const [birdActivityLoading, setBirdActivityLoading] = useState(false);
  const [dailySpeciesLoading, setDailySpeciesLoading] = useState(false);
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
  const [dailyCameraStatsLoading, setDailyCameraStatsLoading] = useState(false);
  
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
        // 백엔드 API 호출 (수정된 URL)
        const response = await axios.get(`${API_BASE_URL}/api/weather/current/`);
        
        if (response.data) {
          setWeatherData({
            location: '전라남도 영광군 소각시도',
            timestamp: response.data.timestamp,
            current: {
              temperature: response.data.current.temperature,
              feels_like: response.data.current.temperature ? response.data.current.temperature - 3 : null, // 체감온도 계산 (임의)
              humidity: response.data.current.humidity,
              wind_speed: response.data.current.wind_speed,
              wind_direction: response.data.current.wind_direction,
              precipitation: response.data.current.precipitation,
              weather_condition: response.data.current.precipitation_type || 'none',
              visibility: response.data.current.visibility // 백엔드에서 제공하는 시정 값 사용
            },
            forecast: response.data.forecast || []
          });
        }
        setWeatherLoading(false);
      } catch (error) {
        console.error('날씨 데이터를 불러오는 중 오류 발생:', error);
        // 오류 시 null로 설정하여 UI에서 '데이터 없음' 표시
        setWeatherData(null);
        setWeatherLoading(false);
      }
    };

    fetchWeatherData();
    
    // 날씨 데이터 갱신 주기를 5분에서 10분으로 늘림
    const intervalId = setInterval(fetchWeatherData, 10 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);
  
  // 수동으로 날씨 데이터 갱신하는 함수
  const refreshWeatherData = useCallback(async () => {
    console.log('날씨 데이터 수동 갱신 중...');
    setWeatherLoading(true);
    
    try {
      // 백엔드 API 호출 (수정된 URL)
      const response = await axios.get(`${API_BASE_URL}/api/weather/current/`);
      
      if (response.data) {
        setWeatherData({
          location: '전라남도 영광군 소각시도',
          timestamp: response.data.timestamp,
          current: {
            temperature: response.data.current.temperature,
            feels_like: response.data.current.temperature ? response.data.current.temperature - 3 : null, // 체감온도 계산 (임의)
            humidity: response.data.current.humidity,
            wind_speed: response.data.current.wind_speed,
            wind_direction: response.data.current.wind_direction,
            precipitation: response.data.current.precipitation,
            weather_condition: response.data.current.precipitation_type || 'none',
            visibility: response.data.current.visibility // 백엔드에서 제공하는 시정 값 사용
          },
          forecast: response.data.forecast || []
        });
        console.log('날씨 데이터 수동 갱신 완료');
      }
    } catch (error) {
      console.error('날씨 데이터 수동 갱신 오류:', error);
      // 오류 시 null로 설정하여 UI에서 '데이터 없음' 표시
      setWeatherData(null);
    } finally {
      setWeatherLoading(false);
    }
  }, []);

  // 조류 활동 데이터 가져오기
  useEffect(() => {
    const fetchBirdActivity = async () => {
      try {
        // today 변수 정의 추가
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜 (YYYY-MM-DD)
        
        // 바운딩 박스 데이터를 API에서 직접 가져오기 
        // (최근 5분 동안의 바운딩 박스 수를 카메라별로 계산 - 1분에서 5분으로 확장)
        const fiveMinutesAgo = new Date(new Date().getTime() - 5 * 60 * 1000).toISOString();
        
        // 바운딩 박스 데이터 가져오기
        const bbResponse = await axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, {
          params: {
            date_from: today,
            date_to: today
          }
        });
        
        if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_data) {
          const bbData = bbResponse.data.bb_data;
          console.log(`오늘 전체 바운딩 박스 데이터: ${bbData.length}개`);
          
          // 현재 시간 기준 5분 이내의 데이터만 필터링
          const recentData = bbData.filter(bb => {
            // detection_time이 최근 5분 이내인지 확인
            try {
              if (!bb.detection_time) return false;
              const bbTime = new Date(bb.detection_time);
              return bbTime >= new Date(fiveMinutesAgo);
            } catch (e) {
              console.warn('시간 형식 오류:', e);
              return false; // 시간 형식 오류가 있는 경우는 제외
            }
          });
          
          console.log(`최근 5분간 바운딩 박스 데이터: ${recentData.length}개`);
          
          // 카메라별로 그룹화
          const camera1Data = recentData.filter(bb => bb.camera_id === 1);
          const camera2Data = recentData.filter(bb => bb.camera_id === 2);
          const camera3Data = recentData.filter(bb => bb.camera_id === 3);
          
          console.log('카메라별 5분 바운딩 박스 원시 개수:', {
            'Camera-1': camera1Data.length,
            'Camera-2': camera2Data.length,
            'Camera-3': camera3Data.length
          });
          
          // SG-01, SG-02, SG-03에 대한 활동 데이터 생성
          // 데이터가 없는 경우에는 0으로 표시 (랜덤 값 생성 제거)
          const turbineData = [
            {
              id: 'SG-01',
              count: camera1Data.length,
              risk: Math.min(camera1Data.length * 0.1, 1),
              timestamp: new Date()
            },
            {
              id: 'SG-02',
              count: camera2Data.length,
              risk: Math.min(camera2Data.length * 0.1, 1),
              timestamp: new Date()
            },
            {
              id: 'SG-03',
              count: camera3Data.length,
              risk: Math.min(camera3Data.length * 0.1, 1),
              timestamp: new Date()
            }
          ];
          
          setBirdActivityData(turbineData);
          console.log('최종 카메라별 활동 데이터:', turbineData.map(item => ({
            id: item.id,
            count: item.count
          })));
        } else {
          // API 응답이 없거나 유효하지 않은 경우 으로 
          // 레이더 트랙 데이터 활용 (기존 로직)
          console.log('바운딩 박스 데이터를 가져오지 못했습니다. 레이더 데이터로 대체합니다.');
          const response = await axios.get(`${API_BASE_URL}/api/radar/track/`);
          
          if (response.data && response.data.tracks) {
            // SG-01, SG-02, SG-03에 맞게 업데이트된 터빈 정보
            const turbines = ['SG-01', 'SG-02', 'SG-03'];
            
            // 각 터빈(카메라)별 데이터 생성 - 기본값 0으로 설정
            const turbineData = turbines.map(id => {
              // 실제 레이더 데이터가 없으므로 0으로 설정
              const trackCount = 0;
              
              return {
                id,
                count: trackCount, 
                risk: 0,
                timestamp: new Date()
              };
            });
            
            setBirdActivityData(turbineData);
            console.log('레이더 기반 카메라별 활동 데이터 생성(대체):', turbineData);
          } else {
            throw new Error('레이더 데이터도 가져오지 못했습니다.');
          }
        }
      } catch (error) {
        console.error('조류 활동 데이터 가져오기 오류:', error);
        // 오류 시에도 UI를 유지하기 위해 기본 데이터 설정 (모두 0으로 설정)
        const turbineData = [
          { id: 'SG-01', count: 0, risk: 0, timestamp: new Date() },
          { id: 'SG-02', count: 0, risk: 0, timestamp: new Date() },
          { id: 'SG-03', count: 0, risk: 0, timestamp: new Date() }
        ];
        setBirdActivityData(turbineData);
        console.log('오류 발생 시 기본 데이터 사용:', turbineData);
      }
    };
    
    fetchBirdActivity();
    
    // 갱신 주기를 30초에서 2분(120초)으로 늘림 (리소스 소모 감소)
    const intervalId = setInterval(fetchBirdActivity, 120 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 수동으로 조류 활동 데이터 갱신하는 함수
  const refreshBirdActivity = useCallback(async () => {
    console.log('조류 활동 데이터 수동 갱신 중...');
    
    // 로딩 상태 설정 (필요한 경우)
    setBirdActivityLoading(true);
    
    try {
      // today 변수 정의 추가
      const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
      
      // 바운딩 박스 데이터 가져오기
      const fiveMinutesAgo = new Date(new Date().getTime() - 5 * 60 * 1000).toISOString();
      
      const bbResponse = await axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, {
        params: {
          date_from: today,
          date_to: today
        }
      });
      
      if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_data) {
        const bbData = bbResponse.data.bb_data;
        
        // 현재 시간 기준 5분 이내의 데이터만 필터링
        const recentData = bbData.filter(bb => {
          try {
            if (!bb.detection_time) return false;
            const bbTime = new Date(bb.detection_time);
            return bbTime >= new Date(fiveMinutesAgo);
          } catch (e) {
            return false;
          }
        });
        
        // 카메라별로 그룹화
        const camera1Data = recentData.filter(bb => bb.camera_id === 1);
        const camera2Data = recentData.filter(bb => bb.camera_id === 2);
        const camera3Data = recentData.filter(bb => bb.camera_id === 3);
        
        // 데이터 생성
        const turbineData = [
          {
            id: 'SG-01',
            count: camera1Data.length,
            risk: Math.min(camera1Data.length * 0.1, 1),
            timestamp: new Date()
          },
          {
            id: 'SG-02',
            count: camera2Data.length,
            risk: Math.min(camera2Data.length * 0.1, 1),
            timestamp: new Date()
          },
          {
            id: 'SG-03',
            count: camera3Data.length,
            risk: Math.min(camera3Data.length * 0.1, 1),
            timestamp: new Date()
          }
        ];
        
        setBirdActivityData(turbineData);
        console.log('조류 활동 데이터 수동 갱신 완료');
      }
    } catch (error) {
      console.error('조류 활동 데이터 수동 갱신 오류:', error);
    } finally {
      // 로딩 상태 해제
      setBirdActivityLoading(false);
    }
  }, []);

  // 충돌 위험 데이터 업데이트
  useEffect(() => {
    const updateCollisionRisks = async () => {
      try {
        // today 변수 정의 추가
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
        
        // 실제 DB에서 위험 알림 데이터 가져오기 
        // 1. DeterrentRecord 데이터를 가져와서 최근 충돌 위험 정보 사용
        const deterrentResponse = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
          params: {
            date_from: today,
            date_to: today,
            sort_by: 'date_desc',
            per_page: 15 // 최대 15개까지 가져오기(5개만 표시하지만 여유있게)
          }
        });
        
        if (deterrentResponse.data && deterrentResponse.data.status === 'success' && deterrentResponse.data.detections) {
          // 감지 데이터 필터링 및 가공
          const detections = deterrentResponse.data.detections.slice(0, 5); // 최대 5개까지 사용(기존 3개에서 변경)
          const timestamp = new Date();
          
          // 각 감지 항목에 대해 바운딩 박스 정보 가져오기
          const risks = [];
          
          for (const detection of detections) {
            try {
              // 해당 감지 ID의 바운딩 박스 정보 가져오기
              const bbResponse = await axios.get(`${API_BASE_URL}/api/detection/bb-info/${detection.detection_id}/`);
              
              if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_info && bbResponse.data.bb_info.length > 0) {
                // 바운딩 박스 정보 활용하여 위험도 계산
                const bbInfo = bbResponse.data.bb_info;
                
                // 감지된 객체 수에 따라 위험도 결정
                let level = 'low';
                if (detection.bb_count > 5) level = 'critical';
                else if (detection.bb_count > 3) level = 'high';
                else if (detection.bb_count > 1) level = 'medium';
                
                // 조류 종 정보 (첫 번째 바운딩 박스의 종 사용)
                const species = bbInfo[0].class_name || 'unknown';
                
                // 거리값은 임의로 설정 (실제로는 카메라와 객체 간 거리를 계산해야 함)
                const distance = Math.floor(Math.random() * 1000 + 500);
                
                // 임의의 속도값 (실제로는 연속 프레임에서 계산해야 함)
                const speed = Math.floor(Math.random() * 20 + 5);
                
                // 위치 정보 (카메라 ID를 기반으로 특정 터빈과 연결)
                const turbineMap = {
                  1: 'SG-01',
                  2: 'SG-02',
                  3: 'SG-03'
                };
                
                const location = turbineMap[detection.camera_id] || 'SG-01';
                
                risks.push({
                  id: detection.detection_id,
                  level,
                  location,
                  distance,
                  species,
                  speed,
                  timestamp,
                  detectionTime: detection.detection_time,
                  bbCount: detection.bb_count
                });
                
                console.log(`충돌 위험 알림 추가: ID ${detection.detection_id}, 레벨 ${level}, 위치 ${location}, 종류 ${species}, 객체 수 ${detection.bb_count}`);
              }
            } catch (bbError) {
              console.error(`바운딩 박스 정보 가져오기 오류 (ID: ${detection.detection_id}):`, bbError);
            }
          }
          
          // 충돌 위험 정보 업데이트
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
          console.log(`현재 최고 위험 레벨: ${highestLevel}, 위험 알림 수: ${risks.length}`);
        } else {
          // API 응답이 없거나 성공이 아닌 경우 대체 방법 사용
          console.warn('API에서 충돌 위험 데이터를 가져올 수 없어 레이더 데이터 활용');
          
          // 기존 레이더 트랙 데이터 활용 방식
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
              .slice(0, 5) // 최대 5개까지 표시(기존 3개에서 변경)
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
                const turbines = ['SG-01', 'SG-02', 'SG-03', 'SG-04', 'SG-05'];
                const location = turbines[Math.floor(Math.random() * turbines.length)];
                
                // 조류 종류 (실제로는 AI 분류가 필요)
                const species = ['까마귀', '독수리', '매', '갈매기'][Math.floor(Math.random() * 4)];
                
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
            console.log(`레이더 데이터 기반 위험 레벨: ${highestLevel}, 위험 알림 수: ${risks.length}`);
          }
        }
      } catch (error) {
        console.error('충돌 위험 데이터 불러오기 오류:', error);
        // 오류 발생 시 빈 배열로 설정 (더미 데이터 제거)
        setCollisionRisks([]);
        setHighestRiskLevel('low');
        console.log('오류로 인해 충돌 위험 데이터 없음');
      }
    };
    
    // 초기 데이터 로드
    updateCollisionRisks();
    console.log('충돌 위험 알림 모니터링 시작');
    
    // 1분에서 3분(180초)으로 갱신 주기 변경
    const intervalId = setInterval(() => {
      console.log('충돌 위험 알림 데이터 갱신 중...');
      updateCollisionRisks();
    }, 180000);
    
    return () => clearInterval(intervalId);
  }, [setHighestRiskLevel]);
  
  // 수동으로 충돌 위험 데이터 갱신하는 함수
  const refreshCollisionRisks = useCallback(async () => {
    console.log('충돌 위험 데이터 수동 갱신 중...');
    // 로딩 상태 설정 (필요한 경우)
    setCollisionRisksLoading(true);
    
    try {
      // 백엔드 API 호출
      const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
      
      // 실제 DB에서 위험 알림 데이터 가져오기
      const deterrentResponse = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
        params: {
          date_from: today,
          date_to: today,
          sort_by: 'date_desc',
          per_page: 15
        }
      });
      
      if (deterrentResponse.data && deterrentResponse.data.status === 'success' && deterrentResponse.data.detections) {
        // 감지 데이터 필터링 및 가공
        const detections = deterrentResponse.data.detections.slice(0, 5);
        const timestamp = new Date();
        
        // 각 감지 항목에 대해 바운딩 박스 정보 가져오기
        const risks = [];
        
        for (const detection of detections) {
          try {
            // 해당 감지 ID의 바운딩 박스 정보 가져오기
            const bbResponse = await axios.get(`${API_BASE_URL}/api/detection/bb-info/${detection.detection_id}/`);
            
            if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_info && bbResponse.data.bb_info.length > 0) {
              // 바운딩 박스 정보 활용하여 위험도 계산
              const bbInfo = bbResponse.data.bb_info;
              
              // 감지된 객체 수에 따라 위험도 결정
              let level = 'low';
              if (detection.bb_count > 5) level = 'critical';
              else if (detection.bb_count > 3) level = 'high';
              else if (detection.bb_count > 1) level = 'medium';
              
              // 조류 종 정보 (첫 번째 바운딩 박스의 종 사용)
              const species = bbInfo[0].class_name || 'unknown';
              
              // 거리값은 임의로 설정
              const distance = Math.floor(Math.random() * 1000 + 500);
              
              // 임의의 속도값
              const speed = Math.floor(Math.random() * 20 + 5);
              
              // 위치 정보
              const turbineMap = {
                1: 'SG-01',
                2: 'SG-02',
                3: 'SG-03'
              };
              
              const location = turbineMap[detection.camera_id] || 'SG-01';
              
              risks.push({
                id: detection.detection_id,
                level,
                location,
                distance,
                species,
                speed,
                timestamp,
                detectionTime: detection.detection_time,
                bbCount: detection.bb_count
              });
            }
          } catch (bbError) {
            console.error(`바운딩 박스 정보 가져오기 오류 (ID: ${detection.detection_id}):`, bbError);
          }
        }
        
        // 충돌 위험 정보 업데이트
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
        console.log('충돌 위험 데이터 수동 갱신 완료');
      }
    } catch (error) {
      console.error('충돌 위험 데이터 수동 갱신 오류:', error);
    } finally {
      // 로딩 상태 해제
      setCollisionRisksLoading(false);
    }
  }, [setHighestRiskLevel]);

  // 종별 누적 현황 및 방위별 출현율 데이터 가져오기
  const [dailySpeciesStats, setDailySpeciesStats] = useState([]);
  const [directionStats, setDirectionStats] = useState([]);
  
  useEffect(() => {
    const fetchSpeciesAndDirectionStats = async () => {
      try {
        setDailySpeciesLoading(true);
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
        
        // 조류 종류별 통계 데이터 가져오기
        const birdResponse = await axios.get(`${API_BASE_URL}/api/export-birds-csv/`);
        
        if (typeof birdResponse.data === 'string' && birdResponse.data.includes('","')) {
          // CSV 데이터 파싱
          const lines = birdResponse.data.split('\n');
          const headers = lines[0].split(',').map(header => 
            header.replace(/"/g, '').trim()
          );
          
          const colors = ['#4caf50', '#ff9800', '#2196f3', '#f44336', '#9c27b0', '#607d8b'];
          
          // 상위 6개 종을 가져옴 (기존 4개에서 변경)
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
          
          // 상위 6개 종 데이터 생성 (기존 4개에서 변경)
          const speciesStats = parsedData.slice(0, 6).map((bird, index) => ({
            name: bird['한글명'] || `종류 ${index + 1}`,
            count: parseInt(bird['감지 건수'] || '0'),
            color: colors[index]
          }));
          
          setDailySpeciesStats(speciesStats);
          console.log('CSV 기반 종별 통계 데이터 로드 완료 (대체 방법):', speciesStats);
        }
        
        // 방위별 출현율 데이터 로직은 그대로 유지
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
        setDailySpeciesLoading(false);
      } catch (error) {
        console.error('종별/방위별 데이터 로드 오류:', error);
        // 오류 시 빈 배열로 설정 (더미 데이터 제거)
        setDailySpeciesStats([]);
        
        setDirectionStats([
          { direction: 'N', value: 0 },
          { direction: 'NE', value: 0 },
          { direction: 'E', value: 0 },
          { direction: 'SE', value: 0 },
          { direction: 'S', value: 0 },
          { direction: 'SW', value: 0 },
          { direction: 'W', value: 0 },
          { direction: 'NW', value: 0 }
        ]);
        setDailySpeciesLoading(false);
      }
    };
    
    fetchSpeciesAndDirectionStats();
    
    // 갱신 주기를 10분으로 변경 (5분에서 변경)
    const intervalId = setInterval(fetchSpeciesAndDirectionStats, 10 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 수동으로 종별 통계 데이터 갱신하는 함수
  const refreshSpeciesStats = useCallback(async () => {
    console.log('종별 통계 데이터 수동 갱신 중...');
    setDailySpeciesLoading(true);
    
    try {
      const today = new Date().toISOString().split('T')[0]; // 오늘 날짜
      
      // 조류 종류별 통계 데이터 가져오기
      const birdResponse = await axios.get(`${API_BASE_URL}/api/export-birds-csv/`);
      
      if (typeof birdResponse.data === 'string' && birdResponse.data.includes('","')) {
        // CSV 데이터 파싱
        const lines = birdResponse.data.split('\n');
        const headers = lines[0].split(',').map(header => 
          header.replace(/"/g, '').trim()
        );
        
        const colors = ['#4caf50', '#ff9800', '#2196f3', '#f44336', '#9c27b0', '#607d8b'];
        
        // 상위 6개 종을 가져옴 (기존 4개에서 변경)
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
        
        // 상위 6개 종 데이터 생성 (기존 4개에서 변경)
        const speciesStats = parsedData.slice(0, 6).map((bird, index) => ({
          name: bird['한글명'] || `종류 ${index + 1}`,
          count: parseInt(bird['감지 건수'] || '0'),
          color: colors[index]
        }));
        
        setDailySpeciesStats(speciesStats);
        
        // 방위별 출현율 데이터도 갱신
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
            // 바운딩 박스 중심 위치에서 방위 계산 
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
        
        console.log('종별 통계 데이터 수동 갱신 완료');
      }
    } catch (error) {
      console.error('종별 통계 데이터 수동 갱신 오류:', error);
    } finally {
      setDailySpeciesLoading(false);
    }
  }, []);

  // 일일 조류 인식 누적 현황 데이터 가져오기
  useEffect(() => {
    const fetchDailyCameraStats = async () => {
      try {
        setDailyCameraStatsLoading(true);
        // 백엔드 API URL
        const today = new Date().toISOString().split('T')[0]; // 오늘 날짜 (YYYY-MM-DD)
        
        // 오늘 하루 조류 인식 현황을 위한 DetectionInfo 데이터 가져오기
        const detectionsResponse = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
          params: {
            date_from: today,
            date_to: today,
            per_page: 10000, // 충분히 큰 값으로 설정하여 모든 데이터 가져오기
            sort_by: 'date_desc' // 날짜 내림차순 정렬
          }
        });
        
        if (detectionsResponse.data && detectionsResponse.data.status === 'success') {
          // 카메라별 감지 개수 계산
          const detections = detectionsResponse.data.detections;
          console.log(`오늘의 감지 데이터 개수: ${detections.length}`);
          
          // 카메라 ID별로 분류
          const camera1Count = detections.filter(d => d.camera_id === 1).reduce((sum, d) => sum + (d.bb_count || 0), 0);
          const camera2Count = detections.filter(d => d.camera_id === 2).reduce((sum, d) => sum + (d.bb_count || 0), 0);
          const camera3Count = detections.filter(d => d.camera_id === 3).reduce((sum, d) => sum + (d.bb_count || 0), 0);
          const totalCount = camera1Count + camera2Count + camera3Count;
          
          // 상태 업데이트
          setDailyCameraStats({
            total: totalCount,
            camera1: camera1Count,
            camera2: camera2Count,
            camera3: camera3Count
          });
          
          console.log('일일 카메라별 탐지 데이터 로드 완료:', { totalCount, camera1Count, camera2Count, camera3Count });
        } else {
          console.warn('API 응답이 유효하지 않습니다:', detectionsResponse.data);
          
          // 대체 방법으로 바운딩 박스 데이터 직접 조회
          const bbResponse = await axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, {
            params: {
              date_from: today,
              date_to: today,
              get_all: true
            }
          });
          
          if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_data) {
            const bbData = bbResponse.data.bb_data;
            console.log(`바운딩 박스 데이터 개수: ${bbData.length}`);
            
            // 카메라별 바운딩 박스 개수 계산
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
            throw new Error('유효한 카메라별 데이터를 가져오지 못했습니다');
          }
        }
        setDailyCameraStatsLoading(false);
      } catch (error) {
        console.error('일일 카메라별 데이터 로드 오류:', error);
        // 오류 발생시 0으로 초기화하지 않고 이전 상태 유지
        setDailyCameraStats(prevStats => {
          if (prevStats.total === 0 && prevStats.camera1 === 0 && prevStats.camera2 === 0 && prevStats.camera3 === 0) {
            // 처음 로드할 때 오류 발생한 경우에만 기본값 설정
            return {
              total: 0,
              camera1: 0,
              camera2: 0,
              camera3: 0
            };
          }
          return prevStats; // 이전 상태 유지
        });
        setDailyCameraStatsLoading(false);
      }
    };
    
    // 데이터 로드
    fetchDailyCameraStats();
    
    // 5분 주기로 자동 갱신 (원래 1분이었던 것을 5분으로 변경)
    const intervalId = setInterval(fetchDailyCameraStats, 5 * 60 * 1000);
    
    return () => clearInterval(intervalId);
  }, []);

  // 수동으로 일일 카메라별 누적 현황 데이터 갱신
  const refreshDailyCameraStats = useCallback(async () => {
    console.log('일일 카메라별 누적 현황 데이터 수동 갱신 중...');
    setDailyCameraStatsLoading(true);
    
    try {
      // 백엔드 API URL
      const today = new Date().toISOString().split('T')[0]; // 오늘 날짜 (YYYY-MM-DD)
      
      // 오늘 하루 조류 인식 현황을 위한 DetectionInfo 데이터 가져오기
      const detectionsResponse = await axios.get(`${API_BASE_URL}/api/detections/filtered/`, {
        params: {
          date_from: today,
          date_to: today,
          per_page: 10000,
          sort_by: 'date_desc'
        }
      });
      
      if (detectionsResponse.data && detectionsResponse.data.status === 'success') {
        // 카메라별 감지 개수 계산
        const detections = detectionsResponse.data.detections;
        
        // 카메라 ID별로 분류
        const camera1Count = detections.filter(d => d.camera_id === 1).reduce((sum, d) => sum + (d.bb_count || 0), 0);
        const camera2Count = detections.filter(d => d.camera_id === 2).reduce((sum, d) => sum + (d.bb_count || 0), 0);
        const camera3Count = detections.filter(d => d.camera_id === 3).reduce((sum, d) => sum + (d.bb_count || 0), 0);
        const totalCount = camera1Count + camera2Count + camera3Count;
        
        // 상태 업데이트
        setDailyCameraStats({
          total: totalCount,
          camera1: camera1Count,
          camera2: camera2Count,
          camera3: camera3Count
        });
        
        console.log('일일 카메라별 탐지 데이터 수동 갱신 완료');
      }
    } catch (error) {
      console.error('일일 카메라별 데이터 수동 갱신 오류:', error);
    } finally {
      setDailyCameraStatsLoading(false);
    }
  }, []);

  // 대시보드 전체 데이터 한 번에 새로고침
  const refreshAllData = useCallback(() => {
    refreshWeatherData();
    refreshCollisionRisks();
    refreshBirdActivity();
    refreshSpeciesStats();
    refreshDailyCameraStats();
    console.log('모든 대시보드 데이터 새로고침 시작');
  }, [refreshWeatherData, refreshCollisionRisks, refreshBirdActivity, refreshSpeciesStats, refreshDailyCameraStats]);

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
        <Box sx={{ display: 'flex', justifyContent: 'flex-end', p: 1 }}>
          <Tooltip title={translate('모든 데이터 새로고침', 'Refresh All Data', language)}>
            <IconButton 
              size="small" 
              onClick={refreshAllData}
              sx={{ 
                color: 'rgba(255, 255, 255, 0.7)',
                '&:hover': { color: 'rgba(255, 255, 255, 0.9)' } 
              }}
            >
              <CachedOutlined fontSize="small" />
            </IconButton>
          </Tooltip>
        </Box>
        
        <Grid container spacing={2.5} direction="column" sx={{ p: 1.5 }}>
          {/* 날씨 정보 위젯 */}
          <Grid item>
            <Box sx={{ position: 'relative' }}>
              <WeatherWidget 
                weatherData={weatherData}
                weatherLoading={weatherLoading}
                language={language}
              />
              <Tooltip title={translate('날씨 정보 새로고침', 'Refresh Weather', language)}>
                <IconButton 
                  size="small" 
                  onClick={refreshWeatherData}
                  disabled={weatherLoading}
                  sx={{ 
                    position: 'absolute', 
                    top: 6, 
                    right: 6,
                    color: 'rgba(255, 255, 255, 0.5)',
                    '&:hover': { color: 'rgba(255, 255, 255, 0.8)' },
                    opacity: 0.7
                  }}
                >
                  <Refresh fontSize="small" />
                </IconButton>
              </Tooltip>
            </Box>
          </Grid>
          
          {/* 충돌 위험 알림 */}
          <Grid item>
            <Box sx={{ position: 'relative' }}>
              <CollisionAlert 
                collisionRisks={collisionRisks}
                language={language}
              />
              <Tooltip title={translate('충돌 위험 알림 새로고침', 'Refresh Collision Alerts', language)}>
                <IconButton 
                  size="small" 
                  onClick={refreshCollisionRisks}
                  disabled={collisionRisksLoading}
                  sx={{ 
                    position: 'absolute', 
                    top: 6, 
                    right: 6,
                    color: 'rgba(255, 255, 255, 0.5)',
                    '&:hover': { color: 'rgba(255, 255, 255, 0.8)' },
                    opacity: 0.7
                  }}
                >
                  <Refresh fontSize="small" />
                </IconButton>
              </Tooltip>
            </Box>
          </Grid>
          
          {/* 조류 근접 현황 */}
          <Grid item>
            <Box sx={{ position: 'relative' }}>
              <BirdProximityStatus 
                birdActivityData={birdActivityData}
                language={language}
              />
              <Tooltip title={translate('조류 접근 현황 새로고침', 'Refresh Bird Activity', language)}>
                <IconButton 
                  size="small" 
                  onClick={refreshBirdActivity}
                  disabled={birdActivityLoading}
                  sx={{ 
                    position: 'absolute', 
                    top: 6, 
                    right: 6,
                    color: 'rgba(255, 255, 255, 0.5)',
                    '&:hover': { color: 'rgba(255, 255, 255, 0.8)' },
                    opacity: 0.7
                  }}
                >
                  <Refresh fontSize="small" />
                </IconButton>
              </Tooltip>
            </Box>
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
          display: showRightPanel ? 'flex' : 'none',
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
          flexDirection: 'column',
          justifyContent: 'space-between'
        }}
      >
        <Box sx={{ display: 'flex', justifyContent: 'flex-end', mb: 1 }}>
          <Tooltip title={translate('통계 데이터 새로고침', 'Refresh Statistics', language)}>
            <IconButton 
              size="small" 
              onClick={() => {
                refreshDailyCameraStats();
                refreshSpeciesStats();
              }}
              sx={{ 
                color: 'rgba(255, 255, 255, 0.7)',
                '&:hover': { color: 'rgba(255, 255, 255, 0.9)' } 
              }}
            >
              <CachedOutlined fontSize="small" />
            </IconButton>
          </Tooltip>
        </Box>
        
        {/* 일일 누적 현황 위젯 */}
        <Paper elevation={0} sx={{...statsWidgetStyle, height: 'calc(33% - 20px)', position: 'relative'}}>
          <DailyStatsChart 
            data={[
              { label: '전체', value: dailyCameraStats.total, color: '#2196f3' },
              { label: '카메라 1', value: dailyCameraStats.camera1, color: '#3E6D9C' },
              { label: '카메라 2', value: dailyCameraStats.camera2, color: '#5E8C61' },
              { label: '카메라 3', value: dailyCameraStats.camera3, color: '#AF7AB3' }
            ]}
          />
          <Tooltip title={translate('일일 카메라별 누적 현황 새로고침', 'Refresh Daily Camera Stats', language)}>
            <IconButton 
              size="small" 
              onClick={refreshDailyCameraStats}
              disabled={dailyCameraStatsLoading}
              sx={{ 
                position: 'absolute', 
                top: 6, 
                right: 6,
                color: 'rgba(255, 255, 255, 0.5)',
                '&:hover': { color: 'rgba(255, 255, 255, 0.8)' },
                opacity: 0.7
              }}
            >
              <Refresh fontSize="small" />
            </IconButton>
          </Tooltip>
        </Paper>
        
        {/* 일일 종별 누적 현황 위젯 */}
        <Paper elevation={0} sx={{...statsWidgetStyle, height: 'calc(33% - 20px)', position: 'relative'}}>
          <SpeciesStatsChart 
            data={dailySpeciesStats}
          />
          <Tooltip title={translate('종별 누적 현황 새로고침', 'Refresh Species Stats', language)}>
            <IconButton 
              size="small" 
              onClick={refreshSpeciesStats}
              disabled={dailySpeciesLoading}
              sx={{ 
                position: 'absolute', 
                top: 6, 
                right: 6,
                color: 'rgba(255, 255, 255, 0.5)',
                '&:hover': { color: 'rgba(255, 255, 255, 0.8)' },
                opacity: 0.7
              }}
            >
              <Refresh fontSize="small" />
            </IconButton>
          </Tooltip>
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