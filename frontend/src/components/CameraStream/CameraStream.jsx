import React, { useState, useEffect, useRef } from 'react';
import {
  Box,
  Typography,
  IconButton,
  CircularProgress,
  Fade
} from '@mui/material';
import {
  Refresh as RefreshIcon,
  ControlCamera as ControlCameraIcon,
  ArrowUpward as ArrowUpIcon,
  ArrowDownward as ArrowDownIcon,
  ArrowBack as ArrowLeftIcon,
  ArrowForward as ArrowRightIcon,
  Add as ZoomInIcon,
  Remove as ZoomOutIcon,
  DragIndicator as DragHandleIcon
} from '@mui/icons-material';
import { translate } from '../../utils/i18n';
import { cameraAPI } from '../../api';

/**
 * 카메라 스트림 컴포넌트
 * 백엔드에서 카메라 프레임을 가져와 표시합니다.
 * 
 * @param {Object} props
 * @param {string} props.cameraId - 카메라 ID
 * @param {string} props.title - 카메라 제목
 * @param {string} props.height - 컴포넌트 높이 (기본값: 100%)
 * @param {string} props.language - 언어 코드 (기본값: 'ko')
 * @param {boolean} props.hideRefreshButton - 새로고침 버튼 숨김 여부
 * @param {boolean} props.hideControls - 모든 컨트롤 숨김 여부
 */
const CameraStream = ({ 
  cameraId, 
  title, 
  height = '100%', 
  language = 'ko',
  hideRefreshButton = false,
  hideControls = false
}) => {
  // 상태 관리
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(false);
  const [frameUrl, setFrameUrl] = useState(null);
  const [showPtzControls, setShowPtzControls] = useState(false);
  
  // PTZ 버튼 위치 및 드래그 관련 상태
  const [position, setPosition] = useState({ x: 20, y: 50 });
  const [isDragging, setIsDragging] = useState(false);
  const [dragStart, setDragStart] = useState({ x: 0, y: 0 });
  
  // 컨테이너 참조
  const containerRef = useRef(null);

  // 백엔드 API 기본 URL - 상대 경로 사용
  const API_BASE_URL = '';

  // 카메라 스트림 URL 생성
  const getStreamUrl = (id) => {
    if (!id) return null;
    return `${API_BASE_URL}/camera/${id}/`;
  };

  // 컴포넌트 마운트 시 프레임 URL 설정
  useEffect(() => {
    if (cameraId) {
      setFrameUrl(getStreamUrl(cameraId));
    }
  }, [cameraId]);

  // 이미지 로드 핸들러
  const handleImageLoad = () => {
    setLoading(false);
    setError(false);
  };

  // 이미지 로드 에러 핸들러
  const handleImageError = () => {
    setLoading(false);
    setError(true);
  };

  // 카메라 스트림 새로고침
  const refreshCameraStream = () => {
    setLoading(true);
    setError(false);
    
    // URL에 타임스탬프를 추가하여 캐시 무효화
    setFrameUrl(`${getStreamUrl(cameraId)}?t=${new Date().getTime()}`);
    
    // 2초 후 자동으로 로딩 상태 해제 (네트워크 오류 방지)
    setTimeout(() => {
      setLoading(false);
    }, 2000);
  };
  
  // PTZ 제어 토글 - 별도의 토글 함수
  const togglePtzControls = () => {
    setShowPtzControls(!showPtzControls);
  };
  
  // PTZ 제어 함수
  const controlPtz = (direction, isContinuous = true) => {
    cameraAPI.controlPTZ(cameraId, direction, isContinuous, 0.7)
      .then(response => {
        console.log('PTZ 제어 성공:', response.data);
      })
      .catch(error => {
        console.error('PTZ 제어 오류:', error);
        // 백업 중지 메소드 호출 - 에러가 발생해도 중지 시도
        if (!isContinuous) {
          cameraAPI.stopPTZ(cameraId, direction)
            .catch(err => console.error('PTZ 중지 오류:', err));
        }
      });
  };

  // PTZ 중지 함수
  const stopPtz = (direction) => {
    controlPtz(direction, false);
  };
  
  // 드래그 시작 핸들러 - 오직 드래그만 처리
  const handleDragStart = (e) => {
    e.preventDefault();
    e.stopPropagation();
    
    setIsDragging(true);
    
    // 시작 위치 저장
    setDragStart({
      x: e.clientX - position.x,
      y: e.clientY - position.y
    });
  };
  
  // 마우스 이동 핸들러 
  const handleDragMove = (e) => {
    if (!isDragging) return;
    
    const container = containerRef.current;
    if (!container) return;
    
    const containerRect = container.getBoundingClientRect();
    const buttonWidth = 80; // 전체 드래그 영역 너비
    const buttonHeight = 40;
    
    // 컨테이너 내에서 허용 가능한 최대 위치 계산
    const maxX = containerRect.width - buttonWidth;
    const maxY = containerRect.height - buttonHeight;
    
    // 새 위치 계산 (제한 범위 내)
    const newX = Math.max(0, Math.min(e.clientX - dragStart.x, maxX));
    const newY = Math.max(0, Math.min(e.clientY - dragStart.y, maxY));
    
    setPosition({ x: newX, y: newY });
  };
  
  // 드래그 종료 핸들러
  const handleDragEnd = () => {
    setIsDragging(false);
  };
  
  // 드래그 이벤트 리스너 등록/해제
  useEffect(() => {
    if (isDragging) {
      document.addEventListener('mousemove', handleDragMove);
      document.addEventListener('mouseup', handleDragEnd);
    } else {
      document.removeEventListener('mousemove', handleDragMove);
      document.removeEventListener('mouseup', handleDragEnd);
    }
    
    return () => {
      document.removeEventListener('mousemove', handleDragMove);
      document.removeEventListener('mouseup', handleDragEnd);
    };
  }, [isDragging]);

  return (
    <Box
      ref={containerRef}
      sx={{
        position: 'relative',
        height,
        backgroundColor: '#111',
        borderRadius: 2,
        overflow: 'hidden',
        border: '1px solid #333',
        boxShadow: '0 4px 8px rgba(0,0,0,0.3)'
      }}
    >
      {/* 헤더 */}
      <Box
        sx={{
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
        }}
      >
        <Typography variant="body1">{title}</Typography>
      </Box>

      {/* 스트림 영역 */}
      <Box
        sx={{
          width: '100%',
          height: '100%',
          position: 'relative',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center'
        }}
      >
        {loading && (
          <Box
            sx={{
              position: 'absolute',
              top: 0, left: 0, right: 0, bottom: 0,
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              backgroundColor: 'rgba(0, 0, 0, 0.5)',
              zIndex: 10
            }}
          >
            <CircularProgress size={40} />
          </Box>
        )}

        {error ? (
          <Box sx={{ textAlign: 'center', padding: 2 }}>
            <Typography variant="body1" sx={{ color: '#ff5252', mb: 1 }}>
              {translate('스트림을 불러올 수 없습니다', 'Cannot load stream', language)}
            </Typography>
          </Box>
        ) : (
          <Box
            component="img"
            src={frameUrl}
            alt={`Camera ${cameraId} Stream`}
            onLoad={handleImageLoad}
            onError={handleImageError}
            loading="eager"
            decoding="async"
            fetchpriority="high"
            sx={{
              width: '100%',
              height: '100%',
              objectFit: 'contain',
              backgroundColor: '#000',
              imageRendering: 'auto',  // 브라우저 최적화 사용
            }}
          />
        )}
        
        {/* PTZ 제어 버튼 */}
        {!error && !hideControls && (
          <>
            {/* 드래그 가능한 컨트롤 패널 - 배경 영역과 버튼 분리 */}
            <Box
              sx={{
                position: 'absolute',
                top: position.y,
                left: position.x,
                zIndex: 999,
                width: 80,
                height: 40,
                backgroundColor: 'rgba(0, 0, 0, 0.6)',
                borderRadius: '20px',
                boxShadow: '0 2px 8px rgba(0,0,0,0.4)',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'space-between',
                padding: '0 5px',
                border: '1px solid rgba(255,255,255,0.1)',
                cursor: isDragging ? 'grabbing' : 'grab',
              }}
              onMouseDown={handleDragStart}
            >
              {/* 드래그 핸들 아이콘 */}
              <DragHandleIcon 
                sx={{ 
                  color: 'rgba(255,255,255,0.7)',
                  fontSize: '20px'
                }} 
              />
              
              {/* PTZ 토글 버튼 - 이 버튼만 클릭 이벤트 처리 */}
              <IconButton
                onClick={togglePtzControls}
                size="small"
                sx={{
                  backgroundColor: showPtzControls ? 'rgba(30, 136, 229, 0.8)' : 'rgba(30, 30, 30, 0.8)',
                  color: '#fff',
                  '&:hover': {
                    backgroundColor: 'rgba(30, 136, 229, 0.8)'
                  },
                  width: 34,
                  height: 34,
                  boxShadow: '0 2px 4px rgba(0,0,0,0.3)'
                }}
              >
                <ControlCameraIcon fontSize="small" />
              </IconButton>
            </Box>
            
            {/* PTZ 제어 패널 */}
            <Fade in={showPtzControls}>
              <Box
                sx={{
                  position: 'absolute',
                  top: position.y + 45,
                  left: position.x - 20,
                  display: 'flex',
                  flexDirection: 'column',
                  alignItems: 'center',
                  zIndex: 998,
                  backgroundColor: 'rgba(0, 0, 0, 0.75)',
                  padding: '10px',
                  borderRadius: '12px',
                  boxShadow: '0 4px 8px rgba(0,0,0,0.5)',
                  border: '1px solid rgba(255,255,255,0.1)'
                }}
              >
                {/* 상단 버튼 (위쪽) */}
                <IconButton
                  sx={{
                    backgroundColor: 'rgba(30, 136, 229, 0.6)',
                    color: '#fff',
                    mb: 1,
                    '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                  }}
                  onMouseDown={() => controlPtz('UP')}
                  onMouseUp={() => stopPtz('UP')}
                  onMouseLeave={() => stopPtz('UP')}
                  onTouchStart={() => controlPtz('UP')}
                  onTouchEnd={() => stopPtz('UP')}
                >
                  <ArrowUpIcon />
                </IconButton>
                
                {/* 중앙 버튼 (좌/우) */}
                <Box sx={{ display: 'flex', justifyContent: 'center', width: '100%', mb: 1 }}>
                  <IconButton
                    sx={{
                      backgroundColor: 'rgba(30, 136, 229, 0.6)',
                      color: '#fff',
                      mr: 2,
                      '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                    }}
                    onMouseDown={() => controlPtz('LEFT')}
                    onMouseUp={() => stopPtz('LEFT')}
                    onMouseLeave={() => stopPtz('LEFT')}
                    onTouchStart={() => controlPtz('LEFT')}
                    onTouchEnd={() => stopPtz('LEFT')}
                  >
                    <ArrowLeftIcon />
                  </IconButton>
                  
                  <IconButton
                    sx={{
                      backgroundColor: 'rgba(30, 136, 229, 0.6)',
                      color: '#fff',
                      '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                    }}
                    onMouseDown={() => controlPtz('RIGHT')}
                    onMouseUp={() => stopPtz('RIGHT')}
                    onMouseLeave={() => stopPtz('RIGHT')}
                    onTouchStart={() => controlPtz('RIGHT')}
                    onTouchEnd={() => stopPtz('RIGHT')}
                  >
                    <ArrowRightIcon />
                  </IconButton>
                </Box>
                
                {/* 하단 버튼 (아래쪽) */}
                <IconButton
                  sx={{
                    backgroundColor: 'rgba(30, 136, 229, 0.6)',
                    color: '#fff',
                    mb: 1,
                    '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                  }}
                  onMouseDown={() => controlPtz('DOWN')}
                  onMouseUp={() => stopPtz('DOWN')}
                  onMouseLeave={() => stopPtz('DOWN')}
                  onTouchStart={() => controlPtz('DOWN')}
                  onTouchEnd={() => stopPtz('DOWN')}
                >
                  <ArrowDownIcon />
                </IconButton>
                
                {/* 줌 버튼 */}
                <Box sx={{ display: 'flex', justifyContent: 'center', width: '100%' }}>
                  <IconButton
                    size="small"
                    sx={{
                      backgroundColor: 'rgba(30, 136, 229, 0.6)',
                      color: '#fff',
                      mr: 2,
                      '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                    }}
                    onMouseDown={() => controlPtz('ZOOM_IN')}
                    onMouseUp={() => stopPtz('ZOOM_IN')}
                    onMouseLeave={() => stopPtz('ZOOM_IN')}
                    onTouchStart={() => controlPtz('ZOOM_IN')}
                    onTouchEnd={() => stopPtz('ZOOM_IN')}
                  >
                    <ZoomInIcon />
                  </IconButton>
                  
                  <IconButton
                    size="small"
                    sx={{
                      backgroundColor: 'rgba(30, 136, 229, 0.6)',
                      color: '#fff',
                      '&:hover': { backgroundColor: 'rgba(30, 136, 229, 0.8)' }
                    }}
                    onMouseDown={() => controlPtz('ZOOM_OUT')}
                    onMouseUp={() => stopPtz('ZOOM_OUT')}
                    onMouseLeave={() => stopPtz('ZOOM_OUT')}
                    onTouchStart={() => controlPtz('ZOOM_OUT')}
                    onTouchEnd={() => stopPtz('ZOOM_OUT')}
                  >
                    <ZoomOutIcon />
                  </IconButton>
                </Box>
              </Box>
            </Fade>
          </>
        )}
      </Box>
    </Box>
  );
};

export default CameraStream; 