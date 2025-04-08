import React, { useState, useEffect } from 'react';
import {
  Box,
  Typography,
  IconButton,
  CircularProgress
} from '@mui/material';
import {
  Refresh as RefreshIcon
} from '@mui/icons-material';
import { translate } from '../../utils/i18n';

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

  return (
    <Box
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
            sx={{
              width: '100%',
              height: '100%',
              objectFit: 'contain',
              backgroundColor: '#000'
            }}
          />
        )}
      </Box>
    </Box>
  );
};

export default CameraStream; 