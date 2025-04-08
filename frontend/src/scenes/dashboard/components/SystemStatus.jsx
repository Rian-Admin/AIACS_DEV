import { Box, Typography, Paper, Grid, LinearProgress, Divider } from '@mui/material';
import { translate } from '../../../utils/i18n';

const SystemStatus = ({ language }) => {
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
        {translate('시스템 상태', 'System Status', language)}
      </Typography>
      
      <Grid container spacing={2}>
        <Grid item xs={6}>
          <Typography variant="body2" color="text.secondary">
            {translate('CPU 사용량', 'CPU Usage', language)}
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'center', mt: 1 }}>
            <Box sx={{ width: '100%', mr: 1 }}>
              <LinearProgress 
                variant="determinate" 
                value={35} 
                sx={{ 
                  height: 8, 
                  borderRadius: 5,
                  backgroundColor: 'rgba(33, 150, 243, 0.2)',
                  '& .MuiLinearProgress-bar': {
                    backgroundColor: '#2196f3',
                  }
                }} 
              />
            </Box>
            <Typography variant="body2" color="text.secondary">35%</Typography>
          </Box>
        </Grid>
        <Grid item xs={6}>
          <Typography variant="body2" color="text.secondary">
            {translate('메모리', 'Memory', language)}
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'center', mt: 1 }}>
            <Box sx={{ width: '100%', mr: 1 }}>
              <LinearProgress 
                variant="determinate" 
                value={65} 
                sx={{ 
                  height: 8, 
                  borderRadius: 5,
                  backgroundColor: 'rgba(156, 39, 176, 0.2)',
                  '& .MuiLinearProgress-bar': {
                    backgroundColor: '#9c27b0',
                  }
                }} 
              />
            </Box>
            <Typography variant="body2" color="text.secondary">65%</Typography>
          </Box>
        </Grid>
        <Grid item xs={6}>
          <Typography variant="body2" color="text.secondary">
            {translate('디스크', 'Disk', language)}
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'center', mt: 1 }}>
            <Box sx={{ width: '100%', mr: 1 }}>
              <LinearProgress 
                variant="determinate" 
                value={28} 
                sx={{ 
                  height: 8, 
                  borderRadius: 5,
                  backgroundColor: 'rgba(76, 175, 80, 0.2)',
                  '& .MuiLinearProgress-bar': {
                    backgroundColor: '#4caf50',
                  }
                }} 
              />
            </Box>
            <Typography variant="body2" color="text.secondary">28%</Typography>
          </Box>
        </Grid>
        <Grid item xs={6}>
          <Typography variant="body2" color="text.secondary">
            {translate('네트워크', 'Network', language)}
          </Typography>
          <Box sx={{ display: 'flex', alignItems: 'center', mt: 1 }}>
            <Box sx={{ width: '100%', mr: 1 }}>
              <LinearProgress 
                variant="determinate" 
                value={42} 
                sx={{ 
                  height: 8, 
                  borderRadius: 5,
                  backgroundColor: 'rgba(255, 152, 0, 0.2)',
                  '& .MuiLinearProgress-bar': {
                    backgroundColor: '#ff9800',
                  }
                }} 
              />
            </Box>
            <Typography variant="body2" color="text.secondary">42%</Typography>
          </Box>
        </Grid>
      </Grid>
      
      <Divider sx={{ my: 2, borderColor: '#1e3a5a' }} />
      
      <Box>
        <Typography variant="body2" color="text.secondary" sx={{ mb: 1 }}>
          {translate('네트워크 상태', 'Network Status', language)}
        </Typography>
        
        <Grid container spacing={1}>
          {[
            { id: 'CAM-01', status: 'online', latency: '12ms' },
            { id: 'CAM-02', status: 'online', latency: '18ms' },
            { id: 'CAM-03', status: 'online', latency: '15ms' },
            { id: 'RAD-01', status: 'online', latency: '8ms' },
            { id: 'WEATHER', status: 'online', latency: '25ms' },
          ].map((device) => (
            <Grid item xs={12} key={device.id}>
              <Box 
                sx={{ 
                  display: 'flex', 
                  alignItems: 'center', 
                  justifyContent: 'space-between',
                  p: 0.5,
                  borderRadius: 1,
                  backgroundColor: device.status === 'online' ? 'rgba(76, 175, 80, 0.1)' : 'rgba(244, 67, 54, 0.1)'
                }}
              >
                <Typography variant="caption" sx={{ color: device.status === 'online' ? '#4caf50' : '#f44336' }}>
                  {device.id}
                </Typography>
                <Typography variant="caption" color="text.secondary">
                  {device.latency}
                </Typography>
              </Box>
            </Grid>
          ))}
        </Grid>
      </Box>
    </Paper>
  );
};

export default SystemStatus; 