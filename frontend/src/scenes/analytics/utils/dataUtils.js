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