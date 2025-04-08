/**
 * 일자를 요일 이름으로 변환합니다.
 * @param {string} dateStr - ISO 날짜 문자열
 * @param {string} language - 언어 코드 ('ko', 'en', 'id' 등)
 * @returns {string} 요일 이름
 */
export const formatDayName = (dateStr, language) => {
  const date = new Date(dateStr);
  const days = {
    ko: ['일', '월', '화', '수', '목', '금', '토'],
    en: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
    id: ['Min', 'Sen', 'Sel', 'Rab', 'Kam', 'Jum', 'Sab']
  };
  return days[language][date.getDay()];
};

/**
 * 날짜 문자열을 월 이름으로 변환합니다.
 * @param {string} monthStr - ISO 날짜 문자열
 * @param {string} language - 언어 코드 ('ko', 'en', 'id' 등)
 * @returns {string} 월 이름
 */
export const formatMonthName = (monthStr, language) => {
  const date = new Date(monthStr);
  const months = {
    ko: ['1월', '2월', '3월', '4월', '5월', '6월', '7월', '8월', '9월', '10월', '11월', '12월'],
    en: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    id: ['Jan', 'Feb', 'Mar', 'Apr', 'Mei', 'Jun', 'Jul', 'Agu', 'Sep', 'Okt', 'Nov', 'Des']
  };
  return months[language][date.getMonth()];
};

/**
 * 시간을 HH:00 형식으로 변환합니다.
 * @param {number} hour - 시간 (0-23)
 * @returns {string} HH:00 형식의 시간
 */
export const formatHourTime = (hour) => {
  return `${hour.toString().padStart(2, '0')}:00`;
};

/**
 * 날짜 형식을 YYYY-MM-DD 형식으로 변환합니다.
 * @param {Date} date - 날짜 객체
 * @returns {string} 포맷된 날짜 문자열
 */
export const formatDate = (date) => {
  if (!date) return '';
  const year = date.getFullYear();
  const month = (date.getMonth() + 1).toString().padStart(2, '0');
  const day = date.getDate().toString().padStart(2, '0');
  return `${year}-${month}-${day}`;
};

/**
 * 한국어 날짜 표시를 위한 함수 (YYYY년 MM월 DD일)
 * @param {string} dateStr - ISO 날짜 문자열
 * @returns {string} 한국어 형식의 날짜 문자열
 */
export const formatKoreanDate = (dateStr) => {
  if (!dateStr) return '';
  const date = new Date(dateStr);
  if (isNaN(date.getTime())) return dateStr; // 유효하지 않은 날짜면 원본 반환
  
  return `${date.getFullYear()}년 ${date.getMonth() + 1}월 ${date.getDate()}일`;
};

/**
 * 날짜에서 해당 월의 주차를 계산합니다.
 * @param {Date} date - 날짜 객체
 * @returns {number} 주차 (1-5)
 */
export const getWeekOfMonth = (date) => {
  const firstDayOfMonth = new Date(date.getFullYear(), date.getMonth(), 1);
  return Math.ceil((date.getDate() + firstDayOfMonth.getDay()) / 7);
};

// 차트 데이터 변환 유틸리티 함수들
/**
 * 주간 데이터를 변환합니다.
 * @param {Array} dailyData - 일별 데이터 배열
 * @param {string} language - 언어 코드
 * @returns {Array} 변환된 차트 데이터
 */
export const prepareWeeklyData = (dailyData, language) => {
  if (!dailyData || !dailyData.length) return [];
  
  return dailyData.map(day => {
    const date = new Date(day.date);
    const month = date.getMonth() + 1;
    const weekOfMonth = getWeekOfMonth(date);
    
    let displayName;
    if (language === 'ko') {
      displayName = `${month}월 ${weekOfMonth}주차`;
    } else if (language === 'en') {
      const weekNames = ['1st', '2nd', '3rd', '4th', '5th'];
      const monthNames = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
      displayName = `${monthNames[month-1]} ${weekNames[weekOfMonth-1]} week`;
    } else {
      const weekNames = ['1st', '2nd', '3rd', '4th', '5th'];
      displayName = `Week ${weekNames[weekOfMonth-1]}, ${month}`;
    }
    
    return {
      name: displayName,
      date: day.date, // 원본 날짜 정보도 유지
      weekInfo: {
        month: month,
        week: weekOfMonth
      },
      count: day.count
    };
  });
};

/**
 * 일별 데이터를 변환합니다.
 * @param {Array} dailyData - 일별 데이터 배열
 * @param {string} language - 언어 코드
 * @returns {Array} 변환된 차트 데이터
 */
export const prepareDailyData = (dailyData, language) => {
  if (!dailyData || !dailyData.length) return [];
  
  return dailyData.map(day => {
    return {
      name: formatDayName(day.date, language), // 요일 표시 (월, 화, 수, 목, 금, 토, 일)
      date: day.date, // 원본 날짜 정보도 유지
      count: day.count
    };
  });
};

/**
 * 종 분포 데이터를 변환합니다.
 * @param {Array} distributionData - 종 분포 데이터 배열
 * @param {string} language - 언어 코드
 * @returns {Array} 변환된 차트 데이터
 */
export const prepareSpeciesDistributionData = (distributionData, language) => {
  if (!distributionData || !distributionData.length) return [];
  
  return distributionData.map(item => ({
    name: translateSpecies(item.species, language),
    value: item.count
  }));
};

/**
 * 시간별 데이터를 변환합니다.
 * @param {Array} hourlyData - 시간별 데이터 배열
 * @returns {Array} 변환된 차트 데이터
 */
export const prepareHourlyData = (hourlyData) => {
  if (!hourlyData || !hourlyData.length) return [];
  
  return hourlyData.map(item => ({
    hour: formatHourTime(item.hour), // 시간 표시 (00:00, 01:00, 02:00, ...)
    count: item.count
  }));
};

/**
 * 월별 데이터를 변환합니다.
 * @param {Array} monthlyData - 월별 데이터 배열
 * @param {string} language - 언어 코드
 * @returns {Array} 변환된 차트 데이터
 */
export const prepareMonthlyData = (monthlyData, language) => {
  if (!monthlyData || !monthlyData.length) return [];
  
  // 현재 월부터 과거 5개월까지 표시 (최신순으로 정렬)
  const sortedData = [...monthlyData].sort((a, b) => {
    const dateA = new Date(a.month);
    const dateB = new Date(b.month);
    return dateB - dateA; // 내림차순 정렬 (최신 월이 먼저)
  });
  
  return sortedData.map(month => {
    const date = new Date(month.month);
    let displayMonth;
    
    if (language === 'ko') {
      displayMonth = `${date.getMonth() + 1}월`;
    } else if (language === 'en') {
      const monthNames = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
      displayMonth = monthNames[date.getMonth()];
    } else {
      displayMonth = `${date.getMonth() + 1}`;
    }
    
    return {
      month: displayMonth,
      count: month.count,
      defenseActivations: month.defenseActivations
    };
  });
};

/**
 * 조류 종류 이름을 번역합니다.
 * @param {string} species - 조류 종류 코드
 * @param {string} language - 언어 코드
 * @returns {string} 번역된 종류 이름
 */
export const translateSpecies = (species, language) => {
  const speciesNames = {
    eagle: { ko: '독수리', en: 'Eagle', id: 'Elang' },
    hawk: { ko: '매', en: 'Hawk', id: 'Elang' },
    falcon: { ko: '팔콘', en: 'Falcon', id: 'Falcon' },
    seagull: { ko: '갈매기', en: 'Seagull', id: 'Camar' },
    crow: { ko: '까마귀', en: 'Crow', id: 'Gagak' },
    sparrow: { ko: '참새', en: 'Sparrow', id: 'Burung gereja' },
    pigeon: { ko: '비둘기', en: 'Pigeon', id: 'Merpati' },
    owl: { ko: '올빼미', en: 'Owl', id: 'Burung hantu' },
    unknown: { ko: '알 수 없음', en: 'Unknown', id: 'Tidak diketahui' }
  };
  
  return speciesNames[species]?.[language] || species;
}; 