import React from 'react';
import { Box, Typography, Paper } from '@mui/material';
import { translate, translateSpecies } from '../../../utils/i18n';

const CollisionAlert = ({ collisionRisks, language }) => {
  // 위험 레벨에 따른 색상 반환
  const getRiskColor = (level) => {
    switch (level) {
      case 'critical':
        return '#d32f2f';
      case 'high':
        return '#f57c00';
      case 'medium':
        return '#fbc02d';
      case 'low':
      default:
        return '#4caf50';
    }
  };

  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 1,
        borderRadius: 2, 
        backgroundColor: '#050A18',
        border: '1px solid rgba(30, 58, 90, 0.3)',
        height: '100%',
        display: 'flex',
        flexDirection: 'column'
      }}
    >
      <Typography 
        variant="subtitle2" 
        fontWeight="bold" 
        sx={{ 
          mb: 0.5,
          color: 'white',
          textShadow: '0 0 5px rgba(255,255,255,0.5)',
          fontSize: '0.85rem'
        }}
      >
        {translate('충돌 위험 알림', 'Collision Risk Alerts', language)}
      </Typography>
      
      <Box sx={{ flex: 1, overflow: 'auto', pb: 0.5 }}>
        {collisionRisks.length > 0 ? (
          <Box>
            {collisionRisks.map((risk) => {
              const riskColor = getRiskColor(risk.level);
              return (
                <Box 
                  key={risk.id} 
                  sx={{ 
                    mb: 0.5,
                    position: 'relative',
                    display: 'flex',
                    alignItems: 'center',
                    p: 0.7,
                    backgroundColor: 'rgba(0, 25, 50, 0.4)',
                    borderRadius: 1,
                    border: '1px solid rgba(30, 58, 90, 0.2)',
                    borderLeft: `3px solid ${riskColor}`,
                    boxShadow: `0 0 10px rgba(0, 150, 255, 0.1), inset 0 0 5px rgba(0, 150, 255, 0.05)`,
                    '&:hover': {
                      backgroundColor: 'rgba(0, 30, 60, 0.5)',
                    }
                  }}
                >
                  {/* 위험도 표시 원형 아이콘 */}
                  <Box
                    sx={{
                      width: 32,
                      height: 32,
                      borderRadius: '50%',
                      backgroundColor: `rgba(${risk.level === 'critical' ? '211, 47, 47' : risk.level === 'high' ? '245, 124, 0' : risk.level === 'medium' ? '251, 192, 45' : '76, 175, 80'}, 0.15)`,
                      display: 'flex',
                      alignItems: 'center',
                      justifyContent: 'center',
                      boxShadow: `0 0 8px ${riskColor}80`,
                      mr: 1
                    }}
                  >
                    <Typography 
                      sx={{ 
                        color: riskColor, 
                        fontWeight: 'bold',
                        fontSize: '1.1rem',
                        textShadow: `0 0 10px ${riskColor}`
                      }}
                    >
                      A
                    </Typography>
                  </Box>
                  
                  {/* 정보 영역 */}
                  <Box sx={{ flexGrow: 1 }}>
                    <Typography 
                      variant="body2" 
                      sx={{ 
                        color: '#A9EAFF',
                        fontWeight: 'bold',
                        textShadow: '0 0 5px rgba(169,234,255,0.5)',
                        fontSize: '0.8rem'
                      }}
                    >
                      {risk.location} - {translateSpecies(risk.species, language)}
                    </Typography>
                  </Box>
                  
                  {/* 시간 표시 */}
                  <Typography 
                    variant="caption" 
                    sx={{ 
                      color: 'rgba(169,234,255,0.7)',
                      textShadow: '0 0 3px rgba(169,234,255,0.4)',
                      fontSize: '0.7rem'
                    }}
                  >
                    {new Date(risk.timestamp).toLocaleTimeString(undefined, {
                      hour: '2-digit',
                      minute: '2-digit',
                      hour12: false
                    })}
                  </Typography>
                </Box>
              );
            })}
          </Box>
        ) : (
          <Box 
            sx={{ 
              display: 'flex', 
              flexDirection: 'column', 
              alignItems: 'center', 
              justifyContent: 'center',
              py: 1,
              height: '100%'
            }}
          >
            <Typography 
              variant="body2" 
              sx={{ 
                color: 'rgba(169,234,255,0.7)',
                textShadow: '0 0 3px rgba(169,234,255,0.4)',
                fontSize: '0.8rem'
              }}
            >
              {translate('데이터가 없습니다', 'No data available', language)}
            </Typography>
          </Box>
        )}
      </Box>
    </Paper>
  );
};

export default CollisionAlert; 