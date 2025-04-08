import React, { useEffect } from 'react';
import {
  Box,
  Grid,
  Paper,
  Typography,
  TextField,
  Button,
  Divider,
  CircularProgress
} from '@mui/material';
import { safeNumberConversion, validateAreaSettings } from './debug'; // 디버깅 유틸리티 임포트

/**
 * 레이더 고급설정 탭 컴포넌트
 */
const AdvancedSettingsTab = ({ 
  language, 
  translate,
  performanceParams,
  setPerformanceParams,
  loading,
  requestPerformanceParams,
  updatePerformanceParams,
  areaSettings,
  setAreaSettings,
  requestAreaSettings,
  updateAreaSettings
}) => {
  // 영역 설정 초기값
  const defaultArea = {
    nArea: 1,
    Area: [
      {
        Valid: 1, // 영역 적용 여부 (0: 미적용, 1: 적용)
        Type: 1,  // 1: 추적금지영역
        Value: 0, // Type에 따른 값 (Type 3은 저속추적금지 속도, Type 4는 RCS 임계값)
        Point: [
          { Longitude: 126.9, Latitude: 37.5 },
          { Longitude: 126.95, Latitude: 37.5 },
          { Longitude: 126.95, Latitude: 37.55 },
          { Longitude: 126.9, Latitude: 37.55 }
        ],
        HeightBottom: 0,
        HeightTop: 1000
      }
    ]
  };
  
  // areaSettings이 없는 경우 초기화
  useEffect(() => {
    if (!areaSettings) {
      console.log('areaSettings이 없음, 기본값으로 초기화합니다.');
      setAreaSettings(defaultArea);
    }
  }, [areaSettings, setAreaSettings]);
  
  // 현재 설정된 영역 정보 또는 기본값
  const currentAreaSettings = areaSettings || defaultArea;
  
  // 디버깅을 위한 useEffect - areaSettings 상태 변화 감지
  useEffect(() => {
    console.log("==== areaSettings 상태가 변경됨 ====");
    console.log("현재 areaSettings:", JSON.stringify(areaSettings, null, 2));
    console.log("currentAreaSettings:", JSON.stringify(currentAreaSettings, null, 2));
  }, [areaSettings]);
  
  // 영역 설정 변경 함수
  const handleAreaSettingsChange = (index, field, value, pointIndex = null) => {
    console.log(`Changing area setting: index=${index}, field=${field}, value=${value}, pointIndex=${pointIndex}`);
    console.log("값 타입:", typeof value);
    
    // 특별히 Valid 필드 처리에 대한 디버깅 로그 추가
    if (field === 'Valid') {
      console.log("영역 적용 값 변경:", value);
      console.log("값 타입:", typeof value);
      console.log("숫자 0인지 확인:", value === 0);
      console.log("toString():", value.toString());
    }
    
    // 깊은 복사 수행 (더 명시적인 방법)
    const newArea = {
      nArea: currentAreaSettings.nArea,
      Area: currentAreaSettings.Area.map((area, i) => {
        if (i !== index) return { ...area };
        
        // 현재 수정 중인 영역
        if (pointIndex !== null) {
          // 좌표 수정
          return {
            ...area,
            Point: area.Point.map((point, j) => {
              if (j !== pointIndex) return { ...point };
              
              // 현재 수정 중인 좌표 포인트
              if (field === 'Longitude') {
                return { ...point, Longitude: Number(value) };
              } else if (field === 'Latitude') {
                return { ...point, Latitude: Number(value) };
              }
              return { ...point };
            })
          };
        } else {
          // 영역 속성 수정
          if (field === 'HeightBottom') {
            return { ...area, HeightBottom: Number(value) };
          } else if (field === 'HeightTop') {
            return { ...area, HeightTop: Number(value) };
          } else if (field === 'Type') {
            return { ...area, Type: Number(value) };
          } else if (field === 'Valid') {
            return { ...area, Valid: Number(value) };
          } else if (field === 'Value') {
            return { ...area, Value: Number(value) };
          }
          return { ...area };
        }
      })
    };
    
    console.log('업데이트할 영역 설정:', JSON.stringify(newArea, null, 2));
    
    // 영역 설정 업데이트
    setAreaSettings(newArea);
  };
  
  // 영역 설정 적용 함수
  const handleApplyAreaSettings = () => {
    // 최종 적용할 영역설정 데이터 확인
    console.log('영역 설정 적용 준비:', JSON.stringify(currentAreaSettings, null, 2));
    
    // Valid 값 특별 로깅
    if (currentAreaSettings && currentAreaSettings.Area && currentAreaSettings.Area.length > 0) {
      const area = currentAreaSettings.Area[0];
      console.log("최종 영역 적용(Valid) 값:", area.Valid);
      console.log("숫자로 변환한 Valid 값:", Number(area.Valid));
      console.log("타입:", typeof area.Valid);
    }
    
    // 영역 설정 데이터 검증 및 타입 변환 (디버깅 유틸리티 사용)
    const validData = validateAreaSettings(currentAreaSettings);
    
    if (!validData) {
      console.error('영역 설정 데이터 검증 실패');
      return;
    }
    
    console.log('최종 영역 설정 데이터:', JSON.stringify(validData, null, 2));
    
    // 영역 설정 적용 요청
    updateAreaSettings(validData);
  };
  
  return (
    <Paper sx={{ p: 3, backgroundColor: '#132f4c', border: '1px solid #1e3a5a' }}>
      <Typography variant="h6" sx={{ mb: 3, color: 'white' }}>
        {translate("성능 파라미터 및 영역 설정", "Performance Parameters & Area Settings", language)}
      </Typography>
      
      {/* 레이더 탐지 영역 설정 섹션 */}
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate("레이더 탐지 영역 설정", "Radar Detection Range Settings", language)}
      </Typography>
      
      <Grid container spacing={2} sx={{ mb: 3 }}>
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("최소 고도", "Minimum Altitude", language)}
            type="number"
            value={performanceParams.rdp.lowAltitude}
            onChange={(e) => setPerformanceParams({
              ...performanceParams,
              rdp: { ...performanceParams.rdp, lowAltitude: parseInt(e.target.value) }
            })}
            InputProps={{ endAdornment: 'm' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("최대 속도", "Maximum Velocity", language)}
            type="number"
            value={performanceParams.rdp.maxVelocity}
            onChange={(e) => setPerformanceParams({
              ...performanceParams,
              rdp: { ...performanceParams.rdp, maxVelocity: parseInt(e.target.value) }
            })}
            InputProps={{ endAdornment: 'm/s' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
      </Grid>
      
      <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />
      
      {/* 탐지 설정 섹션 */}
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate("탐지 설정", "Detection Settings", language)}
      </Typography>
      
      <Grid container spacing={2} sx={{ mb: 3 }}>
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("CFAR 임계값", "CFAR Threshold", language)}
            type="number"
            value={performanceParams.rsp[0].cfar_threshold}
            onChange={(e) => {
              const newRsp = [...performanceParams.rsp];
              newRsp[0] = { ...newRsp[0], cfar_threshold: parseFloat(e.target.value) };
              setPerformanceParams({ ...performanceParams, rsp: newRsp });
            }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("최소 속도", "Minimum Velocity", language)}
            type="number"
            value={performanceParams.rsp[0].minVelocity}
            onChange={(e) => {
              const newRsp = [...performanceParams.rsp];
              newRsp[0] = { ...newRsp[0], minVelocity: parseFloat(e.target.value) };
              setPerformanceParams({ ...performanceParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: 'm/s' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("최소 거리", "Minimum Range", language)}
            type="number"
            value={performanceParams.rsp[0].minRange}
            onChange={(e) => {
              const newRsp = [...performanceParams.rsp];
              newRsp[0] = { ...newRsp[0], minRange: parseFloat(e.target.value) };
              setPerformanceParams({ ...performanceParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: 'm' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
        
        <Grid item xs={12} sm={6}>
          <TextField
            fullWidth
            label={translate("최대 거리", "Maximum Range", language)}
            type="number"
            value={performanceParams.rsp[0].maxRange}
            onChange={(e) => {
              const newRsp = [...performanceParams.rsp];
              newRsp[0] = { ...newRsp[0], maxRange: parseFloat(e.target.value) };
              setPerformanceParams({ ...performanceParams, rsp: newRsp });
            }}
            InputProps={{ endAdornment: 'm' }}
            sx={{
              input: { color: 'white' },
              '& .MuiOutlinedInput-root': {
                '& fieldset': { borderColor: '#1e3a5a' },
                '&:hover fieldset': { borderColor: '#90caf9' },
                '&.Mui-focused fieldset': { borderColor: '#90caf9' },
              },
              '& .MuiInputLabel-root': { color: '#90caf9' }
            }}
          />
        </Grid>
      </Grid>
      
      {/* 설정 버튼 */}
      <Box sx={{ display: 'flex', justifyContent: 'flex-end', mt: 2 }}>
        <Button
          variant="contained"
          color="primary"
          onClick={() => updatePerformanceParams(performanceParams)}
          disabled={loading}
          sx={{ mr: 1 }}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate("설정 적용", "Apply Settings", language)
          )}
        </Button>
        
        <Button
          variant="outlined"
          onClick={requestPerformanceParams}
          disabled={loading}
          sx={{
            borderColor: '#1e3a5a',
            color: '#90caf9',
            '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
          }}
        >
          {translate("기본값 불러오기", "Load Defaults", language)}
        </Button>
      </Box>
      
      <Divider sx={{ my: 3, backgroundColor: '#1e3a5a' }} />
      
      {/* 영역 설정 섹션 */}
      <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
        {translate("레이더 영역 설정", "Radar Area Settings", language)}
      </Typography>
      
      {currentAreaSettings.Area && currentAreaSettings.Area.map((area, areaIndex) => (
        <Box key={`area-${areaIndex}`} sx={{ mb: 3, p: 2, border: '1px solid #1e3a5a', borderRadius: 1 }}>
          <Typography variant="subtitle2" sx={{ color: 'white', mb: 2 }}>
            {translate("영역", "Area", language)} #{areaIndex + 1} - 
            {area.Type === 1 ? translate(" 탐지 영역", " Detection Area", language) : 
             area.Type === 2 ? translate(" 제외 영역", " Exclusion Area", language) : 
             translate(" 기타 영역", " Other Area", language)}
          </Typography>
          
          <Grid container spacing={2} sx={{ mb: 2 }}>
            <Grid item xs={12} sm={4}>
              <TextField
                fullWidth
                select
                label={translate("영역 유형", "Area Type", language)}
                value={area.Type}
                onChange={(e) => {
                  console.log("-------- 영역 유형 변경 시작 --------");
                  console.log("선택된 영역 유형(raw):", e.target.value);
                  console.log("선택된 영역 유형(타입):", typeof e.target.value);
                  // 명시적으로 숫자로 변환하여 전달
                  const numericValue = Number(e.target.value);
                  console.log("변환된 숫자값:", numericValue);
                  handleAreaSettingsChange(areaIndex, 'Type', numericValue);
                  console.log("-------- 영역 유형 변경 완료 --------");
                }}
                SelectProps={{
                  native: true,
                }}
                inputProps={{
                  style: { color: 'white' }
                }}
                sx={{
                  '& .MuiSelect-select': { color: 'white' },
                  '& .MuiOutlinedInput-root': {
                    '& fieldset': { borderColor: '#1e3a5a' },
                    '&:hover fieldset': { borderColor: '#90caf9' },
                    '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                  },
                  '& .MuiInputLabel-root': { color: '#90caf9' }
                }}
              >
                <option value="1">{translate("추적금지영역", "No Tracking Area", language)}</option>
                <option value="2">{translate("자동추적금지영역", "No Track Init Area", language)}</option>
                <option value="3">{translate("저속추적금지영역", "No Low Speed Track Area", language)}</option>
                <option value="4">{translate("저RCS추적금지영역", "No Low RCS Track Area", language)}</option>
              </TextField>
            </Grid>
            
            <Grid item xs={12} sm={4}>
              <TextField
                fullWidth
                select
                label={translate("영역 적용", "Area Valid", language)}
                value={area.Valid}
                onChange={(e) => {
                  console.log("-------- 영역 적용 변경 시작 --------");
                  console.log("선택된 영역 적용(raw):", e.target.value);
                  // 명시적으로 숫자로 변환하여 전달
                  const numericValue = Number(e.target.value);
                  console.log("변환된 숫자값:", numericValue);
                  handleAreaSettingsChange(areaIndex, 'Valid', numericValue);
                  console.log("-------- 영역 적용 변경 완료 --------");
                }}
                SelectProps={{
                  native: true,
                }}
                inputProps={{
                  style: { color: 'white' }
                }}
                sx={{
                  '& .MuiSelect-select': { color: 'white' },
                  '& .MuiOutlinedInput-root': {
                    '& fieldset': { borderColor: '#1e3a5a' },
                    '&:hover fieldset': { borderColor: '#90caf9' },
                    '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                  },
                  '& .MuiInputLabel-root': { color: '#90caf9' }
                }}
              >
                <option value="1">{translate("적용", "Applied", language)}</option>
                <option value="0">{translate("미적용", "Not Applied", language)}</option>
              </TextField>
            </Grid>
            
            <Grid item xs={12} sm={4}>
              <TextField
                fullWidth
                type="number"
                label={translate("설정값", "Value", language)}
                value={area.Value}
                onChange={(e) => {
                  console.log("-------- 설정값 변경 시작 --------");
                  console.log("입력된 설정값(raw):", e.target.value);
                  // 명시적으로 숫자로 변환하여 전달
                  const numericValue = parseInt(e.target.value) || 0;
                  console.log("변환된 숫자값:", numericValue);
                  handleAreaSettingsChange(areaIndex, 'Value', numericValue);
                  console.log("-------- 설정값 변경 완료 --------");
                }}
                helperText={
                  area.Type === 3 ? translate("저속추적금지 속도(m/s)", "Min Speed (m/s)", language) :
                  area.Type === 4 ? translate("RCS 임계값(dB)", "RCS Threshold (dB)", language) :
                  translate("타입에 따른 설정값", "Type-specific Value", language)
                }
                inputProps={{
                  step: "1",
                  style: { color: 'white' }
                }}
                sx={{
                  '& .MuiOutlinedInput-root': {
                    '& fieldset': { borderColor: '#1e3a5a' },
                    '&:hover fieldset': { borderColor: '#90caf9' },
                    '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                  },
                  '& .MuiInputLabel-root': { color: '#90caf9' },
                  '& .MuiFormHelperText-root': { color: '#90caf9' }
                }}
              />
            </Grid>
          </Grid>
          
          <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 1 }}>
            {translate("영역 좌표", "Area Coordinates", language)}
          </Typography>
          
          <Grid container spacing={2}>
            {area.Point && area.Point.map((point, pointIndex) => (
              <Grid item xs={12} sm={6} key={`point-${pointIndex}`}>
                <Box sx={{ p: 1, border: '1px solid #1e3a5a', borderRadius: 1, mb: 1 }}>
                  <Typography variant="body2" sx={{ color: 'white', mb: 1 }}>
                    {translate("포인트", "Point", language)} #{pointIndex + 1}
                  </Typography>
                  
                  <Grid container spacing={2}>
                    <Grid item xs={6}>
                      <TextField
                        fullWidth
                        label={translate("경도", "Longitude", language)}
                        type="number"
                        value={point.Longitude}
                        onChange={(e) => {
                          console.log("-------- 경도 변경 시작 --------");
                          console.log("입력된 경도값(raw):", e.target.value);
                          console.log("입력된 경도값(타입):", typeof e.target.value);
                          // 명시적으로 숫자로 변환하여 전달
                          const numericValue = parseFloat(e.target.value) || 0;
                          console.log("변환된 숫자값:", numericValue);
                          handleAreaSettingsChange(areaIndex, 'Longitude', numericValue, pointIndex);
                          console.log("-------- 경도 변경 완료 --------");
                        }}
                        inputProps={{ 
                          step: "0.000001",
                          style: { color: 'white' } 
                        }}
                        sx={{
                          input: { color: 'white' },
                          '& .MuiOutlinedInput-root': {
                            '& fieldset': { borderColor: '#1e3a5a' },
                            '&:hover fieldset': { borderColor: '#90caf9' },
                            '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                          },
                          '& .MuiInputLabel-root': { color: '#90caf9' }
                        }}
                      />
                    </Grid>
                    
                    <Grid item xs={6}>
                      <TextField
                        fullWidth
                        label={translate("위도", "Latitude", language)}
                        type="number"
                        value={point.Latitude}
                        onChange={(e) => {
                          console.log("-------- 위도 변경 시작 --------");
                          console.log("입력된 위도값(raw):", e.target.value);
                          console.log("입력된 위도값(타입):", typeof e.target.value);
                          // 명시적으로 숫자로 변환하여 전달
                          const numericValue = parseFloat(e.target.value) || 0;
                          console.log("변환된 숫자값:", numericValue);
                          handleAreaSettingsChange(areaIndex, 'Latitude', numericValue, pointIndex);
                          console.log("-------- 위도 변경 완료 --------");
                        }}
                        inputProps={{ 
                          step: "0.000001",
                          style: { color: 'white' } 
                        }}
                        sx={{
                          input: { color: 'white' },
                          '& .MuiOutlinedInput-root': {
                            '& fieldset': { borderColor: '#1e3a5a' },
                            '&:hover fieldset': { borderColor: '#90caf9' },
                            '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                          },
                          '& .MuiInputLabel-root': { color: '#90caf9' }
                        }}
                      />
                    </Grid>
                  </Grid>
                </Box>
              </Grid>
            ))}
          </Grid>
          
          <Grid container spacing={2} sx={{ mb: 2 }}>
            <Grid item xs={12} sm={4}>
              <TextField
                fullWidth
                label={translate("하단 고도", "Bottom Height", language)}
                type="number"
                value={area.HeightBottom}
                onChange={(e) => {
                  console.log("-------- 하단 고도 변경 시작 --------");
                  console.log("입력된 하단 고도(raw):", e.target.value);
                  console.log("입력된 하단 고도(타입):", typeof e.target.value);
                  // 명시적으로 숫자로 변환하여 전달
                  const numericValue = parseInt(e.target.value) || 0;
                  console.log("변환된 숫자값:", numericValue);
                  handleAreaSettingsChange(areaIndex, 'HeightBottom', numericValue);
                  console.log("-------- 하단 고도 변경 완료 --------");
                }}
                InputProps={{ endAdornment: 'm' }}
                inputProps={{
                  step: "1",
                  style: { color: 'white' }
                }}
                sx={{
                  '& .MuiOutlinedInput-root': {
                    '& fieldset': { borderColor: '#1e3a5a' },
                    '&:hover fieldset': { borderColor: '#90caf9' },
                    '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                  },
                  '& .MuiInputLabel-root': { color: '#90caf9' }
                }}
              />
            </Grid>
            
            <Grid item xs={12} sm={4}>
              <TextField
                fullWidth
                label={translate("상단 고도", "Top Height", language)}
                type="number"
                value={area.HeightTop}
                onChange={(e) => {
                  console.log("-------- 상단 고도 변경 시작 --------");
                  console.log("입력된 상단 고도(raw):", e.target.value);
                  console.log("입력된 상단 고도(타입):", typeof e.target.value);
                  // 명시적으로 숫자로 변환하여 전달
                  const numericValue = parseInt(e.target.value) || 0;
                  console.log("변환된 숫자값:", numericValue);
                  handleAreaSettingsChange(areaIndex, 'HeightTop', numericValue);
                  console.log("-------- 상단 고도 변경 완료 --------");
                }}
                InputProps={{ endAdornment: 'm' }}
                inputProps={{
                  step: "1",
                  style: { color: 'white' }
                }}
                sx={{
                  '& .MuiOutlinedInput-root': {
                    '& fieldset': { borderColor: '#1e3a5a' },
                    '&:hover fieldset': { borderColor: '#90caf9' },
                    '&.Mui-focused fieldset': { borderColor: '#90caf9' },
                  },
                  '& .MuiInputLabel-root': { color: '#90caf9' }
                }}
              />
            </Grid>
          </Grid>
        </Box>
      ))}
      
      {/* 영역 설정 버튼 */}
      <Box sx={{ display: 'flex', justifyContent: 'flex-end', mt: 2 }}>
        <Button
          variant="contained"
          color="primary"
          onClick={handleApplyAreaSettings}
          disabled={loading}
          sx={{ mr: 1 }}
        >
          {loading ? (
            <CircularProgress size={24} color="inherit" />
          ) : (
            translate("영역 설정 적용", "Apply Area Settings", language)
          )}
        </Button>
        
        <Button
          variant="outlined"
          onClick={requestAreaSettings}
          disabled={loading}
          sx={{
            borderColor: '#1e3a5a',
            color: '#90caf9',
            '&:hover': { borderColor: '#90caf9', backgroundColor: 'rgba(144, 202, 249, 0.08)' }
          }}
        >
          {translate("영역 설정 불러오기", "Load Area Settings", language)}
        </Button>
      </Box>
    </Paper>
  );
};

export default AdvancedSettingsTab; 