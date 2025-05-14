import { useState, useEffect } from 'react';
import { Routes, Route, Navigate } from 'react-router-dom';
import { CssBaseline, ThemeProvider } from '@mui/material';
import { SnackbarProvider } from 'notistack';
import { ColorModeContext, useMode } from './theme/index';
import useAppStore from './store/useAppStore';

// 씬 임포트
import Layout from './scenes/layout';
import Dashboard from './scenes/dashboard';
import VideoAnalysis from './scenes/videoAnalysis';
import Settings from './scenes/settings';
import RadarMonitoring from './scenes/radarMonitoring';
import WeatherData from './scenes/weatherData';
import Alerts from './scenes/alerts';
import DefenseControl from './scenes/defenseControl';
import Login from './scenes/login';
import Analytics from './scenes/analytics';

function App() {
  const [theme, colorMode] = useMode();
  const [language, setLanguage] = useState('ko'); // 'ko': 한국어, 'en': 영어
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const { radarEnabled, weatherEnabled } = useAppStore();

  // 언어 설정 유지
  useEffect(() => {
    const savedLanguage = localStorage.getItem('language');
    if (savedLanguage) {
      setLanguage(savedLanguage);
    }
  }, []);

  // 언어 변경 함수
  const changeLanguage = (lang) => {
    setLanguage(lang);
    localStorage.setItem('language', lang);
  };

  // 인증 상태 확인
  useEffect(() => {
    const token = localStorage.getItem('token');
    if (token) {
      // 토큰 유효성 검증 로직 추가 예정
      setIsAuthenticated(true);
    }
  }, []);

  // 인증 관련 함수
  const handleLogin = (token) => {
    localStorage.setItem('token', token);
    setIsAuthenticated(true);
  };

  const handleLogout = () => {
    localStorage.removeItem('token');
    setIsAuthenticated(false);
  };

  return (
    <ColorModeContext.Provider value={colorMode}>
      <ThemeProvider theme={theme}>
        <SnackbarProvider maxSnack={3}>
          <CssBaseline />
          <Routes>
            {/* 인증이 필요한 경로 */}
            {isAuthenticated ? (
              <Route element={<Layout 
                language={language} 
                changeLanguage={changeLanguage}
                handleLogout={handleLogout}
              />}>
                <Route path="/" element={<Dashboard language={language} />} />
                {/* <Route path="/camera" element={<CameraMonitoring language={language} />} /> */}
                <Route path="/video-analysis" element={<VideoAnalysis language={language} />} />
                <Route 
                  path="/radar" 
                  element={radarEnabled ? <RadarMonitoring language={language} /> : <Navigate to="/settings" replace />} 
                />
                <Route 
                  path="/weather" 
                  element={weatherEnabled ? <WeatherData language={language} /> : <Navigate to="/settings" replace />} 
                />
                <Route path="/analytics" element={<Analytics language={language} />} />
                <Route path="/alerts" element={<Alerts language={language} />} />
                <Route path="/defense-control" element={<DefenseControl language={language} />} />
                <Route path="/settings" element={<Settings language={language} />} />
              </Route>
            ) : (
              // 인증이 필요없는 경로
              <Route path="*" element={<Login handleLogin={handleLogin} language={language} />} />
            )}
          </Routes>
        </SnackbarProvider>
      </ThemeProvider>
    </ColorModeContext.Provider>
  );
}

export default App; 