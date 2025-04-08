import React, { useState, useEffect } from 'react';
import { Outlet, useLocation } from 'react-router-dom';
import { Box, CssBaseline, Typography } from '@mui/material';
import Topbar from './Topbar';
import Sidebar from './Sidebar';
import { translate } from '../../utils/i18n';

/**
 * 앱의 기본 레이아웃 컴포넌트
 * - 사이드바, 상단바, 메인 콘텐츠 영역 제공
 */
const Layout = ({ language, changeLanguage, handleLogout }) => {
  // 사이드바 초기값을 false(닫힌 상태)로 설정
  const [isSidebarOpen, setIsSidebarOpen] = useState(false);
  const location = useLocation();

  // 현재 경로에 따른 사이드바 토글 (모바일에서는 기본적으로 닫혀있음)
  useEffect(() => {
    const handleResize = () => {
      // 모바일 화면은 항상
      if (window.innerWidth < 768) {
        setIsSidebarOpen(false);
      }
      // 데스크탑에서도 초기 로드 시 닫힌 상태 유지
      // else {
      //   setIsSidebarOpen(true);
      // }
    };

    // 초기 크기 설정 및 리사이즈 이벤트 리스너 추가
    handleResize();
    window.addEventListener('resize', handleResize);

    return () => window.removeEventListener('resize', handleResize);
  }, []);

  // 라우트에 따른 페이지 제목 설정
  const getPageTitle = () => {
    const path = location.pathname;
    
    switch (path) {
      case '/':
        return translate('조류충돌방지시스템', 'AIACS', language);
      case '/camera':
        return translate('카메라 모니터링', 'Camera Monitoring', language);
      case '/video-analysis':
        return translate('동영상 분석', 'Video Analysis', language);
      case '/radar':
        return translate('레이더 모니터링', 'Radar Monitoring', language);
      case '/weather':
        return translate('기상 정보', 'Weather Data', language);
      case '/analytics':
        return translate('분석 및 통계', 'Analytics & Statistics', language);
      case '/alerts':
        return translate('알림 및 이벤트', 'Alerts & Events', language);
      case '/defense-control':
        return translate('방어 시스템 제어', 'Defense System Control', language);
      case '/settings':
        return translate('설정', 'Settings', language);
      default:
        return translate('조류충돌방지시스템', 'Bird Collision Prevention System', language);
    }
  };

  return (
    <Box sx={{ display: 'flex', height: '100vh', overflow: 'hidden' }}>
      <CssBaseline />
      
      {/* 사이드바 컴포넌트 */}
      <Sidebar 
        isSidebarOpen={isSidebarOpen} 
        setIsSidebarOpen={setIsSidebarOpen}
        language={language}
        highestRiskLevel="low"
      />
      
      {/* 메인 콘텐츠 영역 */}
      <Box 
        sx={{ 
          flexGrow: 1, 
          display: 'flex', 
          flexDirection: 'column', 
          overflow: 'hidden',
          marginLeft: isSidebarOpen ? '220px' : 0,
          width: isSidebarOpen ? 'calc(100% - 220px)' : '100%',
          transition: (theme) => theme.transitions.create(['margin', 'width'], {
            easing: theme.transitions.easing.sharp,
            duration: theme.transitions.duration.leavingScreen,
          }),
        }}
      >
        {/* 상단바 컴포넌트 */}
        <Topbar 
          pageTitle={getPageTitle()}
          isSidebarOpen={isSidebarOpen}
          setIsSidebarOpen={setIsSidebarOpen}
          language={language}
          changeLanguage={changeLanguage}
          handleLogout={handleLogout}
          unreadAlertCount={0}
          handleAlertClick={() => {}}
        />
        
        {/* 메인 콘텐츠 */}
        <Box sx={{ 
          flexGrow: 1, 
          p: 1, 
          overflow: 'auto', 
          backgroundColor: (theme) => theme.palette.background.default,
          display: 'flex',
          flexDirection: 'column',
          minHeight: 0
        }}>
          <Outlet />
        </Box>
        
        {/* 하단 상태 표시줄 */}
        <Box
          sx={{
            height: '28px',
            backgroundColor: '#071525',
            borderTop: '1px solid #1e3a5a',
            display: 'flex',
            alignItems: 'center',
            px: 2,
            fontSize: '12px',
            color: '#90caf9'
          }}
        >
          <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
            <Typography variant="caption">
              {translate('CPU: ', 'CPU: ', language)}35%
            </Typography>
            <Typography variant="caption">
              {translate('메모리: ', 'Memory: ', language)}45%
            </Typography>
            <Typography variant="caption">
              {translate('디스크: ', 'Disk: ', language)}28%
            </Typography>
            <Typography variant="caption">
              {translate('네트워크: ', 'Network: ', language)}42%
            </Typography>
          </Box>
          <Box sx={{ flexGrow: 1 }} />
          <Typography variant="caption">
            {translate('업타임: ', 'Uptime: ', language)}3h 45m
          </Typography>
        </Box>
      </Box>
    </Box>
  );
};

export default Layout; 