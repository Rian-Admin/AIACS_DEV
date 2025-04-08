import { create } from 'zustand';
import { persist } from 'zustand/middleware';

/**
 * 앱의 전역 상태를 관리하는 스토어
 * - 언어 설정
 * - 테마 설정
 * - 카메라 관련 상태
 * - 레이더 관련 상태
 * - 알림 상태
 * - 시스템 상태
 */
const useAppStore = create(
  persist(
    (set) => ({
      // ===== 전역 설정 =====
      language: 'ko', // 'ko': 한국어, 'en': 영어
      themeMode: 'dark', // 'dark': 다크 모드, 'light': 라이트 모드
      
      // ===== 카메라 관련 상태 =====
      cameras: [],
      activeCameraId: null,
      cameraLayout: '2x2', // '1x1', '2x2', '3x3', '4x4'
      detectionEnabled: true, // 객체 감지 활성화 여부
      confidenceThreshold: 0.5, // 객체 감지 신뢰도 임계값
      
      // ===== 레이더 관련 상태 =====
      radars: [],
      activeRadarId: null,
      radarHistoryVisible: false, // 레이더 히스토리 표시 여부
      
      // ===== 알림 상태 =====
      alerts: [],
      unreadAlertCount: 0,
      alertSoundEnabled: true, // 알림 소리 활성화 여부
      
      // ===== 날씨 관련 상태 =====
      weatherData: null,
      weatherForecast: [],
      
      // ===== 충돌 위험 상태 =====
      collisionRisks: [],
      highestRiskLevel: 'low', // 'low', 'medium', 'high', 'critical'
      
      // ===== 방어 시스템 상태 =====
      defenseSystems: [],
      defenseSystemActive: false,
      
      // ===== 시스템 상태 =====
      systemStatus: {
        cpu: 0,
        memory: 0,
        disk: 0,
        network: 0,
        uptime: 0,
      },
      
      // ===== 액션 =====
      // 언어 변경
      setLanguage: (language) => set({ language }),
      
      // 테마 변경
      setThemeMode: (themeMode) => set({ themeMode }),
      
      // 카메라 관련 액션
      setCameras: (cameras) => set({ cameras }),
      setActiveCameraId: (activeCameraId) => set({ activeCameraId }),
      setCameraLayout: (cameraLayout) => set({ cameraLayout }),
      setDetectionEnabled: (detectionEnabled) => set({ detectionEnabled }),
      setConfidenceThreshold: (confidenceThreshold) => set({ confidenceThreshold }),
      
      // 레이더 관련 액션
      setRadars: (radars) => set({ radars }),
      setActiveRadarId: (activeRadarId) => set({ activeRadarId }),
      setRadarHistoryVisible: (radarHistoryVisible) => set({ radarHistoryVisible }),
      
      // 알림 관련 액션
      setAlerts: (alerts) => set({ alerts }),
      addAlert: (alert) => set((state) => ({ 
        alerts: [alert, ...state.alerts],
        unreadAlertCount: state.unreadAlertCount + 1
      })),
      markAllAlertsAsRead: () => set({ unreadAlertCount: 0 }),
      setAlertSoundEnabled: (alertSoundEnabled) => set({ alertSoundEnabled }),
      
      // 날씨 관련 액션
      setWeatherData: (weatherData) => set({ weatherData }),
      setWeatherForecast: (weatherForecast) => set({ weatherForecast }),
      
      // 충돌 위험 관련 액션
      setCollisionRisks: (collisionRisks) => set({ collisionRisks }),
      setHighestRiskLevel: (highestRiskLevel) => set({ highestRiskLevel }),
      
      // 방어 시스템 관련 액션
      setDefenseSystems: (defenseSystems) => set({ defenseSystems }),
      setDefenseSystemActive: (defenseSystemActive) => set({ defenseSystemActive }),
      
      // 시스템 상태 관련 액션
      setSystemStatus: (systemStatus) => set({ systemStatus }),
      
      // 모든 상태 초기화 (로그아웃 시)
      resetStore: () => set({
        cameras: [],
        activeCameraId: null,
        cameraLayout: '2x2',
        detectionEnabled: true,
        confidenceThreshold: 0.5,
        radars: [],
        activeRadarId: null,
        radarHistoryVisible: false,
        alerts: [],
        unreadAlertCount: 0,
        weatherData: null,
        weatherForecast: [],
        collisionRisks: [],
        highestRiskLevel: 'low',
        defenseSystems: [],
        defenseSystemActive: false,
        systemStatus: {
          cpu: 0,
          memory: 0,
          disk: 0,
          network: 0,
          uptime: 0,
        },
      }),
    }),
    {
      name: 'aiacs-storage',
      partialize: (state) => ({
        language: state.language,
        themeMode: state.themeMode,
        cameraLayout: state.cameraLayout,
        detectionEnabled: state.detectionEnabled,
        confidenceThreshold: state.confidenceThreshold,
        alertSoundEnabled: state.alertSoundEnabled,
      }),
    }
  )
);

export default useAppStore;