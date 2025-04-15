/**
 * 더미 데이터 생성 함수
 * @returns {Object} 생성된 더미 데이터
 */
export const generateDummyData = () => {
  // 총 탐지 수
  const totalDetections = Math.floor(Math.random() * 1000) + 500;
  
  // 오늘 탐지 수
  const todayDetections = Math.floor(Math.random() * 100) + 10;
  
  // 고위험 탐지 수
  const highRiskDetections = Math.floor(totalDetections * 0.15);
  
  // 종류별 탐지 수
  const speciesDistribution = [
    { name: '독수리', count: Math.floor(Math.random() * 100) + 50 },
    { name: '매', count: Math.floor(Math.random() * 100) + 30 },
    { name: '팔콘', count: Math.floor(Math.random() * 100) + 20 },
    { name: '갈매기', count: Math.floor(Math.random() * 100) + 100 },
    { name: '까마귀', count: Math.floor(Math.random() * 100) + 150 },
    { name: '참새', count: Math.floor(Math.random() * 100) + 200 }
  ];
  
  // 최근 탐지 기록
  const recentDetections = Array(10).fill(0).map((_, i) => {
    const date = new Date();
    date.setHours(date.getHours() - Math.floor(Math.random() * 24));
    
    const species = ['독수리', '매', '팔콘', '갈매기', '까마귀', '참새'][Math.floor(Math.random() * 6)];
    const confidence = Math.floor(Math.random() * 30) + 70;
    const distance = Math.floor(Math.random() * 300) + 50;
    const objectCount = Math.floor(Math.random() * 5) + 1;
    
    return {
      id: `DET-${1000 + i}`,
      timestamp: date.toLocaleString(),
      species,
      confidence: confidence,
      distance: distance,
      objectCount,
      riskLevel: confidence < 80 || distance < 100 ? '고위험' : '저위험'
    };
  });
  
  return {
    totalDetections,
    todayDetections,
    highRiskDetections,
    speciesDistribution,
    recentDetections
  };
};

/**
 * CSV 파일 다운로드 함수
 * @param {Array} data - CSV로 변환할 데이터 배열
 * @param {string} filename - 다운로드할 파일명
 */
export const downloadCSV = (data, filename) => {
  if (!data || data.length === 0) return;
  
  // CSV 형식으로 변환
  const csvContent = convertToCSV(data);
  
  // Blob 생성 및 다운로드
  const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.setAttribute('href', url);
  link.setAttribute('download', filename);
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
};

/**
 * 데이터를 CSV 형식으로 변환
 * @param {Array} data - 변환할 데이터 배열
 * @returns {string} CSV 형식 문자열
 */
const convertToCSV = (data) => {
  if (!data || data.length === 0) return '';
  
  // 헤더 생성
  const header = Object.keys(data[0]).join(',');
  
  // 행 데이터 생성
  const rows = data.map(row => 
    Object.values(row)
      .map(value => `"${value}"`)
      .join(',')
  );
  
  // 헤더와 행 데이터 합치기
  return [header, ...rows].join('\n');
}; 