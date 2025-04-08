import React, { useState, useEffect, useRef } from 'react';
import {
  Box,
  Typography,
  Paper,
  IconButton,
  Tooltip,
  Tabs,
  Tab,
  Grid,
  Button
} from '@mui/material';
import {
  Fullscreen as FullscreenIcon,
  CameraAlt as CameraIcon,
  VolumeUp as VolumeUpIcon,
  VolumeOff as VolumeOffIcon,
  MusicNote as MusicNoteIcon
} from '@mui/icons-material';
import { MapContainer, TileLayer, CircleMarker, Popup, Polygon } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';
import { translate } from '../../../utils/i18n';
import { radarEmulatorAPI, cameraAPI } from '../../../api';
import MapRadarOverlay from './MapRadarOverlay';
import CombinedRadarDisplay from '../../../components/RadarControl/CombinedRadarDisplay';
import CameraStream from '../../../components/CameraStream';
import axios from 'axios';
import { styled } from '@mui/material/styles';

// Leaflet 아이콘 마커 깨짐 방지
delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png',
  iconUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png',
  shadowUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png'
});

// PTZ 컨트롤 스타일 정의
const SidePanel = styled(Paper)(({ theme }) => ({
  backgroundColor: '#0f1620',
  border: '1px solid #1e3a5a',
  borderRadius: 4,
  height: '100%',
  overflowY: 'auto',
  boxShadow: '0 4px 12px rgba(0,0,0,0.3)'
}));

/**
 * 예시 함수: 카메라의 위치, 방향, 시야각으로 "시야 폴리곤" 좌표 생성
 * @param {[number, number]} center  [위도, 경도]
 * @param {number} direction  방향(도)
 * @param {number} angle      시야각(도)
 * @param {number} distance   시야 거리(미터)
 * @returns {Array<[number, number]>} Leaflet Polygon 좌표
 */
function createViewField(center, direction, angle, distance) {
  // 실제 GIS 계산 로직(여기서는 단순 예시)
  // direction(북: 0도 기준), angle(좌우각도), distance(시야 거리)
  // 필요하다면 대지구 계산법으로 수정
  const lat = center[0];
  const lng = center[1];
  const halfAngle = angle / 2;

  // 간단히 "direction ± halfAngle"로 두 점 찍는 예시
  const rad = (deg) => (deg * Math.PI) / 180;
  const earthRadius = 6378137; // 지구 반지름(m)

  const points = [];
  // 가운데 방향 - halfAngle
  for (let offset of [-halfAngle, halfAngle]) {
    const bearing = direction + offset;
    const bearingRad = rad(bearing);

    // 대충 distance만큼 이동한 위치(위도/경도) 계산(단순화)
    const dLat =
      (distance * Math.cos(bearingRad)) / earthRadius * (180 / Math.PI);
    const dLng =
      (distance * Math.sin(bearingRad)) /
      (earthRadius * Math.cos(rad(lat))) *
      (180 / Math.PI);

    const newLat = lat + dLat;
    const newLng = lng + dLng;
    points.push([newLat, newLng]);
  }

  // 폴리곤 형태로 만들기 위해 시작점(center) + 양 끝 점 + center
  return [center, points[0], points[1], center];
}

/**
 * MapView 컴포넌트
 * @param {*} props
 *  - mapCenter: 지도 초기 중심 좌표
 *  - mapRef: 지도 레퍼런스 (필요 시)
 *  - setShowCameraModal: 카메라 모달 표시/숨김 함수 (필요 시)
 *  - cameraStatus: 카메라별 상태 (오프라인/온라인 등)
 *  - language: 다국어 코드
 */
const MapView = ({
  mapCenter = [35.192962, 126.221627],
  mapRef = null,
  setShowCameraModal = () => {},
  cameraStatus = {
    1: { error: false },
    2: { error: false },
    3: { error: false }
  },
  language = 'ko'
}) => {
  // ------ 공통 상태들 ------
  // 내부 mapRef 생성 (외부에서 전달되지 않은 경우 사용)
  const internalMapRef = useRef(null);
  // 실제 사용할 ref (외부에서 전달된 것 또는 내부에서 생성한 것)
  const actualMapRef = mapRef || internalMapRef;
  
  // 탭 상태
  const [currentTab, setCurrentTab] = useState(0);
  // 레이더 오버레이 표시 여부
  const [showRadarOverlay, setShowRadarOverlay] = useState(true);
  // 카메라 오버레이 표시 여부
  const [showCameraOverlay, setShowCameraOverlay] = useState(true);
  // 컴포넌트 마운트 상태 추적
  const isMountedRef = useRef(true);

  // 마운트/언마운트 처리
  useEffect(() => {
    isMountedRef.current = true;
    
    return () => {
      isMountedRef.current = false;
    };
  }, []);

  // ------ 레이더 관련 상태 ------
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [isConnected, setIsConnected] = useState(false);
  const [operationMode, setOperationMode] = useState('');

  // ------ 카메라 관련 상태 ------
  const [cameras, setCameras] = useState([
    { id: '1', position: [35.192962, 126.221627], direction: 0, angle: 120, color: '#3E6D9C' },
    { id: '2', position: [35.192862, 126.221727], direction: 120, angle: 120, color: '#5E8C61' },
    { id: '3', position: [35.192862, 126.221527], direction: 240, angle: 120, color: '#AF7AB3' }
  ]);
  
  // ------ PTZ 제어용 상태 ------
  const [ptzCamera, setPtzCamera] = useState(null);

  // ------ 이벤트 핸들러: 탭 전환 ------
  const handleTabChange = (event, newValue) => {
    console.log(`탭 변경: ${currentTab} → ${newValue}`);
    
    // 이전 탭의 리소스 정리
    if (currentTab === 2 && newValue !== 2) {
      console.log('레이더 탭에서 다른 탭으로 전환: 데이터 정리');
      // 레이더 탭에서 나갈 때는 데이터 정리
      setPlotData([]);
      setTrackData([]);
    }
    
    // 탭 전환 시 MapContainer가 즉시 제거되고 재생성되도록 딜레이 추가
    setTimeout(() => {
      if (isMountedRef.current) {
        setCurrentTab(newValue);
      }
    }, 50);
  };

  // ------ 카메라 목록 가져오기(예시) ------
  // 실제로는 백엔드 API에서 카메라 정보를 가져와 기존 cameras 배열과 병합 가능
  useEffect(() => {
    const fetchCameras = async () => {
      try {
        // API 모듈 사용
        const response = await cameraAPI.getCameras();

        if (response.data && response.data.cameras && response.data.cameras.length > 0) {
          // 기존 cameras 와 merge (단순화 예시)
          const updatedCameras = cameras.map((cam) => {
            const found = response.data.cameras.find((bc) => bc.camera_id === cam.id);
            return found
              ? { ...cam, status: found.status }
              : cam;
          });
          setCameras(updatedCameras);
        }
      } catch (e) {
        console.error('카메라 목록 가져오기 에러:', e);
      }
    };
    fetchCameras();
  }, []);
  
  // PTZ 제어용 카메라 선택 핸들러
  const handlePtzCameraSelect = (cameraId) => {
    // 현재 선택된 카메라와 동일하면 선택 해제
    if (ptzCamera === cameraId) {
      setPtzCamera(null);
    } else {
      // 다른 카메라 선택
      setPtzCamera(cameraId);
    }
  };

  // PTZ 제어 함수
  const controlPtz = (direction, isContinuous = true) => {
    if (!ptzCamera) return;

    axios.post(`/api/ptz/control/`, {
      camera_id: ptzCamera,
      direction: direction,
      is_continuous: isContinuous,
      speed: 0.7
    })
    .then(response => {
      console.log('PTZ 제어 성공:', response.data);
    })
    .catch(error => {
      console.error('PTZ 제어 오류:', error);
      // 백업 중지 메소드 호출 - 에러가 발생해도 중지 시도
      if (!isContinuous) {
        axios.post(`/api/ptz/stop/`, {
          camera_id: ptzCamera,
          direction: direction
        }).catch(err => console.error('PTZ 중지 오류:', err));
      }
    });
  };

  // PTZ 중지 함수
  const stopPtz = (direction) => {
    if (!ptzCamera) return;
    controlPtz(direction, false);
  };

  // PTZ 제어 UI 렌더링 함수
  const renderPtzControls = () => {
    return (
      <Box sx={{ p: 1 }}>
        {ptzCamera ? (
          <>
            <Box sx={{ textAlign: 'center', mb: 1 }}>
              <Typography variant="body2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                {translate(`카메라 ${ptzCamera}`, `Camera ${ptzCamera}`, language)}
              </Typography>
            </Box>
            <Box sx={{ 
              display: 'flex', 
              flexDirection: 'column', 
              alignItems: 'center',
              justifyContent: 'center',
              backgroundColor: '#162b44',
              borderRadius: '8px',
              p: 1.5,
              height: '200px'
            }}>
              {/* PTZ 컨트롤 레이아웃 */}
              <Box sx={{ 
                position: 'relative',
                width: '100%',
                height: '100%',
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center',
                justifyContent: 'center'
              }}>
                {/* 상단: 방향 컨트롤 */}
                <Box sx={{ 
                  position: 'relative', 
                  width: '140px', 
                  height: '140px', 
                  display: 'flex',
                  justifyContent: 'center',
                  alignItems: 'center',
                  mb: 1
                }}>
                  {/* 중앙 원 */}
                  <Box sx={{ 
                    width: '50px', 
                    height: '50px', 
                    borderRadius: '50%', 
                    backgroundColor: '#0d1929',
                    position: 'absolute',
                    zIndex: 1
                  }}></Box>

                  {/* 상 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      top: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('UP')}
                    onMouseUp={() => stopPtz('UP')}
                    onMouseLeave={() => stopPtz('UP')}
                    onTouchStart={() => controlPtz('UP')}
                    onTouchEnd={() => stopPtz('UP')}
                  >
                    <Box component="span">▲</Box>
                  </IconButton>

                  {/* 하 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      bottom: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('DOWN')}
                    onMouseUp={() => stopPtz('DOWN')}
                    onMouseLeave={() => stopPtz('DOWN')}
                    onTouchStart={() => controlPtz('DOWN')}
                    onTouchEnd={() => stopPtz('DOWN')}
                  >
                    <Box component="span">▼</Box>
                  </IconButton>

                  {/* 좌 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      left: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('LEFT')}
                    onMouseUp={() => stopPtz('LEFT')}
                    onMouseLeave={() => stopPtz('LEFT')}
                    onTouchStart={() => controlPtz('LEFT')}
                    onTouchEnd={() => stopPtz('LEFT')}
                  >
                    <Box component="span">◀</Box>
                  </IconButton>

                  {/* 우 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      right: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('RIGHT')}
                    onMouseUp={() => stopPtz('RIGHT')}
                    onMouseLeave={() => stopPtz('RIGHT')}
                    onTouchStart={() => controlPtz('RIGHT')}
                    onTouchEnd={() => stopPtz('RIGHT')}
                  >
                    <Box component="span">▶</Box>
                  </IconButton>
                </Box>

                {/* 하단: 줌 컨트롤 (가로 배치) */}
                <Box sx={{ 
                  display: 'flex', 
                  flexDirection: 'row', 
                  justifyContent: 'center',
                  alignItems: 'center',
                  gap: 2
                }}>
                  <IconButton 
                    size="small"
                    sx={{ 
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      width: '32px',
                      height: '32px',
                      padding: '4px'
                    }}
                    onMouseDown={() => controlPtz('ZOOM_IN')}
                    onMouseUp={() => stopPtz('ZOOM_IN')}
                    onMouseLeave={() => stopPtz('ZOOM_IN')}
                    onTouchStart={() => controlPtz('ZOOM_IN')}
                    onTouchEnd={() => stopPtz('ZOOM_IN')}
                  >
                    <Box component="span">+</Box>
                  </IconButton>
                  <IconButton 
                    size="small"
                    sx={{ 
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      width: '32px',
                      height: '32px',
                      padding: '4px'
                    }}
                    onMouseDown={() => controlPtz('ZOOM_OUT')}
                    onMouseUp={() => stopPtz('ZOOM_OUT')}
                    onMouseLeave={() => stopPtz('ZOOM_OUT')}
                    onTouchStart={() => controlPtz('ZOOM_OUT')}
                    onTouchEnd={() => stopPtz('ZOOM_OUT')}
                  >
                    <Box component="span">-</Box>
                  </IconButton>
                </Box>
              </Box>
            </Box>
          </>
        ) : (
          <Box sx={{ 
            display: 'flex', 
            justifyContent: 'center', 
            alignItems: 'center', 
            height: '200px', 
            color: '#aaa' 
          }}>
            <Typography variant="body2">
              {translate('카메라를 선택하세요', 'Select a camera', language)}
            </Typography>
          </Box>
        )}
      </Box>
    );
  };

  // PTZ용 카메라 목록 컴포넌트
  const PtzCameraListItems = () => {
    return (
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 1, justifyContent: 'center' }}>
        {cameras.map((camera) => {
          const isSelected = ptzCamera === camera.id;
          
          return (
            <Button
              key={`ptz-${camera.id}`}
              variant={isSelected ? "contained" : "outlined"}
              size="small"
              onClick={() => handlePtzCameraSelect(camera.id)}
              sx={{
                minWidth: '40px',
                height: '32px',
                backgroundColor: isSelected ? '#1e88e5' : 'transparent',
                color: isSelected ? '#fff' : '#4caf50',
                borderColor: '#4caf50',
                '&:hover': {
                  backgroundColor: isSelected ? '#1976d2' : 'rgba(30, 136, 229, 0.08)'
                },
                fontSize: '0.8rem',
                fontWeight: isSelected ? 'bold' : 'normal'
              }}
            >
              {translate(`카메라 ${camera.id}`, `Camera ${camera.id}`, language)}
            </Button>
          );
        })}
      </Box>
    );
  };

  // ------ 레이더 데이터 폴링 ------
  useEffect(() => {
    let intervalId = null;
    let isMounted = true;

    const fetchRadarData = async () => {
      if (!isMounted) return;
      try {
        const response = await radarEmulatorAPI.getSimulationStatus();
        const data = response.data || {};

        // 연결 상태
        if (data.connected !== undefined) {
          setIsConnected(data.connected);
        }

        // 운영 모드
        if (data.operation_mode) {
          setOperationMode(data.operation_mode);
        }

        // 플롯 데이터
        if (data.plots && data.plots.length > 0) {
          setPlotData(data.plots);
        } else if (data.Plot && data.Plot.length > 0) {
          setPlotData(data.Plot);
        } else if (data.nPlot && data.Plot && data.Plot.length > 0) {
          setPlotData(data.Plot);
        } else {
          setPlotData([]);
        }

        // 트랙 데이터
        if (data.tracks && data.tracks.length > 0) {
          setTrackData(data.tracks);
        } else if (data.Track && data.Track.length > 0) {
          setTrackData(data.Track);
        } else if (data.nTrack && data.Track && data.Track.length > 0) {
          setTrackData(data.Track);
        } else {
          setTrackData([]);
        }
      } catch (err) {
        console.error('레이더 데이터 가져오기 오류:', err);
        setPlotData([]);
        setTrackData([]);
        setIsConnected(false);
      }
    };

    if (currentTab === 2) {
      // 레이더 모니터링 탭일 때만 폴링
      fetchRadarData();
      intervalId = setInterval(fetchRadarData, 1000);
    }

    return () => {
      isMounted = false;
      if (intervalId) {
        clearInterval(intervalId);
      }
    };
  }, [currentTab]);

  // ------ 스피커 제어 함수 ------
  const playSpeakerSound = (soundId) => {
    axios.post('/camera/speaker/play_sound/', { 
      sound_index: soundId,
      repeat: false
    })
      .then(response => {
        console.log(`스피커 사운드 ${soundId} 재생 성공:`, response.data);
      })
      .catch(error => {
        console.error(`스피커 사운드 ${soundId} 재생 오류:`, error);
      });
  };

  const stopSpeakerSound = () => {
    axios.post('/camera/speaker/stop_sound/')
      .then(response => {
        console.log('스피커 사운드 중지 성공:', response.data);
      })
      .catch(error => {
        console.error('스피커 사운드 중지 오류:', error);
      });
  };

  return (
    <Paper
      elevation={0}
      sx={{
        height: '100%',
        borderRadius: 2,
        backgroundColor: '#0a1929',
        border: '1px solid #1e3a5a',
        overflow: 'hidden',
        display: 'flex',
        flexDirection: 'column'
      }}
    >
      {/* 상단 탭 헤더 */}
      <Box
        sx={{
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
          borderBottom: '1px solid #1e3a5a'
        }}
      >
        <Tabs
          value={currentTab}
          onChange={handleTabChange}
          textColor="inherit"
          sx={{
            '& .MuiTabs-indicator': {
              backgroundColor: 'lightblue'
            },
            '& .MuiTab-root': {
              color: 'white',
              fontWeight: 'bold',
              fontSize: '0.9rem',
              '&.Mui-selected': { color: 'lightblue' }
            }
          }}
        >
          <Tab label={translate('통합 모니터링', 'Integrated Monitoring', language)} />
          <Tab label={translate('카메라 모니터링', 'Camera Monitoring', language)} />
          <Tab label={translate('레이더 모니터링', 'Radar Monitoring', language)} />
        </Tabs>

        {/* 컨트롤 버튼 영역 */}
        <Box sx={{ p: 1, display: 'flex' }}>
          {/* 레이더 오버레이 토글 버튼 */}
          <Tooltip title={translate('레이더 오버레이', 'Radar Overlay', language)}>
            <IconButton
              size="small"
              sx={{
                color: showRadarOverlay ? 'lightblue' : 'white',
                mr: 1
              }}
              onClick={() => setShowRadarOverlay((prev) => !prev)}
            >
              {/* SVG 아이콘 (Radar) */}
              <svg
                xmlns="http://www.w3.org/2000/svg"
                width="16"
                height="16"
                fill="currentColor"
                className="bi bi-radar"
                viewBox="0 0 16 16"
              >
                <path d="M8 1a7 7 0 0 1 7 7v.5A3.49 3.49 0 0 0 13.5 8a.5.5 0 0 1 0-1 4.49 4.49 0 0 1 3.5 4.5A3.5 3.5 0 0 1 13.5 15h-9a3.5 3.5 0 0 1-3.5-3.5A4.49 4.49 0 0 1 4.5 7a.5.5 0 0 1 0 1A3.49 3.49 0 0 0 3 9.5V8a7 7 0 0 1 7-7M8 0a8 8 0 0 0-8 8 4.5 4.5 0 0 0 1.7 3.5A4.49 4.49 0 0 0 0 15.5 4.5 4.5 0 0 0 4.5 20h7a4.5 4.5 0 0 0 4.5-4.5 4.49 4.49 0 0 0-1.7-3.5A4.5 4.5 0 0 0 16 8a8 8 0 0 0-8-8M7 5.5a.5.5 0 0 1 1 0v3a.5.5 0 0 1-1 0z" />
              </svg>
            </IconButton>
          </Tooltip>

          {/* 카메라 오버레이 토글 버튼 */}
          <Tooltip title={translate('카메라 오버레이', 'Camera Overlay', language)}>
            <IconButton
              size="small"
              sx={{
                color: showCameraOverlay ? 'lightblue' : 'white',
                mr: 1
              }}
              onClick={() => setShowCameraOverlay((prev) => !prev)}
            >
              <CameraIcon fontSize="small" />
            </IconButton>
          </Tooltip>

          {/* 스피커 사운드 0 시작 버튼 */}
          <Tooltip title={translate('사운드 0 시작', 'Play Sound 0', language)}>
            <IconButton
              size="small"
              sx={{
                color: 'white',
                backgroundColor: '#1e3a5a',
                mr: 1,
                '&:hover': { backgroundColor: '#2c4f7c' }
              }}
              onClick={() => playSpeakerSound(0)}
            >
              <Box display="flex" alignItems="center">
                <VolumeUpIcon fontSize="small" />
                <Typography variant="caption" sx={{ ml: 0.5 }}>0</Typography>
              </Box>
            </IconButton>
          </Tooltip>

          {/* 스피커 사운드 1 시작 버튼 */}
          <Tooltip title={translate('사운드 1 시작', 'Play Sound 1', language)}>
            <IconButton
              size="small"
              sx={{
                color: 'white',
                backgroundColor: '#1e3a5a',
                mr: 1,
                '&:hover': { backgroundColor: '#2c4f7c' }
              }}
              onClick={() => playSpeakerSound(1)}
            >
              <Box display="flex" alignItems="center">
                <MusicNoteIcon fontSize="small" />
                <Typography variant="caption" sx={{ ml: 0.5 }}>1</Typography>
              </Box>
            </IconButton>
          </Tooltip>

          {/* 스피커 사운드 중단 버튼 */}
          <Tooltip title={translate('사운드 중단', 'Stop Sound', language)}>
            <IconButton
              size="small"
              sx={{
                color: 'white',
                backgroundColor: '#831919', 
                '&:hover': { backgroundColor: '#a82626' }
              }}
              onClick={stopSpeakerSound}
            >
              <VolumeOffIcon fontSize="small" />
            </IconButton>
          </Tooltip>
        </Box>
      </Box>

      {/* 탭 컨텐츠 영역 - 공통 컨테이너 */}
      <Box sx={{ flex: 1, position: 'relative', overflow: 'hidden' }}>
        {/* -- 0. 통합 모니터링 탭 -- */}
        <Box
          sx={{
            position: 'absolute',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            display: currentTab === 0 ? 'block' : 'none',
            zIndex: currentTab === 0 ? 10 : 1
          }}
        >
          {/* 지도 전체 화면 + 카메라 오버레이 */}
          <Box sx={{ position: 'relative', width: '100%', height: '100%' }}>
            {/* 지도 영역 (전체 화면) */}
            <Box sx={{ width: '100%', height: '100%', position: 'relative' }}>
              {currentTab === 0 && (
                <MapContainer
                  center={mapCenter}
                  zoom={17}
                  style={{ width: '100%', height: '100%', borderRadius: '5px' }}
                  ref={actualMapRef}
                  key={`map-container-${currentTab}`}
                >
                  {/* 위성 타일 레이어 */}
                  <TileLayer
                    url="https://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}"
                    subdomains={['mt0', 'mt1', 'mt2', 'mt3']}
                    attribution="&copy; Google Maps"
                    maxZoom={20}
                  />

                  {/* 레이더 오버레이 */}
                  {showRadarOverlay && <MapRadarOverlay language={language} />}

                  {/* 카메라 마커 및 시야각 표시 */}
                  {cameras.map((camera) => (
                    <React.Fragment key={camera.id}>
                      <CircleMarker
                        center={camera.position}
                        radius={10}
                        pathOptions={{
                          fillColor: camera.color,
                          fillOpacity: 1,
                          color: '#ffffff',
                          weight: 2.5
                        }}
                      >
                        <Popup>
                          {translate(`카메라${camera.id}`, `Camera ${camera.id}`, language)}
                        </Popup>
                      </CircleMarker>

                      <Polygon
                        positions={createViewField(
                          camera.position,
                          camera.direction,
                          camera.angle,
                          300
                        )}
                        pathOptions={{
                          color: `${camera.color}B3`, // 대략 70% 투명도
                          fillColor: `${camera.color}33`, // 대략 20% 투명도
                          fillOpacity: 0.7,
                          weight: 2,
                          smoothFactor: 1
                        }}
                      />
                    </React.Fragment>
                  ))}
                </MapContainer>
              )}
            </Box>

            {/* 카메라 스트림 오버레이 (지도 위에 표시) */}
            {showCameraOverlay && (
              <Box
                sx={{
                  position: 'absolute',
                  top: 16,
                  right: 16,
                  width: '16%', // 25%에서 18%로 너비 감소
                  zIndex: 1000,
                  display: 'flex',
                  flexDirection: 'column',
                  gap: 1 // 카메라 간격도 줄임
                }}
              >
                {cameras.slice(0, 3).map((camera) => (
                  <Paper
                    key={camera.id}
                    elevation={3}
                    sx={{
                      borderRadius: 2,
                      overflow: 'hidden',
                      border: `2px solid ${camera.color}`,
                      aspectRatio: '16/9', // 16:9 비율 설정
                      width: '100%'
                    }}
                  >
                    <CameraStream
                      cameraId={camera.id}
                      title={translate(`카메라 ${camera.id}`, `Camera ${camera.id}`, language)}
                      height="100%"
                      language={language}
                      hideRefreshButton={true}
                      hideControls={true}
                    />
                  </Paper>
                ))}
              </Box>
            )}
          </Box>
        </Box>

        {/* -- 1. 카메라 모니터링 탭 -- */}
        <Box
          sx={{
            position: 'absolute',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            display: currentTab === 1 ? 'block' : 'none',
            zIndex: currentTab === 1 ? 10 : 1,
            p: 1
          }}
        >
          <Grid container spacing={1} sx={{ height: '100%' }}>
            {cameras.map((cam) => (
              <Grid key={cam.id} item xs={6} sx={{ height: '50%' }}>
                <CameraStream
                  cameraId={cam.id}
                  title={translate(`카메라 ${cam.id}`, `Camera ${cam.id}`, language)}
                  language={language}
                />
              </Grid>
            ))}

            {/* 카메라가 4개 미만이면 PTZ 컨트롤 패널 표시 */}
            {cameras.length < 4 && (
              <Grid item xs={6} sx={{ height: '50%' }}>
                <SidePanel sx={{ height: '100%' }}>
                  <Box sx={{ p: 2, backgroundColor: '#1e3a5a', display: 'flex', alignItems: 'center', borderTopLeftRadius: 4, borderTopRightRadius: 4 }}>
                    <IconButton size="small" sx={{ color: '#fff', p: 0.5, mr: 1 }}>
                      <CameraIcon fontSize="small" />
                    </IconButton>
                    <Typography variant="subtitle2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                      {translate('PTZ 제어', 'PTZ Control', language)}
                    </Typography>
                  </Box>
                  
                  <Box sx={{ p: 2, borderBottom: '1px solid rgba(255, 255, 255, 0.1)' }}>
                    <Typography variant="body2" sx={{ color: '#aaa', mb: 1, textAlign: 'center' }}>
                      {translate('카메라 선택', 'Select Camera', language)}
                    </Typography>
                    <PtzCameraListItems />
                  </Box>
                  
                  {renderPtzControls()}
                </SidePanel>
              </Grid>
            )}
          </Grid>
        </Box>

        {/* -- 2. 레이더 모니터링 탭 -- */}
        <Box
          sx={{
            position: 'absolute',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            display: currentTab === 2 ? 'block' : 'none',
            zIndex: currentTab === 2 ? 10 : 1,
            backgroundColor: '#0a1929',
            p: 2
          }}
        >
          <Typography variant="h5" color="white" mb={2}>
            {translate('레이더 모니터링', 'Radar Monitoring', language)}{' '}
            <span
              style={{
                color: isConnected ? 'lightgreen' : 'orange',
                fontSize: '0.8em',
                marginLeft: '10px'
              }}
            >
              {isConnected
                ? `(${operationMode || '연결됨'})`
                : '(연결 안됨)'}
            </span>
          </Typography>

          <Grid container spacing={2}>
            {/* 레이더 디스플레이(예시) */}
            <Grid item xs={12} md={9}>
              <Paper
                elevation={0}
                sx={{
                  p: 2,
                  borderRadius: 2,
                  backgroundColor: '#0a1929',
                  border: '1px solid #1e3a5a',
                  height: '100%',
                  display: 'flex',
                  flexDirection: 'column'
                }}
              >
                <Typography variant="h6" mb={2} color="#fff">
                  {translate('통합 레이더 데이터', 'Combined Radar Data', language)}
                  <span
                    style={{ color: '#4fc3f7', marginLeft: '10px', fontSize: '0.8em' }}
                  >
                    플롯: {plotData.length}개
                  </span>
                  <span
                    style={{ color: '#ff9800', marginLeft: '10px', fontSize: '0.8em' }}
                  >
                    트랙: {trackData.length}개
                  </span>
                </Typography>

                <Box
                  sx={{
                    flex: 1,
                    width: '100%',
                    position: 'relative',
                    overflow: 'hidden'
                  }}
                >
                  <CombinedRadarDisplay
                    plotData={plotData}
                    trackData={trackData}
                    language={language}
                    hideAlertSettings
                    key={`radar-display-${currentTab === 2 ? 'active' : 'inactive'}`}
                  />
                </Box>
              </Paper>
            </Grid>

            {/* 레이더 상태 요약(예시) */}
            <Grid item xs={12} md={3}>
              <Paper
                elevation={0}
                sx={{
                  p: 1.5,
                  borderRadius: 2,
                  backgroundColor: '#0a1929',
                  border: '1px solid #1e3a5a',
                  height: '100%'
                }}
              >
                <Typography variant="h6" mb={1.5} fontSize="1rem" color="#fff">
                  {translate('레이더 상태', 'Radar Status', language)}
                </Typography>
                <Box sx={{ display: 'flex', flexDirection: 'column', gap: 1.5 }}>
                  <Box
                    sx={{
                      p: 1,
                      backgroundColor: 'rgba(30, 136, 229, 0.1)',
                      borderRadius: 1,
                      border: '1px solid rgba(30, 136, 229, 0.2)'
                    }}
                  >
                    <Typography
                      variant="caption"
                      sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}
                    >
                      {translate('연결 상태', 'Connection Status', language)}
                    </Typography>
                    <Typography variant="body1" sx={{ color: '#fff', fontSize: '0.9rem' }}>
                      {isConnected
                        ? translate('연결됨', 'Connected', language)
                        : translate('연결 안됨', 'Disconnected', language)}
                    </Typography>
                  </Box>

                  <Box
                    sx={{
                      p: 1,
                      backgroundColor: 'rgba(255, 152, 0, 0.1)',
                      borderRadius: 1,
                      border: '1px solid rgba(255, 152, 0, 0.2)'
                    }}
                  >
                    <Typography
                      variant="caption"
                      sx={{ color: '#ffb74d', display: 'block', mb: 0.5 }}
                    >
                      {translate('운용 모드', 'Operation Mode', language)}
                    </Typography>
                    <Typography variant="body1" sx={{ color: '#fff', fontSize: '0.9rem' }}>
                      {operationMode || translate('알 수 없음', 'Unknown', language)}
                    </Typography>
                  </Box>
                </Box>
              </Paper>
            </Grid>
          </Grid>
        </Box>
      </Box>
    </Paper>
  );
};

export default MapView;
