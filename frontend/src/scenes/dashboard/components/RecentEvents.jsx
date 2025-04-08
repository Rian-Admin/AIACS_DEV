import { Box, Typography, Paper, List, ListItem } from '@mui/material';
import { translate } from '../../../utils/i18n';

const RecentEvents = ({ language }) => {
  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 2, 
        borderRadius: 2, 
        backgroundColor: '#0a1929',
        border: '1px solid #1e3a5a',
      }}
    >
      <Typography variant="h6" fontWeight="bold" sx={{ mb: 2 }}>
        {translate('최근 이벤트', 'Recent Events', language)}
      </Typography>
      
      <List sx={{ p: 0 }}>
        {[
          { 
            time: '09:42:15', 
            message: translate('카메라 1에서 독수리 감지됨', 'Eagle detected on Camera 1', language), 
            type: 'detection' 
          },
          { 
            time: '09:38:22', 
            message: translate('고위험 충돌 경고 - YW-03', 'High Risk Collision Alert - YW-03', language), 
            type: 'warning' 
          },
          { 
            time: '09:35:47', 
            message: translate('카메라 2 연결 재설정됨', 'Camera 2 connection reset', language), 
            type: 'system' 
          },
          { 
            time: '09:30:05', 
            message: translate('기상 데이터 업데이트됨', 'Weather data updated', language), 
            type: 'info' 
          },
        ].map((event, index) => (
          <ListItem 
            key={index} 
            sx={{ 
              px: 1, 
              py: 0.5, 
              borderLeft: `3px solid ${
                event.type === 'warning' ? '#f44336' : 
                event.type === 'detection' ? '#ff9800' : 
                event.type === 'system' ? '#2196f3' : 
                '#4caf50'
              }`,
              mb: 1,
              backgroundColor: 'rgba(0, 0, 0, 0.1)',
              borderRadius: '0 4px 4px 0'
            }}
          >
            <Box sx={{ width: '100%' }}>
              <Typography variant="caption" color="text.secondary">
                {event.time}
              </Typography>
              <Typography variant="body2">
                {event.message}
              </Typography>
            </Box>
          </ListItem>
        ))}
      </List>
    </Paper>
  );
};

export default RecentEvents; 