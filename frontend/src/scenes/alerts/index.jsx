import React, { useState, useEffect, useCallback } from 'react';
import { 
  Box, 
  Typography, 
  Paper, 
  Tab, 
  Tabs, 
  List, 
  ListItem, 
  ListItemText, 
  ListItemIcon, 
  Chip, 
  Divider, 
  Button, 
  IconButton 
} from '@mui/material';
import { translate, formatDateTime } from '../../utils/i18n';
import NotificationsIcon from '@mui/icons-material/Notifications';
import WarningIcon from '@mui/icons-material/Warning';
import InfoIcon from '@mui/icons-material/Info';
import ErrorIcon from '@mui/icons-material/Error';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';
import DeleteIcon from '@mui/icons-material/Delete';

// 알림 타입별 아이콘 매핑
const alertIcons = {
  warning: <WarningIcon sx={{ color: 'warning.main' }} />,
  error: <ErrorIcon sx={{ color: 'error.main' }} />,
  info: <InfoIcon sx={{ color: 'info.main' }} />,
  success: <CheckCircleIcon sx={{ color: 'success.main' }} />
};

/**
 * 알림 및 이벤트 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드 ('ko' 또는 'en')
 * @returns {JSX.Element} 알림 및 이벤트 컴포넌트
 */
const Alerts = ({ language }) => {
  const [tabValue, setTabValue] = useState(0);
  const [alerts, setAlerts] = useState([]);
  const [filteredAlerts, setFilteredAlerts] = useState([]);

  // 현재 탭에 따른 알림 필터링 함수
  const filterAlertsByTab = useCallback((alertsData, tabIndex) => {
    if (tabIndex === 0) return alertsData; // 모든 알림
    if (tabIndex === 1) return alertsData.filter(alert => !alert.read); // 안 읽은 알림
    if (tabIndex === 2) return alertsData.filter(alert => alert.read); // 읽은 알림
    return alertsData;
  }, []);

  // 알림 데이터 가져오기 (실제 구현에서는 API 호출)
  useEffect(() => {
    // 실제 구현에서는 API 호출로 대체
    // 예: const fetchData = async () => {
    //   try {
    //     const response = await fetch('/api/alerts');
    //     const data = await response.json();
    //     setAlerts(data);
    //     setFilteredAlerts(filterAlertsByTab(data, tabValue));
    //   } catch (error) {
    //     console.error('Failed to fetch alerts:', error);
    //   }
    // };
    // fetchData();
    
    // 테스트용 더미 데이터
    const dummyAlerts = [
      {
        id: 1,
        type: 'warning',
        title: {
          ko: '조류 접근 경고',
          en: 'Bird Approach Warning',
          id: 'Peringatan Pendekatan Burung'
        },
        message: {
          ko: '대형 조류가 활주로에 접근 중입니다. 자동 방어 시스템이 작동했습니다.',
          en: 'Large birds approaching the runway. Automatic defense system has been activated.',
          id: 'Burung besar mendekati landasan. Sistem pertahanan otomatis telah diaktifkan.'
        },
        timestamp: new Date(2023, 3, 15, 10, 30),
        read: false
      },
      {
        id: 2,
        type: 'error',
        title: {
          ko: '시스템 오류',
          en: 'System Error',
          id: 'Kesalahan Sistem'
        },
        message: {
          ko: '카메라 3번이 응답하지 않습니다. 기술 지원팀에 문의하세요.',
          en: 'Camera #3 is not responding. Please contact technical support.',
          id: 'Kamera #3 tidak merespons. Silakan hubungi dukungan teknis.'
        },
        timestamp: new Date(2023, 3, 14, 15, 45),
        read: true
      },
      {
        id: 3,
        type: 'info',
        title: {
          ko: '일일 보고서',
          en: 'Daily Report',
          id: 'Laporan Harian'
        },
        message: {
          ko: '어제의 조류 활동 요약이 준비되었습니다. 분석 페이지에서 확인하세요.',
          en: 'Yesterday\'s bird activity summary is ready. Check it on the Analytics page.',
          id: 'Ringkasan aktivitas burung kemarin sudah siap. Periksa di halaman Analitik.'
        },
        timestamp: new Date(2023, 3, 14, 8, 0),
        read: false
      },
      {
        id: 4,
        type: 'success',
        title: {
          ko: '방어 시스템 활성화',
          en: 'Defense System Activated',
          id: 'Sistem Pertahanan Diaktifkan'
        },
        message: {
          ko: '위협 요소가 감지되어 자동 방어 시스템이 성공적으로 활성화되었습니다.',
          en: 'Automatic defense system successfully activated in response to detected threat.',
          id: 'Sistem pertahanan otomatis berhasil diaktifkan sebagai respons terhadap ancaman yang terdeteksi.'
        },
        timestamp: new Date(2023, 3, 13, 12, 15),
        read: false
      },
    ];
    
    setAlerts(dummyAlerts);
    setFilteredAlerts(filterAlertsByTab(dummyAlerts, tabValue));
  }, [tabValue, filterAlertsByTab]);

  // 탭 변경 처리
  const handleTabChange = (event, newValue) => {
    setTabValue(newValue);
    setFilteredAlerts(filterAlertsByTab(alerts, newValue));
  };

  // 알림을 읽음 상태로 변경
  const markAsRead = (id) => {
    const updatedAlerts = alerts.map(alert => 
      alert.id === id ? { ...alert, read: true } : alert
    );
    setAlerts(updatedAlerts);
    setFilteredAlerts(filterAlertsByTab(updatedAlerts, tabValue));
  };

  // 알림 삭제
  const deleteAlert = (id) => {
    const updatedAlerts = alerts.filter(alert => alert.id !== id);
    setAlerts(updatedAlerts);
    setFilteredAlerts(filterAlertsByTab(updatedAlerts, tabValue));
  };

  // 모든 알림 읽음 표시
  const markAllAsRead = () => {
    const updatedAlerts = alerts.map(alert => ({ ...alert, read: true }));
    setAlerts(updatedAlerts);
    setFilteredAlerts(filterAlertsByTab(updatedAlerts, tabValue));
  };

  // 읽지 않은 알림 개수
  const unreadCount = alerts.filter(alert => !alert.read).length;

  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h6" fontWeight="bold" sx={{ mb: 3 }}>
        {translate('알림 및 이벤트', 'Alerts & Events', 'Peringatan & Acara', language)}
      </Typography>
      
      <Paper
        sx={{
          backgroundColor: '#0a1929',
          border: '1px solid #1e3a5a',
          borderRadius: 2,
          overflow: 'hidden'
        }}
      >
        <Box sx={{ borderBottom: 1, borderColor: 'divider', bgcolor: '#122c44' }}>
          <Tabs 
            value={tabValue} 
            onChange={handleTabChange}
            textColor="primary"
            indicatorColor="primary"
            variant="fullWidth"
          >
            <Tab 
              label={translate('전체', 'All', 'Semua', language)} 
              icon={<NotificationsIcon />} 
              iconPosition="start"
            />
            <Tab 
              label={translate('안 읽음', 'Unread', 'Belum Dibaca', language)} 
              icon={
                <Box sx={{ position: 'relative' }}>
                  <NotificationsIcon />
                  {unreadCount > 0 && (
                    <Chip 
                      label={unreadCount} 
                      color="error" 
                      size="small"
                      sx={{ 
                        position: 'absolute', 
                        top: -10, 
                        right: -20,
                        height: 20,
                        minWidth: 20,
                        fontSize: '0.75rem'
                      }}
                    />
                  )}
                </Box>
              } 
              iconPosition="start"
            />
            <Tab 
              label={translate('읽음', 'Read', 'Sudah Dibaca', language)} 
              icon={<CheckCircleIcon />} 
              iconPosition="start"
            />
          </Tabs>
        </Box>

        <Box sx={{ p: 2, display: 'flex', justifyContent: 'flex-end' }}>
          <Button 
            variant="contained" 
            size="small"
            onClick={markAllAsRead}
            disabled={unreadCount === 0}
          >
            {translate('모두 읽음 표시', 'Mark All as Read', 'Tandai Semua Telah Dibaca', language)}
          </Button>
        </Box>

        <Divider />

        <List sx={{ maxHeight: 400, overflow: 'auto' }}>
          {filteredAlerts.length > 0 ? (
            filteredAlerts.map((alert) => (
              <React.Fragment key={alert.id}>
                <ListItem 
                  alignItems="flex-start"
                  secondaryAction={
                    <IconButton edge="end" aria-label="delete" onClick={() => deleteAlert(alert.id)}>
                      <DeleteIcon />
                    </IconButton>
                  }
                  sx={{ 
                    bgcolor: alert.read ? 'transparent' : 'rgba(30, 58, 90, 0.3)',
                    ':hover': {
                      bgcolor: 'rgba(30, 58, 90, 0.5)'
                    },
                    cursor: !alert.read ? 'pointer' : 'default'
                  }}
                  onClick={() => !alert.read && markAsRead(alert.id)}
                >
                  <ListItemIcon>
                    {alertIcons[alert.type]}
                  </ListItemIcon>
                  <ListItemText
                    primary={
                      <Typography variant="subtitle1" color="text.primary">
                        {alert.title[language]}
                        {!alert.read && (
                          <Chip 
                            label={translate('새 알림', 'New', 'Baru', language)} 
                            color="primary" 
                            size="small" 
                            sx={{ ml: 1 }}
                          />
                        )}
                      </Typography>
                    }
                    secondary={
                      <>
                        <Typography variant="body2" color="text.secondary">
                          {alert.message[language]}
                        </Typography>
                        <Typography variant="caption" color="text.secondary" sx={{ mt: 1, display: 'block' }}>
                          {formatDateTime(alert.timestamp, language)}
                        </Typography>
                      </>
                    }
                  />
                </ListItem>
                <Divider component="li" />
              </React.Fragment>
            ))
          ) : (
            <Box sx={{ p: 3, textAlign: 'center' }}>
              <Typography variant="body1" color="text.secondary">
                {translate('알림이 없습니다.', 'No alerts found.', 'Tidak ada peringatan ditemukan.', language)}
              </Typography>
            </Box>
          )}
        </List>
      </Paper>
    </Box>
  );
};

export default Alerts;