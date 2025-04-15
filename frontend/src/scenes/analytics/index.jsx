import React, { useState, useEffect } from 'react';
import { Box, Typography, CircularProgress } from '@mui/material';
import axios from 'axios';

// 컴포넌트 임포트
import FilterSection from './components/FilterSection';
import FilterInfo from './components/FilterInfo';
import StatCards from './components/StatCards';
import DetectionTable from './components/DetectionTable';
import DistributionChart from './components/DistributionChart';
import ActivityChart from './components/ActivityChart';

// 데이터 관련 임포트
import { downloadCSV } from './utils/dataUtils';

/**
 * 분석 및 통계 컴포넌트
 * @param {Object} props 컴포넌트 속성
 * @param {string} props.language 사용할 언어 코드 ('ko' 또는 'en')
 * @returns {JSX.Element} 분석 및 통계 컴포넌트
 */
const Analytics = ({ language }) => {
  // 필터 상태
  const [filters, setFilters] = useState({
    timeRange: 'week',
    species: 'all',
    startDate: new Date(Date.now() - 7 * 24 * 60 * 60 * 1000),
    endDate: new Date(),
    cameraId: '',
    birdClass: ''
  });
  
  // 데이터 및 로딩 상태
  const [detectionData, setDetectionData] = useState(null);
  const [birdClasses, setBirdClasses] = useState([]);
  const [cameras, setCameras] = useState([]);
  const [isLoading, setIsLoading] = useState(false);

  // 카메라 정보 로드
  const loadCameras = async () => {
    try {
      // 백엔드 API 경로 수정 (실제 경로로 수정)
      const response = await axios.get('/api/cameras/');
      
      // API 응답 구조에 따라 적절하게 데이터 추출
      let camerasData = response.data;
      
      // 데이터가 객체이고 cameras 속성이 있는 경우 (예: { cameras: [...] })
      if (camerasData && typeof camerasData === 'object' && camerasData.cameras) {
        camerasData = camerasData.cameras;
      }
      
      // 배열이 아니라면 빈 배열로 설정
      if (!Array.isArray(camerasData)) {
        console.warn('카메라 데이터가 예상된 형식이 아닙니다:', camerasData);
        camerasData = [];
        
        // 개발 단계에서 임시 데이터 사용
        if (process.env.NODE_ENV === 'development') {
          camerasData = [
            { camera_id: 1, wind_turbine_id: '터빈-A', status: '활성' },
            { camera_id: 2, wind_turbine_id: '터빈-B', status: '활성' },
            { camera_id: 3, wind_turbine_id: '터빈-C', status: '비활성' }
          ];
        }
      }
      
      setCameras(camerasData);
    } catch (error) {
      console.error('카메라 정보 로드 오류:', error);
      console.log('백엔드 API 연결 실패, 임시 데이터 사용 중');
      
      // 오류 발생 시 개발 환경에서 임시 데이터 사용
      if (process.env.NODE_ENV === 'development') {
        const dummyCameras = [
          { camera_id: 1, wind_turbine_id: '터빈-A', status: '활성' },
          { camera_id: 2, wind_turbine_id: '터빈-B', status: '활성' },
          { camera_id: 3, wind_turbine_id: '터빈-C', status: '비활성' }
        ];
        setCameras(dummyCameras);
      }
    }
  };

  // 조류 클래스 정보 로드
  const loadBirdClasses = async () => {
    try {
      // birds.csv 파일에서 조류 클래스 정보를 가져옵니다.
      // 백엔드 API 경로 수정 (실제 경로로 수정)
      const response = await axios.get('/api/export-birds-csv/');
      
      // API 응답 구조에 따라 적절하게 데이터 추출
      let birdClassesData = response.data;
      
      // CSV 데이터 파싱 (응답이 CSV 형식인 경우)
      if (typeof birdClassesData === 'string' && birdClassesData.includes('","')) {
        console.log('CSV 데이터 파싱 시도');
        try {
          // CSV 데이터를 행으로 분리
          const lines = birdClassesData.split('\n');
          const headers = lines[0].split(',').map(header => 
            header.replace(/"/g, '').trim()
          );
          
          // 각 행을 객체로 변환
          const parsedData = [];
          for (let i = 1; i < lines.length; i++) {
            // 빈 줄 무시
            if (!lines[i].trim()) continue;
            
            // 쉼표로 분리하기 전에 쌍따옴표로 묶인 필드 처리
            const values = [];
            let inQuotes = false;
            let currentValue = '';
            
            for (let j = 0; j < lines[i].length; j++) {
              const char = lines[i][j];
              
              if (char === '"') {
                inQuotes = !inQuotes;
              } else if (char === ',' && !inQuotes) {
                values.push(currentValue.replace(/"/g, '').trim());
                currentValue = '';
              } else {
                currentValue += char;
              }
            }
            
            // 마지막 필드 추가
            values.push(currentValue.replace(/"/g, '').trim());
            
            // 객체 생성
            const obj = {};
            for (let j = 0; j < headers.length && j < values.length; j++) {
              obj[headers[j]] = values[j];
            }
            
            // class_id 필드 설정
            obj.class_id = obj.ID || i;
            obj.bird_name_ko = obj.한글명 || '';
            obj.scientific_name = obj.학명 || '';
            
            // 비어있는 객체가 아닌 경우에만 추가
            if (obj.bird_name_ko) {
              parsedData.push(obj);
            }
          }
          
          console.log('파싱된 조류 데이터:', parsedData);
          birdClassesData = parsedData;
        } catch (parseError) {
          console.error('CSV 파싱 오류:', parseError);
          // 파싱 실패 시 기본 데이터 사용
          birdClassesData = [];
        }
      }
      
      // 데이터가 객체이고 birds 속성이 있는 경우 (예: { birds: [...] })
      if (birdClassesData && typeof birdClassesData === 'object' && birdClassesData.birds) {
        birdClassesData = birdClassesData.birds;
      }
      
      // 배열이 아니라면 빈 배열로 설정
      if (!Array.isArray(birdClassesData)) {
        console.warn('조류 클래스 데이터가 예상된 형식이 아닙니다:', birdClassesData);
        birdClassesData = [];
        
        // 개발 단계에서 임시 데이터 사용
        if (process.env.NODE_ENV === 'development') {
          birdClassesData = [
            { class_id: 1, bird_name_ko: '검은머리물떼새', scientific_name: 'Haematopus ostralegus' },
            { class_id: 2, bird_name_ko: '알락꼬리마도요', scientific_name: 'Numenius madagascariensis' },
            { class_id: 3, bird_name_ko: '흰이마기러기', scientific_name: 'Anser albifrons' },
            { class_id: 4, bird_name_ko: '마도요', scientific_name: 'Numenius sp.' },
            { class_id: 5, bird_name_ko: '뿔제비갈매기', scientific_name: 'Thalasseus bergii' },
            { class_id: 6, bird_name_ko: '재갈매기', scientific_name: 'Larus argentatus' },
            { class_id: 7, bird_name_ko: '모래도요', scientific_name: 'Calidris alba' }
          ];
        }
      }
      
      setBirdClasses(birdClassesData);
    } catch (error) {
      console.error('조류 클래스 정보 로드 오류:', error);
      console.log('백엔드 API 연결 실패, 임시 데이터 사용 중');
      
      // 오류 발생 시 개발 환경에서 임시 데이터 사용
      if (process.env.NODE_ENV === 'development') {
        const dummyBirdClasses = [
          { class_id: 1, bird_name_ko: '검은머리물떼새', scientific_name: 'Haematopus ostralegus' },
          { class_id: 2, bird_name_ko: '알락꼬리마도요', scientific_name: 'Numenius madagascariensis' },
          { class_id: 3, bird_name_ko: '흰이마기러기', scientific_name: 'Anser albifrons' },
          { class_id: 4, bird_name_ko: '마도요', scientific_name: 'Numenius sp.' },
          { class_id: 5, bird_name_ko: '뿔제비갈매기', scientific_name: 'Thalasseus bergii' },
          { class_id: 6, bird_name_ko: '재갈매기', scientific_name: 'Larus argentatus' },
          { class_id: 7, bird_name_ko: '모래도요', scientific_name: 'Calidris alba' }
        ];
        setBirdClasses(dummyBirdClasses);
      }
    }
  };

  // 백엔드에서 데이터 로드
  const loadData = async () => {
    setIsLoading(true);
    try {
      // 필터 파라미터 준비
      const params = {};
      
      // 시간 범위에 따른 날짜 형식화
      if (filters.timeRange === 'custom') {
        params.date_from = formatDate(filters.startDate);
        params.date_to = formatDate(filters.endDate);
      } else if (filters.timeRange === 'day') {
        params.date_from = formatDate(new Date());
        params.date_to = formatDate(new Date());
      } else if (filters.timeRange === 'week') {
        const weekAgo = new Date();
        weekAgo.setDate(weekAgo.getDate() - 7);
        params.date_from = formatDate(weekAgo);
        params.date_to = formatDate(new Date());
      } else if (filters.timeRange === 'month') {
        const monthAgo = new Date();
        monthAgo.setMonth(monthAgo.getMonth() - 1);
        params.date_from = formatDate(monthAgo);
        params.date_to = formatDate(new Date());
      } else if (filters.timeRange === 'year') {
        const yearAgo = new Date();
        yearAgo.setFullYear(yearAgo.getFullYear() - 1);
        params.date_from = formatDate(yearAgo);
        params.date_to = formatDate(new Date());
      }
      
      // 조류 종류 필터 추가
      if (filters.birdClass && filters.birdClass !== 'all') {
        params.bird_class = filters.birdClass;
      }
      
      // 카메라 필터 추가
      if (filters.cameraId && filters.cameraId !== '') {
        params.camera_id = filters.cameraId;
      }
      
      console.log('백엔드 API 호출 파라미터:', params);
      
      // 실제 백엔드 API 호출 (경로 수정)
      const [detectionsResponse, bbResponse] = await Promise.all([
        axios.get('/api/detections/filtered/', { params }),
        axios.get('/api/bird-analysis/data/', { params })
      ]);
      
      // API 응답 처리
      const detections = detectionsResponse.data.detections || [];
      const bbData = bbResponse.data.bb_data || [];
      
      // 조류 종류별 분포 계산
      const speciesDistribution = {};
      
      bbData.forEach(bb => {
        if (!speciesDistribution[bb.class_name]) {
          speciesDistribution[bb.class_name] = 0;
        }
        speciesDistribution[bb.class_name]++;
      });
      
      // 분포 데이터 배열로 변환
      const distributionArray = Object.entries(speciesDistribution).map(([name, count]) => ({
        name,
        count
      }));
      
      // 바운딩 박스 통계 계산
      const bbSizes = bbData.map(bb => {
        const width = bb.bb_right - bb.bb_left;
        const height = bb.bb_bottom - bb.bb_top;
        return { width, height, area: width * height };
      });
      
      // 평균 및 최대 크기 계산
      const avgSize = bbSizes.length > 0 
        ? bbSizes.reduce((sum, item) => sum + item.width, 0) / bbSizes.length * 100
        : 0;
        
      const avgHeight = bbSizes.length > 0 
        ? bbSizes.reduce((sum, item) => sum + item.height, 0) / bbSizes.length * 100
        : 0;
        
      const maxSize = bbSizes.length > 0 
        ? Math.max(...bbSizes.map(item => item.width)) * 100
        : 0;
        
      const maxHeight = bbSizes.length > 0 
        ? Math.max(...bbSizes.map(item => item.height)) * 100
        : 0;
      
      // 결과 데이터 구성
      const result = {
        totalDetections: detections.length,
        recentDetections: detections.map(detection => ({
          id: detection.detection_id,
          timestamp: detection.detection_time,
          species: '', // 대표 종 추가 필요
          confidence: 0, // 신뢰도 데이터 추가 필요
          distance: 0, // 거리 데이터 추가 필요
          objectCount: detection.bb_count,
          riskLevel: detection.bb_count > 5 ? '고위험' : '저위험',
          camera_id: detection.camera_id
        })),
        speciesDistribution: distributionArray,
        statistics: {
          count: bbData.length,
          avgSize,
          avgHeight,
          maxSize,
          maxHeight
        }
      };
      
      setDetectionData(result);
    } catch (error) {
      console.error('데이터 로드 오류:', error);
      console.log('백엔드 API 연결 실패, 임시 데이터 사용 중');
      
      // 개발 단계에서 임시 더미 데이터 사용
      if (process.env.NODE_ENV === 'development') {
        // 더미 데이터 생성
        const dummyData = {
          totalDetections: 120,
          recentDetections: Array(15).fill(0).map((_, i) => ({
            id: 1000 + i,
            timestamp: new Date(Date.now() - i * 3600000).toISOString(),
            species: ['검은머리물떼새', '알락꼬리마도요', '흰이마기러기', '마도요'][i % 4],
            confidence: 80 + Math.floor(Math.random() * 15),
            distance: 50 + Math.floor(Math.random() * 150),
            objectCount: 1 + Math.floor(Math.random() * 5),
            riskLevel: Math.random() > 0.7 ? '고위험' : '저위험',
            camera_id: 1 + (i % 3)
          })),
          speciesDistribution: [
            { name: '검은머리물떼새', count: 45 },
            { name: '알락꼬리마도요', count: 32 },
            { name: '흰이마기러기', count: 28 },
            { name: '마도요', count: 15 }
          ],
          statistics: {
            count: 120,
            avgSize: 8.5,
            avgHeight: 6.3,
            maxSize: 35.2,
            maxHeight: 16.5
          }
        };
        setDetectionData(dummyData);
      }
    } finally {
      setIsLoading(false);
    }
  };

  // 컴포넌트 마운트 시 데이터 로드
  useEffect(() => {
    loadBirdClasses();
    loadCameras();
    loadData();
  }, []);

  // 필터 변경 핸들러
  const handleFilterChange = (name, value) => {
    setFilters(prev => ({
      ...prev,
      [name]: value
    }));
  };

  // 필터 적용 핸들러
  const handleApplyFilters = () => {
    loadData();
  };

  // CSV 다운로드 핸들러
  const handleCsvDownload = () => {
    if (detectionData && detectionData.recentDetections) {
      downloadCSV(detectionData.recentDetections, `bird_detection_data_${formatDate(new Date())}.csv`);
    }
  };

  // 날짜 포맷 헬퍼 함수
  const formatDate = (date) => {
    if (!date) return '';
    const year = date.getFullYear();
    const month = (date.getMonth() + 1).toString().padStart(2, '0');
    const day = date.getDate().toString().padStart(2, '0');
    return `${year}-${month}-${day}`;
  };

  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h6" fontWeight="bold" sx={{ mb: 2 }}>
        분석 및 통계
      </Typography>
      
      {/* 필터 섹션 */}
      <FilterSection 
        filters={filters}
        onFilterChange={handleFilterChange}
        onApplyFilters={handleApplyFilters}
        onCsvDownload={handleCsvDownload}
        isLoading={isLoading}
        hasData={!!detectionData}
        birdClasses={birdClasses}
        cameras={cameras}
      />

      {/* 필터 정보 표시 */}
      <FilterInfo 
        filters={filters}
        isLoading={isLoading}
        formatDate={formatDate}
        birdClasses={birdClasses}
      />

      {/* 데이터 로딩 중이거나 없을 때 표시 */}
      {isLoading ? (
        <Box sx={{ display: 'flex', justifyContent: 'center', p: 5 }}>
          <CircularProgress />
        </Box>
      ) : !detectionData ? (
        <Box sx={{ p: 4, textAlign: 'center' }}>
          <Typography color="text.secondary">데이터를 불러올 수 없습니다.</Typography>
        </Box>
      ) : (
        <>
          {/* 통계 카드 섹션 */}
          <StatCards data={detectionData} />

          {/* 최근 탐지 목록 */}
          <DetectionTable detections={detectionData.recentDetections} />

          {/* 차트 섹션 */}
          <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2 }}>
            {/* 종류별 분포 차트 */}
            <DistributionChart distribution={detectionData.speciesDistribution} />

            {/* 시간대별 활동 패턴 차트 */}
            <ActivityChart />
          </Box>
        </>
      )}
    </Box>
  );
};

export default Analytics;