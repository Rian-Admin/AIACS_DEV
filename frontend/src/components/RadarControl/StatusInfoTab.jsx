import React from 'react';
import {
  Box,
  Grid,
  Paper,
  Typography,
  Divider,
  FormControlLabel,
  Switch,
  Chip
} from '@mui/material';
import RadarStatusDisplay from './RadarStatusDisplay';

/**
 * 레이더 상태정보 탭 컴포넌트
 */
const StatusInfoTab = ({ 
  language, 
  translate,
  message4001,
  showRawData,
  handleRawDataToggle
}) => {
  // 디버깅을 위한 console.log 추가
  console.log("StatusInfoTab - message4001:", message4001);
  
  // 데이터가 존재하는지 확인
  const hasData = !!message4001;
  
  // 데이터 구조 처리 (소문자 또는 대문자 구조체 모두 처리)
  const rdpData = message4001?.Rdp || message4001?.rdp;
  const rspData = message4001?.Rsp || message4001?.rsp || [];
  
  console.log("StatusInfoTab - hasData:", hasData);
  console.log("StatusInfoTab - rdpData:", rdpData);
  console.log("StatusInfoTab - rspData:", rspData);
  
  // 대체 데이터: 데이터가 없거나 올바르지 않을 때 표시할 기본 데이터
  const dummyRdpData = {
    op_mode: 0,
    track_count: 0,
    plot_count: 0
  };
  
  // 실제 사용할 데이터 (데이터가 없으면 더미 데이터 사용)
  const displayRdpData = rdpData || dummyRdpData;
  const displayRspData = rspData || [];
  
  return (
    <Paper sx={{ p: 3, backgroundColor: '#132f4c', border: '1px solid #1e3a5a' }}>
      <Typography variant="h6" sx={{ mb: 3, color: 'white' }}>
        {translate("레이더 상태정보", "Radar Status Information", language)}
      </Typography>
      
      {/* 운용 상태 표시 */}
      {message4001 && <RadarStatusDisplay data={message4001} language={language} />}
      
      {/* 원시 데이터 보기 옵션 */}
      <Box sx={{ mt: 2, backgroundColor: '#0a1929', p: 2, borderRadius: 1 }}>
        <Typography variant="h6" sx={{ color: 'lightblue', mb: 2 }}>
          {translate("레이더 상태 정보 (4001)", "Radar Status Information (4001)", language)}
        </Typography>
        
        <FormControlLabel
          control={
            <Switch
              checked={showRawData}
              onChange={handleRawDataToggle}
              color="primary"
            />
          }
          label={
            <Typography variant="body2" sx={{ color: 'white' }}>
              {translate("원시 데이터 보기", "Show Raw Data", language)}
            </Typography>
          }
          sx={{ mb: 1 }}
        />
        
        {showRawData && (
          <Box sx={{ mt: 1, backgroundColor: '#071426', p: 1, borderRadius: 1, overflow: 'auto', maxHeight: '400px' }}>
            {message4001 ? (
              <pre style={{ color: '#4fc3f7', fontSize: '0.7rem', whiteSpace: 'pre-wrap', overflowX: 'auto' }}>
                {JSON.stringify(message4001, null, 2)}
              </pre>
            ) : (
              <Typography variant="body2" sx={{ color: '#ffcc80', fontStyle: 'italic' }}>
                {translate("상태 데이터가 없습니다", "No status data available", language)}
              </Typography>
            )}
          </Box>
        )}
      </Box>
      
      {/* 상세 상태 정보 표시 */}
      <Box sx={{ mt: 3 }}>
        <Divider sx={{ my: 2, backgroundColor: '#1e3a5a' }} />
        
        <Typography variant="subtitle1" sx={{ color: 'lightblue', mb: 2 }}>
          {translate("상세 상태 정보", "Detailed Status Information", language)}
        </Typography>
        
        {/* 레이더 컨트롤러 (RDP) 정보 */}
        <Paper sx={{ p: 2, backgroundColor: '#1e3a5a', mb: 2 }}>
          <Typography variant="subtitle2" sx={{ color: 'white', mb: 2 }}>
            {translate("레이더 컨트롤러 (RDP) 정보", "Radar Controller (RDP) Information", language)}
          </Typography>
          
          {/* 데이터가 없는 경우 */}
          {!displayRdpData && (
            <Typography variant="body1" sx={{ color: 'white', textAlign: 'center', py: 2 }}>
              {translate("RDP 데이터가 없습니다", "No RDP data available", language)}
            </Typography>
          )}
          
          {displayRdpData && (
            <Grid container spacing={2}>
              {/* 1행 */}
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("운용 상태", "Operation State", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.State === 0 ? translate("대기중", "Standby", language) :
                     displayRdpData.State === 1 ? translate("초기화중", "Initializing", language) :
                     displayRdpData.State === 2 ? translate("운용중", "Operating", language) :
                     translate("알 수 없음", "Unknown", language)}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("시리얼 번호", "Serial Number", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.RdpSerialNo || displayRdpData.SerialNo || 0}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("사용 시간", "Operation Time", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {Math.floor((displayRdpData.OpTime || 0) / 3600)} {translate("시간", "hours", language)} {' '}
                    {Math.floor(((displayRdpData.OpTime || 0) % 3600) / 60)} {translate("분", "minutes", language)}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("스캔 번호", "Scan Number", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.Scan || 0}
                  </Typography>
                </Box>
              </Grid>
              
              {/* 2행 */}
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("고장 상태", "Fault Status", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                    <Chip 
                      size="small"
                      label={
                        displayRdpData.RdpFault === 1 ? translate("정상", "Normal", language) :
                        displayRdpData.RdpFault === 2 ? translate("점검요망", "Check Required", language) :
                        displayRdpData.RdpFault === 3 ? translate("고장", "Fault", language) :
                        translate("알 수 없음", "Unknown", language)
                      }
                      color={
                        displayRdpData.RdpFault === 1 ? "success" :
                        displayRdpData.RdpFault === 2 ? "warning" :
                        displayRdpData.RdpFault === 3 ? "error" : "default"
                      }
                      sx={{ minWidth: '60px' }}
                    />
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("트랙 개수", "Track Count", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.TrackNum || 0}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("호스트 트랙 개수", "Host Track Count", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.HostTrackNum || 0}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("플롯 개수", "Plot Count", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {displayRdpData.PlotNum || 0}
                  </Typography>
                </Box>
              </Grid>
              
              {/* 3행 */}
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("NTP 서버 연결", "NTP Server Connection", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                    <Chip 
                      size="small"
                      label={displayRdpData.NtpServerConnect ? translate("연결됨", "Connected", language) : translate("연결 안됨", "Disconnected", language)}
                      color={displayRdpData.NtpServerConnect ? "success" : "error"}
                      sx={{ minWidth: '60px' }}
                    />
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("패널 개수", "Panel Count", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {message4001?.RadarPanelNum || 0}
                  </Typography>
                </Box>
              </Grid>
              
              <Grid item xs={12} sm={6} md={3}>
                <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                  <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                    {translate("타임스탬프", "Timestamp", language)}
                  </Typography>
                  <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                    {message4001?.header?.tv_sec ? 
                      new Date(message4001.header.tv_sec * 1000).toLocaleTimeString() :
                      translate("정보 없음", "No data", language)
                    }
                  </Typography>
                </Box>
              </Grid>
            </Grid>
          )}
        </Paper>
        
        {/* 레이더 패널 정보 */}
        <Paper sx={{ p: 2, backgroundColor: '#1e3a5a' }}>
          <Typography variant="subtitle2" sx={{ color: 'white', mb: 2 }}>
            {translate("레이더 패널 정보", "Radar Panel Information", language)}
          </Typography>
          
          {/* 데이터가 없는 경우 */}
          {displayRspData.length === 0 && (
            <Typography variant="body1" sx={{ color: 'white', textAlign: 'center', py: 2 }}>
              {translate("패널 데이터가 없습니다", "No panel data available", language)}
            </Typography>
          )}
          
          {displayRspData.map((panel, panelIndex) => (
            <Box key={panelIndex} sx={{ mb: panelIndex < displayRspData.length - 1 ? 3 : 0 }}>
              <Typography variant="subtitle2" sx={{ color: '#90caf9', mb: 2 }}>
                {translate("패널", "Panel", language)} #{panelIndex + 1}
              </Typography>
              
              <Grid container spacing={2}>
                {/* 패널 1행 */}
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("모델", "Model", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.Mode || 0}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("시리얼 번호", "Serial Number", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.SerialNo || 0}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("패널 상태", "Panel State", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.State === 0 ? translate("부팅중", "Booting", language) :
                       panel.State === 1 ? translate("대기중", "Standby", language) :
                       panel.State === 13 ? translate("운용중 (송신ON)", "Operating (Tx ON)", language) :
                       panel.State === 14 ? translate("운용중 (송신OFF)", "Operating (Tx OFF)", language) :
                       translate("알 수 없음", "Unknown", language)}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("사용 시간", "Operation Time", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {Math.floor((panel.OpTime || 0) / 3600)} {translate("시간", "hours", language)} {' '}
                      {Math.floor(((panel.OpTime || 0) % 3600) / 60)} {translate("분", "minutes", language)}
                    </Typography>
                  </Box>
                </Grid>
                
                {/* 패널 2행 */}
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("Dwell 번호", "Dwell Number", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.Dwell || 0}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("송신패널 상태", "Tx Panel Status", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                      <Chip 
                        size="small"
                        label={
                          panel.TxPanelFault === 1 ? translate("정상", "Normal", language) :
                          panel.TxPanelFault === 2 ? translate("점검요망", "Check Required", language) :
                          panel.TxPanelFault === 3 ? translate("고장", "Fault", language) :
                          translate("알 수 없음", "Unknown", language)
                        }
                        color={
                          panel.TxPanelFault === 1 ? "success" :
                          panel.TxPanelFault === 2 ? "warning" :
                          panel.TxPanelFault === 3 ? "error" : "default"
                        }
                        sx={{ minWidth: '60px' }}
                      />
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("수신패널 상태", "Rx Panel Status", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                      <Chip 
                        size="small"
                        label={
                          panel.RxPanelFault === 1 ? translate("정상", "Normal", language) :
                          panel.RxPanelFault === 2 ? translate("점검요망", "Check Required", language) :
                          panel.RxPanelFault === 3 ? translate("고장", "Fault", language) :
                          translate("알 수 없음", "Unknown", language)
                        }
                        color={
                          panel.RxPanelFault === 1 ? "success" :
                          panel.RxPanelFault === 2 ? "warning" :
                          panel.RxPanelFault === 3 ? "error" : "default"
                        }
                        sx={{ minWidth: '60px' }}
                      />
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("프로세서 상태", "Processor Status", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                      <Chip 
                        size="small"
                        label={
                          panel.ProcessorFault === 1 ? translate("정상", "Normal", language) :
                          panel.ProcessorFault === 2 ? translate("점검요망", "Check Required", language) :
                          panel.ProcessorFault === 3 ? translate("고장", "Fault", language) :
                          translate("알 수 없음", "Unknown", language)
                        }
                        color={
                          panel.ProcessorFault === 1 ? "success" :
                          panel.ProcessorFault === 2 ? "warning" :
                          panel.ProcessorFault === 3 ? "error" : "default"
                        }
                        sx={{ minWidth: '60px' }}
                      />
                    </Typography>
                  </Box>
                </Grid>
                
                {/* 패널 3행 */}
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("송신패널 온도", "Tx Panel Temp", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.TxPanelTemp !== undefined ? `${panel.TxPanelTemp}°C` : translate("정보 없음", "No data", language)}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("수신패널 온도", "Rx Panel Temp", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.RxPanelTemp !== undefined ? `${panel.RxPanelTemp}°C` : translate("정보 없음", "No data", language)}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("프로세서 온도", "Processor Temp", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold' }}>
                      {panel.ProcessorTemp !== undefined && panel.ProcessorTemp > -270 ? `${panel.ProcessorTemp.toFixed(1)}°C` : translate("정보 없음", "No data", language)}
                    </Typography>
                  </Box>
                </Grid>
                
                <Grid item xs={12} sm={6} md={3}>
                  <Box sx={{ backgroundColor: '#0a1929', p: 1.5, borderRadius: 1, height: '100%' }}>
                    <Typography variant="caption" sx={{ color: '#90caf9', display: 'block', mb: 0.5 }}>
                      {translate("냉각팬 상태", "Fan Status", language)}
                    </Typography>
                    <Typography variant="body2" sx={{ color: 'white', fontWeight: 'bold', display: 'flex', alignItems: 'center' }}>
                      <Chip 
                        size="small"
                        label={
                          panel.FanFault === 1 ? translate("정상", "Normal", language) :
                          panel.FanFault === 2 ? translate("점검요망", "Check Required", language) :
                          panel.FanFault === 3 ? translate("고장", "Fault", language) :
                          translate("알 수 없음", "Unknown", language)
                        }
                        color={
                          panel.FanFault === 1 ? "success" :
                          panel.FanFault === 2 ? "warning" :
                          panel.FanFault === 3 ? "error" : "default"
                        }
                        sx={{ minWidth: '60px' }}
                      />
                    </Typography>
                  </Box>
                </Grid>
              </Grid>
            </Box>
          ))}
        </Paper>
      </Box>
    </Paper>
  );
};

export default StatusInfoTab; 