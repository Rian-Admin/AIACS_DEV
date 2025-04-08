import React, { useState, useEffect } from 'react';
import { Box, IconButton, Typography, Button, Menu, MenuItem, Badge, Tooltip } from '@mui/material';
import { translate } from '../../utils/i18n';

// 아이콘
import {
  Menu as MenuIcon,
  Notifications as NotificationsIcon,
  Translate as TranslateIcon,
  KeyboardArrowDown as ArrowDownIcon,
} from '@mui/icons-material';

const Topbar = ({ 
  pageTitle, 
  isSidebarOpen, 
  setIsSidebarOpen, 
  language, 
  changeLanguage, 
  handleLogout, 
  unreadAlertCount, 
  handleAlertClick 
}) => {
  const [currentTime, setCurrentTime] = useState(new Date());
  const [currentDate, setCurrentDate] = useState('');
  
  // 언어 메뉴
  const [languageAnchorEl, setLanguageAnchorEl] = useState(null);
  const isLanguageMenuOpen = Boolean(languageAnchorEl);

  // 시간 업데이트 효과
  useEffect(() => {
    const timer = setInterval(() => {
      setCurrentTime(new Date());
    }, 1000);

    // 날짜 포맷 설정
    const updateDate = () => {
      const today = new Date();
      
      if (language === 'ko') {
        setCurrentDate(`${today.getFullYear()}.${String(today.getMonth() + 1).padStart(2, '0')}.${String(today.getDate()).padStart(2, '0')}`);
      } else if (language === 'id') {
        setCurrentDate(`${String(today.getDate()).padStart(2, '0')}-${String(today.getMonth() + 1).padStart(2, '0')}-${today.getFullYear()}`);
      } else {
        setCurrentDate(`${today.getFullYear()}/${String(today.getMonth() + 1).padStart(2, '0')}/${String(today.getDate()).padStart(2, '0')}`);
      }
    };
    
    updateDate();
    
    return () => clearInterval(timer);
  }, [language]);

  // 시간 포맷 함수
  const formatTime = (date) => {
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');
    return `${hours}:${minutes}:${seconds}`;
  };

  // 언어 메뉴 열기/닫기
  const handleLanguageMenuOpen = (event) => {
    setLanguageAnchorEl(event.currentTarget);
  };

  const handleLanguageMenuClose = () => {
    setLanguageAnchorEl(null);
  };

  const handleLanguageChange = (lang) => {
    changeLanguage(lang);
    handleLanguageMenuClose();
  };

  return (
    <Box
      sx={{
        display: 'flex',
        justifyContent: 'space-between',
        p: 2,
        backgroundColor: '#0a1929',
        boxShadow: '0 2px 4px rgba(0, 0, 0, 0.2)',
        background: 'linear-gradient(90deg, #0a1929 0%, #0d47a1 50%, #0a1929 100%)',
        borderBottom: '1px solid #1e3a5a',
        position: 'relative',
        zIndex: 10
      }}
    >
      {/* 왼쪽 영역: 타이틀 */}
      <Box display="flex" alignItems="center">
        <IconButton 
          color="inherit" 
          onClick={() => setIsSidebarOpen(!isSidebarOpen)} 
          sx={{ mr: 1 }}
        >
          <MenuIcon />
        </IconButton>
        <Typography variant="h5" fontWeight="bold" color="white">
          {pageTitle}
        </Typography>
      </Box>

      {/* 오른쪽 영역: 시간, 언어, 알림 */}
      <Box display="flex" alignItems="center" gap={2}>
        {/* 언어 선택 */}
        <Button
          onClick={handleLanguageMenuOpen}
          color="inherit"
          startIcon={<TranslateIcon />}
          endIcon={<ArrowDownIcon />}
          size="small"
          sx={{ 
            borderRadius: '4px', 
            border: '1px solid rgba(255, 255, 255, 0.23)',
            padding: '4px 8px',
            minWidth: 'unset',
            fontSize: '0.8rem'
          }}
        >
          {language === 'ko' ? '한국어' : language === 'en' ? 'English' : 'Bahasa Indonesia'}
        </Button>
        <Menu
          anchorEl={languageAnchorEl}
          open={isLanguageMenuOpen}
          onClose={handleLanguageMenuClose}
          PaperProps={{
            sx: { 
              backgroundColor: '#0a1929', 
              color: '#ffffff',
              border: '1px solid #1e3a5a',
              minWidth: '120px'
            }
          }}
        >
          <MenuItem 
            onClick={() => handleLanguageChange('ko')}
            selected={language === 'ko'}
          >
            한국어
          </MenuItem>
          <MenuItem 
            onClick={() => handleLanguageChange('en')}
            selected={language === 'en'}
          >
            English
          </MenuItem>
          <MenuItem 
            onClick={() => handleLanguageChange('id')}
            selected={language === 'id'}
          >
            Bahasa Indonesia
          </MenuItem>
        </Menu>

        {/* 알림 버튼 */}
        <IconButton 
          color="inherit"
          onClick={handleAlertClick}
        >
          <Badge badgeContent={unreadAlertCount} color="secondary">
            <NotificationsIcon />
          </Badge>
        </IconButton>

        {/* 시간 표시 */}
        <Box 
          sx={{ 
            textAlign: "right",
            background: "rgba(0,0,0,0.2)",
            padding: "5px 10px",
            borderRadius: "3px"
          }}
        >
          <Typography variant="h6">
            {formatTime(currentTime)}
          </Typography>
          <Typography variant="caption">
            {currentDate}
          </Typography>
        </Box>
      </Box>
    </Box>
  );
};

export default Topbar; 