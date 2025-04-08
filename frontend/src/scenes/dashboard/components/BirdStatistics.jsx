import { Box, Typography, Paper, LinearProgress } from '@mui/material';
import { translate } from '../../../utils/i18n';

const BirdStatistics = ({ language }) => {
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
        {translate('조류 감지 통계', 'Bird Detection Statistics', language)}
      </Typography>
      
      <Box>
        <Box sx={{ mb: 2 }}>
          <Typography variant="body2" color="text.secondary" sx={{ mb: 0.5 }}>
            {translate('오늘 감지된 조류', 'Birds Detected Today', language)}
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'baseline' }}>
            <Typography variant="h4" fontWeight="bold">
              36
            </Typography>
            <Typography variant="body2" color="#4caf50" sx={{ ml: 1 }}>
              +12%
            </Typography>
          </Box>
        </Box>
        
        <Typography variant="body2" color="text.secondary" sx={{ mb: 1 }}>
          {translate('조류 종류별 분포', 'Species Distribution', language)}
        </Typography>
        
        <Box sx={{ mb: 1 }}>
          <Box sx={{ display: 'flex', justifyContent: 'space-between', mb: 0.5 }}>
            <Typography variant="caption">
              {translate('까마귀', 'Crow', language)}
            </Typography>
            <Typography variant="caption">
              45%
            </Typography>
          </Box>
          <LinearProgress 
            variant="determinate" 
            value={45} 
            sx={{ 
              height: 5, 
              borderRadius: 5,
              backgroundColor: 'rgba(33, 150, 243, 0.2)',
              '& .MuiLinearProgress-bar': {
                backgroundColor: '#2196f3',
              }
            }} 
          />
        </Box>
        
        <Box sx={{ mb: 1 }}>
          <Box sx={{ display: 'flex', justifyContent: 'space-between', mb: 0.5 }}>
            <Typography variant="caption">
              {translate('독수리', 'Eagle', language)}
            </Typography>
            <Typography variant="caption">
              25%
            </Typography>
          </Box>
          <LinearProgress 
            variant="determinate" 
            value={25} 
            sx={{ 
              height: 5, 
              borderRadius: 5,
              backgroundColor: 'rgba(244, 67, 54, 0.2)',
              '& .MuiLinearProgress-bar': {
                backgroundColor: '#f44336',
              }
            }} 
          />
        </Box>
        
        <Box sx={{ mb: 1 }}>
          <Box sx={{ display: 'flex', justifyContent: 'space-between', mb: 0.5 }}>
            <Typography variant="caption">
              {translate('매', 'Hawk', language)}
            </Typography>
            <Typography variant="caption">
              20%
            </Typography>
          </Box>
          <LinearProgress 
            variant="determinate" 
            value={20} 
            sx={{ 
              height: 5, 
              borderRadius: 5,
              backgroundColor: 'rgba(255, 152, 0, 0.2)',
              '& .MuiLinearProgress-bar': {
                backgroundColor: '#ff9800',
              }
            }} 
          />
        </Box>
        
        <Box>
          <Box sx={{ display: 'flex', justifyContent: 'space-between', mb: 0.5 }}>
            <Typography variant="caption">
              {translate('기타', 'Others', language)}
            </Typography>
            <Typography variant="caption">
              10%
            </Typography>
          </Box>
          <LinearProgress 
            variant="determinate" 
            value={10} 
            sx={{ 
              height: 5, 
              borderRadius: 5,
              backgroundColor: 'rgba(76, 175, 80, 0.2)',
              '& .MuiLinearProgress-bar': {
                backgroundColor: '#4caf50',
              }
            }} 
          />
        </Box>
      </Box>
    </Paper>
  );
};

export default BirdStatistics; 