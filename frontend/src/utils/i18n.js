/**
 * 현재 언어 설정에 따라 텍스트를 번역하는 함수
 * @param {string} ko 한국어 텍스트
 * @param {string} en 영어 텍스트
 * @param {string} id 인도네시아어 텍스트 (선택적)
 * @param {string} language 사용할 언어 코드 ('ko', 'en' 또는 'id')
 * @returns {string} 번역된 텍스트
 */
export const translate = (ko, en, id, language = 'ko') => {
  if (language === 'ko') return ko;
  if (language === 'id' && id) return id;
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
  } else if (language === 'id') {
    return new Date(date).toLocaleDateString('id-ID', {
      year: 'numeric',
      month: 'long',
      day: 'numeric',
      weekday: 'long',
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
  } else if (language === 'id') {
    return new Date(date).toLocaleTimeString('id-ID', {
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
  } else if (language === 'id') {
    return new Date(date).toLocaleString('id-ID', {
      year: 'numeric',
      month: 'long',
      day: 'numeric',
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
    'dashboard': { ko: '대시보드', en: 'Dashboard', id: 'Dasbor' },
    'camera': { ko: '카메라 모니터링', en: 'Camera Monitoring', id: 'Pemantauan Kamera' },
    'video': { ko: '동영상 분석', en: 'Video Analysis', id: 'Analisis Video' },
    'radar': { ko: '레이더 모니터링', en: 'Radar Monitoring', id: 'Pemantauan Radar' },
    'weather': { ko: '기상 정보', en: 'Weather Data', id: 'Data Cuaca' },
    'analytics': { ko: '분석 및 통계', en: 'Analytics & Statistics', id: 'Analitik & Statistik' },
    'alerts': { ko: '알림 및 이벤트', en: 'Alerts & Events', id: 'Peringatan & Acara' },
    'defense': { ko: '방어 시스템 제어', en: 'Defense System Control', id: 'Kontrol Sistem Pertahanan' },
    'settings': { ko: '설정', en: 'Settings', id: 'Pengaturan' },
  };
  
  if (!categories[category]) return category;
  
  if (language === 'ko') return categories[category].ko;
  if (language === 'id') return categories[category].id;
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
    'eagle': { ko: '독수리', en: 'Eagle', id: 'Elang' },
    'hawk': { ko: '매', en: 'Hawk', id: 'Elang Alap' },
    'falcon': { ko: '팔콘', en: 'Falcon', id: 'Falcon' },
    'owl': { ko: '올빼미', en: 'Owl', id: 'Burung Hantu' },
    'seagull': { ko: '갈매기', en: 'Seagull', id: 'Camar' },
    'crow': { ko: '까마귀', en: 'Crow', id: 'Gagak' },
    'sparrow': { ko: '참새', en: 'Sparrow', id: 'Burung Gereja' },
    'pigeon': { ko: '비둘기', en: 'Pigeon', id: 'Merpati' },
    'goose': { ko: '거위', en: 'Goose', id: 'Angsa' },
    'duck': { ko: '오리', en: 'Duck', id: 'Bebek' },
    'pelican': { ko: '펠리컨', en: 'Pelican', id: 'Pelikan' },
    'crane': { ko: '두루미', en: 'Crane', id: 'Bangau' },
    'heron': { ko: '왜가리', en: 'Heron', id: 'Cangak' },
    'stork': { ko: '황새', en: 'Stork', id: 'Bangau' },
    'swallow': { ko: '제비', en: 'Swallow', id: 'Burung Layang-layang' },
    'kite': { ko: '솔개', en: 'Kite', id: 'Elang Layang' },
    'buzzard': { ko: '말똥가리', en: 'Buzzard', id: 'Elang Buteo' },
    'unknown': { ko: '미확인', en: 'Unknown', id: 'Tidak Diketahui' },
  };
  
  if (!speciesList[species]) return species;
  
  if (language === 'ko') return speciesList[species].ko;
  if (language === 'id') return speciesList[species].id;
  return speciesList[species].en;
}; 