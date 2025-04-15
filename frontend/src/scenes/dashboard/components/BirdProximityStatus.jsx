import { Box, Typography, Paper } from '@mui/material';
import { translate } from '../../../utils/i18n';

const BirdProximityStatus = ({ birdActivityData, language }) => {
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
        {translate('조류 실시간 근접 현황', 'Bird Real-Time Proximity Status', language)}
      </Typography>
      
      <Box sx={{ height: 180, display: 'flex', flexDirection: 'column' }}>
        <Box sx={{ flex: 1, display: 'flex', alignItems: 'flex-end' }}>
          {birdActivityData.map((item) => (
            <Box 
              key={item.id} 
              sx={{ 
                flex: 1, 
                display: 'flex', 
                flexDirection: 'column', 
                alignItems: 'center', 
                justifyContent: 'flex-end', 
                height: '100%' 
              }}
            >
              <Box 
                sx={{ 
                  width: '60%', 
                  height: `${item.count * 10}%`, 
                  backgroundColor: `rgba(33, 150, 243, ${0.3 + item.risk * 0.7})`, 
                  borderTopLeftRadius: 3, 
                  borderTopRightRadius: 3,
                  transition: 'height 0.3s ease-in-out'
                }} 
              />
              <Typography variant="caption" sx={{ mt: 0.5 }}>
                {item.id}
              </Typography>
            </Box>
          ))}
        </Box>
        <Box 
          sx={{ 
            display: 'flex', 
            justifyContent: 'space-between', 
            mt: 1, 
            borderTop: '1px solid #1e3a5a', 
            pt: 0.5 
          }}
        >
          {[0, 2, 4, 6, 8, 10, 12, 14].map(num => (
            <Typography key={num} variant="caption">
              {num}
            </Typography>
          ))}
        </Box>
      </Box>
    </Paper>
  );
};

export default BirdProximityStatus; 