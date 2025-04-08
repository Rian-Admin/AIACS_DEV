import { useEffect, useRef, useState } from 'react';
import { Box, useTheme, Typography, Slider, FormControlLabel, Switch, Button } from '@mui/material';
import L from 'leaflet';
import { useMap } from 'react-leaflet';
import { radarEmulatorAPI } from '../../../api';
import 'leaflet/dist/leaflet.css';

const MapRadarOverlay = ({ language }) => {
  const map = useMap();
  const theme = useTheme();
  const canvasRef = useRef(null);
  const bufferCanvasRef = useRef(null); // 더블 버퍼링을 위한 두 번째 캔버스
  const overlayRef = useRef(null);
  const resetControlRef = useRef(null);
  const alarmControlRef = useRef(null);
  const alarmSoundRef = useRef(null);
  const [radarData, setRadarData] = useState({ plots: [], tracks: [] });
  const [initialViewSet, setInitialViewSet] = useState(false); // 초기 뷰 설정 여부 확인
  const isUpdatingRef = useRef(false); // 업데이트 플래그
  const alarmPauseTimerRef = useRef(null); // 타이머 참조 추가
  
  // localStorage에서 경보 설정 가져오기
  const getSavedAlarmDistance = () => {
    const saved = localStorage.getItem('radarAlarmDistance');
    return saved ? parseInt(saved, 10) : 2000; // 기본값 2km
  };
  
  const getSavedAlarmEnabled = () => {
    const saved = localStorage.getItem('radarAlarmEnabled');
    return saved ? saved === 'true' : false;
  };
  
  const [alarmDistance, setAlarmDistance] = useState(getSavedAlarmDistance());
  const [isAlarmEnabled, setIsAlarmEnabled] = useState(getSavedAlarmEnabled());
  const [isAlarmTriggered, setIsAlarmTriggered] = useState(false);
  const [alarmPausedUntil, setAlarmPausedUntil] = useState(null); // 경보 일시 중지 시간 추가
  
  // 레이더 위치 설정 (소각시도 좌표로 변경)
  const radarPosition = {
    latitude: 37.243100, // 위도
    longitude: 126.652140 // 경도
  };
  
  // 레이더 최대 범위 (미터 단위)
  const maxRadarRange = 4000; // 4km로 수정하여 레이더 모니터링 페이지와 일치시킴
  
  // 경보 설정 변경 시 localStorage에 저장하는 함수
  const updateAlarmSettings = (distance, enabled) => {
    // localStorage에 저장
    localStorage.setItem('radarAlarmDistance', distance.toString());
    localStorage.setItem('radarAlarmEnabled', enabled.toString());
    
    // 상태 업데이트
    setAlarmDistance(distance);
    setIsAlarmEnabled(enabled);
    
    console.log(`경보 설정 업데이트: 거리=${distance}m, 활성화=${enabled}`);
    
    // 비활성화 상태로 변경 시 경보 리셋
    if (!enabled && isAlarmTriggered) {
      resetAlarm();
    }
  };
  
  // 경보 리셋하는 함수 추가
  const resetAlarm = () => {
    // 경보 상태 초기화
    setIsAlarmTriggered(false);
    
    // 경보음 중지
    if (alarmSoundRef.current) {
      alarmSoundRef.current.pause();
      alarmSoundRef.current.currentTime = 0;
    }
    // 맵 경보 메시지 제거
    removeMapAlertMessage();
    
    // 1분 동안 경보 일시 중지
    const pauseUntil = new Date();
    pauseUntil.setMinutes(pauseUntil.getMinutes() + 1);
    setAlarmPausedUntil(pauseUntil);
    
    // 기존 타이머가 있으면 제거
    if (alarmPauseTimerRef.current) {
      clearTimeout(alarmPauseTimerRef.current);
    }
    
    // 1분 후에 경보 일시 중지 해제
    alarmPauseTimerRef.current = setTimeout(() => {
      setAlarmPausedUntil(null);
      console.log('경보 일시 중지 해제됨');
    }, 60000); // 1분 = 60000ms
  };
  
  // 경보 즉시 재개 함수 추가
  const resumeAlarm = () => {
    // 타이머 취소
    if (alarmPauseTimerRef.current) {
      clearTimeout(alarmPauseTimerRef.current);
      alarmPauseTimerRef.current = null;
    }
    
    // 일시 중지 상태 해제
    setAlarmPausedUntil(null);
    console.log('경보 감지 수동으로 재개됨');
  };
  
  // 다른 컴포넌트에서 경보 설정 변경 시 이벤트 리스너
  useEffect(() => {
    const handleStorageChange = (e) => {
      if (e.key === 'radarAlarmDistance') {
        setAlarmDistance(parseInt(e.newValue, 10));
      } else if (e.key === 'radarAlarmEnabled') {
        setIsAlarmEnabled(e.newValue === 'true');
      }
    };
    
    window.addEventListener('storage', handleStorageChange);
    
    return () => {
      window.removeEventListener('storage', handleStorageChange);
    };
  }, []);
  
  // 경보음 초기화
  useEffect(() => {
    // 경보음 생성
    const alarmSound = new Audio();
    alarmSound.src = "data:audio/wav;base64,//uQRAAAAWMSLwUIYAAsYkXgoQwAEaYLWfkWgAI0wWs/ItAAAGDgYtAgAyN+QWaAAihwMWm4G8QQRDiMcCBcH3Cc+CDv/7xA4Tvh9Rz/y8QADBwMWgQAZG/ILNAARQ4GLTcDeIIIhxGOBAuD7hOfBB3/94gcJ3w+o5/5eIAIAAAVwWgQAVQ2ORaIQwEMAJiDg95G4nQL7mQVWI6GwRcfsZAcsKkJvxgxEjzFUgfHoSQ9Qq7KNwqHwuB13MA4a1q/DmBrHgPcmjiGoh//EwC5nGPEmS4RcfkVKOhJf+WOgoxJclFz3kgn//dBA+ya1GhurNn8zb//9NNutNuhz31f////9vt///z+IdAEAAAK4LQIAKobHItEIYCGAExBwe8jcToF9zIKrEdDYIuP2MgOWFSE34wYiR5iqQPj0JIeoVdlG4VD4XA67mAcNa1fhzA1jwHuTRxDUQ//iYBczjHiTJcIuPyKlHQkv/LHQUYkuSi57yQT//uggfZNajQ3Vmz+Zt//+mm3Wm3Q576v////+32///5/EOgAAADVghQAAAAA//uQZAUAB1WI0PZugAAAAAoQwAAAEk3nRd2qAAAAACiDgAAAAAAABCqEEQRLCgwpBGMlJkIz8jKhGvj4k6jzRnqasNKIeoh5gI7BJaC1A1AoNBjJgbyApVS4IDlZgDU5WUAxEKDNmmALHzZp0Fkz1FMTmGFl1FMEyodIavcCAUHDWrKAIA4aa2ooFollowEAAAAAAADpm1QAEByZDgABAAbQZvANoACJAA+gNwBbQDKQFmA88BMAJkAPwB2wGLAEeAPcAb0B7QGDgBDgO+AUEA9YBoQFPAK2AFUAH4ArgB5gFpgPTA9YB4YEDAVuA94D8wHxAPKAgMB8YAEwEpgP4gf8A+sCAoH4QQhBCYEJQP+ghGCEYIRghGCEgH/AQlBCUD9YITAf8BCcEHAQfBB4D/gIOg/7B+kD6YH5wPkgfKA+0B/wEBgP8gfgA/ECAoH+wP3g/gCB4IJgf+BCcEJgP8AAAAAAAAAA"; // 간단한 "삐" 소리
    alarmSound.loop = true;
    alarmSoundRef.current = alarmSound;
    
    return () => {
      if (alarmSoundRef.current) {
        alarmSoundRef.current.pause();
        alarmSoundRef.current = null;
      }
    };
  }, []);
  
  // 중심점 리셋 함수
  const resetMapView = () => {
    const radarLatLng = L.latLng(radarPosition.latitude, radarPosition.longitude);
    map.setView(radarLatLng, 14);
    console.log('맵 뷰 리셋됨');
  };
  
  // 경보 컨트롤 상태 업데이트 함수
  const updateAlarmControlState = () => {
    if (!map || !alarmControlRef.current) return;
    
    // DOM 요소 찾기
    const toggleButton = document.getElementById('alarm-toggle-button');
    const toggleStatus = document.getElementById('alarm-toggle-status');
    const slider = document.getElementById('alarm-distance-slider');
    const sliderValue = document.getElementById('alarm-distance-value');
    const alarmStatus = document.getElementById('alarm-status');
    const alarmResetButton = document.getElementById('alarm-reset-button');
    const alarmPauseStatus = document.getElementById('alarm-pause-status');
    const alarmResumeButton = document.getElementById('alarm-resume-button');
    
    if (toggleButton && toggleStatus && slider && sliderValue) {
      // 경보 활성화 버튼 업데이트
      toggleButton.textContent = isAlarmEnabled ? '경보 비활성화' : '경보 활성화';
      toggleButton.style.backgroundColor = isAlarmEnabled ? '#ff9800' : '#4CAF50';
      
      // 상태 레이블 업데이트
      if (alarmPausedUntil && new Date() < alarmPausedUntil) {
        toggleStatus.textContent = '일시 중지됨';
        toggleStatus.style.color = '#ffcc00';
      } else {
        toggleStatus.textContent = isAlarmEnabled ? '감지 중' : '감지 정지';
        toggleStatus.style.color = isAlarmEnabled ? 'lime' : 'white';
      }
      
      // 슬라이더 값 업데이트
      slider.value = alarmDistance.toString();
      sliderValue.textContent = `${(alarmDistance / 1000).toFixed(1)}km`;
    }
    
    // 경보 상태 업데이트
    if (alarmStatus) {
      alarmStatus.style.display = isAlarmTriggered ? 'inline' : 'none';
    }
    
    // 일시 중지 상태 업데이트
    if (alarmPauseStatus) {
      if (alarmPausedUntil && new Date() < alarmPausedUntil) {
        const remainingSecs = Math.ceil((alarmPausedUntil - new Date()) / 1000);
        alarmPauseStatus.textContent = `경보 일시 중지: ${remainingSecs}초 남음`;
        alarmPauseStatus.style.display = 'block';
      } else {
        alarmPauseStatus.style.display = 'none';
      }
    }
    
    // 경보 재개 버튼 업데이트
    if (alarmResumeButton) {
      if (alarmPausedUntil && new Date() < alarmPausedUntil) {
        alarmResumeButton.style.display = 'block';
      } else {
        alarmResumeButton.style.display = 'none';
      }
    }
    
    // 알람 컨트롤 컨테이너
    const container = document.getElementById('radar-alarm-control');
    
    // 경보 발생 시 추가 강조 표시 및 리셋 버튼 표시
    if (isAlarmTriggered) {
      // 깜빡임 애니메이션을 위한 스타일 추가
      const existingStyle = document.getElementById('alarm-blink-style');
      if (!existingStyle) {
        const style = document.createElement('style');
        style.id = 'alarm-blink-style';
        style.textContent = `
          @keyframes alarm-blink {
            0% { background-color: rgba(30, 58, 90, 0.9); }
            50% { background-color: rgba(255, 0, 0, 0.7); }
            100% { background-color: rgba(30, 58, 90, 0.9); }
          }
        `;
        document.head.appendChild(style);
      }
      
      // 컨테이너 배경 깜빡임 효과
      if (container) {
        container.style.animation = 'alarm-blink 1s infinite';
      }
      
      // 경보 리셋 버튼 표시
      if (alarmResetButton) {
        alarmResetButton.style.display = 'block';
      }
    } else {
      if (container) {
        container.style.animation = '';
      }
      
      // 경보 리셋 버튼 계속 표시 (숨기지 않음)
      if (alarmResetButton) {
        alarmResetButton.style.display = 'block';
      }
    }
  };
  
  // 경보 컨트롤 추가
  useEffect(() => {
    if (!map) {
      console.log('맵 객체가 없습니다.');
      return;
    }
    
    console.log('경보 컨트롤 추가 시도...');
    
    // 이전에 추가된 모든 컨트롤 제거 (중복 방지)
    if (alarmControlRef.current) {
      console.log('기존 컨트롤 제거');
      map.removeControl(alarmControlRef.current);
      alarmControlRef.current = null;
    }
    
    // 혹시 모를 중복 컨트롤 제거 - 기존 컨트롤과 동일한 클래스를 가진 모든 요소 찾기
    const existingControls = document.querySelectorAll('.radar-alarm-control');
    console.log(`기존 경보 컨트롤 요소 수: ${existingControls.length}`);
    existingControls.forEach(control => {
      if (control && control.parentNode) {
        control.parentNode.removeChild(control);
      }
    });
    
    // 경보 설정 컨트롤 생성
    const AlarmControl = L.Control.extend({
      options: {
        position: 'bottomright' // 오른쪽 아래로 변경
      },
      
      onAdd: function() {
        console.log('컨트롤 onAdd 함수 실행');
        const container = L.DomUtil.create('div', 'leaflet-bar leaflet-control radar-alarm-control');
        container.style.backgroundColor = 'rgba(30, 58, 90, 0.9)';
        container.style.padding = '10px';
        container.style.width = '300px';
        container.style.borderRadius = '5px';
        container.style.color = 'white';
        container.style.boxShadow = '0 1px 5px rgba(0,0,0,0.4)';
        container.style.zIndex = '1000'; // z-index 추가
        container.id = 'radar-alarm-control'; // 고유 ID 추가
        
        // 경보 설정 헤더
        const header = document.createElement('div');
        header.innerHTML = '<h3 style="margin: 0 0 10px 0; font-size: 14px;">경보 설정</h3>';
        container.appendChild(header);
        
        // 경보 활성화 토글
        const toggleContainer = document.createElement('div');
        toggleContainer.style.display = 'flex';
        toggleContainer.style.alignItems = 'center';
        toggleContainer.style.marginBottom = '10px';
        
        const toggleLabel = document.createElement('label');
        toggleLabel.htmlFor = 'alarm-toggle';
        toggleLabel.textContent = '경보 상태:';
        toggleLabel.style.marginRight = '10px';
        toggleLabel.style.flex = '0 0 80px';
        
        // 체크박스 제거하고 상태 표시 텍스트만 유지
        const toggleStatus = document.createElement('span');
        toggleStatus.id = 'alarm-toggle-status';
        toggleStatus.textContent = isAlarmEnabled ? '감지 중' : '감지 정지';
        toggleStatus.style.color = isAlarmEnabled ? 'lime' : 'white';
        toggleStatus.style.marginRight = '10px';
        
        toggleContainer.appendChild(toggleLabel);
        toggleContainer.appendChild(toggleStatus);
        
        // 경보 일시 중지 상태 표시 추가
        const alarmPauseStatus = document.createElement('div');
        alarmPauseStatus.id = 'alarm-pause-status';
        alarmPauseStatus.style.width = '100%';
        alarmPauseStatus.style.marginBottom = '10px';
        alarmPauseStatus.style.padding = '5px';
        alarmPauseStatus.style.backgroundColor = 'rgba(255, 204, 0, 0.7)';
        alarmPauseStatus.style.color = 'black';
        alarmPauseStatus.style.borderRadius = '3px';
        alarmPauseStatus.style.textAlign = 'center';
        alarmPauseStatus.style.fontWeight = 'bold';
        alarmPauseStatus.style.display = 'none';
        container.appendChild(alarmPauseStatus);
        
        // 경보 즉시 재개 버튼 추가
        const alarmResumeButton = document.createElement('button');
        alarmResumeButton.id = 'alarm-resume-button';
        alarmResumeButton.textContent = '경보 즉시 재개';
        alarmResumeButton.style.padding = '5px';
        alarmResumeButton.style.backgroundColor = '#4CAF50'; // 초록색
        alarmResumeButton.style.color = 'white';
        alarmResumeButton.style.border = 'none';
        alarmResumeButton.style.borderRadius = '3px';
        alarmResumeButton.style.cursor = 'pointer';
        alarmResumeButton.style.width = '100%';
        alarmResumeButton.style.marginBottom = '10px';
        alarmResumeButton.style.display = 'none'; // 기본적으로 숨김
        
        alarmResumeButton.addEventListener('click', function() {
          resumeAlarm();
        });
        
        container.appendChild(alarmResumeButton);
        
        // 경보 상태 표시
        const alarmStatus = document.createElement('span');
        alarmStatus.id = 'alarm-status';
        alarmStatus.style.marginLeft = '10px';
        alarmStatus.style.display = 'none';
        alarmStatus.textContent = '⚠️ 경보 발생!';
        alarmStatus.style.color = 'red';
        alarmStatus.style.fontWeight = 'bold';
        alarmStatus.style.fontSize = '16px'; // 글씨 크기 증가
        alarmStatus.style.textShadow = '1px 1px 2px rgba(0,0,0,0.7)'; // 가독성을 위한 텍스트 그림자 추가
        
        toggleContainer.appendChild(alarmStatus);
        container.appendChild(toggleContainer);
        
        // 경보 활성화/비활성화 버튼 추가
        const toggleButton = document.createElement('button');
        toggleButton.id = 'alarm-toggle-button';
        toggleButton.textContent = isAlarmEnabled ? '경보 비활성화' : '경보 활성화';
        toggleButton.style.padding = '5px';
        toggleButton.style.backgroundColor = isAlarmEnabled ? '#ff9800' : '#4CAF50';
        toggleButton.style.color = 'white';
        toggleButton.style.border = 'none';
        toggleButton.style.borderRadius = '3px';
        toggleButton.style.cursor = 'pointer';
        toggleButton.style.width = '100%';
        toggleButton.style.marginBottom = '10px';
        
        // DOM 요소에 현재 상태 기록
        toggleButton.dataset.enabled = isAlarmEnabled.toString();
        
        // 이벤트 버블링 방지 및 단일 이벤트 리스너 사용
        L.DomEvent.on(toggleButton, 'click', function(e) {
          // 이벤트 전파 중지
          L.DomEvent.stopPropagation(e);
          L.DomEvent.preventDefault(e);
          
          // 버튼의 현재 상태값 가져오기 (dataset 사용)
          const currentState = toggleButton.dataset.enabled === 'true';
          const newState = !currentState;
          
          console.log(`경보 토글 버튼 클릭: ${currentState} -> ${newState}`);
          
          // DOM 요소 상태 업데이트
          toggleButton.dataset.enabled = newState.toString();
          toggleButton.textContent = newState ? '경보 비활성화' : '경보 활성화';
          toggleButton.style.backgroundColor = newState ? '#ff9800' : '#4CAF50';
          toggleStatus.textContent = newState ? '감지 중' : '감지 정지';
          toggleStatus.style.color = newState ? 'lime' : 'white';
          
          // React 상태 업데이트
          updateAlarmSettings(alarmDistance, newState);
        }, this);
        
        container.appendChild(toggleButton);
        
        // 경보 거리 설정
        const sliderContainer = document.createElement('div');
        sliderContainer.style.marginBottom = '10px';
        
        const sliderLabel = document.createElement('div');
        sliderLabel.textContent = '경보 거리:';
        sliderLabel.style.marginBottom = '5px';
        
        const sliderWrapper = document.createElement('div');
        sliderWrapper.style.display = 'flex';
        sliderWrapper.style.alignItems = 'center';
        
        const slider = document.createElement('input');
        slider.type = 'range';
        slider.min = '500';
        slider.max = '4000';
        slider.step = '100';
        slider.value = alarmDistance.toString();
        slider.style.flex = '1';
        slider.id = 'alarm-distance-slider';
        slider.style.width = '100%'; // 너비 지정
        
        const sliderValue = document.createElement('span');
        sliderValue.textContent = `${(alarmDistance / 1000).toFixed(1)}km`;
        sliderValue.style.marginLeft = '10px';
        sliderValue.style.width = '50px';
        sliderValue.id = 'alarm-distance-value';
        
        slider.addEventListener('input', function() {
          const value = parseInt(this.value, 10);
          sliderValue.textContent = `${(value / 1000).toFixed(1)}km`;
          // 경보 거리만 업데이트하고 활성화 상태는 변경하지 않음
          localStorage.setItem('radarAlarmDistance', value.toString());
          setAlarmDistance(value);
        });
        
        sliderWrapper.appendChild(slider);
        sliderWrapper.appendChild(sliderValue);
        
        sliderContainer.appendChild(sliderLabel);
        sliderContainer.appendChild(sliderWrapper);
        container.appendChild(sliderContainer);
        
        // 기본값 버튼
        const resetButton = document.createElement('button');
        resetButton.textContent = '기본값으로 재설정';
        resetButton.style.padding = '5px';
        resetButton.style.backgroundColor = '#1976d2';
        resetButton.style.color = 'white';
        resetButton.style.border = 'none';
        resetButton.style.borderRadius = '3px';
        resetButton.style.cursor = 'pointer';
        resetButton.style.width = '100%';
        resetButton.style.marginBottom = '10px';
        
        resetButton.addEventListener('click', function() {
          updateAlarmSettings(2000, isAlarmEnabled);
        });
        
        container.appendChild(resetButton);
        
        // 경보 리셋 버튼 추가
        const alarmResetButton = document.createElement('button');
        alarmResetButton.textContent = '현재 경보만 중지';
        alarmResetButton.style.padding = '5px';
        alarmResetButton.style.backgroundColor = 'red';
        alarmResetButton.style.color = 'white';
        alarmResetButton.style.border = 'none';
        alarmResetButton.style.borderRadius = '3px';
        alarmResetButton.style.cursor = 'pointer';
        alarmResetButton.style.width = '100%';
        alarmResetButton.style.display = 'block'; // 항상 표시하도록 설정
        alarmResetButton.id = 'alarm-reset-button';
        
        alarmResetButton.addEventListener('click', function() {
          resetAlarm();
        });
        
        container.appendChild(alarmResetButton);
        
        // 이벤트 전파 방지
        L.DomEvent.disableClickPropagation(container);
        L.DomEvent.disableScrollPropagation(container);
        
        // 초기 상태 업데이트
        setTimeout(updateAlarmControlState, 0);
        
        console.log('컨트롤 컨테이너 생성 완료');
        return container;
      }
    });
    
    // 컨트롤 추가
    try {
      alarmControlRef.current = new AlarmControl();
      map.addControl(alarmControlRef.current);
      console.log('경보 컨트롤이 성공적으로 추가되었습니다.');
      
      // 컨트롤 DOM 요소 강제 표시
      setTimeout(() => {
        const alarmControl = document.getElementById('radar-alarm-control');
        if (alarmControl) {
          console.log('경보 컨트롤 요소를 찾았습니다. 스타일 적용 중...');
          alarmControl.style.display = 'block';
          alarmControl.style.position = 'absolute';
          alarmControl.style.bottom = '40px'; // top 대신 bottom 사용
          alarmControl.style.right = '10px'; // left 대신 right 사용
          alarmControl.style.zIndex = '1000';
        } else {
          console.log('경보 컨트롤 요소를 찾을 수 없습니다.');
        }
      }, 500);
    } catch (error) {
      console.error('경보 컨트롤 추가 중 오류:', error);
    }
    
    return () => {
      console.log('경보 컨트롤 정리 중...');
      if (alarmControlRef.current) {
        try {
          map.removeControl(alarmControlRef.current);
          console.log('경보 컨트롤이 성공적으로 제거되었습니다.');
        } catch (error) {
          console.error('경보 컨트롤 제거 중 오류:', error);
        }
        alarmControlRef.current = null;
      }
    };
  }, [map]); // 의존성 배열에서 alarmDistance와 isAlarmEnabled 제거
  
  // 경보 상태 업데이트
  useEffect(() => {
    updateAlarmControlState();
  }, [isAlarmEnabled, alarmDistance, isAlarmTriggered]);
  
  // 리셋 버튼 추가
  useEffect(() => {
    if (!map) return;
    
    // 이미 버튼이 추가되어 있다면 추가하지 않음
    if (resetControlRef.current) return;
    
    // 사용자 정의 컨트롤 생성
    const ResetControl = L.Control.extend({
      options: {
        position: 'bottomleft' // 위치를 bottomleft로 변경
      },
      
      onAdd: function() {
        const container = L.DomUtil.create('div', 'leaflet-bar leaflet-control');
        const button = L.DomUtil.create('a', '', container);
        
        // 버튼 스타일 및 텍스트 설정
        button.innerHTML = '⟳'; // 리셋 아이콘
        button.title = language === 'ko' ? '중심점 리셋' : 'Reset View';
        button.href = '#';
        button.style.fontSize = '20px'; // 크기 증가
        button.style.fontWeight = 'bold';
        button.style.textDecoration = 'none';
        button.style.lineHeight = '36px'; // 크기 증가
        button.style.textAlign = 'center';
        button.style.width = '36px'; // 크기 증가
        button.style.height = '36px'; // 크기 증가
        button.style.display = 'block';
        button.style.backgroundColor = '#1976d2'; // 파란색 배경
        button.style.color = 'white'; // 흰색 텍스트
        button.style.border = '2px solid white'; // 흰색 테두리 추가
        button.style.borderRadius = '4px';
        button.style.boxShadow = '0 2px 5px rgba(0,0,0,0.3)'; // 그림자 효과 추가
        
        // 마우스 오버 효과
        button.onmouseover = function() {
          this.style.backgroundColor = '#2196f3';
        };
        
        // 마우스 아웃 효과
        button.onmouseout = function() {
          this.style.backgroundColor = '#1976d2';
        };
        
        // 클릭 이벤트 처리
        L.DomEvent.on(button, 'click', function(e) {
          L.DomEvent.stopPropagation(e);
          L.DomEvent.preventDefault(e);
          resetMapView();
        });
        
        return container;
      }
    });
    
    // 컨트롤 추가
    resetControlRef.current = new ResetControl();
    map.addControl(resetControlRef.current);
    
    return () => {
      // 컴포넌트 언마운트 시 컨트롤 제거
      if (resetControlRef.current) {
        map.removeControl(resetControlRef.current);
        resetControlRef.current = null;
      }
    };
  }, [map, language]);
  
  // API에서 레이더 데이터 가져오기
  useEffect(() => {
    const fetchRadarData = async () => {
      try {
        const response = await radarEmulatorAPI.getSimulationStatus();
        
        // 플롯 데이터 처리
        const plotData = response.data.plots || response.data.Plot || [];
        
        // 트랙 데이터 처리
        const trackData = response.data.tracks || response.data.Track || [];
        
        setRadarData({ plots: plotData, tracks: trackData });
        console.log('API 레이더 데이터 로드:', { plots: plotData.length, tracks: trackData.length });
      } catch (error) {
        console.error('레이더 데이터 가져오기 오류:', error);
        setRadarData({ plots: [], tracks: [] });
      }
    };

    fetchRadarData();
    const interval = setInterval(fetchRadarData, 3000); // 3초마다 업데이트
    
    return () => clearInterval(interval);
  }, []);
  
  // 맵 초기화 및 오버레이 생성 (최초 1회만 실행)
  useEffect(() => {
    if (!map || !canvasRef.current) return;
    
    // 버퍼 캔버스 초기화
    if (!bufferCanvasRef.current) {
      bufferCanvasRef.current = document.createElement('canvas');
      bufferCanvasRef.current.width = 3000; // 해상도 증가
      bufferCanvasRef.current.height = 3000; // 해상도 증가
    }
    
    // 레이더 위치를 LatLng 객체로 변환
    const radarLatLng = L.latLng(radarPosition.latitude, radarPosition.longitude);
    
    // 레이더 범위를 맵에 맞게 조정
    const bounds = calculateRadarBounds(radarLatLng, maxRadarRange, map);
    
    // 새 오버레이 생성
    overlayRef.current = L.imageOverlay(canvasRef.current.toDataURL(), bounds, {
      opacity: 0.9, // 불투명도 증가
      interactive: false,
    }).addTo(map);
    
    // 최초 로딩 시에만 맵 중심을 레이더 위치로 설정 (한 번만 실행)
    if (!initialViewSet) {
      resetMapView();
      setInitialViewSet(true);
    }
    
    // 맵 이동 중 상태 플래그
    let isMoving = false;
    
    // 맵 이동 시작 이벤트
    const handleMoveStart = () => {
      isMoving = true;
    };
    
    // 맵 이동 중 이벤트
    const handleMove = debounce(() => {
      if (!isUpdatingRef.current && isMoving) {
        // 이동 중에는 경계만 업데이트하고 이미지는 유지
        updateOverlayBoundsOnly();
      }
    }, 10); // 빠른 반응을 위해 10ms로 설정
    
    // 맵 이동/줌 종료 이벤트
    const handleMoveEnd = debounce(() => {
      isMoving = false;
      if (!isUpdatingRef.current) {
        // 이동이 완료된 후에만 전체 오버레이 업데이트
        updateOverlay(true);
      }
    }, 200); // 이동이 완전히 끝난 후 업데이트하도록 200ms로 설정
    
    // 이벤트 리스너 등록
    map.on('movestart', handleMoveStart);
    map.on('move', handleMove);
    map.on('moveend', handleMoveEnd);
    map.on('zoomstart', handleMoveStart);
    map.on('zoom', handleMove);
    map.on('zoomend', handleMoveEnd);
    
    // 첫 번째 오버레이 그리기
    drawRadarOverlay();
    
    return () => {
      // 이벤트 리스너 제거
      map.off('movestart', handleMoveStart);
      map.off('move', handleMove);
      map.off('moveend', handleMoveEnd);
      map.off('zoomstart', handleMoveStart);
      map.off('zoom', handleMove);
      map.off('zoomend', handleMoveEnd);
      
      if (overlayRef.current) {
        map.removeLayer(overlayRef.current);
        overlayRef.current = null;
      }
    };
  }, [map, initialViewSet]); // 맵 객체가 변경될 때만 실행
  
  // 경계만 업데이트하는 함수 (맵 이동 중 호출)
  const updateOverlayBoundsOnly = () => {
    if (!map || !overlayRef.current) return;
    
    const radarLatLng = L.latLng(radarPosition.latitude, radarPosition.longitude);
    const bounds = calculateRadarBounds(radarLatLng, maxRadarRange, map);
    
    // 이미지는 유지하고 경계만 업데이트
    overlayRef.current.setBounds(bounds);
  };
  
  // 오버레이 업데이트 함수
  const updateOverlay = (fullUpdate = false) => {
    if (!map || !overlayRef.current || !canvasRef.current) return;
    
    // 업데이트 플래그 설정
    isUpdatingRef.current = true;
    
    const radarLatLng = L.latLng(radarPosition.latitude, radarPosition.longitude);
    const bounds = calculateRadarBounds(radarLatLng, maxRadarRange, map);
    
    // 경계 먼저 업데이트
    overlayRef.current.setBounds(bounds);
    
    // 전체 업데이트가 필요한 경우에만 이미지도 업데이트
    if (fullUpdate) {
      // 버퍼 캔버스에 그리기
      drawRadarOverlay();
      
      // 새 이미지 URL 생성
      const newUrl = canvasRef.current.toDataURL();
      
      // 이미지 미리 로드
      const img = new Image();
      img.onload = () => {
        // 새 이미지 적용
        overlayRef.current.setUrl(newUrl);
        
        // 업데이트 플래그 해제
        isUpdatingRef.current = false;
      };
      img.src = newUrl;
    } else {
      // 경계만 업데이트한 경우 바로 플래그 해제
      isUpdatingRef.current = false;
    }
  };
  
  // 디바운스 함수 구현
  const debounce = (func, delay) => {
    let timeoutId;
    return function() {
      const context = this;
      const args = arguments;
      clearTimeout(timeoutId);
      timeoutId = setTimeout(() => {
        func.apply(context, args);
      }, delay);
    };
  };
  
  // 일시 중지 상태 정기 업데이트
  useEffect(() => {
    const updateInterval = setInterval(() => {
      if (alarmPausedUntil) {
        updateAlarmControlState();
        
        // 일시 중지 시간이 지났는지 확인
        if (new Date() > alarmPausedUntil) {
          setAlarmPausedUntil(null);
        }
      }
    }, 1000); // 1초마다 업데이트
    
    return () => clearInterval(updateInterval);
  }, [alarmPausedUntil]);
  
  // 레이더 데이터 변경 시 오버레이만 업데이트
  useEffect(() => {
    if (!map || !canvasRef.current || !overlayRef.current) return;
    
    // 업데이트 플래그 설정
    isUpdatingRef.current = true;
    
    // 버퍼 캔버스에 레이더 데이터 그리기
    drawRadarOverlay();
    
    // 오버레이 이미지 업데이트 (경계는 업데이트하지 않음)
    const canvas = canvasRef.current;
    const dataUrl = canvas.toDataURL();
    
    // 이미지 미리 로드하여 깜빡임 방지
    const img = new Image();
    img.onload = () => {
      overlayRef.current.setUrl(dataUrl);
      isUpdatingRef.current = false;
    };
    img.src = dataUrl;
    
    // 경보 처리 로직
    if (isAlarmEnabled) {
      let tracksInsideAlarmRadius = 0;
      let plotsInsideAlarmRadius = 0;
      
      // 트랙 데이터 확인
      if (radarData.tracks && radarData.tracks.length > 0) {
        radarData.tracks.forEach(track => {
          if (track.position && Array.isArray(track.position) && track.position.length >= 2) {
            const [x, y] = track.position;
            const range = Math.sqrt(x * x + y * y);
            
            if (range <= alarmDistance) {
              tracksInsideAlarmRadius++;
            }
          }
        });
      }
      
      // 플롯 데이터 확인
      if (radarData.plots && radarData.plots.length > 0) {
        radarData.plots.forEach(plot => {
          if (plot.position && Array.isArray(plot.position) && plot.position.length >= 2) {
            const [x, y] = plot.position;
            const range = Math.sqrt(x * x + y * y);
            
            if (range <= alarmDistance) {
              plotsInsideAlarmRadius++;
            }
          }
        });
      }
      
      const totalObjectsInsideAlarmRadius = tracksInsideAlarmRadius + plotsInsideAlarmRadius;
      
      // 경보가 일시 중지되었는지 확인
      const isAlarmPaused = alarmPausedUntil && new Date() < alarmPausedUntil;
      
      // 경보 상태 업데이트
      if (totalObjectsInsideAlarmRadius > 0 && !isAlarmPaused) {
        if (!isAlarmTriggered) {
          // 처음 경보가 발생할 때
          setIsAlarmTriggered(true);
          // 경보음 재생
          if (alarmSoundRef.current) {
            alarmSoundRef.current.play().catch(err => console.error('경보음 재생 실패:', err));
          }
        }
        
        // 경보 발생 여부와 관계없이 항상 메시지 업데이트
        // 이렇게 하면 경보 거리를 변경하거나 새 객체가 감지될 때 메시지가 업데이트됨
        showMapAlertMessage(map, tracksInsideAlarmRadius, plotsInsideAlarmRadius);
      } else if (isAlarmTriggered) {
        // 경보 거리 내에 객체가 없는데 경보가 활성화된 경우
        setIsAlarmTriggered(false);
        // 경보음 중지
        if (alarmSoundRef.current) {
          alarmSoundRef.current.pause();
          alarmSoundRef.current.currentTime = 0;
        }
        
        // 맵 경보 메시지 제거
        removeMapAlertMessage();
      }
    } else if (isAlarmTriggered) {
      setIsAlarmTriggered(false);
      // 경보음 중지
      if (alarmSoundRef.current) {
        alarmSoundRef.current.pause();
        alarmSoundRef.current.currentTime = 0;
      }
      
      // 맵 경보 메시지 제거
      removeMapAlertMessage();
    }
  }, [radarData, isAlarmEnabled, alarmDistance, alarmPausedUntil]);
  
  // 맵 중앙에 큰 경보 메시지 표시 함수
  const showMapAlertMessage = (map, trackCount, plotCount) => {
    // 기존 메시지 제거
    removeMapAlertMessage();
    
    // 새 메시지 생성
    const alertDiv = document.createElement('div');
    alertDiv.id = 'map-alarm-message';
    alertDiv.style.position = 'absolute';
    alertDiv.style.top = '10px'; // 화면 상단으로 위치 이동
    alertDiv.style.left = '50%'; // 좌우 중앙
    alertDiv.style.transform = 'translateX(-50%)'; // 좌우 중앙 정렬
    alertDiv.style.backgroundColor = 'rgba(255, 0, 0, 0.8)'; // 불투명도 증가
    alertDiv.style.color = 'white';
    alertDiv.style.padding = '12px 25px'; // 패딩 조정
    alertDiv.style.borderRadius = '10px';
    alertDiv.style.fontSize = '22px'; // 글씨 크기 조정
    alertDiv.style.fontWeight = 'bold';
    alertDiv.style.zIndex = '1000';
    alertDiv.style.textAlign = 'center';
    alertDiv.style.boxShadow = '0 0 20px rgba(255, 0, 0, 0.9)'; // 그림자 강화
    alertDiv.style.animation = 'pop-in-top 0.5s, pulse-top 1.5s infinite';
    alertDiv.style.textShadow = '2px 2px 4px rgba(0,0,0,0.7)';
    alertDiv.style.border = '2px solid white'; // 테두리 추가
    
    // 트랙과 플롯의 수에 따라 다른 메시지 표시
    const totalCount = trackCount + plotCount;
    if (totalCount > 0) {
      alertDiv.innerHTML = `⚠️ 경보 발생! 조류 ${totalCount}마리 접근`;
    } else {
      alertDiv.innerHTML = `⚠️ 경보 발생!`;
    }
    
    // 애니메이션을 위한 스타일 추가
    const style = document.createElement('style');
    style.textContent = `
      @keyframes pop-in-top {
        0% { transform: translateX(-50%) scale(0); opacity: 0; }
        70% { transform: translateX(-50%) scale(1.1); opacity: 1; }
        100% { transform: translateX(-50%) scale(1); opacity: 1; }
      }
      @keyframes pulse-top {
        0% { transform: translateX(-50%) scale(1); }
        50% { transform: translateX(-50%) scale(1.05); }
        100% { transform: translateX(-50%) scale(1); }
      }
    `;
    document.head.appendChild(style);
    
    // 맵 컨테이너에 메시지 추가
    const mapContainer = map.getContainer();
    mapContainer.appendChild(alertDiv);
    
    // 페이드 아웃 애니메이션 추가
    const fadeStyle = document.createElement('style');
    fadeStyle.textContent = `
      @keyframes fade-out {
        from { opacity: 1; }
        to { opacity: 0; }
      }
    `;
    document.head.appendChild(fadeStyle);
  };
  
  // 맵 경보 메시지 제거 함수
  const removeMapAlertMessage = () => {
    const existingAlert = document.getElementById('map-alarm-message');
    if (existingAlert && existingAlert.parentNode) {
      existingAlert.parentNode.removeChild(existingAlert);
    }
  };
  
  // 레이더 범위 계산 함수
  const calculateRadarBounds = (center, rangeInMeters, map) => {
    // 미터 단위를 위도/경도 범위로 변환 (근사값)
    const latDelta = (rangeInMeters / 111000); // 1도 위도는 약 111km
    const lngDelta = (rangeInMeters / (111000 * Math.cos(center.lat * Math.PI / 180))); // 경도는 위도에 따라 달라짐
    
    // 범위 계산
    const northLat = center.lat + latDelta;
    const southLat = center.lat - latDelta;
    const eastLng = center.lng + lngDelta;
    const westLng = center.lng - lngDelta;
    
    return L.latLngBounds(
      L.latLng(southLat, westLng), // 남서
      L.latLng(northLat, eastLng)  // 북동
    );
  };
  
  // 캔버스에 레이더 데이터 그리기 함수
  const drawRadarOverlay = () => {
    if (!canvasRef.current || !bufferCanvasRef.current) return;
    
    // 먼저 버퍼 캔버스에 그리기
    const bufferCanvas = bufferCanvasRef.current;
    const bufferCtx = bufferCanvas.getContext('2d', { alpha: true, desynchronized: false });
    
    // 캔버스 크기 설정 - 고해상도로 설정
    bufferCanvas.width = 3000;  // 해상도 3배 증가
    bufferCanvas.height = 3000; // 해상도 3배 증가
    
    // 선 선명도 개선을 위한 설정
    bufferCtx.imageSmoothingEnabled = false;
    
    // 캔버스 초기화
    bufferCtx.clearRect(0, 0, bufferCanvas.width, bufferCanvas.height);
    
    // 캔버스 중심점
    const centerX = bufferCanvas.width / 2;
    const centerY = bufferCanvas.height / 2;
    
    // 레이더 범위 그리기 (반투명 원)
    bufferCtx.beginPath();
    bufferCtx.arc(centerX, centerY, centerX * 0.95, 0, 2 * Math.PI);
    bufferCtx.fillStyle = 'rgba(0, 32, 64, 0.4)'; // 배경 불투명도 증가
    bufferCtx.fill();
    
    // 경보 거리 표시 (활성화된 경우)
    if (isAlarmEnabled) {
      const alarmRadius = (alarmDistance / maxRadarRange) * centerX * 0.95;
      
      bufferCtx.beginPath();
      bufferCtx.arc(centerX, centerY, alarmRadius, 0, 2 * Math.PI);
      bufferCtx.strokeStyle = 'rgba(255, 0, 0, 1)'; // 완전 불투명
      bufferCtx.lineWidth = 5; // 선 두께 증가
      bufferCtx.stroke();
      
      // 경보 거리 레이블
      bufferCtx.fillStyle = 'rgba(255, 0, 0, 1)'; // 완전 불투명
      bufferCtx.font = 'bold 18px Arial'; // 글꼴 크기 증가
      bufferCtx.textAlign = 'center';
      bufferCtx.fillText(
        `경보 거리: ${(alarmDistance / 1000).toFixed(1)}km`,
        centerX,
        centerY - alarmRadius - 25
      );
    }
    
    // 레이더 그리드 그리기
    drawRadarGrid(bufferCtx, centerX, centerY, centerX * 0.95);
    
    // 플롯 및 트랙 데이터 그리기
    drawRadarData(bufferCtx, centerX, centerY, centerX * 0.95);
    
    // 버퍼 캔버스 내용을 메인 캔버스로 복사
    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d', { alpha: true, desynchronized: false });
    canvas.width = bufferCanvas.width;
    canvas.height = bufferCanvas.height;
    
    // 최대 선명도 설정
    ctx.imageSmoothingEnabled = false;
    ctx.drawImage(bufferCanvas, 0, 0);
  };
  
  // 레이더 그리드 그리기 함수
  const drawRadarGrid = (ctx, centerX, centerY, radius) => {
    // 캔버스 설정으로 선명도 향상
    ctx.lineCap = 'round';
    ctx.lineJoin = 'round';
    
    // 글로우 효과 함수
    const addGlow = (color, width) => {
      ctx.shadowColor = color;
      ctx.shadowBlur = width;
      ctx.shadowOffsetX = 0;
      ctx.shadowOffsetY = 0;
    };
    
    // 동심원 그리기 (더 세밀하게)
    for (let r = 0.125; r <= 1; r += 0.125) {
      ctx.beginPath();
      ctx.arc(centerX, centerY, radius * r, 0, 2 * Math.PI);
      // 주요 원과 중간 원의 스타일 구분
      if (r % 0.25 === 0) {
        addGlow('rgba(0, 255, 0, 0.7)', 3);
        ctx.strokeStyle = 'rgba(0, 255, 0, 1)'; // 초록색 - 주요 원 (완전 불투명)
        ctx.lineWidth = 3; // 두께 증가
      } else {
        addGlow('rgba(0, 255, 0, 0.4)', 1);
        ctx.strokeStyle = 'rgba(0, 255, 0, 0.8)'; // 초록색 - 중간 원 (높은 불투명도)
        ctx.lineWidth = 1.8; // 두께 증가
      }
      ctx.stroke();
      ctx.shadowColor = 'transparent'; // 그림자 효과 제거
      
      // 거리 표시 (km) - 주요 원에만 표시
      if (r % 0.25 === 0) {
        const distanceKm = (maxRadarRange * r / 1000).toFixed(1);
        ctx.fillStyle = 'rgba(0, 255, 0, 1)'; // 완전 불투명
        ctx.font = 'bold 15px Arial'; // 글꼴 크기 증가
        ctx.textAlign = 'left';
        ctx.fillText(`${distanceKm}km`, centerX + 5, centerY - radius * r - 5);
      }
    }
    
    // 방위선 그리기 (22.5도 간격으로 변경)
    for (let angle = 0; angle < 360; angle += 22.5) {
      const radians = (angle - 90) * Math.PI / 180;
      ctx.beginPath();
      ctx.moveTo(centerX, centerY);
      ctx.lineTo(
        centerX + Math.cos(radians) * radius,
        centerY + Math.sin(radians) * radius
      );
      
      // 주요 방향(45도 간격)과 중간 방향(22.5도 간격) 스타일 구분
      if (angle % 45 === 0) {
        addGlow('rgba(0, 255, 0, 0.7)', 3);
        ctx.strokeStyle = 'rgba(0, 255, 0, 1)'; // 초록색 - 주요 방향 (완전 불투명)
        ctx.lineWidth = 3; // 두께 증가
      } else {
        addGlow('rgba(0, 255, 0, 0.4)', 1);
        ctx.strokeStyle = 'rgba(0, 255, 0, 0.8)'; // 초록색 - 중간 방향 (높은 불투명도)
        ctx.lineWidth = 1.8; // 두께 증가
      }
      ctx.stroke();
      ctx.shadowColor = 'transparent'; // 그림자 효과 제거
      
      // 각도 표시 - 주요 방향(45도 간격)에만 표시
      if (angle % 45 === 0) {
        ctx.fillStyle = 'rgba(0, 255, 0, 1)'; // 완전 불투명
        ctx.font = 'bold 15px Arial'; // 글꼴 크기 증가
        ctx.textAlign = 'center';
        ctx.fillText(
          `${angle}°`,
          centerX + Math.cos(radians) * (radius - 35),
          centerY + Math.sin(radians) * (radius - 35)
        );
      }
    }
    
    // 대각선 그리드 추가 (십자형 그리드)
    // 수평/수직 및 대각선 격자 그리기
    const gridCount = 8; // 격자 분할 수
    for (let i = 1; i < gridCount; i++) {
      const position = (i / gridCount) * radius * 2;
      
      // 수평선
      ctx.beginPath();
      ctx.moveTo(centerX - radius, centerY - radius + position);
      ctx.lineTo(centerX + radius, centerY - radius + position);
      ctx.strokeStyle = 'rgba(0, 255, 0, 0.6)'; // 초록색 - 격자선 (불투명도 증가)
      ctx.lineWidth = 1.5; // 두께 증가
      ctx.stroke();
      
      // 수직선
      ctx.beginPath();
      ctx.moveTo(centerX - radius + position, centerY - radius);
      ctx.lineTo(centerX - radius + position, centerY + radius);
      ctx.strokeStyle = 'rgba(0, 255, 0, 0.6)'; // 초록색 - 격자선 (불투명도 증가)
      ctx.lineWidth = 1.5; // 두께 증가
      ctx.stroke();
    }
    
    // 외곽선 강조
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
    addGlow('rgba(0, 255, 0, 0.8)', 5);
    ctx.strokeStyle = 'rgba(0, 255, 0, 1)'; // 초록색 - 외곽선 (완전 불투명)
    ctx.lineWidth = 4; // 두께 증가
    ctx.stroke();
    ctx.shadowColor = 'transparent'; // 그림자 효과 제거
  };
  
  // 플롯 및 트랙 데이터 그리기 함수
  const drawRadarData = (ctx, centerX, centerY, radius) => {
    const { plots, tracks } = radarData;
    
    // 캔버스 설정으로 선명도 향상
    ctx.lineCap = 'round';
    ctx.lineJoin = 'round';
    
    // 글로우 효과 설정
    ctx.shadowColor = 'rgba(255, 255, 255, 0.7)';
    ctx.shadowBlur = 3;
    ctx.shadowOffsetX = 0;
    ctx.shadowOffsetY = 0;
    
    // 유효한 플롯 필터링 (0, 0, 0 위치 제외)
    const validPlots = plots ? plots.filter(plot => 
      plot.position && 
      plot.position.length >= 2 && 
      !(plot.position[0] === 0 && plot.position[1] === 0 && (plot.position[2] || 0) === 0)
    ) : [];
    
    // 유효한 트랙 필터링
    const validTracks = tracks ? tracks.filter(track => 
      track.position && 
      track.position.length >= 2 &&
      !(track.position[0] === 0 && track.position[1] === 0 && (track.position[2] || 0) === 0)
    ) : [];
    
    console.log(`유효한 플롯: ${validPlots.length}개, 유효한 트랙: ${validTracks.length}개`);
    
    // 플롯 그리기 (작은 점)
    validPlots.forEach(plot => {
      // 직교 좌표를 극좌표로 변환 (x, y -> range, azimuth)
      const [x, y] = plot.position;
      const polarCoords = cartesianToPolar(x, y);
      
      // 정규화된 범위와 방위각
      const normRange = Math.min(polarCoords.range / maxRadarRange, 1);
      const azimuth = polarCoords.azimuth;
      
      // 극좌표를 캔버스 좌표로 변환
      const canvasX = centerX + normRange * radius * Math.cos((azimuth - 90) * Math.PI / 180);
      const canvasY = centerY + normRange * radius * Math.sin((azimuth - 90) * Math.PI / 180);
      
      // 플롯 그리기
      ctx.beginPath();
      ctx.arc(canvasX, canvasY, 6, 0, 2 * Math.PI); // 크기 증가
      ctx.fillStyle = 'rgba(0, 255, 255, 1)'; // 완전 불투명
      ctx.fill();
      
      // 테두리 추가
      ctx.lineWidth = 1.5;
      ctx.strokeStyle = 'rgba(255, 255, 255, 1)';
      ctx.stroke();
      
      // 플롯 ID 표시
      if (plot.id && typeof plot.id === 'number') {
        ctx.font = 'bold 14px Arial'; // 글꼴 크기 및 두께 증가
        ctx.fillStyle = 'rgba(255, 255, 255, 1)'; // 완전 불투명 흰색
        ctx.textAlign = 'center';
        ctx.fillText(`P${plot.id}`, canvasX, canvasY - 12);
      }
    });
    
    // 트랙 그리기 (화살표)
    validTracks.forEach(track => {
      // 직교 좌표를 극좌표로 변환 (x, y -> range, azimuth)
      const [x, y] = track.position;
      const polarCoords = cartesianToPolar(x, y);
      
      // 정규화된 범위와 방위각
      const normRange = Math.min(polarCoords.range / maxRadarRange, 1);
      const azimuth = polarCoords.azimuth;
      
      // 극좌표를 캔버스 좌표로 변환
      const canvasX = centerX + normRange * radius * Math.cos((azimuth - 90) * Math.PI / 180);
      const canvasY = centerY + normRange * radius * Math.sin((azimuth - 90) * Math.PI / 180);
      
      // 트랙 유형에 따른 색상 결정
      const trackColor = track.type === 1 ? 'rgba(255, 255, 0, 1)' : 'rgba(255, 165, 0, 1)'; // 완전 불투명
      
      // 트랙 글로우 효과
      ctx.shadowColor = trackColor;
      ctx.shadowBlur = 5;
      
      // 트랙 그리기 (다이아몬드 모양)
      ctx.beginPath();
      ctx.moveTo(canvasX, canvasY - 10); // 크기 증가
      ctx.lineTo(canvasX + 10, canvasY); // 크기 증가
      ctx.lineTo(canvasX, canvasY + 10); // 크기 증가
      ctx.lineTo(canvasX - 10, canvasY); // 크기 증가
      ctx.closePath();
      ctx.fillStyle = trackColor;
      ctx.fill();
      ctx.lineWidth = 2; // 두께 증가
      ctx.strokeStyle = 'rgba(255, 255, 255, 1)'; // 완전 불투명 흰색
      ctx.stroke();
      
      // 트랙 ID 표시
      if (track.id) {
        ctx.shadowBlur = 3; // 텍스트에 약한 글로우 효과
        ctx.font = 'bold 15px Arial'; // 글꼴 크기 및 두께 증가
        ctx.fillStyle = 'rgba(255, 255, 255, 1)'; // 완전 불투명 흰색
        ctx.textAlign = 'center';
        ctx.fillText(`T${track.id}`, canvasX, canvasY - 15);
      }
      
      // 속도 벡터 그리기 (방향 표시)
      if (track.velocity && track.velocity.length >= 2) {
        const [vx, vy] = track.velocity;
        const speed = Math.sqrt(vx*vx + vy*vy);
        
        if (speed > 0) {
          // 방향 각도 계산 (y축이 반대이므로 vy를 -로 변환)
          const angle = Math.atan2(-vy, vx);
          
          // 화살표 길이는 속도에 비례 (스케일 조정)
          const arrowLength = Math.min(speed * 0.5, 40); // 길이 증가
          
          // 화살표 끝점
          const arrowX = canvasX + Math.cos(angle) * arrowLength;
          const arrowY = canvasY + Math.sin(angle) * arrowLength;
          
          // 화살표 선 그리기
          ctx.beginPath();
          ctx.moveTo(canvasX, canvasY);
          ctx.lineTo(arrowX, arrowY);
          ctx.strokeStyle = trackColor;
          ctx.lineWidth = 3; // 두께 증가
          ctx.stroke();
          
          // 화살표 머리 그리기
          const headSize = 7; // 크기 증가
          ctx.beginPath();
          ctx.moveTo(arrowX, arrowY);
          ctx.lineTo(
            arrowX - headSize * Math.cos(angle - Math.PI / 6),
            arrowY - headSize * Math.sin(angle - Math.PI / 6)
          );
          ctx.lineTo(
            arrowX - headSize * Math.cos(angle + Math.PI / 6),
            arrowY - headSize * Math.sin(angle + Math.PI / 6)
          );
          ctx.closePath();
          ctx.fillStyle = trackColor;
          ctx.fill();
          
          // 속도 표시
          ctx.shadowBlur = 4; // 텍스트에 글로우 효과
          ctx.font = 'bold 14px Arial'; // 글꼴 크기 및 두께 증가
          ctx.fillStyle = 'rgba(255, 255, 255, 1)'; // 완전 불투명 흰색
          ctx.textAlign = 'center';
          ctx.fillText(`${speed.toFixed(0)}m/s`, (canvasX + arrowX) / 2, (canvasY + arrowY) / 2 - 8);
        }
      }
      
      // 그림자 효과 제거
      ctx.shadowColor = 'transparent';
    });
    
    // 레이더 중심점 (원점) 표시
    ctx.beginPath();
    ctx.arc(centerX, centerY, 8, 0, 2 * Math.PI); // 크기 증가
    ctx.fillStyle = 'rgba(255, 0, 0, 1)'; // 완전 불투명
    ctx.fill();
    
    // 중심점에 테두리 추가
    ctx.lineWidth = 2;
    ctx.strokeStyle = 'rgba(255, 255, 255, 1)';
    ctx.stroke();
    
    // 좌표계 정보 표시
    ctx.shadowColor = 'rgba(0, 0, 0, 0.7)';
    ctx.shadowBlur = 4;
    ctx.font = 'bold 16px Arial'; // 글꼴 크기 및 두께 증가
    ctx.fillStyle = 'rgba(255, 255, 255, 1)'; // 완전 불투명
    ctx.textAlign = 'left';
    ctx.fillText(`Range: ${maxRadarRange}m`, 20, 30);
    ctx.fillText(`Plots: ${validPlots.length}, Tracks: ${validTracks.length}`, 20, 60);
    
    // 그림자 제거
    ctx.shadowColor = 'transparent';
  };
  
  // 직교 좌표를 극좌표로 변환하는 함수 (y축이 북쪽이 되도록 조정)
  const cartesianToPolar = (x, y) => {
    // x, y 값이 양수인 경우 레이더 데이터 기준으로 변환
    // 여기서 y가 북쪽(앞쪽) 방향이므로 방위각 계산 조정
    const range = Math.sqrt(x*x + y*y);
    
    // 기준선이 북쪽(y축 양의 방향)이 0도
    // x가 동쪽(오른쪽), y가 북쪽(위쪽)인 좌표계 가정
    let azimuth = Math.atan2(x, y) * 180 / Math.PI;
    
    // 음수 각도 처리
    if (azimuth < 0) {
      azimuth += 360;
    }
    
    return { range, azimuth };
  };
  
  // 컴포넌트 언마운트 시 타이머 정리
  useEffect(() => {
    return () => {
      if (alarmSoundRef.current) {
        alarmSoundRef.current.pause();
        alarmSoundRef.current = null;
      }
      
      // 타이머 정리
      if (alarmPauseTimerRef.current) {
        clearTimeout(alarmPauseTimerRef.current);
      }
    };
  }, []);
  
  return (
    <Box 
      sx={{ 
        width: '100%', 
        height: '100%',
        position: 'relative',
        '& .leaflet-container': { 
          height: '100%', 
          width: '100%',
          zIndex: 0
        }
      }}
    >
      {/* 이 컴포넌트는 React-Leaflet 내부에서 렌더링되므로 여기서 컨트롤 UI를 추가하지 않습니다. 
          대신 leaflet 컨트롤을 사용하여 맵 위에 직접 UI 요소를 배치합니다. */}
      {/* 투명 캔버스 - 맵 위에 오버레이되는 이미지를 위한 소스로 사용됨 */}
      <canvas 
        ref={canvasRef} 
        width={1000} 
        height={1000} 
        style={{ display: 'none' }} 
      />
    </Box>
  );
};

export default MapRadarOverlay; 
