/**
 * 현재 언어 설정에 따라 텍스트를 번역하는 함수
 * @param {string} ko 한국어 텍스트
 * @param {string} en 영어 텍스트
 * @param {string} language 사용할 언어 코드 ('ko' 또는 'en')
 * @returns {string} 번역된 텍스트
 */
export const translate = (ko, en, language = 'ko') => {
  if (language === 'ko') return ko;
  return en;
};

/**
 * 날짜 형식화 함수
 * @param {Date} date 날짜 객체
 * @param {string} language 사용할 언어 코드
 * @returns {string} 형식화된 날짜 문자열
 */
export const formatDate = (date, language = 'ko') => {
  if (!date) return '';
  
  if (language === 'ko') {
    return new Date(date).toLocaleDateString('ko-KR', {
      year: 'numeric',
      month: '2-digit',
      day: '2-digit',
      weekday: 'short',
    });
  } else {
    return new Date(date).toLocaleDateString('en-US', {
      year: 'numeric',
      month: 'short',
      day: 'numeric',
      weekday: 'short',
    });
  }
};

/**
 * 시간 형식화 함수
 * @param {Date} date 시간 객체
 * @param {string} language 사용할 언어 코드
 * @returns {string} 형식화된 시간 문자열
 */
export const formatTime = (date, language = 'ko') => {
  if (!date) return '';
  
  if (language === 'ko') {
    return new Date(date).toLocaleTimeString('ko-KR', {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: false,
    });
  } else {
    return new Date(date).toLocaleTimeString('en-US', {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: true,
    });
  }
};

/**
 * 날짜와 시간 형식화 함수
 * @param {Date} date 날짜 및 시간 객체
 * @param {string} language 사용할 언어 코드
 * @returns {string} 형식화된 날짜 및 시간 문자열
 */
export const formatDateTime = (date, language = 'ko') => {
  if (!date) return '';
  
  if (language === 'ko') {
    return new Date(date).toLocaleString('ko-KR', {
      year: 'numeric',
      month: '2-digit',
      day: '2-digit',
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: false,
    });
  } else {
    return new Date(date).toLocaleString('en-US', {
      year: 'numeric',
      month: 'short',
      day: 'numeric',
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: true,
    });
  }
};

/**
 * 카테고리명 번역 함수
 * @param {string} category 카테고리 키
 * @param {string} language 사용할 언어 코드
 * @returns {string} 번역된 카테고리명
 */
export const translateCategory = (category, language = 'ko') => {
  const categories = {
    'dashboard': { ko: '대시보드', en: 'Dashboard' },
    'camera': { ko: '카메라 모니터링', en: 'Camera Monitoring' },
    'video': { ko: '동영상 분석', en: 'Video Analysis' },
    'radar': { ko: '레이더 모니터링', en: 'Radar Monitoring' },
    'weather': { ko: '기상 정보', en: 'Weather Data' },
    'analytics': { ko: '분석 및 통계', en: 'Analytics & Statistics' },
    'alerts': { ko: '알림 및 이벤트', en: 'Alerts & Events' },
    'defense': { ko: '방어 시스템 제어', en: 'Defense System Control' },
    'settings': { ko: '설정', en: 'Settings' },
  };
  
  if (!categories[category]) return category;
  
  if (language === 'ko') return categories[category].ko;
  return categories[category].en;
};

/**
 * 조류 종류 번역 함수
 * @param {string} species 조류 종류 키
 * @param {string} language 사용할 언어 코드
 * @returns {string} 번역된 조류 종류명
 */
export const translateSpecies = (species, language = 'ko') => {
  const speciesList = {
    'eagle': { ko: '독수리', en: 'Eagle' },
    'hawk': { ko: '매', en: 'Hawk' },
    'falcon': { ko: '팔콘', en: 'Falcon' },
    'owl': { ko: '올빼미', en: 'Owl' },
    'seagull': { ko: '갈매기', en: 'Seagull' },
    'crow': { ko: '까마귀', en: 'Crow' },
    'sparrow': { ko: '참새', en: 'Sparrow' },
    'pigeon': { ko: '비둘기', en: 'Pigeon' },
    'goose': { ko: '거위', en: 'Goose' },
    'duck': { ko: '오리', en: 'Duck' },
    'pelican': { ko: '펠리컨', en: 'Pelican' },
    'crane': { ko: '두루미', en: 'Crane' },
    'heron': { ko: '왜가리', en: 'Heron' },
    'stork': { ko: '황새', en: 'Stork' },
    'swallow': { ko: '제비', en: 'Swallow' },
    'kite': { ko: '솔개', en: 'Kite' },
    'buzzard': { ko: '말똥가리', en: 'Buzzard' },
    'unknown': { ko: '미확인', en: 'Unknown' },
  };
  
  if (!speciesList[species]) return species;
  
  if (language === 'ko') return speciesList[species].ko;
  return speciesList[species].en;
}; 