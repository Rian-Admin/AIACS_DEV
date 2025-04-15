import React, { useState, useEffect } from 'react';
import { Box, Typography, CircularProgress } from '@mui/material';

// 컴포넌트 임포트
import FilterSection from './components/FilterSection';
import FilterInfo from './components/FilterInfo';
import StatCards from './components/StatCards';
import DetectionTable from './components/DetectionTable';
import DistributionChart from './components/DistributionChart';
import ActivityChart from './components/ActivityChart';

// 데이터 관련 임포트
import { generateDummyData, downloadCSV } from './utils/dataUtils';

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
  });
  
  // 데이터 및 로딩 상태
  const [detectionData, setDetectionData] = useState(null);
  const [isLoading, setIsLoading] = useState(false);

  // 백엔드에서 데이터 로드
  const loadData = async () => {
    setIsLoading(true);
    try {
      // 실제로는 여기서 백엔드 API를 호출
      console.log('백엔드에서 데이터 로드 중...', filters);
      
      // 백엔드 호출 대신 임시 더미 데이터 생성
      const dummyData = generateDummyData();
      
      // API 호출 시뮬레이션
      setTimeout(() => {
        setDetectionData(dummyData);
        setIsLoading(false);
      }, 1500);
    } catch (error) {
      console.error('데이터 로드 오류:', error);
      setIsLoading(false);
    }
  };

  // 컴포넌트 마운트 시 데이터 로드
  useEffect(() => {
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
      />

      {/* 필터 정보 표시 */}
      <FilterInfo 
        filters={filters}
        isLoading={isLoading}
        formatDate={formatDate}
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