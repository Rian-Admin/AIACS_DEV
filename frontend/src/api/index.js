import axios from 'axios';

// API 서버의 기본 URL 설정
// 상대 경로 사용하면 Nginx 프록시를 통해 요청이 전달됨
const API_BASE_URL = '';  // 상대 경로로 변경 (공백으로)

// Axios 인스턴스 생성
const api = axios.create({
  baseURL: API_BASE_URL,
  timeout: 30000, // 30초
  headers: {
    'Content-Type': 'application/json',
  },
});

// 요청 인터셉터 - 토큰 추가
api.interceptors.request.use(
  (config) => {
    const token = localStorage.getItem('token');
    if (token) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

// 응답 인터셉터 - 오류 처리
api.interceptors.response.use(
  (response) => {
    return response;
  },
  async (error) => {
    const originalRequest = error.config;
    
    // 401 오류 (인증 만료) 처리
    if (error.response && error.response.status === 401 && !originalRequest._retry) {
      originalRequest._retry = true;
      
      try {
        // 토큰 갱신 로직 (필요시 구현)
        const refreshToken = localStorage.getItem('refreshToken');
        if (refreshToken) {
          // 리프레시 토큰을 사용하여 새 액세스 토큰 요청
          const response = await axios.post(`${API_BASE_URL}/auth/refresh`, {
            refreshToken,
          });
          
          const { token } = response.data;
          localStorage.setItem('token', token);
          
          // 새 토큰으로 원래 요청 재시도
          api.defaults.headers.common.Authorization = `Bearer ${token}`;
          return api(originalRequest);
        }
      } catch (refreshError) {
        // 토큰 갱신 실패 시 로그아웃 처리
        localStorage.removeItem('token');
        localStorage.removeItem('refreshToken');
        window.location.href = '/login';
      }
    }
    
    return Promise.reject(error);
  }
);

// API 모듈 그룹화
const weatherAPI = {
  // 현재 날씨 데이터 가져오기
  getCurrentWeather: () => api.get('/api/weather/current/'),
  
  // 날씨 데이터 수동 업데이트
  updateWeather: () => api.post('/api/weather/update/'),
  
  // 날씨 이력 데이터 가져오기
  getWeatherHistory: () => api.get('/api/weather/history/'),
};

const cameraAPI = {
  // 카메라 목록 가져오기
  getCameras: () => api.get('/api/cameras/'),
  
  // 카메라 상세 정보 가져오기
  getCameraById: (id) => api.get(`/api/cameras/${id}/`),
  
  // 카메라 설정 업데이트
  updateCameraSettings: (id, settings) => api.put(`/api/cameras/${id}/settings/`, settings),
  
  // PTZ 명령 전송
  sendPTZCommand: (id, command) => api.post(`/api/cameras/${id}/ptz/`, command),
  
  // PTZ 제어 명령 전송 (cameraMonitoring 전용)
  controlPTZ: (cameraId, direction, isContinuous = true, speed = 0.7) => 
    api.post('/api/ptz/control/', {
      camera_id: cameraId,
      direction: direction,
      is_continuous: isContinuous,
      speed: speed
    }),
  
  // PTZ 중지 명령 전송
  stopPTZ: (cameraId, direction) => 
    api.post('/api/ptz/stop/', {
      camera_id: cameraId,
      direction: direction
    }),
  
  // 프리셋 관리
  getPresets: (id) => api.get(`/api/cameras/${id}/presets/`),
  savePreset: (id, preset) => api.post(`/api/cameras/${id}/presets/`, preset),
  moveToPreset: (id, presetId) => api.post(`/api/cameras/${id}/presets/${presetId}/move/`),
  deletePreset: (id, presetId) => api.delete(`/api/cameras/${id}/presets/${presetId}/`),
};

const radarAPI = {
  // 레이더 목록 가져오기
  getRadars: () => api.get('/radars'),
  
  // 레이더 데이터 가져오기
  getRadarData: (id, params) => api.get(`/radars/${id}/data`, { params }),
  
  // 레이더 설정 업데이트
  updateRadarSettings: (id, settings) => api.put(`/radars/${id}/settings`, settings),
  
  // 과거 레이더 데이터 가져오기
  getHistoricalData: (id, startTime, endTime) => 
    api.get(`/radars/${id}/history`, { params: { startTime, endTime } }),
};

// 레이더 에뮬레이터 API
const radarEmulatorAPI = {
  // 연결 상태 확인
  getConnectionStatus: () => api.get('/api/radar/connection/'),
  
  // 에뮬레이터에 연결
  connect: (ip, port = 4102) => api.post('/api/radar/connection/', { ip, port }),
  
  // 에뮬레이터 연결 종료
  disconnect: () => api.delete('/api/radar/connection/'),
  
  // 레이더 상태 조회
  getState: () => api.get('/api/radar/state/'),
  
  // 레이더 상태 초기화
  clearState: () => api.delete('/api/radar/state/'),
  
  // 사용 가능한 명령어 목록 조회
  getCommands: () => api.get('/api/radar/commands/'),
  
  // 명령어 상세 정보 조회
  getCommandDetails: (commandId) => api.get(`/api/radar/commands/${commandId}/`),
  
  // 명령어 전송
  sendCommand: (commandId) => api.post('/api/radar/commands/', { command_id: commandId }),
  
  // 메시지 정보 조회 (타입별)
  getMessages: (messageType) => messageType 
    ? api.get(`/api/radar/messages/${messageType}/`)
    : api.get('/api/radar/messages/'),
    
  // 4001 메시지(상태 정보) 조회
  getStatus4001: () => api.get('/api/radar/messages/4001/'),
  
  // 기타 메시지 조회
  getOtherMessages: () => api.get('/api/radar/messages/other/'),
  
  // 시뮬레이션 관련 API
  getSimulationStatus: () => api.get('/api/radar/simulation/'),
  
  sendSimulationScenario: (scenarioData) => api.post('/api/radar/simulation/', scenarioData),
  
  stopSimulation: () => api.delete('/api/radar/simulation/'),
  
  // 사전 정의된 시나리오 템플릿 가져오기 (새 기능 - 향후 구현)
  getScenarioTemplates: () => api.get('/api/radar/simulation/templates/'),
  
  // 플롯/트랙 활성화
  activatePlotTrack: () => api.post('/api/radar/plot_track/', {
    PlotSendFlag: 1,
    PlotType: 3,
    TrackSendFlag: 1,
    TrackType: 3
  }),
  
  // 운용 모드 시작
  startOperation: () => api.post('/api/radar/operation/', { OperationMode: 2 }),
};

const detectionAPI = {
  // 객체 감지 결과 가져오기
  getDetections: (params) => api.get('/detections', { params }),
  
  // 감지 설정 업데이트
  updateDetectionSettings: (settings) => api.put('/detections/settings', settings),
  
  // 특정 카메라의 감지 결과 가져오기
  getCameraDetections: (cameraId, params) => 
    api.get(`/cameras/${cameraId}/detections`, { params }),
};

const analyticsAPI = {
  // 통계 데이터 가져오기
  getStatistics: (params) => api.get('/analytics/statistics', { params }),
  
  // 히트맵 데이터 가져오기
  getHeatmap: (params) => api.get('/analytics/heatmap', { params }),
  
  // 시간별 분석 데이터 가져오기
  getTimeAnalysis: (params) => api.get('/analytics/time', { params }),
  
  // 조류 종류별 분석 데이터 가져오기
  getSpeciesAnalysis: (params) => api.get('/analytics/species', { params }),
};

const alertAPI = {
  // 알림 목록 가져오기
  getAlerts: (params) => api.get('/alerts', { params }),
  
  // 알림 설정 업데이트
  updateAlertSettings: (settings) => api.put('/alerts/settings', settings),
  
  // 알림 상태 업데이트
  updateAlertStatus: (id, status) => api.put(`/alerts/${id}/status`, { status }),
  
  // 푸시 알림 구독
  subscribePushNotifications: (subscription) => 
    api.post('/alerts/push-subscription', subscription),
};

const defenseAPI = {
  // 방어 시스템 목록 가져오기
  getDefenseSystems: () => api.get('/defense-systems'),
  
  // 방어 시스템 상태 가져오기
  getDefenseSystemStatus: (id) => api.get(`/defense-systems/${id}/status`),
  
  // 방어 시스템 활성화/비활성화
  toggleDefenseSystem: (id, active) => 
    api.put(`/defense-systems/${id}/toggle`, { active }),
  
  // 방어 명령 전송
  sendDefenseCommand: (id, command) => 
    api.post(`/defense-systems/${id}/command`, command),
  
  // 퇴치 기록 데이터 가져오기
  getDeterrentRecords: (params) => api.get('/api/deterrent-records', { params }),
};

const videoAPI = {
  // 동영상 목록 가져오기
  getVideos: (params) => api.get('/videos', { params }),
  
  // 동영상 업로드
  uploadVideo: (formData, onProgress) => 
    api.post('/videos/upload', formData, {
      headers: {
        'Content-Type': 'multipart/form-data',
      },
      onUploadProgress: (progressEvent) => {
        const percentCompleted = Math.round(
          (progressEvent.loaded * 100) / progressEvent.total
        );
        if (onProgress) onProgress(percentCompleted);
      },
    }),
  
  // 동영상 분석 요청
  analyzeVideo: (videoId, settings) => api.post(`/videos/${videoId}/analyze`, settings),
  
  // 분석 결과 가져오기
  getVideoAnalysisResults: (videoId) => api.get(`/videos/${videoId}/analysis`),
};

const authAPI = {
  // 로그인
  login: (credentials) => api.post('/auth/login', credentials),
  
  // 로그아웃
  logout: () => api.post('/auth/logout'),
  
  // 사용자 정보 가져오기
  getProfile: () => api.get('/auth/profile'),
  
  // 비밀번호 변경
  changePassword: (passwords) => api.post('/auth/change-password', passwords),
};

const settingsAPI = {
  // 시스템 설정 가져오기
  getSystemSettings: () => api.get('/settings/system'),
  
  // 시스템 설정 업데이트
  updateSystemSettings: (settings) => api.put('/settings/system', settings),
  
  // 사용자 설정 가져오기
  getUserSettings: () => api.get('/settings/user'),
  
  // 사용자 설정 업데이트
  updateUserSettings: (settings) => api.put('/settings/user', settings),
};

// 트랙 종료 요청
export const deleteTrack = async (trackId) => {
  try {
    const response = await axios.post(`${API_BASE_URL}/api/radar/track/delete`, { track_id: trackId });
    return response.data;
  } catch (error) {
    console.error('트랙 종료 요청 오류:', error);
    throw error;
  }
};

// 종료된 트랙 정보 조회
export const getDeletedTracks = async () => {
  try {
    const response = await axios.get(`${API_BASE_URL}/api/radar/track/deleted`);
    return response.data;
  } catch (error) {
    console.error('종료된 트랙 정보 조회 오류:', error);
    throw error;
  }
};

// API 모듈 내보내기
export {
  api,
  weatherAPI,
  cameraAPI,
  radarAPI,
  radarEmulatorAPI,
  detectionAPI,
  analyticsAPI,
  alertAPI,
  defenseAPI,
  videoAPI,
  authAPI,
  settingsAPI,
}; 