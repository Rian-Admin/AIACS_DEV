import React from 'react';
import { Box, Typography, Paper, Grid } from '@mui/material';
import { translate } from '../../utils/i18n';

const VideoAnalysis = ({ language }) => {
  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h4" sx={{ mb: 3 }}>
        {translate('동영상 분석', 'Video Analysis', language)}
      </Typography>
      
      <Paper
        sx={{
          p: 3,
          backgroundColor: '#0a1929',
          border: '1px solid #1e3a5a',
          borderRadius: 2
        }}
      >
        <Typography variant="body1">
          {translate('동영상 분석 컴포넌트가 여기에 구현될 예정입니다.', 
                   'Video analysis component will be implemented here.', 
                   language)}
        </Typography>
      </Paper>
    </Box>
  );
};

export default VideoAnalysis; 