import React, { useState, useEffect, useRef } from 'react';
import {
  Box,
  Tab,
  Tabs,
  Paper,
  Grid,
  Typography,
  TextField,
  Button,
  Divider,
  CircularProgress,
  FormControlLabel,
  Switch,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogContentText,
  DialogActions,
  List,
  ListItem,
  ListItemText,
  Chip,
  Snackbar,
  Alert,
  TableContainer,
  Table,
  TableHead,
  TableBody,
  TableRow,
  TableCell,
  IconButton
} from '@mui/material';
import PowerSettingsNewIcon from '@mui/icons-material/PowerSettingsNew';
import PlayArrowIcon from '@mui/icons-material/PlayArrow';
import StopIcon from '@mui/icons-material/Stop';
import LinkIcon from '@mui/icons-material/Link';
import ArrowRightAltIcon from '@mui/icons-material/ArrowRightAlt';
import DeleteIcon from '@mui/icons-material/Delete';
import axios from 'axios';
import RadarStatusDisplay from './RadarStatusDisplay';
import CombinedRadarDisplay from './CombinedRadarDisplay';
import ConnectionTab from './ConnectionTab';
import BasicSettingsTab from './BasicSettingsTab';
import AdvancedSettingsTab from './AdvancedSettingsTab';
import OperationTab from './OperationTab';
import StatusInfoTab from './StatusInfoTab';
import ScenarioTab from './ScenarioTab';
import { validateAreaSettings } from './debug'; // 디버깅 유틸리티 임포트
import { 
  getRadarStatus, connectToRadar, disconnectFromRadar, 
  sendRadarCommand, sendTrackCommand, getCommands, getCommand,
  getSimulationStatus, startSimulation, stopSimulation,
  deleteTrack, getDeletedTracks  // 트랙 종료 관련 API 추가
} from '../../api';

// API 엔드포인트 설정
const API_BASE_URL = process.env.REACT_APP_API_URL || 'http://localhost:8000';

// TabPanel 컴포넌트
function TabPanel(props) {
  const { children, value, index, ...other } = props;

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`radar-tabpanel-${index}`}
      aria-labelledby={`radar-tab-${index}`}
      {...other}
    >
      {value === index && <Box sx={{ p: 0 }}>{children}</Box>}
    </div>
  );
}

const RadarControl = ({ language }) => {
  // 상태 관리
  const [currentTab, setCurrentTab] = useState(0);
  const [ip, setIp] = useState('192.168.219.78');
  const [port, setPort] = useState('4102');
  const [connected, setConnected] = useState(false);
  const [loading, setLoading] = useState(false);
  const [radarState, setRadarState] = useState(null);
  const [message4001, setMessage4001] = useState(null);
  const [message4003, setMessage4003] = useState(null); // 운용파라미터 응답 메시지
  const [message4005, setMessage4005] = useState(null); // 시스템파라미터 응답 메시지
  const [message4034, setMessage4034] = useState(null); // 영역정보 응답 메시지
  const [otherMessage, setOtherMessage] = useState(null);
  const [statusLog, setStatusLog] = useState([]);
  const [show4001, setShow4001] = useState(true);
  const [selectedCommand, setSelectedCommand] = useState('');
  const [commandDetails, setCommandDetails] = useState(null);
  const [changeLog, setChangeLog] = useState([]);
  const [changedFields, setChangedFields] = useState([]);
  const [showChangeLog, setShowChangeLog] = useState(false);
  const [prevMessage4001, setPrevMessage4001] = useState(null);
  const [plotData, setPlotData] = useState([]);
  const [trackData, setTrackData] = useState([]);
  const [operationMode, setOperationMode] = useState(0);
  const [showRawData, setShowRawData] = useState(false);
  const [successSnackbar, setSuccessSnackbar] = useState({ open: false, message: '' });
  const [errorDialog, setErrorDialog] = useState({ open: false, message: '' });
  const [pollingInterval, setPollingInterval] = useState(1000);
  const [commands, setCommands] = useState([]);
  const [plotSendFlag, setPlotSendFlag] = useState(1); // 플롯 데이터 전송 여부 (0: 전송안함, 1: 전송)
  const [trackSendFlag, setTrackSendFlag] = useState(1); // 트랙 데이터 전송 여부 (0: 전송안함, 1: 전송)
  const changeLogRef = useRef(null); // 변경 로그 스크롤 ref
  
  // 성능 파라미터
  const [performanceParams, setPerformanceParams] = useState({
    rdp: {
      lowAltitude: 0,      // 최소 고도 (m)
      maxVelocity: 500     // 최대 속도 (m/s)
    },
    rsp: [
      {
        cfar_threshold: -55, // CFAR 임계값 (dB)
        maxRange: 10000,    // 최대 거리 (m)
        minVelocity: 0,     // 최소 속도 (m/s)
        minRange: 0,        // 최소 거리 (m)
      }
    ]
  });
  
  // 영역 설정 파라미터
  const [areaSettings, setAreaSettings] = useState(null);
  
  // 운용 시작/정지 상태
  const [isOperating, setIsOperating] = useState(false);
  
  // 운용 파라미터 (3014)
  const [operationParams, setOperationParams] = useState({
    rdp: {
      longitude: 126.97806, // 경도 (도)
      latitude: 37.56667,   // 위도 (도)
      altitude: 100,        // 고도 (m)
    },
    rsp: [
      {
        panel_num: 0,       // 패널 번호
        tilt: 0,            // 틸트 각도 (도)
        slant: 0,           // 슬랜트 각도 (도)
        heading: 0,         // 헤딩 각도 (도)
      }
    ]
  });
  
  // 언어 텍스트 번역 함수
  const translate = (korean, english, language) => {
    // 인도네시아어 지원 추가 (기본값은 영어와 동일)
    if (language === 'ko') return korean;
    if (language === 'id') {
      // 기본적으로 영어를 반환하고, 추후 인도네시아어 텍스트를 추가할 수 있음
      return english;
    }
    return english;
  };

  // 에러 처리 함수
  const handleError = (message) => {
    setErrorDialog({ open: true, message });
  };
  
  // 에러 메시지 표시 함수
  const showErrorMessage = (message) => {
    setErrorDialog({ open: true, message });
  };
  
  // 성공 메시지 표시 함수
  const showSuccessMessage = (message) => {
    setSuccessSnackbar({ open: true, message });
  };

  // 메시지 처리 함수
  const processMessage = (message) => {
    try {
      console.log("메시지 수신 (원본):", message);
      console.log("메시지 타입:", typeof message);
      
      let data;
      // 문자열인 경우 JSON으로 파싱
      if (typeof message === 'string') {
        try {
          data = JSON.parse(message);
        } catch (err) {
          console.error("JSON 파싱 오류:", err);
          return;
        }
      } else {
        // 이미 객체인 경우 그대로 사용
        data = message;
      }
      
      console.log("메시지 파싱 결과:", data);
      
      // 헤더 내부에 ID가 있는 경우 (V20 프로토콜)
      if (data.header && data.header.id !== undefined) {
        console.log("헤더 내부 ID 감지:", data.header.id);
        
        // ID 기반으로 메시지 처리
        const messageId = data.header.id;
        
        if (messageId === 4003) {
          console.log('운용 파라미터 정보 메시지 수신 (헤더 형식):', data);
          setMessage4003(data);
          console.log("message4003 상태 설정 완료 (헤더 형식):", data);
          showSuccessMessage('운용 파라미터 정보가 수신되었습니다.');
          return;
        } else if (messageId === 4005) {
          console.log('시스템 파라미터 정보 메시지 수신 (헤더 형식):', data);
          setMessage4005(data);
          console.log("message4005 상태 설정 완료 (헤더 형식):", data);
          showSuccessMessage('시스템 파라미터 정보가 수신되었습니다.');
          return;
        } else if (messageId === 4034) {
          console.log('영역 정보 메시지 수신 (헤더 형식):', data);
          // 영역 정보의 모든 값이 숫자형인지 확인
          try {
            // 영역 정보의 모든 값이 문자열인 경우 숫자로 변환
            if (data.Area && Array.isArray(data.Area)) {
              console.log("영역 정보 타입 변환 전:", JSON.stringify(data, null, 2));
              
              data.Area = data.Area.map(area => ({
                ...area,
                Type: Number(area.Type),
                HeightBottom: Number(area.HeightBottom),
                HeightTop: Number(area.HeightTop),
                Point: area.Point.map(point => ({
                  ...point,
                  Longitude: Number(point.Longitude),
                  Latitude: Number(point.Latitude)
                }))
              }));
              
              console.log("영역 정보 타입 변환 후:", JSON.stringify(data, null, 2));
            }
          } catch (err) {
            console.error("영역 정보 타입 변환 오류:", err);
          }
          
          setMessage4034(data);
          console.log("message4034 상태 설정 완료 (헤더 형식):", data);
          showSuccessMessage('영역 정보가 수신되었습니다.');
          return;
        }
      }
      
      // 메시지 ID가 없는 경우 처리 중단
      if (!data || !data.id) {
        console.error("메시지 ID가 없음:", data);
        return;
      }
      
      console.log("메시지 ID:", data.id);
      
      // 메시지 ID 확인
      if (data.id === 4001) {
        // 상태 정보 메시지
        console.log("4001 메시지 처리 시작:", data);
        setMessage4001(data);
        console.log("4001 메시지 처리 완료");
        
        // 운용 모드 업데이트 (데이터 구조 체크 강화)
        if (data.rdp) {
          if (data.rdp.op_mode !== undefined) {
          setOperationMode(data.rdp.op_mode);
        }
        
        // 레이더 상태 정보 업데이트
        setRadarState({
            last_update: data.time_sec || Date.now() / 1000,
            op_mode: data.rdp.op_mode || 0,
            track_count: data.rdp.track_count || 0,
            plot_count: data.rdp.plot_count || 0,
            panels: data.rsp || []
          });
        } else if (data.Rdp) {
          // 대문자 스타일 구조체 처리
          if (data.Rdp.op_mode !== undefined) {
            setOperationMode(data.Rdp.op_mode);
          }
          
          // 레이더 상태 정보 업데이트 (대문자 스타일)
          setRadarState({
            last_update: data.time_sec || Date.now() / 1000,
            op_mode: data.Rdp.op_mode || 0,
            track_count: data.Rdp.track_count || 0,
            plot_count: data.Rdp.plot_count || 0,
            panels: data.Rsp || []
          });
        }
      } else if (data.id === 4003) {
        // 운용 파라미터 정보 메시지 (3013 요청에 대한 응답)
        console.log('운용 파라미터 정보 메시지 수신:', data);
        setMessage4003(data);
        console.log("message4003 상태 설정 완료:", data);
        showSuccessMessage('운용 파라미터 정보가 수신되었습니다.');
      } else if (data.id === 4005) {
        // 시스템 파라미터 정보 메시지 (3011 요청에 대한 응답)
        console.log('시스템 파라미터 정보 메시지 수신:', data);
        setMessage4005(data);
        console.log("message4005 상태 설정 완료:", data);
        showSuccessMessage('시스템 파라미터 정보가 수신되었습니다.');
      } else if (data.id === 4034) {
        // 영역 정보 메시지 (3034 또는 3035 요청에 대한 응답)
        console.log('영역 정보 메시지 수신:', data);
        
        // 영역 정보의 모든 값이 숫자형인지 확인
        try {
          // 영역 정보의 모든 값이 문자열인 경우 숫자로 변환
          if (data.Area && Array.isArray(data.Area)) {
            console.log("영역 정보 타입 변환 전:", JSON.stringify(data, null, 2));
            
            data.Area = data.Area.map(area => ({
              ...area,
              Type: Number(area.Type),
              HeightBottom: Number(area.HeightBottom),
              HeightTop: Number(area.HeightTop),
              Point: area.Point.map(point => ({
                ...point,
                Longitude: Number(point.Longitude),
                Latitude: Number(point.Latitude)
              }))
            }));
            
            console.log("영역 정보 타입 변환 후:", JSON.stringify(data, null, 2));
          }
        } catch (err) {
          console.error("영역 정보 타입 변환 오류:", err);
        }
        
        setMessage4034(data);
        console.log("message4034 상태 설정 완료:", data);
        showSuccessMessage('영역 정보가 수신되었습니다.');
      } else if (data.id === 5001) {
        // 플롯 정보 메시지
        // 극좌표계 데이터 변환 및 저장
        if (data.plots && Array.isArray(data.plots)) {
          const polarPlots = data.plots.map(plot => ({
            ...plot,
            plot_id: plot.plot_id || 0,
            azimuth: plot.azimuth || 0,
            range: plot.range || 0,
            altitude: plot.altitude || 0,
            velocity: plot.velocity || 0,
            time_sec: data.time_sec
          }));
          
          setPlotData(polarPlots);
        }
      } else if (data.id === 5002) {
        // 트랙 정보 메시지
        // 극좌표계 데이터 변환 및 저장
        if (data.tracks && Array.isArray(data.tracks)) {
          const polarTracks = data.tracks.map(track => ({
            ...track,
            track_id: track.track_id || 0,
            azimuth: track.azimuth || 0,
            range: track.range || 0,
            altitude: track.altitude || 0,
            velocity: track.velocity || 0,
            time_sec: data.time_sec
          }));
          
          setTrackData(polarTracks);
        }
      } else {
        // 기타 메시지
        console.log("기타 메시지 수신:", data);
        setOtherMessage(data);
      }
    } catch (error) {
      console.error('메시지 처리 오류:', error);
    }
  };
  
  // 사용 가능한 명령어 목록 가져오기
  const fetchCommands = async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`);
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      const data = await response.json();
      setCommands(data.commands || []);
    } catch (error) {
      handleError(error.message);
    }
  };
  
  // 명령어 상세 정보 가져오기
  const fetchCommandDetails = async (commandId) => {
    if (!commandId) return;
    
    try {
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/${commandId}/`);
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      const data = await response.json();
      setCommandDetails(data.command);
    } catch (error) {
      handleError(error.message);
    }
  };
  
  // 레이더 상태 정보 가져오기
  const fetchRadarState = async () => {
    try {
      console.log("레이더 상태 정보 요청 시작");
      const response = await fetch(`${API_BASE_URL}/api/radar/state/`);
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      const data = await response.json();
      console.log("레이더 상태 정보 응답:", data);
      
      // 연결 상태 업데이트
      setConnected(data.connected);
      setRadarState(data);
      
      // 4001 메시지가 있으면 업데이트 (연결 상태와 관계없이)
      if (data.last_4001_state) {
        console.log("4001 상태 메시지 업데이트:", data.last_4001_state);
          setMessage4001(data.last_4001_state);
        }
        
      // 기타 메시지 업데이트
        if (data.last_other_state) {
          setOtherMessage(data.last_other_state);
      }
    } catch (error) {
      console.error("상태 업데이트 실패:", error);
    }
  };
  
  // 변경 로그 표시 토글
  const handleChangeLogToggle = () => {
    setShowChangeLog(!showChangeLog);
  };
  
  // 변경 로그 초기화
  const clearChangeLog = () => {
    setChangeLog([]);
  };
  
  // 4001 메시지 조회
  const fetch4001Message = async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/api/radar/messages/4001/`);
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      const data = await response.json();
      
      // 항상 메시지를 업데이트 (show4001 조건 제거)
      if (data.message) {
        // 이전 메시지와 현재 메시지 비교하여 변경된 필드 감지
        if (message4001) {
          const changedKeys = [];
          const timestamp = new Date().toLocaleTimeString();
          const newChanges = [];
          
          for (const key in data.message) {
            if (JSON.stringify(data.message[key]) !== JSON.stringify(message4001[key])) {
              changedKeys.push(key);
              newChanges.push({
                timestamp,
                field: key,
                oldValue: message4001[key],
                newValue: data.message[key]
              });
            }
          }
          
          if (newChanges.length > 0) {
            setChangeLog(prevLog => [...newChanges, ...prevLog].slice(0, 100)); // 최대 100개 항목 유지
          }
          
          setChangedFields(changedKeys);
          setPrevMessage4001(message4001);
        }
        
        setMessage4001(data.message);
      }
    } catch (error) {
      console.error("4001 메시지 조회 실패:", error);
    }
  };
  
  // 기타 메시지 조회
  const fetchOtherMessage = async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/api/radar/messages/other/`);
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      const data = await response.json();
      
      // 새 데이터가 있는 경우에만 갱신
      if (data.message && data.has_new_data) {
        setOtherMessage(data.message);
      }
    } catch (error) {
      console.error("기타 메시지 조회 실패:", error);
    }
  };
  
  // 폴링 주기 변경 핸들러
  const handlePollingIntervalChange = (event) => {
    setPollingInterval(parseInt(event.target.value));
  };
  
  // 컴포넌트 마운트 시 초기 상태 정보 가져오기
  useEffect(() => {
    console.log("컴포넌트 마운트 - 초기 상태 정보 요청");
        fetchRadarState();
        fetch4001Message();
  }, []);  // 의존성 배열을 비워 컴포넌트 마운트 시 한 번만 실행
  
  // WebSocket 연결 설정
  useEffect(() => {
    // 초기 데이터 로드
    fetchCommands();
    
    // 주기적인 상태 업데이트를 위한 폴링 설정
    const statusInterval = setInterval(() => {
      if (connected) {
        fetch4001Message();
        fetchRadarState();
      }
    }, 2000); // 2초마다 상태 정보 요청
    
    // 컴포넌트 언마운트 시 정리
    return () => {
      clearInterval(statusInterval);
    };
  }, [connected]);  // connected 상태가 변경될 때 재실행
  
  // message4034가 업데이트될 때 areaSettings도 업데이트
  useEffect(() => {
    if (message4034) {
      console.log('message4034 업데이트됨, areaSettings 업데이트 중...');
      // 깊은 복사를 통해 새 객체 참조 생성
      const newAreaSettings = JSON.parse(JSON.stringify(message4034));
      setAreaSettings(newAreaSettings);
      console.log('areaSettings 업데이트 완료:', newAreaSettings);
    }
  }, [message4034]);
  
  // 변경 로그 스크롤 관리
  useEffect(() => {
    if (changeLogRef.current && showChangeLog) {
      changeLogRef.current.scrollTop = 0;
    }
  }, [changeLog, showChangeLog]);

  // 탭 변경 처리 함수
  const handleTabChange = (event, newValue) => {
    setCurrentTab(newValue);
  };

  // 연결/연결 해제 처리 함수
  const toggleConnection = async () => {
    try {
      setLoading(true);
      
      if (connected) {
        // 연결 해제 요청 - connection 엔드포인트 사용
        const response = await fetch(`${API_BASE_URL}/api/radar/connection/`, {
          method: 'DELETE',
        });
        
        if (!response.ok) {
          throw new Error(`HTTP 오류: ${response.status}`);
        }
        
        const data = await response.json();
        if (data.success) {
          setConnected(false);
          // 연결 해제 시 데이터 초기화
          setPlotData([]);
          setTrackData([]);
          setMessage4001(null);
          setPrevMessage4001(null);
          setChangeLog([]);
          showSuccessMessage('레이더 연결이 해제되었습니다.');
        } else {
          throw new Error(data.message || '연결 해제 실패');
        }
      } else {
        // 연결 요청 - connection 엔드포인트 사용
        const response = await fetch(`${API_BASE_URL}/api/radar/connection/`, {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ ip, port: parseInt(port) }),
        });
        
        if (!response.ok) {
          throw new Error(`HTTP 오류: ${response.status}`);
        }
        
        const data = await response.json();
        if (data.success) {
            setConnected(true);
          showSuccessMessage('레이더 연결이 성공적으로 이루어졌습니다.');
          
          // 연결 후 초기 데이터 요청
          await fetchRadarState();
          await fetch4001Message();
          
          // 문서에 따른 초기 설정 요청
          await sendTimeSync(); // 2) 시간동기명령을 보낸다
          await requestOperationParams(); // 4) 운용파라미터를 요청(3013)
          await requestPerformanceParams(); // 5) 시스템파라미터를 요청(3011)
          await requestBeamSchedule(); // 6) 빔스케쥴정보 요청(3031)
          await requestAreaSettings(); // 7) 영역설정정보 요청(3034)
          } else {
          throw new Error(data.message || '연결 실패');
        }
      }
    } catch (error) {
      handleError(`연결 ${connected ? '해제' : ''} 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 시간 동기화 요청 함수 (3001)
  const sendTimeSync = async () => {
    try {
      setLoading(true);
      
      // 현재 시간 기준 Unix 타임스탬프 생성 (초 및 마이크로초)
      const now = new Date();
      const tv_sec = Math.floor(now.getTime() / 1000);
      const tv_usec = (now.getTime() % 1000) * 1000;
      
      // 백엔드 프로토콜에 맞는 명확한 형식으로 데이터 구성
      const requestData = {
        command_id: 3001,
        params: {
          header: {
            id: 3001,
            tv_sec: tv_sec,
            tv_usec: tv_usec
          },
          tv_sec: tv_sec,
          tv_usec: tv_usec
        }
      };
      
      console.log("시간 동기화 요청 데이터:", JSON.stringify(requestData, null, 2));
      
      // 문서에 따르면 시간동기명령은 3001
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(requestData),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
      showSuccessMessage('시간 동기화가 완료되었습니다.');
      } else {
        throw new Error(data.error || '시간 동기화 실패');
      }
    } catch (error) {
      handleError(`시간 동기화 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 운용 파라미터 요청 함수 (3013)
  const requestOperationParams = async () => {
    try {
      setLoading(true);
      
      // 문서에 따르면 운용파라미터요청은 3013
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          command_id: 3013,
          params: {}
        }),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        // 서버로부터 응답 받은 후 4003 메시지는 processMessage에서 처리됨
        showSuccessMessage('운용 파라미터 요청이 완료되었습니다.');
      } else {
        throw new Error(data.error || '운용 파라미터 요청 실패');
      }
    } catch (error) {
      handleError(`운용 파라미터 요청 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 성능 파라미터 요청 함수 (3011)
  const requestPerformanceParams = async () => {
    try {
      setLoading(true);
      
      // 문서에 따르면 성능파라미터요청은 3011
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          command_id: 3011,
          params: {}
        }),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        // 서버로부터 응답 받은 후 4005 메시지는 processMessage에서 처리됨
        showSuccessMessage('성능 파라미터 요청이 완료되었습니다.');
      } else {
        throw new Error(data.error || '성능 파라미터 요청 실패');
      }
    } catch (error) {
      handleError(`성능 파라미터 요청 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 빔 스케줄 요청 함수 (3031)
  const requestBeamSchedule = async () => {
    try {
      setLoading(true);
      
      // 문서에 따르면 빔스케쥴정보요청은 3031
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          command_id: 3031,
          params: {}
        }),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        // 서버로부터 응답 받은 후 4031 메시지는 processMessage에서 처리됨
        showSuccessMessage('빔 스케줄 요청이 완료되었습니다.');
      } else {
        throw new Error(data.error || '빔 스케줄 요청 실패');
      }
    } catch (error) {
      handleError(`빔 스케줄 요청 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 영역 설정 정보 요청 함수 (3034)
  const requestAreaSettings = async () => {
    try {
      console.log('영역 설정 정보 요청 시작...');
      setLoading(true);
      
      // 현재 시간 기준 Unix 타임스탬프 생성 (초 및 마이크로초)
      const now = new Date();
      const tv_sec = Math.floor(now.getTime() / 1000);
      const tv_usec = (now.getTime() % 1000) * 1000;
      
      // 문서에 따르면 영역설정정보요청은 3034
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          command_id: 3034,
          params: {
            header: {
              id: 3034,
              tv_sec: tv_sec,
              tv_usec: tv_usec
            }
          }
        }),
      });
      
      if (!response.ok) {
        // HTTP 500 오류는 백엔드에서 명령어를 찾을 수 없는 경우 발생
        if (response.status === 500) {
          console.warn('영역 정보 요청(3034)이 지원되지 않을 수 있습니다. 최근 변경된 값을 유지합니다.');
          // UI 상태를 초기화하지 않고 경고만 표시
          showSuccessMessage('영역 설정 정보를 불러올 수 없습니다. 최근 변경 값이 유지됩니다.');
          return;
        }
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        // 서버로부터 응답 받은 후 4034 메시지는 processMessage에서 처리됨
        showSuccessMessage('영역 설정 요청이 완료되었습니다.');
      } else {
        throw new Error(data.error || '영역 설정 요청 실패');
      }
    } catch (error) {
      console.error('영역 설정 요청 실패:', error);
      handleError(`영역 설정 요청 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 운용 파라미터 업데이트 함수 (3014)
  const updateOperationParams = async (params) => {
    try {
      setLoading(true);
      
      console.log("원본 운용 파라미터:", params);
      
      // 백엔드 형식에 맞게 데이터 변환
      const convertedParams = {
        "Rdp": {
          "Longitude": parseFloat(params.rdp.longitude) || 0.0,
          "Latitude": parseFloat(params.rdp.latitude) || 0.0,
          "Altitude": parseFloat(params.rdp.altitude) || 0.0,
          "AzimuthOffset": 0.0,
          "NtpServer": [0, 0, 0, 0]
        },
        "RadarPanelNum": 1,
        "Rsp": [
          {
            "Longitude": parseFloat(params.rdp.longitude) || 0.0,
            "Latitude": parseFloat(params.rdp.latitude) || 0.0,
            "Altitude": parseFloat(params.rdp.altitude) || 0.0,
            "Tilt": parseFloat(params.rsp[0].tilt) || 0.0,
            "Slant": parseFloat(params.rsp[0].slant) || 0.0,
            "Heading": parseFloat(params.rsp[0].heading) || 0.0
          }
        ]
      };
      
      console.log("변환된 운용 파라미터:", convertedParams);
      
      // 문서에 따르면 운용파라미터설정요청은 3014
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ 
        command_id: 3014,
          params: convertedParams 
        }),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        showSuccessMessage('운용 파라미터가 업데이트되었습니다.');
        // 업데이트 후 최신 정보 다시 요청
        requestOperationParams();
      } else {
        throw new Error(data.error || '운용 파라미터 업데이트 실패');
      }
    } catch (error) {
      handleError(`운용 파라미터 업데이트 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 성능 파라미터 업데이트 함수 (3012)
  const updatePerformanceParams = async (params) => {
    try {
      setLoading(true);
      
      console.log("원본 성능 파라미터:", params);
      
      // 백엔드 형식에 맞게 데이터 변환
      const convertedParams = {
        "Rdp": {
          "LowAltitude": parseInt(params.rdp.lowAltitude) || 0,
          "MaxVelocity": parseInt(params.rdp.maxVelocity) || 0
        },
        "RadarPanelNum": 1,
        "Rsp": [
          {
            "CFAR_Threshold": parseInt(params.rsp[0].cfar_threshold) || 0,
            "MinRange": parseFloat(params.rsp[0].minRange) || 0.0,
            "MaxRange": parseFloat(params.rsp[0].maxRange) || 0.0,
            "MinVelocity": parseFloat(params.rsp[0].minVelocity) || 0.0,
            "TxAtten": parseInt(params.rsp[0].txAtten) || 0,
            "RxAtten": parseInt(params.rsp[0].rxAtten) || 0
          }
        ]
      };
      
      console.log("변환된 성능 파라미터:", convertedParams);
      
      // 문서에 따르면 성능파라미터설정요청은 3012
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ 
        command_id: 3012,
          params: convertedParams 
        }),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        showSuccessMessage('성능 파라미터가 업데이트되었습니다.');
        // 업데이트 후 최신 정보 다시 요청
        requestPerformanceParams();
      } else {
        throw new Error(data.error || '성능 파라미터 업데이트 실패');
      }
    } catch (error) {
      handleError(`성능 파라미터 업데이트 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 영역 설정 업데이트 함수 (3035)
  const updateAreaSettings = async (settings) => {
    try {
      // 파라미터 검증
      if (!settings || !settings.Area || !Array.isArray(settings.Area)) {
        throw new Error('영역 설정 정보가 유효하지 않습니다.');
      }
      
      // 영역 설정 유효성 검사
      const isValid = validateAreaSettings(settings);
      if (!isValid) {
        throw new Error('영역 설정 유효성 검사 실패');
      }
      
      console.log("영역 설정 업데이트 요청:", settings);
      setLoading(true);
      
      // 백엔드 프로토콜에 맞는 형식으로 데이터 구성
      const convertedParams = {
        nArea: settings.nArea,
        Area: settings.Area.map(area => {
          // Valid 값이 숫자가 아닌 경우를 대비해 명시적으로 숫자로 변환
          let validValue = parseInt(area.Valid);
          
          // 중요: 숫자 0은 falsy 값이므로 별도 검사 필요
          // NaN인 경우에만 기본값 1을 사용
          if (isNaN(validValue)) {
            validValue = 1;
          }
          
          console.log("원본 Valid 값:", area.Valid, "변환 후 Valid 값:", validValue);
          
          return {
            Valid: validValue,  // 영역 적용 여부 (0: 적용하지 않음, 1: 적용함)
            Type: parseInt(area.Type) || 1,    // 영역 타입 (기본값 1)
            HeightBottom: parseInt(area.HeightBottom) || 0,
            HeightTop: parseInt(area.HeightTop) || 1000,
            Point: area.Point.map(point => ({
              Longitude: parseFloat(point.Longitude) || 0,
              Latitude: parseFloat(point.Latitude) || 0
            }))
          };
        })
      };
      
      console.log("변환된 영역 설정 파라미터:", convertedParams);
      
      // 문서에 따르면 영역설정변경요청은 3035
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          command_id: 3035, 
          params: convertedParams
        }),
      });
      
      console.log("영역 설정 요청 응답:", response.status);
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      console.log("영역 설정 요청 응답 데이터:", data);
      
      if (data.message) {
        showSuccessMessage('영역 설정이 업데이트되었습니다.');
        // 업데이트 후 최신 정보 다시 요청
        requestAreaSettings();
      } else {
        throw new Error(data.error || '영역 설정 업데이트 실패');
      }
    } catch (error) {
      handleError(`영역 설정 업데이트 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };
  
  // 운용 시작 함수 (3002)
  const startOperation = async () => {
    try {
      setLoading(true);
      
      // 현재 시간 기준 Unix 타임스탬프 생성 (초 및 마이크로초)
      const now = new Date();
      const tv_sec = Math.floor(now.getTime() / 1000);
      const tv_usec = (now.getTime() % 1000) * 1000;
      
      // 백엔드 프로토콜에 맞는 명확한 형식으로 데이터 구성
      const requestData = {
        command_id: 3002,
        params: {
          header: {
            id: 3002,
            tv_sec: tv_sec,
            tv_usec: tv_usec
          },
          op_mode: 1 // 1: 시작
        }
      };
      
      console.log("운용 시작 요청 데이터:", JSON.stringify(requestData, null, 2));
      
      // 문서에 따르면 레이더운용제어는 3002, op_mode 파라미터로 1(시작)을 전달
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(requestData),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        setIsOperating(true);
        showSuccessMessage('레이더 운용이 시작되었습니다.');
      } else {
        throw new Error(data.error || '운용 시작 실패');
      }
    } catch (error) {
      handleError(`운용 시작 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };

  // 운용 정지 함수 (3002)
  const stopOperation = async () => {
    try {
      setLoading(true);
      
      // 현재 시간 기준 Unix 타임스탬프 생성 (초 및 마이크로초)
      const now = new Date();
      const tv_sec = Math.floor(now.getTime() / 1000);
      const tv_usec = (now.getTime() % 1000) * 1000;
      
      // 백엔드 프로토콜에 맞는 명확한 형식으로 데이터 구성
      const requestData = {
        command_id: 3002,
        params: {
          header: {
            id: 3002,
            tv_sec: tv_sec,
            tv_usec: tv_usec
          },
          op_mode: 0 // 0: 정지
        }
      };
      
      console.log("운용 정지 요청 데이터:", JSON.stringify(requestData, null, 2));
      
      // 문서에 따르면 레이더운용제어는 3002, op_mode 파라미터로 0(정지)을 전달
      const response = await fetch(`${API_BASE_URL}/api/radar/commands/`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(requestData),
      });
      
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      
      const data = await response.json();
      if (data.message) {
        setIsOperating(false);
        // 운용 정지 시 시나리오 데이터 초기화 (기본 배경 플롯은 유지될 수 있음)
        console.log("운용 정지: 시나리오 데이터 초기화");
        // 플롯과 트랙 데이터를 초기화
        setPlotData([]);
        setTrackData([]);
        showSuccessMessage('레이더 운용이 정지되었습니다.');
      } else {
        throw new Error(data.error || '운용 정지 실패');
      }
    } catch (error) {
      handleError(`운용 정지 실패: ${error.message}`);
    } finally {
      setLoading(false);
    }
  };

  // 원시 데이터 표시 토글 함수
  const handleRawDataToggle = () => {
    setShowRawData(!showRawData);
  };

  // 탭 라벨 배열에 '시나리오' 추가
  const tabLabels = ['연결', '운용', '기본설정', '고급설정', '상태정보', '시나리오'];

  // 트랙 클래스명 반환 함수 추가
  const getTrackClassName = (classId) => {
    switch (classId) {
      case 1: return "항공기";
      case 2: return "드론";
      case 3: return "조류";
      case 4: return "기타";
      default: return "미분류";
    }
  };

  // 성공/에러 메시지 표시 함수
  const showNotification = (message, type = 'success') => {
    if (type === 'success') {
      showSuccessMessage(message);
    } else {
      showErrorMessage(message);
    }
  };

  // 데이터 갱신 트리거 상태 추가
  const [refreshTrigger, setRefreshTrigger] = useState(0);

  // 트랙 테이블에 종료 버튼 렌더링 추가 
  const renderTrackTable = () => {
    return (
      <TableContainer component={Paper} sx={{ mb: 2, backgroundColor: 'background.paper' }}>
        <Table size="small">
          <TableHead>
            <TableRow>
              <TableCell>ID</TableCell>
              <TableCell>클래스</TableCell>
              <TableCell>거리(m)</TableCell>
              <TableCell>방위각(°)</TableCell>
              <TableCell>고도(m)</TableCell>
              <TableCell>속도(m/s)</TableCell>
              <TableCell>시간</TableCell>
              <TableCell>작업</TableCell> {/* 작업 열 추가 */}
            </TableRow>
          </TableHead>
          <TableBody>
            {trackData.length > 0 ? (
              trackData.map((track, index) => {
                // 트랙 속도 계산
                let trackSpeed = 0;
                if (track.velocity && Array.isArray(track.velocity) && track.velocity.length >= 2) {
                  const [vx, vy, vz] = track.velocity;
                  trackSpeed = Math.sqrt(vx*vx + vy*vy + (vz || 0)*(vz || 0));
                } else {
                  trackSpeed = track.speed || 0;
                }
                
                // 트랙 시간 포맷팅
                let trackTime = 'N/A';
                if (track.tv_sec) {
                  trackTime = new Date(track.tv_sec * 1000).toLocaleTimeString();
                } else if (track.time) {
                  if (track.time > 1000000000) { // 유닉스 타임스탬프로 가정
                    trackTime = new Date(track.time * 1000).toLocaleTimeString();
                  } else {
                    trackTime = track.time.toString();
                  }
                }
                
                return (
                  <TableRow key={index} hover>
                    <TableCell>{track.trackId || 'N/A'}</TableCell>
                    <TableCell>{getTrackClassName(track.Class)}</TableCell>
                    <TableCell>{track.Range ? track.Range.toFixed(1) : 'N/A'}</TableCell>
                    <TableCell>{track.Azimuth ? track.Azimuth.toFixed(1) : 'N/A'}</TableCell>
                    <TableCell>{track.Elevation ? track.Elevation.toFixed(1) : 'N/A'}</TableCell>
                    <TableCell>{trackSpeed ? trackSpeed.toFixed(1) : 'N/A'}</TableCell>
                    <TableCell>{trackTime}</TableCell>
                    <TableCell>
                      <IconButton 
                        size="small" 
                        color="error" 
                        onClick={() => handleDeleteTrack(track.trackId)}
                        title="트랙 종료"
                      >
                        <DeleteIcon fontSize="small" />
                      </IconButton>
                    </TableCell>
                  </TableRow>
                );
              })
            ) : (
              <TableRow>
                <TableCell colSpan={8} align="center">
                  표시할 트랙 데이터가 없습니다
                </TableCell>
              </TableRow>
            )}
          </TableBody>
        </Table>
      </TableContainer>
    );
  };

  // 트랙 종료 처리 함수 추가
  const handleDeleteTrack = async (trackId) => {
    if (!trackId) {
      showNotification('트랙 ID가 없습니다', 'error');
      return;
    }

    try {
      // 로딩 상태 표시
      setLoading(true);
      
      // 트랙 종료 요청 API 호출
      const response = await deleteTrack(trackId);
      
      // 성공 메시지 표시
      showNotification(response.message || `트랙 ${trackId} 종료 요청 성공`, 'success');
      
      // 상태 업데이트 (필요시 트랙 데이터 다시 가져오기)
      setRefreshTrigger(prev => prev + 1);
    } catch (error) {
      const errorMessage = error.response?.data?.message || `트랙 ${trackId} 종료 요청 실패`;
      showNotification(errorMessage, 'error');
    } finally {
      // 로딩 상태 해제
      setLoading(false);
    }
  };

  return (
    <div>
      <Paper 
        elevation={3} 
        sx={{
          backgroundColor: '#0e2945',
          borderRadius: '8px',
          overflow: 'hidden',
          mb: 3,
          '& .MuiTabs-indicator': {
            backgroundColor: '#42a5f5',
            height: '4px'
          }
        }}
      >
        <Tabs
          value={currentTab}
          onChange={(e, newValue) => setCurrentTab(newValue)}
          variant="scrollable"
          scrollButtons="auto"
          sx={{ mb: 2, borderBottom: 1, borderColor: 'divider' }}
        >
          {tabLabels.map((label, index) => (
            <Tab 
              key={index} 
              label={translate(label, getEnglishLabel(label), language)} 
              sx={{ minWidth: 'auto' }} 
            />
          ))}
        </Tabs>

      {/* 연결 탭 */}
      <TabPanel value={currentTab} index={0}>
          <ConnectionTab 
            language={language}
            translate={translate}
            ip={ip}
            setIp={setIp}
            port={port}
            setPort={setPort}
            connected={connected}
            loading={loading}
            toggleConnection={toggleConnection}
            sendTimeSync={sendTimeSync}
            requestOperationParams={requestOperationParams}
            requestPerformanceParams={requestPerformanceParams}
            requestBeamSchedule={requestBeamSchedule}
            requestAreaSettings={requestAreaSettings}
            message4001={message4001}
            message4003={message4003}
            message4005={message4005}
            message4034={message4034}
          />
      </TabPanel>

      {/* 기본설정 탭 */}
      <TabPanel value={currentTab} index={1}>
          <BasicSettingsTab 
            language={language}
            translate={translate}
            operationParams={operationParams}
            setOperationParams={setOperationParams}
            loading={loading}
            requestOperationParams={requestOperationParams}
            updateOperationParams={updateOperationParams}
          />
      </TabPanel>

      {/* 고급설정 탭 */}
      <TabPanel value={currentTab} index={2}>
          <AdvancedSettingsTab 
            language={language}
            translate={translate}
            performanceParams={performanceParams}
            setPerformanceParams={setPerformanceParams}
            loading={loading}
            requestPerformanceParams={requestPerformanceParams}
            updatePerformanceParams={updatePerformanceParams}
            areaSettings={areaSettings}
            setAreaSettings={setAreaSettings}
            requestAreaSettings={requestAreaSettings}
            updateAreaSettings={updateAreaSettings}
          />
        </TabPanel>
        
        {/* 운용 탭 */}
        <TabPanel value={currentTab} index={3}>
          <OperationTab 
            language={language}
            translate={translate}
            loading={loading}
            operationMode={operationMode}
            startOperation={startOperation}
            stopOperation={stopOperation}
            message4001={message4001}
            plotData={plotData}
            trackData={trackData}
            showRawData={showRawData}
            handleRawDataToggle={handleRawDataToggle}
            pollingInterval={pollingInterval}
            handlePollingIntervalChange={handlePollingIntervalChange}
            isOperating={isOperating}
            onDeleteTrack={handleDeleteTrack}
          />
        </TabPanel>

        {/* 상태정보 탭 */}
        <TabPanel value={currentTab} index={4}>
          <StatusInfoTab 
            language={language}
            translate={translate}
            message4001={message4001}
            showRawData={showRawData}
            handleRawDataToggle={handleRawDataToggle}
          />
      </TabPanel>

      {/* 새로운 시나리오 탭 패널 추가 */}
      <TabPanel value={currentTab} index={5}>
        <ScenarioTab 
          language={language}
          translate={translate}
        />
      </TabPanel>
      </Paper>
      
      {/* 에러 다이얼로그 */}
      <Dialog open={errorDialog.open} onClose={() => setErrorDialog({ ...errorDialog, open: false })}>
        <DialogTitle sx={{ backgroundColor: '#d32f2f', color: 'white' }}>
          {translate("오류", "Error", language)}
        </DialogTitle>
        <DialogContent sx={{ mt: 2 }}>
          <DialogContentText>
            {errorDialog.message}
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => setErrorDialog({ ...errorDialog, open: false })} color="primary">
            {translate("확인", "OK", language)}
            </Button>
        </DialogActions>
      </Dialog>
      
      {/* 성공 알림 */}
      <Snackbar
        open={successSnackbar.open} 
        autoHideDuration={3000} 
        onClose={() => setSuccessSnackbar({ ...successSnackbar, open: false })}
        anchorOrigin={{ vertical: 'bottom', horizontal: 'center' }}
      >
        <Alert onClose={() => setSuccessSnackbar({ ...successSnackbar, open: false })} severity="success" variant="filled">
          {successSnackbar.message}
        </Alert>
      </Snackbar>
    </div>
  );
};

// getEnglishLabel 함수 추가 (영어 라벨 매핑 함수)
const getEnglishLabel = (koreanLabel) => {
  const labelMap = {
    '연결': 'Connection',
    '운용': 'Operation',
    '기본설정': 'Basic Settings',
    '고급설정': 'Advanced Settings',
    '상태정보': 'Status Info',
    '시나리오': 'Scenario'
  };
  return labelMap[koreanLabel] || koreanLabel;
};

export default RadarControl; 