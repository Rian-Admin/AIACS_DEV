/**
 * 레이더 제어 시스템 디버깅 헬퍼 함수
 * 이 파일은 레이더 시스템과의 통신 및 값 처리에 도움이 되는 유틸리티 함수를 제공합니다.
 */

/**
 * 값이 숫자인지 확인하고 숫자로 변환하는 함수
 * @param {any} value - 확인할 값
 * @param {number} defaultValue - 기본값 (변환 실패 시 사용)
 * @param {boolean} isInteger - 정수 변환 여부
 * @returns {number} 변환된 숫자 값
 */
export const safeNumberConversion = (value, defaultValue = 0, isInteger = false) => {
  // 값이 없거나 빈 문자열인 경우 기본값 반환
  if (value === null || value === undefined || value === '') {
    return defaultValue;
  }
  
  // 숫자로 변환
  const converted = isInteger ? parseInt(value, 10) : parseFloat(value);
  
  // NaN 확인 후 기본값 또는 변환값 반환
  return isNaN(converted) ? defaultValue : converted;
};

/**
 * 객체를 깊은 복사하며 모든 값을 적절한 타입으로 변환하는 함수
 * @param {Object} obj - 변환할 객체
 * @param {Object} config - 변환 설정 (필드별 타입 지정)
 * @returns {Object} 변환된 객체
 */
export const safeObjectConversion = (obj, config = {}) => {
  // 객체가 아닌 경우 그대로 반환
  if (typeof obj !== 'object' || obj === null) {
    return obj;
  }
  
  // 배열인 경우 각 항목에 대해 재귀적으로 처리
  if (Array.isArray(obj)) {
    return obj.map(item => safeObjectConversion(item, config));
  }
  
  // 객체인 경우 각 속성에 대해 타입 변환 수행
  const result = {};
  
  for (const key in obj) {
    if (Object.prototype.hasOwnProperty.call(obj, key)) {
      const value = obj[key];
      
      // 해당 필드에 대한 타입 설정이 있는 경우
      if (config[key]) {
        if (config[key] === 'int') {
          result[key] = safeNumberConversion(value, 0, true);
        } else if (config[key] === 'float') {
          result[key] = safeNumberConversion(value, 0, false);
        } else if (config[key] === 'object' && typeof value === 'object') {
          result[key] = safeObjectConversion(value, config[`${key}_nested`] || {});
        } else {
          result[key] = value;
        }
      } else if (typeof value === 'object' && value !== null) {
        // 내부 객체에 대해 재귀적으로 처리
        result[key] = safeObjectConversion(value, config[`${key}_nested`] || {});
      } else {
        // 기타 값은 그대로 복사
        result[key] = value;
      }
    }
  }
  
  return result;
};

/**
 * Area 설정 객체를 검증하고 타입을 보정하는 함수
 * @param {Object} areaSettings - 영역 설정 객체
 * @returns {Object} 검증 및 보정된 영역 설정 객체
 */
export const validateAreaSettings = (areaSettings) => {
  if (!areaSettings || typeof areaSettings !== 'object') {
    console.error('유효하지 않은 영역 설정 객체:', areaSettings);
    return null;
  }
  
  // 깊은 복사 수행
  const validatedSettings = JSON.parse(JSON.stringify(areaSettings));
  
  // nArea 필드 검증
  validatedSettings.nArea = safeNumberConversion(validatedSettings.nArea, 1, true);
  
  // Area 배열 검증
  if (!Array.isArray(validatedSettings.Area)) {
    console.error('Area 필드가 배열이 아닙니다:', validatedSettings);
    validatedSettings.Area = [];
    return validatedSettings;
  }
  
  // 각 영역 항목 검증
  validatedSettings.Area = validatedSettings.Area.map(area => {
    if (!area || typeof area !== 'object') {
      return {
        Type: 1,
        HeightBottom: 0,
        HeightTop: 1000,
        Point: [
          { Longitude: 0, Latitude: 0 },
          { Longitude: 0, Latitude: 0 },
          { Longitude: 0, Latitude: 0 },
          { Longitude: 0, Latitude: 0 }
        ]
      };
    }
    
    // 기본 필드 검증
    const validatedArea = {
      Valid: (() => {
        const parsed = safeNumberConversion(area.Valid, NaN, true);
        return isNaN(parsed) ? 1 : parsed;
      })(),
      Type: safeNumberConversion(area.Type, 1, true),
      Value: safeNumberConversion(area.Value, 0, true),
      HeightBottom: safeNumberConversion(area.HeightBottom, 0, true),
      HeightTop: safeNumberConversion(area.HeightTop, 1000, true)
    };
    
    // Point 배열 검증
    if (!Array.isArray(area.Point)) {
      validatedArea.Point = [
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 }
      ];
    } else {
      validatedArea.Point = area.Point.map(point => {
        if (!point || typeof point !== 'object') {
          return { Longitude: 0, Latitude: 0 };
        }
        
        return {
          Longitude: safeNumberConversion(point.Longitude, 0, false),
          Latitude: safeNumberConversion(point.Latitude, 0, false)
        };
      });
      
      // 포인트는 최소 4개 필요
      while (validatedArea.Point.length < 4) {
        validatedArea.Point.push({ Longitude: 0, Latitude: 0 });
      }
    }
    
    return validatedArea;
  });
  
  // 영역은 최소 1개 필요
  if (validatedSettings.Area.length === 0) {
    validatedSettings.Area.push({
      Type: 1,
      HeightBottom: 0,
      HeightTop: 1000,
      Point: [
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 },
        { Longitude: 0, Latitude: 0 }
      ]
    });
  }
  
  return validatedSettings;
};

/**
 * 포인트 좌표 값을 업데이트하고 디버깅하는 함수
 * 이 함수는 직접 호출되지 않고, 기존 코드에 대한 설명을 제공합니다.
 * 
 * 문제 원인 및 해결책:
 * 1. 문자열로 전달된 입력값이 숫자로 변환되지 않는 문제
 *    - TextField의 onChange에서 e.target.value는 항상 문자열
 *    - handleAreaSettingsChange에서 parseFloat/parseInt를 호출해야 함
 *    - 문자열을 Number()로 명시적 변환 필요
 * 
 * 2. option 요소의 value 속성 문제
 *    - value={1} 대신 value="1"로 설정해야 문자열로 전달됨
 *    - 수정: <option value="1">탐지 영역</option>
 * 
 * 3. 상태 업데이트 비동기 문제
 *    - setAreaSettings는 비동기로 상태를 업데이트하므로 즉시 반영되지 않음
 *    - useState를 사용할 때는 새 객체 참조를 생성해야 함 (깊은 복사)
 * 
 * 4. 백엔드 응답 처리 문제
 *    - 백엔드에서 반환된 4034 메시지의 값이 문자열일 수 있음
 *    - 이 경우 Number()로 명시적 변환 필요
 * 
 * @param {Object} areaSettings - 현재 영역 설정 객체
 * @param {number} areaIndex - 영역 인덱스
 * @param {number} pointIndex - 포인트 인덱스
 * @param {string} field - 수정할 필드 ('Longitude' 또는 'Latitude')
 * @param {string|number} value - 사용자 입력값
 * @returns {Object} - 업데이트된 영역 설정 객체
 */
export const debugUpdateCoordinate = (areaSettings, areaIndex, pointIndex, field, value) => {
  // 1. 깊은 복사 수행 (JSON 직렬화/역직렬화)
  const newArea = JSON.parse(JSON.stringify(areaSettings));
  
  // 2. 명시적 숫자 변환 (문자열 -> 숫자)
  const numericValue = parseFloat(value) || 0;
  
  // 3. 값 업데이트
  newArea.Area[areaIndex].Point[pointIndex][field] = numericValue;
  
  return newArea;
};

export default {
  safeNumberConversion,
  safeObjectConversion,
  validateAreaSettings,
  debugUpdateCoordinate
}; 