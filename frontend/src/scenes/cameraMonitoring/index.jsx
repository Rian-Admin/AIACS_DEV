import React, { useState, useEffect, useRef } from 'react';
import { 
  Box, 
  Typography, 
  Paper,
  Grid,
  List,
  ListItem,
  ListItemIcon,
  ListItemText,
  Divider,
  IconButton,
  Button,
  CircularProgress
} from '@mui/material';
import { 
  Videocam as VideocamIcon,
  Close as CloseIcon, 
  Fullscreen as FullscreenIcon,
  Camera as CameraIcon,
  ViewComfy as ViewComfyIcon,
  Refresh as RefreshIcon,
  PlayArrow as PlayArrowIcon
} from '@mui/icons-material';
import { styled } from '@mui/material/styles';
import { translate } from '../../utils/i18n';
import { cameraAPI } from '../../api';
import axios from 'axios';
import CameraStream from '../../components/CameraStream/CameraStream';

// Improved styling for better proportions and design
const CameraFeed = styled(Box)(({ theme }) => ({
  position: 'relative',
  height: '100%',
  backgroundColor: '#111',
  borderRadius: 4,
  overflow: 'hidden',
  border: '1px solid #333',
  boxShadow: '0 4px 8px rgba(0,0,0,0.3)'
}));

const CameraHeader = styled(Box)(({ theme }) => ({
  position: 'absolute',
  top: 0,
  left: 0,
  right: 0,
  padding: '8px 12px',
  backgroundColor: 'rgba(0, 0, 0, 0.7)',
  color: '#fff',
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  zIndex: 10,
  backdropFilter: 'blur(4px)'
}));

const CameraFooter = styled(Box)(({ theme }) => ({
  position: 'absolute',
  bottom: 0,
  left: 0,
  right: 0,
  padding: '8px 12px',
  backgroundColor: 'rgba(0, 0, 0, 0.7)',
  color: '#fff',
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  backdropFilter: 'blur(4px)'
}));

const LiveBadge = styled(Box)(({ theme }) => ({
  backgroundColor: '#f44336',
  color: '#fff',
  padding: '2px 6px',
  borderRadius: 4,
  fontSize: '0.7rem',
  fontWeight: 'bold',
  display: 'flex',
  alignItems: 'center',
  marginRight: 8,
  boxShadow: '0 2px 4px rgba(0,0,0,0.3)'
}));

const SidePanel = styled(Paper)(({ theme }) => ({
  backgroundColor: '#0f1620',
  border: '1px solid #1e3a5a',
  borderRadius: 4,
  height: '100%',
  overflowY: 'auto',
  boxShadow: '0 4px 12px rgba(0,0,0,0.3)'
}));

const CameraListItem = styled(ListItem, {
  shouldForwardProp: (prop) => prop !== 'active'
})(({ theme, active }) => ({
  cursor: 'pointer',
  backgroundColor: active ? 'rgba(30, 136, 229, 0.2)' : 'transparent',
  padding: '8px 12px',
  borderRadius: 4,
  margin: '4px 0',
  '&:hover': {
    backgroundColor: 'rgba(30, 136, 229, 0.1)'
  }
}));

const ControlBar = styled(Box)(({ theme }) => ({
  backgroundColor: '#0a1929',
  borderTop: '1px solid rgba(255, 255, 255, 0.1)',
  padding: '12px 16px',
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  boxShadow: '0 -2px 8px rgba(0,0,0,0.2)'
}));

const TimelineContainer = styled(Box)(({ theme }) => ({
  backgroundColor: '#152238',
  height: '36px',
  flex: 1,
  borderRadius: 18,
  position: 'relative',
  margin: '0 20px',
  border: '1px solid #1e3a5a',
  overflow: 'hidden',
  boxShadow: 'inset 0 2px 4px rgba(0,0,0,0.3)'
}));

const TimeSegment = styled(Box, {
  shouldForwardProp: (prop) => prop !== 'active'
})(({ theme, active }) => ({
  position: 'absolute',
  height: '100%',
  backgroundColor: active ? 'rgba(33, 150, 243, 0.3)' : 'transparent',
  borderRight: '1px solid rgba(255, 255, 255, 0.1)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  color: active ? '#fff' : '#aaa',
  fontSize: '0.7rem',
  fontWeight: active ? 'bold' : 'normal'
}));

const CameraMonitoring = ({ language = 'ko' }) => {
  // 카메라 목록 상태 추가
  const [cameras, setCameras] = useState([]);
  // 초기 활성화 카메라 설정
  const [activeCameras, setActiveCameras] = useState([]);
  const [currentTime, setCurrentTime] = useState(new Date());
  const [layout, setLayout] = useState('2x2'); // '2x2', '1+3', '1x1'
  const [cameraStatus, setCameraStatus] = useState({});
  const [loading, setLoading] = useState({});
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState(null);
  // 전체화면 모달 상태
  const [fullscreenCamera, setFullscreenCamera] = useState(null);
  // PTZ 제어용 선택된 카메라 상태 추가
  const [selectedCamera, setSelectedCamera] = useState(null);
  const iframeRefs = useRef({});

  // 백엔드에서 카메라 목록 가져오기
  useEffect(() => {
    const fetchCameras = async () => {
      try {
        const response = await cameraAPI.getCameras();
        if (response.data && response.data.cameras) {
          const formattedCameras = response.data.cameras.map(camera => ({
            id: camera.camera_id,
            label: camera.camera_id,
            type: 'dome',
            rtspAddress: camera.rtsp_address,
            status: camera.status
          }));
          
          setCameras(formattedCameras);
          
          if (formattedCameras.length > 0) {
            setActiveCameras(formattedCameras.slice(0, 4).map(c => c.id));
            
            const initialLoading = {};
            formattedCameras.forEach(camera => {
              initialLoading[camera.id] = true;
            });
            setLoading(initialLoading);
          }
        }
      } catch (error) {
        console.error('재시도 중 오류 발생:', error);
        setError('백엔드 서버에 연결할 수 없습니다. 서버가 실행 중인지 확인하세요.');
      } finally {
        setIsLoading(false);
      }
    };
    
    fetchCameras();
  }, []);

  // Update current time
  useEffect(() => {
    const timer = setInterval(() => {
      setCurrentTime(new Date());
    }, 1000);
    return () => clearInterval(timer);
  }, []);

  // iframe 참조 설정
  const setIframeRef = (cameraId, element) => {
    iframeRefs.current[cameraId] = element;
  };

  // iframe 로드 완료 처리
  const handleIframeLoad = (cameraId) => {
    console.log(`Camera ${cameraId} stream loaded`);
    setCameraStatus(prev => ({
      ...prev,
      [cameraId]: { online: true, error: false }
    }));
    setLoading(prev => ({
      ...prev,
      [cameraId]: false
    }));
  };

  // iframe 로드 오류 처리
  const handleIframeError = (cameraId) => {
    // 로딩 상태 해제
    setLoading(prev => ({
      ...prev,
      [cameraId]: false
    }));
    
    // 에러 상태 설정
    setCameraStatus(prev => ({
      ...prev,
      [cameraId]: { 
        online: false, 
        error: true,
        timestamp: Date.now()  // 새 타임스탬프로 업데이트
      }
    }));
    
    // 오류 발생 시 5초 후 자동으로 다시 시도 (3초에서 5초로 변경)
    // 단, 활성 카메라 목록에 있는 경우에만
    setTimeout(() => {
      if (activeCameras.includes(cameraId)) {
        refreshCameraStream(cameraId);
      }
    }, 5000);
  };

  // 카메라 스트림 새로고침
  const refreshCameraStream = (cameraId) => {
    // 새로고침 중임을 표시
    setLoading(prev => ({
      ...prev,
      [cameraId]: true
    }));
    
    // 새 타임스탬프를 통한 캐시 방지
    const timestamp = Date.now();
    
    setCameraStatus(prev => ({
      ...prev,
      [cameraId]: { 
        ...prev[cameraId],
        timestamp: timestamp,
        error: false,
        retryCount: 0 // 재시도 카운터 초기화
      }
    }));
    
    // 2초 후 로딩 상태 자동 해제 (로딩 애니메이션이 계속 표시되는 문제 방지)
    setTimeout(() => {
      setLoading(prev => ({
        ...prev,
        [cameraId]: false
      }));
    }, 2000);
  };

  // useEffect를 사용하여 주기적으로 모든 활성 카메라 새로고침
  useEffect(() => {
    // 1분마다 모든 활성 카메라 스트림 새로고침 (30초에서 1분으로 변경)
    const refreshInterval = setInterval(() => {
      if (activeCameras.length > 0) {
        // 모든 카메라를 동시에 새로고침하지 않고 순차적으로 처리
        activeCameras.forEach((cameraId, index) => {
          setTimeout(() => {
            refreshCameraStream(cameraId);
          }, index * 500); // 각 카메라마다 500ms 간격으로 새로고침
        });
      }
    }, 60000); // 1분으로 변경

    return () => clearInterval(refreshInterval);
  }, [activeCameras]); // activeCameras가 변경될 때마다 인터벌 재설정

  // Time formatting function
  const formatTime = (date) => {
    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');
    return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
  };

  // Camera selection handler
  const handleCameraSelect = (cameraId) => {
    // 현재 활성화된 카메라 목록 복사
    let newActiveCameras = [...activeCameras];
    
    if (activeCameras.includes(cameraId)) {
      // 이미 선택된 카메라라면 제거
      newActiveCameras = newActiveCameras.filter(id => id !== cameraId);
    } else if (activeCameras.length < 4) {
      // 4개 미만이면 추가
      newActiveCameras.push(cameraId);
    } else {
      // 이미 4개가 있으면 가장 오래된 것 제거하고 새로 추가
      newActiveCameras.shift();
      newActiveCameras.push(cameraId);
    }
    
    // 새 활성화 카메라 설정
    setActiveCameras(newActiveCameras);
  };

  const handleLayoutChange = (newLayout) => {
    // 이전 레이아웃 저장
    const prevLayout = layout;
    
    // 새 레이아웃 설정
    setLayout(newLayout);
    
    // 활성 카메라 확인 및 조정
    if (activeCameras.length === 0 && cameras.length > 0) {
      setActiveCameras([cameras[0].id]);
    }
    
    // 1+3 레이아웃으로 변경하면서 카메라가 1개 미만인 경우, 자동으로 최대 4개까지 채움
    if (newLayout === '1+3' && cameras.length > 0) {
      let newActive = [...activeCameras];
      
      // 활성 카메라가 없으면 첫 번째 카메라를 메인으로 추가
      if (newActive.length === 0) {
        newActive.push(cameras[0].id);
      }
      
      // 나머지 카메라 슬롯 채우기 (총 4개까지)
      for (let i = 0; i < cameras.length && newActive.length < 4; i++) {
        if (!newActive.includes(cameras[i].id)) {
          newActive.push(cameras[i].id);
        }
      }
      
      if (JSON.stringify(newActive) !== JSON.stringify(activeCameras)) {
        setActiveCameras(newActive);
      }
    }
    
    // 레이아웃이 실제로 변경된 경우에만 새로고침 수행 (500ms 지연 후)
    if (newLayout !== prevLayout) {
      setTimeout(() => {
        // 선택된 카메라만 새로고침 (전체가 아님)
        const mainCamera = activeCameras[0];
        if (mainCamera) {
          refreshCameraStream(mainCamera);
        }
      }, 500);
    }
  };

  // Timeline segments generation
  const renderTimeSegments = () => {
    const segments = [];
    const now = new Date();
    const hour = now.getHours();

    for (let i = 0; i < 12; i++) {
      const h = (hour - 5 + i + 24) % 24;
      const width = 100 / 12;
      segments.push(
        <TimeSegment
          key={i}
          active={i === 5}
          sx={{
            left: `${i * width}%`,
            width: `${width}%`,
          }}
        >
          {String(h).padStart(2, '0')}:00
        </TimeSegment>
      );
    }
    
    return segments;
  };

  // Render camera grid based on selected layout
  const renderCameraGrid = () => {
    // 모든 레이아웃에서 항상 한 대의 카메라만 표시하도록 함
    if (activeCameras.length > 0) {
      const cameraId = activeCameras[0];
      const camera = cameras.find(c => c.id === cameraId);
      
      return (
        <Box sx={{ height: 'calc(100vh - 180px)', maxHeight: 'calc(100vh - 180px)', p: 1, margin: '0 auto', overflow: 'hidden' }}>
          {camera ? renderCameraBox(camera, 0, 12) : renderEmptyBox()}
        </Box>
      );
    } else {
      // 활성 카메라가 없는 경우 메시지 표시
      return (
        <Box sx={{ 
          height: 'calc(100vh - 180px)', 
          display: 'flex', 
          alignItems: 'center', 
          justifyContent: 'center',
          color: '#aaa'
        }}>
          <Typography variant="h6">
            {translate('카메라를 선택하세요', 'Select a camera', language)}
          </Typography>
        </Box>
      );
    }
  };
  
  // Render empty camera box
  const renderEmptyBox = () => (
    <Paper 
      sx={{ 
        height: '100%', 
        display: 'flex', 
        alignItems: 'center', 
        justifyContent: 'center',
        backgroundColor: '#1a2035',
        color: '#5e6a81',
        borderRadius: 4,
        boxShadow: 'inset 0 2px 8px rgba(0,0,0,0.2)'
      }}
    >
      <Typography variant="body2">
        {translate('카메라를 선택하세요', 'Select a camera', language)}
      </Typography>
    </Paper>
  );
  
  // 전체화면 모달 토글
  const toggleFullscreen = (camera) => {
    if (fullscreenCamera && fullscreenCamera.id === camera.id) {
      setFullscreenCamera(null);
    } else {
      setFullscreenCamera(camera);
    }
  };

  // Render camera box
  const renderCameraBox = (camera, index, gridSize) => {
    if (!camera) return renderEmptyBox();
    
    const cameraId = camera.id;
    const hasError = cameraStatus[cameraId]?.error;
    const timestamp = cameraStatus[cameraId]?.timestamp || Date.now();
    const streamUrl = `/camera/${cameraId}/`;

    // 새 창에서 스트림 열기
    const openInNewWindow = () => {
      // 절대 URL 생성 (현재 도메인 기준)
      const absoluteUrl = `${window.location.origin}${streamUrl}`;
      window.open(absoluteUrl, `camera_${cameraId}`, 'width=1920,height=1080');
    };
    
    return (
      <Box sx={{ height: '100%', width: '100%', display: 'flex', flexDirection: 'column', overflow: 'hidden' }}>
        <CameraFeed>
          <CameraHeader>
            <Box sx={{ display: 'flex', alignItems: 'center' }}>
              <LiveBadge sx={{ backgroundColor: hasError ? '#777' : '#f44336' }}>
                {hasError ? 'ERROR' : 'LIVE'}
              </LiveBadge>
              <Typography variant="caption" sx={{ fontSize: '0.8rem', fontWeight: 'medium' }}>
                {camera.label}
              </Typography>
            </Box>
            <Box>
              <IconButton 
                size="small" 
                sx={{ color: '#fff', p: 1 }}
                onClick={() => {
                  refreshCameraStream(cameraId);
                }}
                aria-label={`${translate('새로고침', 'Refresh', language)} ${camera.label}`}
              >
                <RefreshIcon fontSize="small" />
              </IconButton>
              <IconButton 
                size="small" 
                sx={{ color: '#fff', p: 1 }}
                onClick={() => {
                  const newActiveCameras = activeCameras.filter(id => id !== camera.id);
                  setActiveCameras(newActiveCameras);
                }}
                aria-label={`${translate('닫기', 'Close', language)} ${camera.label}`}
              >
                <CloseIcon fontSize="small" />
              </IconButton>
            </Box>
          </CameraHeader>
          
          {/* CameraStream 컴포넌트 사용 */}
          <Box sx={{ width: '100%', height: '100%', position: 'relative' }}>
            <CameraStream 
              cameraId={cameraId}
              title={camera.label}
              height="100%"
              language={language}
            />
            
            {hasError && (
              <Box
                sx={{
                  position: 'absolute',
                  bottom: 8,
                  right: 8,
                  backgroundColor: 'rgba(255, 0, 0, 0.7)',
                  color: 'white',
                  padding: '4px 8px',
                  borderRadius: 4,
                  fontSize: '0.7rem',
                  zIndex: 5
                }}
              >
                {translate('연결 오류', 'Connection Error', language)}
              </Box>
            )}
          </Box>
          
          <CameraFooter>
            <Typography variant="caption" sx={{ fontSize: '0.8rem' }}>
              {formatTime(currentTime)}
            </Typography>
            <Box>
              <IconButton 
                size="small" 
                sx={{ color: '#fff', p: 1 }}
                onClick={openInNewWindow}
                aria-label={`${translate('새 창에서 열기', 'Open in new window', language)} ${camera.label}`}
              >
                <FullscreenIcon fontSize="small" />
              </IconButton>
            </Box>
          </CameraFooter>
        </CameraFeed>
      </Box>
    );
  };

  // 모든 카메라 새로고침
  const refreshAllCameras = () => {
    activeCameras.forEach(cameraId => {
      refreshCameraStream(cameraId);
    });
  };

  // 카메라 목록 아이템 수정
  const CameraListItems = () => {
    return cameras.map((camera) => {
      const isActive = activeCameras.includes(camera.id);
      const hasError = cameraStatus[camera.id]?.error;
      const isInactive = camera.status === 'inactive';
      
      return (
        <CameraListItem 
          key={camera.id}
          active={isActive} 
          onClick={() => handleCameraSelect(camera.id)}
          {...{
            component: 'div',  // li 대신 div 사용
            disableGutters: true,
            dense: true
          }}
        >
          <ListItemIcon sx={{ minWidth: 36 }}>
            <VideocamIcon 
              sx={{ 
                color: hasError ? '#f44336' : isInactive ? '#999' : '#4caf50', 
                fontSize: '1.25rem',
                filter: !hasError && !isInactive ? 'drop-shadow(0 0 2px rgba(76, 175, 80, 0.5))' : 'none'
              }} 
            />
          </ListItemIcon>
          <ListItemText 
            primary={camera.label} 
            sx={{ 
              color: '#fff', 
              '& .MuiListItemText-primary': { 
                fontSize: '0.9rem',
                fontWeight: isActive ? 'bold' : 'normal'
              }
            }} 
          />
        </CameraListItem>
      );
    });
  };

  // Camera selection handler for PTZ control
  const handlePtzCameraSelect = (cameraId) => {
    // 현재 선택된 카메라와 동일하면 선택 해제
    if (selectedCamera === cameraId) {
      setSelectedCamera(null);
    } else {
      // 다른 카메라 선택
      setSelectedCamera(cameraId);
    }
  };

  // PTZ 제어 함수
  const controlPtz = (direction, isContinuous = true) => {
    if (!selectedCamera) return;

    cameraAPI.controlPTZ(selectedCamera, direction, isContinuous, 0.7)
      .then(response => {
        console.log('PTZ 제어 성공:', response.data);
      })
      .catch(error => {
        console.error('PTZ 제어 오류:', error);
        // 백업 중지 메소드 호출 - 에러가 발생해도 중지 시도
        if (!isContinuous) {
          cameraAPI.stopPTZ(selectedCamera, direction)
            .catch(err => console.error('PTZ 중지 오류:', err));
        }
      });
  };

  // PTZ 중지 함수
  const stopPtz = (direction) => {
    if (!selectedCamera) return;
    controlPtz(direction, false);
  };

  // PTZ 제어 UI 렌더링 함수
  const renderPtzControls = () => {
    return (
      <Box sx={{ p: 2 }}>
        {selectedCamera ? (
          <>
            <Box sx={{ textAlign: 'center', mb: 2 }}>
              <Typography variant="body2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                {translate('카메라', 'Camera', language)}: {selectedCamera}
              </Typography>
            </Box>
            <Box sx={{ 
              display: 'flex', 
              flexDirection: 'column', 
              alignItems: 'center',
              backgroundColor: '#162b44',
              borderRadius: '8px',
              p: 2
            }}>
              {/* 상/하/좌/우 버튼 */}
              <Box sx={{ display: 'flex', justifyContent: 'center', mb: 1 }}>
                <IconButton 
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
                  }}
                  onMouseDown={() => controlPtz('UP')}
                  onMouseUp={() => stopPtz('UP')}
                  onMouseLeave={() => stopPtz('UP')}
                  onTouchStart={() => controlPtz('UP')}
                  onTouchEnd={() => stopPtz('UP')}
                >
                  <Box component="span">▲</Box>
                </IconButton>
              </Box>
              <Box sx={{ display: 'flex', justifyContent: 'space-between', width: '100%', mb: 1 }}>
                <IconButton 
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
                  }}
                  onMouseDown={() => controlPtz('LEFT')}
                  onMouseUp={() => stopPtz('LEFT')}
                  onMouseLeave={() => stopPtz('LEFT')}
                  onTouchStart={() => controlPtz('LEFT')}
                  onTouchEnd={() => stopPtz('LEFT')}
                >
                  <Box component="span">◀</Box>
                </IconButton>

                <Box sx={{ 
                  width: '30px', 
                  height: '30px', 
                  borderRadius: '50%', 
                  backgroundColor: '#0d1929',
                  display: 'flex',
                  justifyContent: 'center',
                  alignItems: 'center'
                }}></Box>

                <IconButton 
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
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
              <Box sx={{ display: 'flex', justifyContent: 'center', mb: 3 }}>
                <IconButton 
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
                  }}
                  onMouseDown={() => controlPtz('DOWN')}
                  onMouseUp={() => stopPtz('DOWN')}
                  onMouseLeave={() => stopPtz('DOWN')}
                  onTouchStart={() => controlPtz('DOWN')}
                  onTouchEnd={() => stopPtz('DOWN')}
                >
                  <Box component="span">▼</Box>
                </IconButton>
              </Box>

              {/* 줌 버튼 */}
              <Box sx={{ display: 'flex', justifyContent: 'space-around', width: '70%' }}>
                <IconButton 
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
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
                  sx={{ 
                    color: '#fff', 
                    backgroundColor: '#1e3a5a', 
                    '&:hover': { backgroundColor: '#2c4f7c' }
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
            <Box sx={{ mt: 2, textAlign: 'center' }}>
              <Typography variant="body2" sx={{ color: '#1e88e5' }}>
                {translate('카메라', 'Camera', language)}: {selectedCamera}
              </Typography>
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

  // PTZ용 카메라 목록 아이템
  const PtzCameraListItems = () => {
    return (
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 1, justifyContent: 'center' }}>
        {cameras.map((camera) => {
          const isSelected = selectedCamera === camera.id;
          const hasError = cameraStatus[camera.id]?.error;
          const isInactive = camera.status === 'inactive';
          
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
                color: isSelected ? '#fff' : hasError ? '#f44336' : isInactive ? '#999' : '#4caf50',
                borderColor: hasError ? '#f44336' : isInactive ? '#555' : '#4caf50',
                '&:hover': {
                  backgroundColor: isSelected ? '#1976d2' : 'rgba(30, 136, 229, 0.08)'
                },
                fontSize: '0.8rem',
                fontWeight: isSelected ? 'bold' : 'normal'
              }}
            >
              {camera.label}
            </Button>
          );
        })}
      </Box>
    );
  };

  return (
    <Box sx={{ display: 'flex', flexDirection: 'column', height: '100vh', maxHeight: '100vh', backgroundColor: '#0A1929', overflow: 'hidden' }}>
      {/* Main Content */}
      <Box sx={{ display: 'flex', flexGrow: 1, p: 2, overflow: 'hidden' }}>
        {/* Left Sidebar - Camera List and PTZ Control */}
        <Box sx={{ width: 240, mr: 2, minWidth: 240, display: 'flex', flexDirection: 'column' }}>
          {/* Camera List Panel */}
          <SidePanel sx={{ mb: 2, flex: 1 }}>
            <Box sx={{ p: 2, backgroundColor: '#1e3a5a', display: 'flex', alignItems: 'center', borderTopLeftRadius: 4, borderTopRightRadius: 4 }}>
              <IconButton size="small" sx={{ color: '#fff', p: 0.5, mr: 1 }}>
                <CameraIcon fontSize="small" />
              </IconButton>
              <Typography variant="subtitle2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                {translate('장치 목록', 'Device List', language)}
              </Typography>
            </Box>
            
            <Divider sx={{ borderColor: 'rgba(255, 255, 255, 0.1)', mx: 2 }} />
            
            <List sx={{ p: 2 }}>
              {isLoading ? (
                <Box sx={{ display: 'flex', justifyContent: 'center', p: 2 }}>
                  <CircularProgress size={30} sx={{ color: '#4caf50' }} />
                </Box>
              ) : error ? (
                <Typography variant="body2" sx={{ color: '#ff5252', p: 2, textAlign: 'center' }}>
                  {translate(error, error, language)}
                </Typography>
              ) : cameras.length > 0 ? (
                <CameraListItems />
              ) : (
                <Typography variant="body2" sx={{ color: '#aaa', p: 2, textAlign: 'center' }}>
                  {translate('카메라가 없습니다', 'No cameras available', language)}
                </Typography>
              )}
            </List>
          </SidePanel>
          
          {/* PTZ Control Panel */}
          <SidePanel sx={{ flex: 1 }}>
            <Box sx={{ p: 2, backgroundColor: '#1e3a5a', display: 'flex', alignItems: 'center', borderTopLeftRadius: 4, borderTopRightRadius: 4 }}>
              <IconButton size="small" sx={{ color: '#fff', p: 0.5, mr: 1 }}>
                <CameraIcon fontSize="small" />
              </IconButton>
              <Typography variant="subtitle2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                {translate('PTZ 제어', 'PTZ Control', language)}
              </Typography>
            </Box>
            
            <Divider sx={{ borderColor: 'rgba(255, 255, 255, 0.1)', mx: 2 }} />
            
            {/* PTZ 카메라 선택 목록 */}
            <Box sx={{ p: 2, borderBottom: '1px solid rgba(255, 255, 255, 0.1)' }}>
              <Typography variant="body2" sx={{ color: '#aaa', mb: 1, textAlign: 'center' }}>
                {translate('카메라 선택', 'Select Camera', language)}
              </Typography>
              {isLoading ? (
                <Box sx={{ display: 'flex', justifyContent: 'center', p: 2 }}>
                  <CircularProgress size={30} sx={{ color: '#4caf50' }} />
                </Box>
              ) : error ? (
                <Typography variant="body2" sx={{ color: '#ff5252', p: 2, textAlign: 'center' }}>
                  {translate(error, error, language)}
                </Typography>
              ) : cameras.length > 0 ? (
                <PtzCameraListItems />
              ) : (
                <Typography variant="body2" sx={{ color: '#aaa', p: 2, textAlign: 'center' }}>
                  {translate('카메라가 없습니다', 'No cameras available', language)}
                </Typography>
              )}
            </Box>
            
            {/* PTZ 컨트롤 버튼 */}
            {renderPtzControls()}
          </SidePanel>
        </Box>

        {/* Camera Grid */}
        <Box sx={{ flexGrow: 1, display: 'flex', flexDirection: 'column', height: '100%', overflow: 'hidden' }}>
          {/* 백엔드 연결 오류가 있는 경우 메시지 표시 */}
          {error ? (
            <Box sx={{ 
              display: 'flex', 
              flexDirection: 'column',
              alignItems: 'center', 
              justifyContent: 'center', 
              height: '100%',
              color: '#aaa'
            }}>
              <IconButton 
                size="large" 
                sx={{ 
                  color: '#ff5252', 
                  mb: 2, 
                  '&:hover': { 
                    backgroundColor: 'rgba(255, 82, 82, 0.1)' 
                  }
                }}
                onClick={() => {
                  setIsLoading(true);
                  setError(null);
                  const fetchCameras = async () => {
                    try {
                      const response = await cameraAPI.getCameras();
                      if (response.data && response.data.cameras) {
                        const formattedCameras = response.data.cameras.map(camera => ({
                          id: camera.camera_id,
                          label: camera.camera_id,
                          type: 'dome',
                          rtspAddress: camera.rtsp_address,
                          status: camera.status
                        }));
                        
                        setCameras(formattedCameras);
                        
                        if (formattedCameras.length > 0) {
                          setActiveCameras(formattedCameras.slice(0, 4).map(c => c.id));
                          
                          const initialLoading = {};
                          formattedCameras.forEach(camera => {
                            initialLoading[camera.id] = true;
                          });
                          setLoading(initialLoading);
                        }
                      }
                    } catch (error) {
                      console.error('재시도 중 오류 발생:', error);
                      setError('백엔드 서버에 연결할 수 없습니다. 서버가 실행 중인지 확인하세요.');
                    } finally {
                      setIsLoading(false);
                    }
                  };
                  
                  fetchCameras();
                }}
              >
                <RefreshIcon sx={{ fontSize: 60 }} />
              </IconButton>
              <Typography variant="h6" sx={{ color: '#ff5252' }}>
                {translate('서버 연결 오류', 'Server Connection Error', language)}
              </Typography>
              <Typography variant="body2" sx={{ color: '#ff7777', mt: 1, textAlign: 'center', maxWidth: '80%' }}>
                {translate(error, error, language)}
              </Typography>
              <Button 
                variant="outlined" 
                color="error" 
                sx={{ mt: 3 }}
                onClick={() => {
                  setIsLoading(true);
                  setError(null);
                  const fetchCameras = async () => {
                    try {
                      const response = await cameraAPI.getCameras();
                      if (response.data && response.data.cameras) {
                        const formattedCameras = response.data.cameras.map(camera => ({
                          id: camera.camera_id,
                          label: camera.camera_id,
                          type: 'dome',
                          rtspAddress: camera.rtsp_address,
                          status: camera.status
                        }));
                        
                        setCameras(formattedCameras);
                        
                        if (formattedCameras.length > 0) {
                          setActiveCameras(formattedCameras.slice(0, 4).map(c => c.id));
                          
                          const initialLoading = {};
                          formattedCameras.forEach(camera => {
                            initialLoading[camera.id] = true;
                          });
                          setLoading(initialLoading);
                        }
                      }
                    } catch (error) {
                      console.error('재시도 중 오류 발생:', error);
                      setError('백엔드 서버에 연결할 수 없습니다. 서버가 실행 중인지 확인하세요.');
                    } finally {
                      setIsLoading(false);
                    }
                  };
                  
                  fetchCameras();
                }}
              >
                {translate('다시 시도', 'Retry', language)}
              </Button>
            </Box>
          ) : !isLoading && cameras.length === 0 ? (
            <Box sx={{ 
              display: 'flex', 
              flexDirection: 'column',
              alignItems: 'center', 
              justifyContent: 'center', 
              height: '100%',
              color: '#aaa'
            }}>
              <CameraIcon sx={{ fontSize: 60, mb: 2, opacity: 0.5 }} />
              <Typography variant="h6" sx={{ color: '#aaa' }}>
                {translate('사용 가능한 카메라가 없습니다', 'No cameras available', language)}
              </Typography>
              <Typography variant="body2" sx={{ color: '#777', mt: 1 }}>
                {translate('백엔드에서 카메라를 추가하세요', 'Please add cameras in the backend', language)}
              </Typography>
            </Box>
          ) : (
            <>
              {/* Layout Control Buttons */}
              <Box sx={{ 
                display: 'flex', 
                justifyContent: 'flex-end', 
                mb: 2,
                backgroundColor: 'rgba(30, 58, 90, 0.3)',
                borderRadius: 2,
                p: 1
              }}>
                <Button 
                  size="small" 
                  startIcon={<ViewComfyIcon />}
                  onClick={() => handleLayoutChange('2x2')}
                  sx={{ 
                    color: layout === '2x2' ? '#2196f3' : '#fff',
                    backgroundColor: layout === '2x2' ? 'rgba(33, 150, 243, 0.1)' : 'transparent',
                    borderRadius: 1,
                    mr: 1,
                    '&:hover': {
                      backgroundColor: 'rgba(33, 150, 243, 0.2)'
                    }
                  }}
                >
                  2×2 Grid
                </Button>
                <Button 
                  size="small"
                  onClick={() => handleLayoutChange('1+3')}
                  sx={{ 
                    color: layout === '1+3' ? '#2196f3' : '#fff',
                    backgroundColor: layout === '1+3' ? 'rgba(33, 150, 243, 0.1)' : 'transparent',
                    borderRadius: 1,
                    mr: 1,
                    '&:hover': {
                      backgroundColor: 'rgba(33, 150, 243, 0.2)'
                    }
                  }}
                >
                  1+3 View
                </Button>
                <Button 
                  size="small"
                  onClick={() => handleLayoutChange('1x1')}
                  sx={{ 
                    color: layout === '1x1' ? '#2196f3' : '#fff',
                    backgroundColor: layout === '1x1' ? 'rgba(33, 150, 243, 0.1)' : 'transparent',
                    borderRadius: 1,
                    '&:hover': {
                      backgroundColor: 'rgba(33, 150, 243, 0.2)'
                    }
                  }}
                >
                  Single View
                </Button>
              </Box>
              
              {/* Camera Display Area */}
              <Box sx={{ flexGrow: 1, height: '100%' }}>
                {renderCameraGrid()}
              </Box>
            </>
          )}
        </Box>
      </Box>

      {/* Bottom Control Bar */}
      <ControlBar>
        <Box sx={{ display: 'flex', alignItems: 'center' }}>
          <IconButton 
            size="small" 
            onClick={refreshAllCameras}
            sx={{ 
              color: 'white', 
              padding: '8px',
              backgroundColor: 'rgba(255, 255, 255, 0.05)',
              mr: 1,
              '&:hover': {
                backgroundColor: 'rgba(255, 255, 255, 0.1)',
              }
            }}
          >
            <RefreshIcon fontSize="small" />
          </IconButton>
          <Button
            size="small"
            variant="outlined"
            startIcon={<PlayArrowIcon />}
            sx={{
              color: '#fff',
              borderColor: 'rgba(255, 255, 255, 0.3)',
              borderRadius: 4,
              px: 2
            }}
          >
            Live
          </Button>
        </Box>

        <TimelineContainer>
          {renderTimeSegments()}
        </TimelineContainer>

        <Box sx={{ display: 'flex', alignItems: 'center' }}>
          <Typography variant="body2" sx={{ color: 'white', fontWeight: 'medium', mx: 1 }}>
            {formatTime(currentTime)}
          </Typography>
        </Box>
      </ControlBar>

      {/* 전체화면 모달 완전 리팩토링 */}
      {fullscreenCamera && (
        <Box
          sx={{
            position: 'fixed',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            backgroundColor: 'rgba(0, 0, 0, 0.9)',
            zIndex: 9999,
            display: 'flex',
            flexDirection: 'column',
            padding: 2
          }}
          role="dialog"
          aria-modal="true"
          aria-labelledby="fullscreen-title"
        >
          <Box sx={{ 
            display: 'flex', 
            justifyContent: 'space-between', 
            alignItems: 'center', 
            mb: 2, 
            color: '#fff'
          }}>
            <Typography variant="h6" id="fullscreen-title">
              {fullscreenCamera.label} - {translate('전체화면', 'Fullscreen', language)}
            </Typography>
            <IconButton 
              onClick={() => setFullscreenCamera(null)} 
              sx={{ color: '#fff' }}
              aria-label={translate('전체화면 닫기', 'Close fullscreen', language)}
            >
              <CloseIcon />
            </IconButton>
          </Box>
          
          {/* 전체 화면 컨테이너 - object-fit: contain 사용하여 전체 영상 표시 */}
          <Box sx={{ 
            flexGrow: 1, 
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'center',
            height: 'calc(100vh - 100px)',
            backgroundColor: '#000'
          }}>
            <Box
              component="img"
              src={`/camera/${fullscreenCamera.id}/?t=${Date.now()}`}
              sx={{
                width: '100%',
                height: '100%',
                objectFit: 'contain'
              }}
              alt={`Fullscreen Camera ${fullscreenCamera.label}`}
            />
          </Box>
        </Box>
      )}
    </Box>
  );
};

export default CameraMonitoring;