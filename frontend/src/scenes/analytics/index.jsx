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

// 파일 상단에 추가
const API_BASE_URL = process.env.REACT_APP_API_URL || '';

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
      }
      
      setCameras(camerasData);
    } catch (error) {
      console.error('카메라 정보 로드 오류:', error);
      console.log('백엔드 API 연결 실패');
      setCameras([]); // 빈 배열로 설정
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
      }
      
      setBirdClasses(birdClassesData);
    } catch (error) {
      console.error('조류 클래스 정보 로드 오류:', error);
      console.log('백엔드 API 연결 실패');
      setBirdClasses([]); // 빈 배열로 설정
    }
  };

  // 백엔드에서 데이터 로드
  const loadData = async () => {
    setIsLoading(true);
    try {
      // 필터 파라미터 준비
      const params = {
        get_all: 'true',  // 조류 분석 데이터 API용
        per_page: 10000   // 감지 기록 API용 - 페이지 크기를 매우 크게 설정하여 모든 데이터 가져오기
      };
      
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
      
      // URL이 백엔드 서버의 실제 URL을 가리키도록 수정
      // const API_BASE_URL = 'http://localhost:8000';
      
      // 실제 백엔드 API 호출
      let detectionsResponse, bbResponse;
      
      try {
        [detectionsResponse, bbResponse] = await Promise.all([
          axios.get(`${API_BASE_URL}/api/detections/filtered/`, { params }),
          axios.get(`${API_BASE_URL}/api/bird-analysis/data/`, { params })
        ]);
        
        console.log('감지 데이터 응답:', detectionsResponse.data);
        console.log('바운딩 박스 데이터 응답:', bbResponse.data);
        
        if (!detectionsResponse.data || !bbResponse.data) {
          throw new Error('API 응답에 데이터가 없습니다.');
        }
      } catch (apiError) {
        console.error('API 호출 오류:', apiError);
        throw apiError; // 상위 catch 블록으로 전달
      }
      
      // API 응답 처리
      const detections = detectionsResponse.data.detections || [];
      const bbData = bbResponse.data.bb_data || [];
      
      console.log(`API에서 가져온 감지 수: ${detections.length}, 바운딩 박스 수: ${bbData.length}`);
      
      // 데이터가 너무 많은 경우 처리 방법 변경
      // 조류 종류별 분포 계산 - 일반 for 루프 사용
      const speciesDistribution = {};
      
      // 청크 단위로 처리하여 스택 오버플로우 방지
      const processChunk = (array, start, chunkSize, processor) => {
        const end = Math.min(start + chunkSize, array.length);
        for (let i = start; i < end; i++) {
          processor(array[i]);
        }
        return end;
      };
      
      // 메모리 효율적인 방법으로 바운딩 박스 데이터 처리
      const CHUNK_SIZE = 10000; // 한 번에 처리할 항목 수
      let currentIndex = 0;
      
      while (currentIndex < bbData.length) {
        currentIndex = processChunk(bbData, currentIndex, CHUNK_SIZE, (bb) => {
          if (!speciesDistribution[bb.class_name]) {
            speciesDistribution[bb.class_name] = 0;
          }
          speciesDistribution[bb.class_name]++;
        });
        
        // 비동기 처리 허용하여 UI 블로킹 방지
        if (currentIndex < bbData.length) {
          await new Promise(resolve => setTimeout(resolve, 0));
        }
      }
      
      // 분포 데이터 배열로 변환
      const distributionArray = Object.entries(speciesDistribution).map(([name, count]) => ({
        name,
        count
      })).sort((a, b) => b.count - a.count); // 개수 기준 내림차순 정렬
      
      // 바운딩 박스 통계 계산 - 청크 단위로 처리
      const bbSizes = { 
        totalWidth: 0, 
        totalHeight: 0, 
        maxWidth: 0, 
        maxHeight: 0,
        count: 0
      };
      
      currentIndex = 0;
      while (currentIndex < bbData.length) {
        currentIndex = processChunk(bbData, currentIndex, CHUNK_SIZE, (bb) => {
          const width = bb.bb_right - bb.bb_left;
          const height = bb.bb_bottom - bb.bb_top;
          
          bbSizes.totalWidth += width;
          bbSizes.totalHeight += height;
          bbSizes.maxWidth = Math.max(bbSizes.maxWidth, width);
          bbSizes.maxHeight = Math.max(bbSizes.maxHeight, height);
          bbSizes.count++;
        });
        
        // 비동기 처리 허용
        if (currentIndex < bbData.length) {
          await new Promise(resolve => setTimeout(resolve, 0));
        }
      }
      
      // 평균 계산
      const avgSize = bbSizes.count > 0 ? (bbSizes.totalWidth / bbSizes.count) * 100 : 0;
      const avgHeight = bbSizes.count > 0 ? (bbSizes.totalHeight / bbSizes.count) * 100 : 0;
      const maxSize = bbSizes.maxWidth * 100;
      const maxHeight = bbSizes.maxHeight * 100;
      
      // 감지 데이터 처리 - 메모리 효율적인 방식으로
      // 필요한 경우 감지 데이터도 청크 단위로 처리
      const processedDetections = [];
      currentIndex = 0;
      
      // 감지 데이터의 크기가 적으면 모두 처리, 크면 최근 1000개만 처리
      const detectionsToProcess = detections.length > 1000 
        ? detections.slice(-1000) // 최근 1000개만
        : detections;
      
      while (currentIndex < detectionsToProcess.length) {
        currentIndex = processChunk(detectionsToProcess, currentIndex, 200, (detection) => {
          // 해당 감지의 바운딩 박스 찾기 - 필터링 대신 맵 사용
          const detectionBBMap = new Map();
          
          // 바운딩 박스 데이터에서 현재 detection_id와 일치하는 항목만 처리
          for (let i = 0; i < bbData.length; i++) {
            if (bbData[i].detection_id === detection.detection_id) {
              const className = bbData[i].class_name;
              detectionBBMap.set(className, (detectionBBMap.get(className) || 0) + 1);
            }
          }
          
          // 가장 많은 종류 찾기
          let dominantSpecies = '';
          let maxCount = 0;
          
          detectionBBMap.forEach((count, species) => {
            if (count > maxCount) {
              maxCount = count;
              dominantSpecies = species;
            }
          });
          
          processedDetections.push({
            id: detection.detection_id,
            timestamp: detection.detection_time,
            species: dominantSpecies,
            confidence: 95, // 임시
            distance: 100, // 임시
            objectCount: detection.bb_count,
            riskLevel: detection.bb_count > 3 ? '고위험' : '저위험',
            camera_id: detection.camera_id
          });
        });
        
        // 비동기 처리 허용
        if (currentIndex < detectionsToProcess.length) {
          await new Promise(resolve => setTimeout(resolve, 0));
        }
      }
      
      // 결과 데이터 구성
      const result = {
        totalDetections: detections.length,
        recentDetections: processedDetections,
        speciesDistribution: distributionArray,
        statistics: {
          count: bbData.length,
          avgSize,
          avgHeight,
          maxSize,
          maxHeight
        }
      };
      
      // 디버깅용 로그 출력
      console.log("최종 데이터:", result);
      console.log("총 탐지 횟수(totalDetections):", result.totalDetections);
      console.log("조류 개체 수(statistics.count):", result.statistics.count);
      
      setDetectionData(result);
    } catch (error) {
      console.error('데이터 로드 오류:', error);
      alert('백엔드 서버에 연결할 수 없습니다. 서버가 실행 중인지 확인해주세요.');
      setDetectionData(null); // 데이터를 null로 설정하여 UI에 오류 메시지 표시
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

          {/* 3개 컴포넌트를 가로로 배열 */}
          <Box sx={{ 
            display: 'grid', 
            gridTemplateColumns: { 
              xs: '1fr', 
              md: '1fr 1fr', 
              lg: '1fr 1.5fr 1.5fr' 
            },
            gridAutoRows: 'minmax(420px, auto)',
            gap: 2,
            mb: 2
          }}>
            {/* 종류별 분포 차트 */}
            <Box>
              <DistributionChart distribution={detectionData.speciesDistribution} />
            </Box>

            {/* 시간대별 활동 패턴 차트 */}
            <Box>
              <ActivityChart />
            </Box>

            {/* 최근 탐지 목록 */}
            <Box>
              <DetectionTable detections={detectionData.recentDetections} />
            </Box>
          </Box>
        </>
      )}
    </Box>
  );
};

export default Analytics;