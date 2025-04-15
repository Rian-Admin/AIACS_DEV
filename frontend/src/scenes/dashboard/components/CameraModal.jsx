import React, { useState, useEffect } from 'react';
import {
  Box,
  Dialog,
  DialogContent,
  DialogTitle,
  IconButton,
  Grid,
  Typography
} from '@mui/material';
import {
  Close as CloseIcon,
  ArrowLeft as ArrowLeftIcon,
  ArrowRight as ArrowRightIcon
} from '@mui/icons-material';
import { translate } from '../../../utils/i18n';
import CameraStream from '../../../components/CameraStream';

/**
 * 카메라 스트림 모달 컴포넌트
 * 선택한 카메라 스트림을 확대 모달로 표시합니다.
 */
const CameraModal = ({ showCameraModal, setShowCameraModal, language = 'ko' }) => {
  // 선택된 카메라 ID
  const [selectedCamera, setSelectedCamera] = useState('1');
  
  // 사용 가능한 카메라 목록 (실제로는 API에서 가져올 수 있음)
  const cameras = [
    { id: '1', name: translate('카메라 1', 'Camera 1', language) },
    { id: '2', name: translate('카메라 2', 'Camera 2', language) },
    { id: '3', name: translate('카메라 3', 'Camera 3', language) }
  ];

  // 모달 닫기 핸들러
  const handleClose = () => {
    setShowCameraModal(false);
  };

  // 다음/이전 카메라로 이동
  const handlePrevCamera = () => {
    const currentIndex = cameras.findIndex(cam => cam.id === selectedCamera);
    const prevIndex = (currentIndex - 1 + cameras.length) % cameras.length;
    setSelectedCamera(cameras[prevIndex].id);
  };

  const handleNextCamera = () => {
    const currentIndex = cameras.findIndex(cam => cam.id === selectedCamera);
    const nextIndex = (currentIndex + 1) % cameras.length;
    setSelectedCamera(cameras[nextIndex].id);
  };

  return (
    <Dialog
      open={showCameraModal}
      onClose={handleClose}
      fullWidth
      maxWidth="lg"
      sx={{
        '& .MuiDialog-paper': {
          backgroundColor: '#0a1929',
          color: '#fff',
          borderRadius: 2
        }
      }}
    >
      <DialogTitle sx={{ m: 0, p: 2, display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
        <Typography variant="h6">
          {translate('카메라 모니터링', 'Camera Monitoring', language)}
        </Typography>
        <IconButton
          aria-label="close"
          onClick={handleClose}
          sx={{ color: 'grey.500' }}
          size="large"
        >
          <CloseIcon />
        </IconButton>
      </DialogTitle>

      <DialogContent sx={{ p: 2 }}>
        <Grid container spacing={2}>
          {/* 메인 카메라 뷰 */}
          <Grid item xs={12} sx={{ position: 'relative', height: '60vh' }}>
            <Box sx={{ position: 'relative', height: '100%', borderRadius: 2 }}>
              <CameraStream
                cameraId={selectedCamera}
                title={cameras.find(cam => cam.id === selectedCamera)?.name || ''}
                language={language}
              />
            </Box>

            {/* 이전/다음 카메라 버튼 */}
            <Box sx={{ position: 'absolute', left: '16px', top: '50%', transform: 'translateY(-50%)' }}>
              <IconButton 
                onClick={handlePrevCamera} 
                sx={{ bgcolor: 'rgba(0,0,0,0.3)', color: '#fff', '&:hover': { bgcolor: 'rgba(0,0,0,0.5)' } }}
              >
                <ArrowLeftIcon />
              </IconButton>
            </Box>
            <Box sx={{ position: 'absolute', right: '16px', top: '50%', transform: 'translateY(-50%)' }}>
              <IconButton 
                onClick={handleNextCamera} 
                sx={{ bgcolor: 'rgba(0,0,0,0.3)', color: '#fff', '&:hover': { bgcolor: 'rgba(0,0,0,0.5)' } }}
              >
                <ArrowRightIcon />
              </IconButton>
            </Box>
          </Grid>

          {/* 카메라 썸네일 목록 */}
          <Grid item xs={12}>
            <Grid container spacing={1} sx={{ mt: 1 }}>
              {cameras.map((camera) => (
                <Grid item xs={4} key={camera.id}>
                  <Box
                    onClick={() => setSelectedCamera(camera.id)}
                    sx={{
                      cursor: 'pointer',
                      border: selectedCamera === camera.id ? '2px solid #4fc3f7' : '2px solid transparent',
                      borderRadius: 1,
                      overflow: 'hidden',
                      height: '120px'
                    }}
                  >
                    <CameraStream
                      cameraId={camera.id}
                      title={camera.name}
                      height="100%"
                      language={language}
                    />
                  </Box>
                </Grid>
              ))}
            </Grid>
          </Grid>
        </Grid>
      </DialogContent>
    </Dialog>
  );
};

export default CameraModal; 