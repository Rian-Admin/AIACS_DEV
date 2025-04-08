import React from 'react';
import { Link, useLocation } from 'react-router-dom';
import { Box, Drawer, IconButton, Typography, List, ListItem, ListItemButton, ListItemIcon, ListItemText } from '@mui/material';
import { ChevronLeft as ChevronLeftIcon } from '@mui/icons-material';
import { translate } from '../../utils/i18n';
import rianLogo from '../../assets/images/rian_60px.png';

// 임포트할 아이콘들
import {
  Dashboard as DashboardIcon,
  CameraAlt as CameraIcon,
  VideoLibrary as VideoIcon,
  Radar as RadarIcon,
  WbSunny as WeatherIcon,
  Analytics as AnalyticsIcon,
  Notifications as NotificationsIcon,
  Shield as DefenseIcon,
  Settings as SettingsIcon
} from '@mui/icons-material';

// 메뉴 아이템 정의
const menuItems = [
  { id: 'dashboard', path: '/', icon: <DashboardIcon />, label: { ko: '대시보드', en: 'Dashboard' } },
  { id: 'camera', path: '/camera', icon: <CameraIcon />, label: { ko: '조류 모니터링', en: 'Bird Monitoring' } },
  { id: 'video', path: '/video-analysis', icon: <VideoIcon />, label: { ko: '동영상 분석', en: 'Video Analysis' } },
  { id: 'radar', path: '/radar', icon: <RadarIcon />, label: { ko: '레이더 모니터링', en: 'Radar Monitoring' } },
  { id: 'weather', path: '/weather', icon: <WeatherIcon />, label: { ko: '기상 정보', en: 'Weather Data' } },
  { id: 'analytics', path: '/analytics', icon: <AnalyticsIcon />, label: { ko: '분석 및 통계', en: 'Analytics & Statistics' } },
  { id: 'alerts', path: '/alerts', icon: <NotificationsIcon />, label: { ko: '알림 및 이벤트', en: 'Alerts & Events' } },
  { id: 'defense', path: '/defense-control', icon: <DefenseIcon />, label: { ko: '방어 시스템 제어', en: 'Defense System Control' } },
  { id: 'settings', path: '/settings', icon: <SettingsIcon />, label: { ko: '설정', en: 'Settings' } },
];

const Sidebar = ({ isSidebarOpen, setIsSidebarOpen, language, highestRiskLevel }) => {
  const location = useLocation();
  
  return (
    <Drawer
      variant="persistent"
      anchor="left"
      open={isSidebarOpen}
      PaperProps={{
        sx: {
          width: 220,
          backgroundColor: '#0a1929',
          color: 'white',
          borderRight: '1px solid #1e3a5a',
        },
      }}
    >
      <Box
        sx={{
          display: 'flex',
          flexDirection: 'column',
          height: '100%',
        }}
      >
        {/* 헤더 영역 */}
        <Box
          sx={{
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'space-between',
            padding: '10px 10px',
            borderBottom: '1px solid #1e3a5a',
          }}
        >
          <Box sx={{ display: 'flex', alignItems: 'center' }}>
            {/* 로고와 텍스트 제거 */}
          </Box>
          <IconButton onClick={() => setIsSidebarOpen(false)} sx={{ color: 'white' }}>
            <ChevronLeftIcon />
          </IconButton>
        </Box>

        {/* 메뉴 리스트 */}
        <List sx={{ flexGrow: 1, overflow: 'auto', py: 0 }}>
          {menuItems.map((item) => {
            const isSelected = location.pathname === item.path;
            return (
              <ListItem key={item.id} disablePadding>
                <ListItemButton
                  component={Link}
                  to={item.path}
                  selected={isSelected}
                  sx={{
                    mb: 0.5,
                    py: 1,
                    '&.Mui-selected': {
                      backgroundColor: 'rgba(33, 150, 243, 0.2)',
                      borderRight: '4px solid #2196f3',
                      '&:hover': {
                        backgroundColor: 'rgba(33, 150, 243, 0.3)',
                      },
                    },
                    '&:hover': {
                      backgroundColor: 'rgba(33, 150, 243, 0.1)',
                    },
                  }}
                >
                  <ListItemIcon sx={{ color: isSelected ? '#2196f3' : '#90caf9', minWidth: 40 }}>
                    {item.icon}
                  </ListItemIcon>
                  <ListItemText 
                    primary={language === 'ko' ? item.label.ko : item.label.en}
                    primaryTypographyProps={{
                      fontSize: 14,
                      fontWeight: isSelected ? 'bold' : 'normal',
                    }}
                  />
                </ListItemButton>
              </ListItem>
            );
          })}
        </List>

        {/* 푸터 영역 */}
        <Box
          sx={{
            padding: '10px',
            borderTop: '1px solid #1e3a5a',
            textAlign: 'center',
          }}
        >
          <Typography variant="caption" color="text.secondary">
            AIACS v1.0.0
          </Typography>
        </Box>
      </Box>
    </Drawer>
  );
};

export default Sidebar; 