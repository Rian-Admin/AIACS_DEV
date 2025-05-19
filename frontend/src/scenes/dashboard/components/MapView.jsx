import React, { useState, useEffect, useRef, useCallback } from 'react';
import {
  Box,
  Typography,
  Paper,
  IconButton,
  Tooltip,
  Tabs,
  Tab,
  Grid,
  Button,
  List,
  ListItem,
  ListItemText
} from '@mui/material';
import {
  Fullscreen as FullscreenIcon,
  CameraAlt as CameraIcon,
  VolumeUp as VolumeUpIcon,
  VolumeOff as VolumeOffIcon,
  MusicNote as MusicNoteIcon,
  Refresh as RefreshIcon
} from '@mui/icons-material';
import { MapContainer, TileLayer, CircleMarker, Popup, Polygon, useMap } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';
import { translate } from '../../../utils/i18n';
import { radarEmulatorAPI, cameraAPI } from '../../../api';
import CombinedRadarDisplay from '../../../components/RadarControl/CombinedRadarDisplay';
import CameraStream from '../../../components/CameraStream';
import axios from 'axios';
import { styled } from '@mui/material/styles';
import useAppStore from '../../../store/useAppStore';

// Leaflet 아이콘 마커 깨짐 방지
delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png',
  iconUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png',
  shadowUrl:
    'https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png'
});

// PTZ 컨트롤 스타일 정의
const SidePanel = styled(Paper)(({ theme }) => ({
  backgroundColor: '#0f1620',
  border: '1px solid #1e3a5a',
  borderRadius: 4,
  height: '100%',
  overflowY: 'auto',
  boxShadow: '0 4px 12px rgba(0,0,0,0.3)'
}));

/**
 * 예시 함수: 카메라의 위치, 방향, 시야각으로 "시야 폴리곤" 좌표 생성
 * @param {[number, number]} center  [위도, 경도]
 * @param {number} direction  방향(도)
 * @param {number} angle      시야각(도)
 * @param {number} distance   시야 거리(미터)
 * @returns {Array<[number, number]>} Leaflet Polygon 좌표
 */
function createViewField(center, direction, angle, distance) {
  const lat = center[0];
  const lng = center[1];
  
  // 부채꼴 시야각을 위한 점 생성
  const rad = (deg) => (deg * Math.PI) / 180;
  const earthRadius = 6378137; // 지구 반지름(m)
  
  // 부채꼴을 구성하기 위한 점 배열
  const points = [];
  
  // 시작점 (카메라 위치)
  points.push(center);
  
  // 시야각의 절반
  const halfAngle = angle / 2;
  
  // 부채꼴을 완성하기 위해 12개의 점 생성 (각도를 세분화)
  const segments = 12;
  for (let i = 0; i <= segments; i++) {
    // direction - halfAngle부터 direction + halfAngle까지 각도로 점 생성
    const bearing = direction - halfAngle + (angle * i / segments);
    const bearingRad = rad(bearing);
    
    // distance만큼 이동한 위치(위도/경도) 계산
    const dLat =
      (distance * Math.cos(bearingRad)) / earthRadius * (180 / Math.PI);
    const dLng =
      (distance * Math.sin(bearingRad)) /
      (earthRadius * Math.cos(rad(lat))) *
      (180 / Math.PI);
    
    const newLat = lat + dLat;
    const newLng = lng + dLng;
    points.push([newLat, newLng]);
  }
  
  // 폴리곤을 닫기 위해 시작점 추가
  points.push(center);
  
  // 부채꼴 폴리곤 반환
  return points;
}

// 원형 레이더 오버레이 표시를 위한 컴포넌트
const RadarCircleOverlay = React.memo(({ center }) => {
  const map = useMap();
  
  useEffect(() => {
    // 맵이 로드될 때와 중심이 변경될 때마다 실행
    if (map) {
      const circleRadius = 300; // 미터 단위
      const overlayGroup = L.layerGroup().addTo(map);
      
      // 각도를 라디안으로 변환하는 함수 (컴포넌트 내 전역으로 정의)
      const rad = (deg) => (deg * Math.PI) / 180;
      
      // 원형 레이더 경계선 (메인 원) - 네온 효과 적용
      L.circle(center, {
        radius: circleRadius,
        color: '#00e0e0',
        fillColor: 'rgba(0, 224, 224, 0.02)',
        weight: 1.8,
        opacity: 0.7,
        className: 'neon-circle-outer',
      }).addTo(overlayGroup);
      
      // 내부 동심원 - 네온 효과 적용
      L.circle(center, {
        radius: circleRadius * 0.7,
        color: '#00e0e0',
        fillColor: 'transparent',
        weight: 1.5,
        opacity: 0.6,
        className: 'neon-circle-middle',
      }).addTo(overlayGroup);
      
      L.circle(center, {
        radius: circleRadius * 0.4,
        color: '#00e0e0',
        fillColor: 'transparent',
        weight: 1.2,
        opacity: 0.5,
        className: 'neon-circle-inner',
      }).addTo(overlayGroup);
      
      // 중앙 마커 - 은은하게 빛나는 효과
      L.circle(center, {
        radius: 4,
        color: '#ffffff',
        fillColor: '#00e0e0',
        fillOpacity: 0.8,
        weight: 1.5,
        className: 'neon-center-dot',
      }).addTo(overlayGroup);
      
      // 네온 효과 CSS를 동적으로 추가
      if (!document.getElementById('neon-css')) {
        const style = document.createElement('style');
        style.id = 'neon-css';
        style.innerHTML = `
          .neon-circle-outer {
            filter: drop-shadow(0 0 4px rgba(0, 224, 224, 0.6));
          }
          .neon-circle-middle, .neon-circle-inner {
            filter: drop-shadow(0 0 2px rgba(0, 224, 224, 0.5));
          }
          .neon-center-dot {
            filter: drop-shadow(0 0 4px rgba(0, 224, 224, 0.7));
          }
          .neon-sector-line {
            filter: drop-shadow(0 0 2px rgba(0, 224, 224, 0.5));
          }
          .sector-polygon-1 {
            filter: drop-shadow(0 0 3px rgba(0, 224, 224, 0.4));
          }
          .sector-polygon-2 {
            filter: drop-shadow(0 0 3px rgba(112, 208, 112, 0.4));
          }
          .sector-polygon-3 {
            filter: drop-shadow(0 0 3px rgba(224, 112, 224, 0.4));
          }
          .sector-label {
            text-shadow: 0 0 3px rgba(0, 224, 224, 0.6), 0 0 5px rgba(0, 224, 224, 0.4);
            font-weight: bold;
          }
        `;
        document.head.appendChild(style);
      }
      
      // 섹터 구분선 - 네온 효과 적용
      const createSectorLine = (angle) => {
        const angleRad = rad(angle);
        const endLat = center[0] + (circleRadius * Math.cos(angleRad) / 111320);
        const endLng = center[1] + (circleRadius * Math.sin(angleRad) / (111320 * Math.cos(rad(center[0]))));
        
        L.polyline([center, [endLat, endLng]], {
          color: '#00e0e0',
          weight: 1.5,
          opacity: 0.7,
          className: 'neon-sector-line',
        }).addTo(overlayGroup);
      };
      
      // 섹터를 120도씩 균등하게 나누기 위한 구분선 (0도, 120도, 240도)
      createSectorLine(0);
      createSectorLine(120);
      createSectorLine(240);
      
      // 거리 표시 레이블 추가 - 네온 효과 적용
      const addDistanceLabel = (distance, text) => {
        const labelLat = center[0] + (distance * Math.cos(Math.PI / 4) / 111320);
        const labelLng = center[1] + (distance * Math.sin(Math.PI / 4) / (111320 * Math.cos(Math.PI / 180 * center[0])));
        
        L.marker([labelLat, labelLng], {
          icon: L.divIcon({
            html: `<div style="color:#00e0e0; font-weight:bold; font-size:12px; text-shadow: 0 0 3px rgba(0, 224, 224, 0.7);">${text}</div>`,
            className: 'distance-label',
            iconSize: [60, 20],
            iconAnchor: [30, 10],
          })
        }).addTo(overlayGroup);
      };
      
      // 거리 레이블 추가
      addDistanceLabel(circleRadius * 0.4, '120m');
      addDistanceLabel(circleRadius * 0.7, '210m');
      addDistanceLabel(circleRadius, '300m');
      
      // 섹터 레이블 추가 - 네온 효과 적용 - 위치 조정
      const addSectorLabel = (angle, text, color) => {
        const angleRad = rad(angle);
        const labelDistance = circleRadius * 0.6; // 원래 거리로 복원
        
        // 오프셋 제거하고 각 섹터의 중심에 정확히 배치
        const labelLat = center[0] + (labelDistance * Math.cos(angleRad) / 111320);
        const labelLng = center[1] + (labelDistance * Math.sin(angleRad) / (111320 * Math.cos(rad(center[0]))));
        
        L.marker([labelLat, labelLng], {
          icon: L.divIcon({
            html: `<div style="color:${color}; font-weight:bold; font-size:15px; text-align:center; text-shadow: 0 0 3px ${color}, 0 0 5px rgba(255,255,255,0.3);">${text}</div>`,
            className: 'sector-label',
            iconSize: [100, 30],
            iconAnchor: [50, 15],
          })
        }).addTo(overlayGroup);
      };
      
      // 섹터 1, 2, 3 모두 표시 - 각 섹터의 정확한 중심에 배치
      addSectorLabel(60, 'CAMERA 1', '#00e0e0');    // 시안(청록색) - 1번 섹터 (0~120도 사이)
      addSectorLabel(180, 'CAMERA 2', '#70d070');   // 차트리우스(밝은 녹색) - 2번 섹터 (120~240도 사이)
      addSectorLabel(300, 'CAMERA 3', '#e070e0');   // 마젠타(분홍색) - 3번 섹터 (240~360도 사이)

      return () => {
        // 컴포넌트 언마운트 시 오버레이 제거
        map.removeLayer(overlayGroup);
      };
    }
  }, [map, center]);
  
  return null;
});

/**
 * MapView 컴포넌트
 * @param {*} props
 *  - mapCenter: 지도 초기 중심 좌표
 *  - mapRef: 지도 레퍼런스 (필요 시)
 *  - setShowCameraModal: 카메라 모달 표시/숨김 함수 (필요 시)
 *  - cameraStatus: 카메라별 상태 (오프라인/온라인 등)
 *  - language: 다국어 코드
 *  - isPanelOpen: 패널 열림 상태
 *  - leftPanelOpen: 왼쪽 패널 열림 상태
 *  - rightPanelOpen: 오른쪽 패널 열림 상태
 */
const MapView = ({
  mapCenter = [35.193097, 126.221395],
  mapRef = null,
  setShowCameraModal = () => {},
  cameraStatus = {
    1: { error: false },
    2: { error: false },
    3: { error: false }
  },
  language = 'ko',
  isPanelOpen = true,
  leftPanelOpen = true,
  rightPanelOpen = true
}) => {
  // ------ 공통 상태들 ------
  // 내부 mapRef 생성 (외부에서 전달되지 않은 경우 사용)
  const internalMapRef = useRef(null);
  // 실제 사용할 ref (외부에서 전달된 것 또는 내부에서 생성한 것)
  const actualMapRef = mapRef || internalMapRef;
  
  // 전역 상태 가져오기
  const { radarEnabled } = useAppStore();
  
  // 마지막으로 알려진 맵 객체
  const mapInstanceRef = useRef(null);
  
  // 패널 상태 변경 추적
  const prevLeftPanelOpen = useRef(leftPanelOpen);
  const prevRightPanelOpen = useRef(rightPanelOpen);
  
  // ------- 로그 데이터 관련 -------
  const [logEntries, setBirdLogs] = useState([]);
  const [isLoading, setIsLoading] = useState(false);
  
  // 애니메이션 효과를 위한 새 로그 상태
  const [newLogs, setNewLogs] = useState([]);
  
  // 로그 데이터 가져오기
  const fetchBirdLogs = useCallback(async () => {
    if (isLoading) return; // 이미 로딩 중이면 중복 호출 방지
    
    setIsLoading(true);
    try {
      // 백엔드 API 호출 - 최대 3번 재시도
      let retries = 0;
      const maxRetries = 3;
      let response = null;
      
      while (retries < maxRetries) {
        try {
          response = await axios.get('/api/detections/filtered/?per_page=20&sort_by=date_desc', {
            timeout: 5000 // 5초 타임아웃 설정
          });
          break; // 성공하면 루프 종료
        } catch (retryError) {
          retries++;
          console.warn(`로그 데이터 가져오기 재시도 (${retries}/${maxRetries})`, retryError);
          if (retries >= maxRetries) throw retryError;
          await new Promise(r => setTimeout(r, 1000)); // 1초 후 재시도
        }
      }
      
      if (response && response.data && response.data.detections) {
        // 기본 데이터 매핑
        const basicLogs = response.data.detections.map(detection => ({
          id: detection.detection_id,
          timestamp: detection.detection_time,
          species: '로딩 중...',
          count: detection.bb_count || detection.object_count || 1,
          camera: detection.camera_id || 1,
          riskLevel: detection.bb_count > 3 ? 'high' : 'low'
        }));
        
        // 종 정보 가져오기 - 비동기 병렬 처리
        const processedLogs = await Promise.all(
          basicLogs.map(async (log) => {
            try {
              // 바운딩 박스 정보에서 조류 종 가져오기 (타임아웃 설정)
              const bbResponse = await axios.get(`/api/detection/bb-info/${log.id}/`, {
                timeout: 3000
              });
              if (bbResponse.data && bbResponse.data.status === 'success' && bbResponse.data.bb_info && bbResponse.data.bb_info.length > 0) {
                // 가장 많은 종류 찾기
                const speciesCounts = {};
                bbResponse.data.bb_info.forEach(bb => {
                  const species = bb.class_name;
                  speciesCounts[species] = (speciesCounts[species] || 0) + 1;
                });
                
                // 가장 많이 나타난 종 결정
                let dominantSpecies = '';
                let maxCount = 0;
                
                Object.entries(speciesCounts).forEach(([species, count]) => {
                  if (count > maxCount) {
                    maxCount = count;
                    dominantSpecies = species;
                  }
                });
                
                // 종 정보 업데이트
                return {
                  ...log,
                  species: dominantSpecies || '미확인 조류'
                };
              }
            } catch (err) {
              console.error(`ID ${log.id}의 바운딩 박스 정보 가져오기 오류:`, err);
            }
            
            // 바운딩 박스 정보를 가져오지 못한 경우 기본값 사용
            return {
              ...log,
              species: '데이터 로딩 실패'
            };
          })
        );
        
        // 새로운 로그 항목 식별
        if (logEntries.length > 0) {
          const existingIds = new Set(logEntries.map(log => log.id));
          const newLogItems = processedLogs.filter(log => !existingIds.has(log.id));
          
          // 새 항목이 있으면 애니메이션 효과와 함께 추가
          if (newLogItems.length > 0) {
            setNewLogs(newLogItems);
            
            // 0.5초 후 새 항목을 기존 항목 앞에 추가
            setTimeout(() => {
              setBirdLogs(prev => {
                // 중복 항목 제거를 위해 Set 사용
                const uniqueLogsMap = new Map();
                
                // 새 로그 항목 추가
                newLogItems.forEach(log => {
                  uniqueLogsMap.set(log.id, log);
                });
                
                // 기존 로그 항목 추가 (중복되지 않는 경우에만)
                prev.forEach(log => {
                  if (!uniqueLogsMap.has(log.id)) {
                    uniqueLogsMap.set(log.id, log);
                  }
                });
                
                // Map에서 배열로 변환하고 최대 20개만 반환
                return Array.from(uniqueLogsMap.values()).slice(0, 20);
              });
              setNewLogs([]);
            }, 500);
          }
        } else {
          // 최초 로드 시에는 바로 설정
          setBirdLogs(processedLogs.slice(0, 20));
        }
      } else {
        // 빈 데이터인 경우 또는 데이터 구조가 예상과 다른 경우
        console.error('로그 데이터 형식 오류 또는 빈 데이터', response?.data);
        // 오류 시 빈 배열 반환
        setBirdLogs([]);
      }
    } catch (error) {
      console.error('로그 데이터 가져오기 오류:', error);
      // 오류 시 빈 배열 반환
      setBirdLogs([]);
    } finally {
      setIsLoading(false);
    }
  }, [logEntries, isLoading]);
  
  // 컴포넌트 마운트 시 로그 데이터 가져오기 및 10초마다 자동 새로고침
  useEffect(() => {
    fetchBirdLogs();
    
    // 10초마다 자동 새로고침
    const intervalId = setInterval(() => {
      fetchBirdLogs();
    }, 10000);
    
    // 컴포넌트 언마운트 시 interval 정리
    return () => {
      clearInterval(intervalId);
    };
  }, [fetchBirdLogs]);
  
  // 탭 상태
  const [currentTab, setCurrentTab] = useState(0);
  // 레이더 오버레이 표시 여부
  const [showRadarOverlay, setShowRadarOverlay] = useState(true);
  // 카메라 오버레이 표시 여부
  const [showCameraOverlay, setShowCameraOverlay] = useState(true);
  // 컴포넌트 마운트 상태 추적
  const isMountedRef = useRef(true);

  // 패널 상태 변경 시 맵 컨테이너 크기 재조정
  useEffect(() => {
    if (mapInstanceRef.current) {
      // 패널 상태가 실제로 변경되었는지 확인
      const leftChanged = prevLeftPanelOpen.current !== leftPanelOpen;
      const rightChanged = prevRightPanelOpen.current !== rightPanelOpen;
      
      if (leftChanged || rightChanged) {
        // 현재 중심 및 줌 레벨 저장
        const currentCenter = mapInstanceRef.current.getCenter();
        const currentZoom = mapInstanceRef.current.getZoom();
        
        // 레이아웃 변경 후 맵 크기 재계산에 딜레이 추가
        setTimeout(() => {
          if (mapInstanceRef.current) {
            // 맵 크기 재계산
            mapInstanceRef.current.invalidateSize({
              animate: true,
              pan: true,
              debounceMoveEnd: true,
              duration: 0.25
            });
            
            // 중심점 복원
            mapInstanceRef.current.setView(currentCenter, currentZoom, {
              animate: true,
              duration: 0.5,
              noMoveStart: true
            });
          }
        }, 300);
        
        // 상태 업데이트
        prevLeftPanelOpen.current = leftPanelOpen;
        prevRightPanelOpen.current = rightPanelOpen;
      }
    }
  }, [leftPanelOpen, rightPanelOpen]);

  // 맵 객체에 대한 레퍼런스 저장
  const handleMapInstance = useCallback((instance) => {
    if (instance) {
      mapInstanceRef.current = instance;
      
      // 외부에서 제공된 ref에 맵 인스턴스 저장
      if (actualMapRef && typeof actualMapRef === 'object') {
        actualMapRef.current = {
          ...actualMapRef.current,
          leafletElement: instance
        };
      }
      
      // 초기 생성 시 맵 중심 설정
      setTimeout(() => {
        if (instance) {
          instance.setView(mapCenter, instance.getZoom());
          
          // 맵 중심 유지를 위한 이벤트 리스너 추가
          instance.on('resize', () => {
            instance.setView(instance.getCenter(), instance.getZoom(), {
              animate: false
            });
          });
        }
      }, 100);
    }
  }, [actualMapRef, mapCenter]);

  // 마운트/언마운트 처리
  useEffect(() => {
    isMountedRef.current = true;
    
    return () => {
      isMountedRef.current = false;
    };
  }, []);

  // ------ 레이더 관련 상태 ------
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [isConnected, setIsConnected] = useState(false);
  const [operationMode, setOperationMode] = useState('');

  // ------ 카메라 관련 상태 ------
  const [cameras, setCameras] = useState([
    { id: '1', position: [35.192962, 126.221627], direction: 60, angle: 120, color: '#00e0e0', sector: 'CAMERA 1' },
    { id: '2', position: [35.192962, 126.221627], direction: 180, angle: 120, color: '#70d070', sector: 'CAMERA 2' },
    { id: '3', position: [35.192962, 126.221627], direction: 300, angle: 120, color: '#e070e0', sector: 'CAMERA 3' }
  ]);
  
  // 카메라 위치들의 중심점 계산
  const calculateCamerasCenter = useCallback(() => {
    if (cameras.length === 0) return mapCenter;
    
    // 모든 카메라 위치의 위도, 경도 평균 계산
    const sumLat = cameras.reduce((sum, camera) => sum + camera.position[0], 0);
    const sumLng = cameras.reduce((sum, camera) => sum + camera.position[1], 0);
    
    return [sumLat / cameras.length, sumLng / cameras.length];
  }, [cameras, mapCenter]);
  
  // 카메라들의 중심점 계산
  const camerasCenter = calculateCamerasCenter();
  
  // ------ PTZ 제어용 상태 ------
  const [ptzCamera, setPtzCamera] = useState(null);

  // ------ 이벤트 핸들러: 탭 전환 ------
  const handleTabChange = (event, newValue) => {
    // 레이더 기능이 비활성화되어 있을 때 레이더 탭(2)으로 전환 시도시 무시
    if (!radarEnabled && newValue === 2) {
      return;
    }
    
    console.log(`탭 변경: ${currentTab} → ${newValue}`);
    
    // 이전 탭의 리소스 정리
    if (currentTab === 2 && newValue !== 2) {
      console.log('레이더 탭에서 다른 탭으로 전환: 데이터 정리');
      // 레이더 탭에서 나갈 때는 데이터 정리
      setPlotData([]);
      setTrackData([]);
    }
    
    // 탭 전환 시 MapContainer가 즉시 제거되고 재생성되도록 딜레이 추가
    setTimeout(() => {
      if (isMountedRef.current) {
        setCurrentTab(newValue);
      }
    }, 50);
  };

  // ------ 카메라 목록 가져오기 ------
  // 실제로는 백엔드 API에서 카메라 정보를 가져와 기존 cameras 배열과 병합 가능
  useEffect(() => {
    const fetchCameras = async () => {
      try {
        // API 모듈 사용
        const response = await cameraAPI.getCameras();

        if (response.data && response.data.cameras && response.data.cameras.length > 0) {
          // 기존 cameras 와 merge (단순화 예시)
          const updatedCameras = cameras.map((cam) => {
            const found = response.data.cameras.find((bc) => bc.camera_id === cam.id);
            return found
              ? { ...cam, status: found.status }
              : cam;
          });
          setCameras(updatedCameras);
        }
      } catch (e) {
        console.error('카메라 목록 가져오기 에러:', e);
      }
    };
    fetchCameras();
  }, []);
  
  // PTZ 제어용 카메라 선택 핸들러
  const handlePtzCameraSelect = (cameraId) => {
    // 현재 선택된 카메라와 동일하면 선택 해제
    if (ptzCamera === cameraId) {
      setPtzCamera(null);
    } else {
      // 다른 카메라 선택
      setPtzCamera(cameraId);
    }
  };

  // PTZ 제어 함수
  const controlPtz = (direction, isContinuous = true) => {
    if (!ptzCamera) return;

    axios.post(`/api/ptz/control/`, {
      camera_id: ptzCamera,
      direction: direction,
      is_continuous: isContinuous,
      speed: 0.7
    })
    .then(response => {
      console.log('PTZ 제어 성공:', response.data);
    })
    .catch(error => {
      console.error('PTZ 제어 오류:', error);
      // 백업 중지 메소드 호출 - 에러가 발생해도 중지 시도
      if (!isContinuous) {
        axios.post(`/api/ptz/stop/`, {
          camera_id: ptzCamera,
          direction: direction
        }).catch(err => console.error('PTZ 중지 오류:', err));
      }
    });
  };

  // PTZ 중지 함수
  const stopPtz = (direction) => {
    if (!ptzCamera) return;
    controlPtz(direction, false);
  };

  // PTZ 제어 UI 렌더링 함수
  const renderPtzControls = () => {
    return (
      <Box sx={{ p: 1 }}>
        {ptzCamera ? (
          <>
            <Box sx={{ textAlign: 'center', mb: 1 }}>
              <Typography variant="body2" sx={{ color: '#fff', fontWeight: 'bold' }}>
                {translate(`카메라 ${ptzCamera}`, `Camera ${ptzCamera}`, language)}
              </Typography>
            </Box>
            <Box sx={{ 
              display: 'flex', 
              flexDirection: 'column', 
              alignItems: 'center',
              justifyContent: 'center',
              backgroundColor: '#162b44',
              borderRadius: '8px',
              p: 1.5,
              height: '200px'
            }}>
              {/* PTZ 컨트롤 레이아웃 */}
              <Box sx={{ 
                position: 'relative',
                width: '100%',
                height: '100%',
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center',
                justifyContent: 'center'
              }}>
                {/* 상단: 방향 컨트롤 */}
                <Box sx={{ 
                  position: 'relative', 
                  width: '140px', 
                  height: '140px', 
                  display: 'flex',
                  justifyContent: 'center',
                  alignItems: 'center',
                  mb: 1
                }}>
                  {/* 중앙 원 */}
                  <Box sx={{ 
                    width: '50px', 
                    height: '50px', 
                    borderRadius: '50%', 
                    backgroundColor: '#0d1929',
                    position: 'absolute',
                    zIndex: 1
                  }}></Box>

                  {/* 상 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      top: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('UP')}
                    onMouseUp={() => stopPtz('UP')}
                    onMouseLeave={() => stopPtz('UP')}
                    onTouchStart={() => controlPtz('UP')}
                    onTouchEnd={() => stopPtz('UP')}
                  >
                    <Box component="span">▲</Box>
                  </IconButton>

                  {/* 하 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      bottom: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('DOWN')}
                    onMouseUp={() => stopPtz('DOWN')}
                    onMouseLeave={() => stopPtz('DOWN')}
                    onTouchStart={() => controlPtz('DOWN')}
                    onTouchEnd={() => stopPtz('DOWN')}
                  >
                    <Box component="span">▼</Box>
                  </IconButton>

                  {/* 좌 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      left: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('LEFT')}
                    onMouseUp={() => stopPtz('LEFT')}
                    onMouseLeave={() => stopPtz('LEFT')}
                    onTouchStart={() => controlPtz('LEFT')}
                    onTouchEnd={() => stopPtz('LEFT')}
                  >
                    <Box component="span">◀</Box>
                  </IconButton>

                  {/* 우 버튼 */}
                  <IconButton 
                    size="small"
                    sx={{ 
                      position: 'absolute',
                      right: 0,
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      zIndex: 2,
                      padding: '4px',
                      minWidth: '32px',
                      width: '32px',
                      height: '32px'
                    }}
                    onMouseDown={() => controlPtz('RIGHT')}
                    onMouseUp={() => stopPtz('RIGHT')}
                    onMouseLeave={() => stopPtz('RIGHT')}
                    onTouchStart={() => controlPtz('RIGHT')}
                    onTouchEnd={() => stopPtz('RIGHT')}
                  >
                    <Box component="span">▶</Box>
                  </IconButton>
                </Box>

                {/* 하단: 줌 컨트롤 (가로 배치) */}
                <Box sx={{ 
                  display: 'flex', 
                  flexDirection: 'row', 
                  justifyContent: 'center',
                  alignItems: 'center',
                  gap: 2
                }}>
                  <IconButton 
                    size="small"
                    sx={{ 
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      width: '32px',
                      height: '32px',
                      padding: '4px'
                    }}
                    onMouseDown={() => controlPtz('ZOOM_IN')}
                    onMouseUp={() => stopPtz('ZOOM_IN')}
                    onMouseLeave={() => stopPtz('ZOOM_IN')}
                    onTouchStart={() => controlPtz('ZOOM_IN')}
                    onTouchEnd={() => stopPtz('ZOOM_IN')}
                  >
                    <Box component="span">+</Box>
                  </IconButton>
                  <IconButton 
                    size="small"
                    sx={{ 
                      color: '#fff', 
                      backgroundColor: '#1e3a5a', 
                      '&:hover': { backgroundColor: '#2c4f7c' },
                      width: '32px',
                      height: '32px',
                      padding: '4px'
                    }}
                    onMouseDown={() => controlPtz('ZOOM_OUT')}
                    onMouseUp={() => stopPtz('ZOOM_OUT')}
                    onMouseLeave={() => stopPtz('ZOOM_OUT')}
                    onTouchStart={() => controlPtz('ZOOM_OUT')}
                    onTouchEnd={() => stopPtz('ZOOM_OUT')}
                  >
                    <Box component="span">-</Box>
                  </IconButton>
                </Box>
              </Box>
            </Box>
          </>
        ) : (
          <Box sx={{ 
            display: 'flex', 
            justifyContent: 'center', 
            alignItems: 'center', 
            height: '200px', 
            color: '#aaa' 
          }}>
            <Typography variant="body2">
              {translate('카메라를 선택하세요', 'Select a camera', language)}
            </Typography>
          </Box>
        )}
      </Box>
    );
  };

  // PTZ용 카메라 목록 컴포넌트
  const PtzCameraListItems = () => {
    return (
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 1, justifyContent: 'center' }}>
        {cameras.map((camera) => {
          const isSelected = ptzCamera === camera.id;
          
          return (
            <Button
              key={`ptz-${camera.id}`}
              variant={isSelected ? "contained" : "outlined"}
              size="small"
              onClick={() => handlePtzCameraSelect(camera.id)}
              sx={{
                minWidth: '40px',
                height: '32px',
                backgroundColor: isSelected ? '#1e88e5' : 'transparent',
                color: isSelected ? '#fff' : '#4caf50',
                borderColor: '#4caf50',
                '&:hover': {
                  backgroundColor: isSelected ? '#1976d2' : 'rgba(30, 136, 229, 0.08)'
                },
                fontSize: '0.8rem',
                fontWeight: isSelected ? 'bold' : 'normal'
              }}
            >
              {translate(`카메라 ${camera.id}`, `Camera ${camera.id}`, language)}
            </Button>
          );
        })}
      </Box>
    );
  };

  // ------ 레이더 데이터 폴링 ------
  useEffect(() => {
    // 레이더 기능이 비활성화되어 있으면 데이터 폴링하지 않음
    if (!radarEnabled) return;
    
    let intervalId = null;
    let isMounted = true;

    const fetchRadarData = async () => {
      if (!isMounted) return;
      
      try {
        // 레이더 데이터 가져오기 - 최대 3번 재시도
        let retries = 0;
        const maxRetries = 3;
        let response = null;
        
        while (retries < maxRetries) {
          try {
            response = await radarEmulatorAPI.getSimulationStatus();
            break; // 성공하면 루프 종료
          } catch (retryError) {
            retries++;
            console.warn(`레이더 데이터 가져오기 재시도 (${retries}/${maxRetries})`, retryError);
            if (retries >= maxRetries) throw retryError;
            await new Promise(r => setTimeout(r, 500)); // 0.5초 후 재시도
          }
        }
        
        const data = response?.data || {};

        // 연결 상태
        if (data.connected !== undefined) {
          setIsConnected(data.connected);
        } else {
          // connected 필드가 없으면 오류로 간주
          setIsConnected(false);
        }

        // 운영 모드
        if (data.operation_mode) {
          setOperationMode(data.operation_mode);
        } else {
          setOperationMode('데이터 없음');
        }

        // 플롯 데이터
        if (data.plots && data.plots.length > 0) {
          setPlotData(data.plots);
        } else if (data.Plot && data.Plot.length > 0) {
          setPlotData(data.Plot);
        } else if (data.nPlot && data.Plot && data.Plot.length > 0) {
          setPlotData(data.Plot);
        } else {
          setPlotData([]);
        }

        // 트랙 데이터
        if (data.tracks && data.tracks.length > 0) {
          setTrackData(data.tracks);
        } else if (data.Track && data.Track.length > 0) {
          setTrackData(data.Track);
        } else if (data.nTrack && data.Track && data.Track.length > 0) {
          setTrackData(data.Track);
        } else {
          setTrackData([]);
        }
      } catch (err) {
        console.error('레이더 데이터 가져오기 오류:', err);
        setPlotData([]);
        setTrackData([]);
        setIsConnected(false);
        setOperationMode('연결 오류');
      }
    };

    if (currentTab === 2) {
      // 레이더 모니터링 탭일 때만 폴링
      fetchRadarData();
      intervalId = setInterval(fetchRadarData, 1000);
    }

    return () => {
      isMounted = false;
      if (intervalId) {
        clearInterval(intervalId);
      }
    };
  }, [currentTab, radarEnabled]);

  // ------ 스피커 제어 함수 ------
  const playSpeakerSound = (soundId) => {
    axios.post('/camera/speaker/play_sound/', { 
      sound_index: soundId,
      repeat: false
    })
      .then(response => {
        console.log(`스피커 사운드 ${soundId} 재생 성공:`, response.data);
      })
      .catch(error => {
        console.error(`스피커 사운드 ${soundId} 재생 오류:`, error);
      });
  };

  const stopSpeakerSound = () => {
    axios.post('/camera/speaker/stop_sound/')
      .then(response => {
        console.log('스피커 사운드 중지 성공:', response.data);
      })
      .catch(error => {
        console.error('스피커 사운드 중지 오류:', error);
      });
  };

  return (
    <Paper
      elevation={0}
      sx={{
        height: '100%',
        borderRadius: 2,
        backgroundColor: '#0a1929',
        border: '1px solid #1e3a5a',
        overflow: 'hidden',
        display: 'flex',
        flexDirection: 'column'
      }}
    >
      {/* 상단 탭 헤더 */}
      <Box
        sx={{
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
          borderBottom: '1px solid #1e3a5a'
        }}
      >
        <Tabs
          value={currentTab}
          onChange={handleTabChange}
          textColor="inherit"
          sx={{
            '& .MuiTabs-indicator': {
              backgroundColor: 'lightblue'
            },
            '& .MuiTab-root': {
              color: 'white',
              fontWeight: 'bold',
              fontSize: '0.9rem',
              '&.Mui-selected': { color: 'lightblue' }
            }
          }}
        >
          <Tab label={translate('개별 카메라 화면', 'Individual Camera View', language)} />
          <Tab label={translate('조류 위치 현황', 'Bird Location Status', language)} />
          {radarEnabled && (
            <Tab label={translate('조류 레이더 현황', 'Bird Radar Status', language)} />
          )}
        </Tabs>

        {/* 컨트롤 버튼 영역 */}
        <Box sx={{ p: 1, display: 'flex' }}>
          {/* 레이더 오버레이 토글 버튼 - 레이더 기능 활성화된 경우에만 표시 */}
          {radarEnabled && (
            <Tooltip title={translate('레이더 오버레이', 'Radar Overlay', language)}>
              <IconButton
                size="small"
                sx={{
                  color: showRadarOverlay ? 'lightblue' : 'white',
                  mr: 1
                }}
                onClick={() => setShowRadarOverlay((prev) => !prev)}
              >
                {/* SVG 아이콘 (Radar) */}
                <svg
                  xmlns="http://www.w3.org/2000/svg"
                  width="16"
                  height="16"
                  fill="currentColor"
                  className="bi bi-radar"
                  viewBox="0 0 16 16"
                >
                  <path d="M8 1a7 7 0 0 1 7 7v.5A3.49 3.49 0 0 0 13.5 8a.5.5 0 0 1 0-1 4.49 4.49 0 0 1 3.5 4.5A3.5 3.5 0 0 1 13.5 15h-9a3.5 3.5 0 0 1-3.5-3.5A4.49 4.49 0 0 1 4.5 7a.5.5 0 0 1 0 1A3.49 3.49 0 0 0 3 9.5V8a7 7 0 0 1 7-7M8 0a8 8 0 0 0-8 8 4.5 4.5 0 0 0 1.7 3.5A4.49 4.49 0 0 0 0 15.5 4.5 4.5 0 0 0 4.5 20h7a4.5 4.5 0 0 0 4.5-4.5 4.49 4.49 0 0 0-1.7-3.5A4.5 4.5 0 0 0 16 8a8 8 0 0 0-8-8M7 5.5a.5.5 0 0 1 1 0v3a.5.5 0 0 1-1 0z" />
                </svg>
              </IconButton>
            </Tooltip>
          )}

          {/* 카메라 오버레이 토글 버튼 */}
          <Tooltip title={translate('카메라 오버레이', 'Camera Overlay', language)}>
            <IconButton
              size="small"
              sx={{
                color: showCameraOverlay ? 'lightblue' : 'white',
                mr: 1
              }}
              onClick={() => setShowCameraOverlay((prev) => !prev)}
            >
              <CameraIcon fontSize="small" />
            </IconButton>
          </Tooltip>

          {/* 스피커 사운드 0 시작 버튼 */}
          <Tooltip title={translate('사운드 0 시작', 'Play Sound 0', language)}>
            <IconButton
              size="small"
              sx={{
                color: 'white',
                backgroundColor: '#1e3a5a',
                mr: 1,
                '&:hover': { backgroundColor: '#2c4f7c' }
              }}
              onClick={() => playSpeakerSound(0)}
            >
              <Box display="flex" alignItems="center">
                <VolumeUpIcon fontSize="small" />
          
              </Box>
            </IconButton>
          </Tooltip>

          {/* 스피커 사운드 중단 버튼 */}
          <Tooltip title={translate('사운드 중단', 'Stop Sound', language)}>
            <IconButton
              size="small"
              sx={{
                color: 'white',
                backgroundColor: '#831919', 
                '&:hover': { backgroundColor: '#a82626' }
              }}
              onClick={stopSpeakerSound}
            >
              <VolumeOffIcon fontSize="small" />
            </IconButton>
          </Tooltip>
        </Box>
      </Box>

      {/* 탭 컨텐츠 영역 - 공통 컨테이너 */}
      <Box sx={{ flex: 1, position: 'relative', overflow: 'hidden' }}>
        {/* -- 0. 통합 모니터링 탭 -- */}
        <Box
          sx={{
            position: 'absolute',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            display: currentTab === 0 ? 'block' : 'none',
            zIndex: currentTab === 0 ? 10 : 1,
            p: 1,
            backgroundColor: '#020b18'
          }}
        >
          <Grid container spacing={1} sx={{ height: '100%' }}>
            {cameras.map((cam) => (
              <Grid key={cam.id} item xs={12} md={6} sx={{ height: '50%' }}>
                <Paper elevation={0} sx={{ 
                  height: '100%', 
                  backgroundColor: '#041530',
                  border: '1px solid #00c5ff',
                  borderRadius: '4px',
                  boxShadow: '0 0 10px rgba(0, 197, 255, 0.3)',
                  overflow: 'hidden',
                  position: 'relative'
                }}>
                  <Box sx={{ 
                    position: 'absolute',
                    top: 0,
                    left: 0,
                    padding: '6px 12px',
                    zIndex: 10,
                    color: '#00c5ff',
                    fontWeight: 'bold',
                    fontSize: '0.9rem',
                    letterSpacing: '1px'
                  }}>
                    CAMERA {cam.id}
                  </Box>
                  <CameraStream
                    cameraId={cam.id}
                    title={translate(`카메라 ${cam.id}`, `Camera ${cam.id}`, language)}
                    language={language}
                    containerStyles={{
                      border: 'none',
                      backgroundColor: 'transparent'
                    }}
                    titleStyles={{
                      display: 'none'
                    }}
                  />
                </Paper>
              </Grid>
            ))}

            {/* 카메라가 4개 미만이면 로그 패널 표시 */}
            {cameras.length < 4 && (
              <Grid item xs={6} sx={{ height: '50%' }}>
                <SidePanel sx={{ 
                  height: '100%',
                  backgroundColor: '#041530',
                  border: '1px solid #00c5ff',
                  boxShadow: '0 0 10px rgba(0, 197, 255, 0.3)',
                  borderRadius: '4px'
                }}>
                  <Box sx={{ 
                    p: 0.8, 
                    py: 0.7,
                    backgroundColor: '#041530', 
                    display: 'flex', 
                    justifyContent: 'space-between',
                    alignItems: 'center', 
                    borderTopLeftRadius: 0, 
                    borderTopRightRadius: 0,
                    borderBottom: '1px solid rgba(0, 197, 255, 0.4)'
                  }}>
                    <Typography variant="subtitle2" sx={{ 
                      color: '#00c5ff', 
                      fontWeight: 'bold',
                      fontSize: '0.9rem',
                      ml: 0.5,
                      letterSpacing: '1px',
                      textTransform: 'uppercase'
                    }}>
                      {translate('알림 로그', 'Alert Log', language)}
                    </Typography>
                    <Box sx={{ flexGrow: 1 }} />
                    <Tooltip title={translate('새로고침', 'Refresh', language)}>
                      <span>
                        <IconButton 
                          size="small" 
                          onClick={fetchBirdLogs}
                          disabled={isLoading}
                          sx={{ color: '#00c5ff' }}
                        >
                          <RefreshIcon fontSize="small" />
                        </IconButton>
                      </span>
                    </Tooltip>
                  </Box>
                  
                  <Box sx={{ 
                    height: 'calc(100% - 50px)', 
                    overflow: 'auto',
                    p: 0,
                    backgroundColor: '#041530' 
                  }}>
                    {isLoading && logEntries.length === 0 ? (
                      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100%' }}>
                        <Typography variant="body2" sx={{ color: '#aaa' }}>
                          {translate('로딩 중...', 'Loading...', language)}
                        </Typography>
                      </Box>
                    ) : logEntries.length > 0 ? (
                      <Box sx={{ p: 0 }}>
                        {/* 새로 추가되는 로그 (애니메이션 효과) */}
                        {newLogs.map((log, index) => (
                          <Box
                            key={`new-${log.id}-${index}`}
                            sx={{ 
                              mb: 0, 
                              height: '52px',
                              borderRadius: 0,
                              backgroundColor: '#041530',
                              borderBottom: '1px solid rgba(0, 197, 255, 0.3)',
                              borderLeft: 'none',
                              borderRight: 'none',
                              borderTop: 'none',
                              overflow: 'hidden',
                              position: 'relative',
                              boxShadow: 'inset 0 0 15px rgba(0,0,0,0.3)',
                              filter: 'contrast(1.05) brightness(0.95)',
                              // 애니메이션 효과
                              animation: 'fadeInTop 0.5s ease-out forwards',
                              opacity: 0,
                              transform: 'translateY(-20px)',
                              '@keyframes fadeInTop': {
                                '0%': {
                                  opacity: 0,
                                  transform: 'translateY(-20px)'
                                },
                                '100%': {
                                  opacity: 1,
                                  transform: 'translateY(0)'
                                }
                              }
                            }}
                          >
                            {/* 경고 아이콘 */}
                            <Box 
                              sx={{ 
                                position: 'absolute', 
                                left: 10, 
                                top: '50%',
                                transform: 'translateY(-50%)',
                                color: '#ffb700',
                                display: 'flex',
                                alignItems: 'center',
                                zIndex: 2
                              }}
                            >
                              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                                <path d="M12 22C17.5228 22 22 17.5228 22 12C22 6.47715 17.5228 2 12 2C6.47715 2 2 6.47715 2 12C2 17.5228 6.47715 22 12 22Z" fill="#ffb700" fillOpacity="0.2" stroke="#ffb700" strokeWidth="2"/>
                                <path d="M12 8V12" stroke="#ffb700" strokeWidth="2" strokeLinecap="round"/>
                                <circle cx="12" cy="16" r="1" fill="#ffb700"/>
                              </svg>
                            </Box>
                            
                            {/* 컨텐츠 */}
                            <Box sx={{ 
                              pl: 5, 
                              pr: 2, 
                              py: 0.5,
                              display: 'flex',
                              flexDirection: 'column',
                              justifyContent: 'space-between',
                              height: '100%',
                              zIndex: 3,
                              position: 'relative'
                            }}>
                              <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                                <Typography sx={{ 
                                  color: '#00c5ff', 
                                  fontWeight: 'bold',
                                  fontSize: '0.85rem',
                                  letterSpacing: '0.5px'
                                }}>
                                  {`SG-${String(log.camera).padStart(2, '0')}`}
                                </Typography>
                                <Typography sx={{ 
                                  color: '#00c5ff', 
                                  fontSize: '0.75rem',
                                  ml: 1
                                }}>
                                  {log.timestamp ? new Date(log.timestamp).toLocaleTimeString('ko-KR', {hour: '2-digit', minute: '2-digit', second: '2-digit', hour12: false}) : '09:30:49'}
                                </Typography>
                              </Box>
                              <Box sx={{ 
                                display: 'flex', 
                                justifyContent: 'space-between',
                                alignItems: 'center'
                              }}>
                                <Typography 
                                  variant="caption" 
                                  sx={{ 
                                    color: '#8eacbb',
                                    fontSize: '0.75rem'
                                  }}
                                >
                                  {log.species}
                                </Typography>
                                <Typography 
                                  variant="caption" 
                                  sx={{ 
                                    color: '#8eacbb',
                                    fontSize: '0.7rem'
                                  }}
                                >
                                  {translate('개수', 'Count', language)}: {log.count}
                                </Typography>
                              </Box>
                            </Box>
                          </Box>
                        ))}
                        
                        {/* 기존 로그 항목 */}
                        {logEntries.map((log, index) => (
                          <Box
                            key={`log-${log.id}-${index}`}
                            sx={{ 
                              mb: 0, 
                              height: '52px',
                              borderRadius: 0,
                              backgroundColor: '#041530',
                              borderBottom: '1px solid rgba(0, 197, 255, 0.3)',
                              borderLeft: 'none',
                              borderRight: 'none',
                              borderTop: 'none',
                              overflow: 'hidden',
                              position: 'relative',
                              transition: 'all 0.2s ease'
                            }}
                          >
                            {/* 경고 아이콘 */}
                            <Box 
                              sx={{ 
                                position: 'absolute', 
                                left: 10, 
                                top: '50%',
                                transform: 'translateY(-50%)',
                                color: '#ffb700',
                                display: 'flex',
                                alignItems: 'center',
                                zIndex: 2
                              }}
                            >
                              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                                <path d="M12 22C17.5228 22 22 17.5228 22 12C22 6.47715 17.5228 2 12 2C6.47715 2 2 6.47715 2 12C2 17.5228 6.47715 22 12 22Z" fill="#ffb700" fillOpacity="0.2" stroke="#ffb700" strokeWidth="2"/>
                                <path d="M12 8V12" stroke="#ffb700" strokeWidth="2" strokeLinecap="round"/>
                                <circle cx="12" cy="16" r="1" fill="#ffb700"/>
                              </svg>
                            </Box>
                            
                            {/* 컨텐츠 */}
                            <Box sx={{ 
                              pl: 5, 
                              pr: 2, 
                              py: 0.5,
                              display: 'flex',
                              flexDirection: 'column',
                              justifyContent: 'space-between',
                              height: '100%',
                              zIndex: 3,
                              position: 'relative'
                            }}>
                              <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                                <Typography sx={{ 
                                  color: '#00c5ff', 
                                  fontWeight: 'bold',
                                  fontSize: '0.85rem',
                                  letterSpacing: '0.5px'
                                }}>
                                  {`SG-${String(log.camera).padStart(2, '0')}`}
                                </Typography>
                                <Typography sx={{ 
                                  color: '#00c5ff', 
                                  fontSize: '0.75rem',
                                  ml: 1
                                }}>
                                  {log.timestamp ? new Date(log.timestamp).toLocaleTimeString('ko-KR', {hour: '2-digit', minute: '2-digit', second: '2-digit', hour12: false}) : '09:30:49'}
                                </Typography>
                              </Box>
                              <Box sx={{ 
                                display: 'flex', 
                                justifyContent: 'space-between',
                                alignItems: 'center'
                              }}>
                                <Typography 
                                  variant="caption" 
                                  sx={{ 
                                    color: '#8eacbb',
                                    fontSize: '0.75rem'
                                  }}
                                >
                                  {log.species}
                                </Typography>
                                <Typography 
                                  variant="caption" 
                                  sx={{ 
                                    color: '#8eacbb',
                                    fontSize: '0.7rem'
                                  }}
                                >
                                  {translate('개수', 'Count', language)}: {log.count}
                                </Typography>
                              </Box>
                            </Box>
                          </Box>
                        ))}
                      </Box>
                    ) : (
                      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100%', backgroundColor: '#041530' }}>
                        <Typography variant="body2" sx={{ color: '#00c5ff', textAlign: 'center' }}>
                          {translate('데이터가 없습니다', 'No data available', language)}
                        </Typography>
                      </Box>
                    )}
                  </Box>
                </SidePanel>
              </Grid>
            )}
          </Grid>
        </Box>

        {/* -- 1. 지도 모니터링 탭 -- */}
        <Box
          sx={{
            position: 'absolute',
            top: 0,
            left: 0,
            right: 0,
            bottom: 0,
            display: currentTab === 1 ? 'block' : 'none',
            zIndex: currentTab === 1 ? 10 : 1
          }}
        >
          {/* 지도 전체 화면 + 카메라 오버레이 */}
          <Box sx={{ position: 'relative', width: '100%', height: '100%' }}>
            {/* 지도 영역 (전체 화면) */}
            <Box sx={{ width: '100%', height: '100%', position: 'relative' }}>
              {currentTab === 1 && (
                <MapContainer
                  center={mapCenter}
                  zoom={17}
                  style={{ 
                    width: '100%', 
                    height: '100%', 
                    borderRadius: '5px',
                    transition: 'all 0.3s ease-in-out'
                  }}
                  ref={actualMapRef}
                  key={`map-container-${currentTab}-${leftPanelOpen ? 'left-open' : 'left-closed'}-${rightPanelOpen ? 'right-open' : 'right-closed'}`}
                  whenCreated={handleMapInstance}
                  zoomControl={false} // 줌 컨트롤을 비활성화하고 별도로 추가
                  attributionControl={false} // attribution 컨트롤 비활성화
                >
                  {/* 위성 타일 레이어 */}
                  <TileLayer
                    url="https://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}"
                    subdomains={['mt0', 'mt1', 'mt2', 'mt3']}
                    attribution=""
                    maxZoom={20}
                  />
                  
                  {/* 레이더 스타일 원형 오버레이 추가 - 카메라들의 중심점 사용 */}
                  <RadarCircleOverlay center={camerasCenter} />
                  
                  {/* 맵 로드 완료 후 줌 컨트롤 및 홈 버튼 추가 */}
                  <ZoomAndHomeControl mapCenter={mapCenter} />
                  
                  {/* 카메라 마커 및 시야각 표시 */}
                  {cameras.map((camera) => (
                    <React.Fragment key={camera.id}>
                      <CircleMarker
                        center={camera.position}
                        radius={10}
                        pathOptions={{
                          fillColor: camera.color,
                          fillOpacity: 0.8,
                          color: '#ffffff',
                          weight: 2
                        }}
                      >
                        <Popup>
                          {translate(`카메라${camera.id}`, `Camera ${camera.id}`, language)}
                        </Popup>
                      </CircleMarker>
                      
                      {/* 시야각을 부채꼴로 표시 - 네온 효과 적용 */}
                      <Polygon
                        positions={createViewField(
                          camera.position,
                          camera.direction,
                          camera.angle,
                          300
                        )}
                        pathOptions={{
                          color: camera.color,
                          fillColor: camera.color,
                          fillOpacity: 0.13,
                          weight: 1.8,
                          opacity: 0.7,
                          // 네온 효과를 위한 클래스 지정
                          className: `sector-polygon-${camera.id}`,
                        }}
                      >
                        <Popup>
                          <span style={{ 
                            color: camera.color, 
                            fontWeight: 'bold',
                            textShadow: `0 0 2px ${camera.color}` 
                          }}>
                            {camera.sector} - {translate('시야각', 'View Angle', language)}: {camera.angle}°
                          </span>
                        </Popup>
                      </Polygon>

                      {/* 부채꼴 내부에 섹터 텍스트 추가 - 네온 효과 적용 */}
                      {mapInstanceRef.current && camera.position && (
                        <div 
                          style={{
                            position: 'absolute',
                            // 방향 각도 기준으로 부채꼴 중앙에 위치 (NaN 방지를 위한 안전장치 추가)
                            left: `${(mapInstanceRef.current.latLngToLayerPoint(camera.position)?.x || 0) + (Math.cos((camera.direction * Math.PI) / 180) * 90) - 35}px`,
                            top: `${(mapInstanceRef.current.latLngToLayerPoint(camera.position)?.y || 0) + (Math.sin((camera.direction * Math.PI) / 180) * 90) - 10}px`,
                            color: camera.color,
                            fontWeight: 'bold',
                            fontSize: '13px',
                            textShadow: `0 0 3px ${camera.color}`,
                            whiteSpace: 'nowrap',
                            zIndex: 1000,
                            pointerEvents: 'none',
                            opacity: 0.85
                          }}
                          className="leaflet-zoom-animated"
                        >
                          {camera.sector}
                        </div>
                      )}
                    </React.Fragment>
                  ))}
                </MapContainer>
              )}
            </Box>

            {/* 카메라 스트림 오버레이 (지도 위에 표시) */}
            {showCameraOverlay && (
              <Box
                sx={{
                  position: 'absolute',
                  top: 16,
                  right: 16,
                  width: '16%', // 25%에서 18%로 너비 감소
                  zIndex: 1000,
                  display: 'flex',
                  flexDirection: 'column',
                  gap: 1 // 카메라 간격도 줄임
                }}
              >
                {cameras.map((camera) => (
                  <Paper
                    key={camera.id}
                    elevation={3}
                    sx={{
                      borderRadius: 2,
                      overflow: 'hidden',
                      border: `2px solid ${camera.color}`,
                      aspectRatio: '16/9', // 16:9 비율 설정
                      width: '100%'
                    }}
                  >
                    <CameraStream
                      cameraId={camera.id}
                      title={translate(`카메라 ${camera.id}`, `Camera ${camera.id}`, language)}
                      height="100%"
                      language={language}
                      hideRefreshButton={true}
                      hideControls={true}
                    />
                  </Paper>
                ))}
              </Box>
            )}
          </Box>
        </Box>

        {/* -- 2. 레이더 모니터링 탭 -- 레이더 기능 활성화된 경우에만 표시 */}
        {radarEnabled && (
          <Box
            sx={{
              position: 'absolute',
              top: 0,
              left: 0,
              right: 0,
              bottom: 0,
              display: currentTab === 2 ? 'block' : 'none',
              zIndex: currentTab === 2 ? 10 : 1,
              backgroundColor: '#0a1929',
              p: 2
            }}
          >
            <Typography variant="h5" color="white" mb={2}>
              {translate('레이더 모니터링', 'Radar Monitoring', language)}{' '}
              <span
                style={{
                  color: isConnected ? 'lightgreen' : 'orange',
                  fontSize: '0.8em',
                  marginLeft: '10px'
                }}
              >
                {isConnected
                  ? `(${operationMode || '연결됨'})`
                  : '(연결 안됨)'}
              </span>
            </Typography>

            <Grid container spacing={2}>
              {/* 레이더 디스플레이(예시) */}
              <Grid item xs={12} md={9}>
                <Paper
                  elevation={0}
                  sx={{
                    p: 2,
                    borderRadius: 2,
                    backgroundColor: '#0a1929',
                    border: '1px solid #1e3a5a',
                    height: '100%',
                    display: 'flex',
                    flexDirection: 'column'
                  }}
                >
                  <Typography variant="h6" mb={2} color="#fff">
                    {translate('통합 레이더 데이터', 'Combined Radar Data', language)}
                    <span
                      style={{ color: '#4fc3f7', marginLeft: '10px', fontSize: '0.8em' }}
                    >
                      플롯: {plotData.length}개
                    </span>
                    <span
                      style={{ color: '#ff9800', marginLeft: '10px', fontSize: '0.8em' }}
                    >
                      트랙: {trackData.length}개
                    </span>
                  </Typography>

                  <Box
                    sx={{
                      flex: 1,
                      width: '100%',
                      position: 'relative',
                      overflow: 'hidden'
                    }}
                  >
                    <CombinedRadarDisplay
                      plotData={plotData}
                      trackData={trackData}
                      language={language}
                      hideAlertSettings
                      key={`radar-display-${currentTab === 2 ? 'active' : 'inactive'}`}
                    />
                  </Box>
                </Paper>
              </Grid>

              {/* 레이더 상태 요약(예시) */}
              <Grid item xs={12} md={3}>
                <Paper
                  elevation={0}
                  sx={{
                    p: 1.5,
                    borderRadius: 2,
                    backgroundColor: '#0a1929',
                    border: '1px solid #1e3a5a',
                    height: '100%'
                  }}
                >
                  <Typography variant="h6" mb={1.5} fontSize="1rem" color="#fff">
                    {translate('레이더 상태', 'Radar Status', language)}
                  </Typography>
                  <Box sx={{ display: 'flex', flexDirection: 'column', gap: 1.5 }}>
                    <Box
                      sx={{
                        p: 1,
                        backgroundColor: 'rgba(30, 136, 229, 0.1)',
                        borderRadius: 1,
                        border: '1px solid rgba(30, 136, 229, 0.2)'
                      }}
                    >
                      <Typography
                        variant="caption"
                        sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}
                      >
                        {translate('연결 상태', 'Connection Status', language)}
                      </Typography>
                      <Typography variant="body1" sx={{ color: '#fff', fontSize: '0.9rem' }}>
                        {isConnected
                          ? translate('연결됨', 'Connected', language)
                          : translate('연결 안됨', 'Disconnected', language)}
                      </Typography>
                    </Box>

                    <Box
                      sx={{
                        p: 1,
                        backgroundColor: 'rgba(255, 152, 0, 0.1)',
                        borderRadius: 1,
                        border: '1px solid rgba(255, 152, 0, 0.2)'
                      }}
                    >
                      <Typography
                        variant="caption"
                        sx={{ color: '#ffb74d', display: 'block', mb: 0.5 }}
                      >
                        {translate('운용 모드', 'Operation Mode', language)}
                      </Typography>
                      <Typography variant="body1" sx={{ color: '#fff', fontSize: '0.9rem' }}>
                        {operationMode || translate('알 수 없음', 'Unknown', language)}
                      </Typography>
                    </Box>
                  </Box>
                </Paper>
              </Grid>
            </Grid>
          </Box>
        )}
      </Box>
    </Paper>
  );
};

export default MapView;

// 줌 및 홈 버튼 컨트롤 컴포넌트
const ZoomAndHomeControl = ({ mapCenter }) => {
  const map = useMap();
  
  useEffect(() => {
    if (map) {
      // 기존 컨트롤 제거 (중복 방지)
      if (map.zoomControl) {
        map.removeControl(map.zoomControl);
      }
      
      // 커스텀 줌 컨트롤 생성
      const zoomControl = L.control.zoom({
        position: 'topleft',
        zoomInTitle: '확대',
        zoomOutTitle: '축소'
      });
      
      // 홈 버튼 컨트롤 생성
      const homeControl = L.Control.extend({
        options: {
          position: 'bottomleft'
        },
        
        onAdd: function() {
          const container = L.DomUtil.create('div', 'leaflet-bar leaflet-control');
          const button = L.DomUtil.create('a', 'leaflet-control-home', container);
          
          button.innerHTML = '⌂';  // 홈 아이콘
          button.href = '#';
          button.title = '원위치로 돌아가기';
          button.style.fontWeight = 'bold';
          button.style.fontSize = '16px';
          button.style.lineHeight = '26px';
          button.style.textAlign = 'center';
          button.style.display = 'block';
          button.style.width = '30px';
          button.style.height = '30px';
          
          L.DomEvent
            .on(button, 'click', L.DomEvent.stopPropagation)
            .on(button, 'click', L.DomEvent.preventDefault)
            .on(button, 'click', function() {
              // 원래 위치와 줌 레벨로 지도 이동
              map.setView(mapCenter, 17, { animate: true });
            });
          
          return container;
        }
      });
      
      // 컨트롤 추가
      zoomControl.addTo(map);
      const homeInstance = new homeControl();
      homeInstance.addTo(map);
      
      // 참조 저장
      map.zoomControl = zoomControl;
      map._homeControl = homeInstance; // 별도 속성으로 저장
      
      return () => {
        // 안전하게 컨트롤 제거
        try {
          if (map && map.zoomControl) {
            map.removeControl(map.zoomControl);
          }
          if (map && map._homeControl) {
            map.removeControl(map._homeControl);
          }
        } catch (error) {
          console.warn('Error removing map controls:', error);
        }
      };
    }
  }, [map, mapCenter]);
  
  return null;
};
