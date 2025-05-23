import React from 'react';
import { Box, Typography, Paper, Grid } from '@mui/material';
import { translate } from '../../utils/i18n';

const DefenseControl = ({ language }) => {
  return (
    <Box sx={{ p: 2 }}>
      <Typography variant="h4" sx={{ mb: 3 }}>
        {translate('방어 시스템 제어', 'Defense System Control', language)}
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
          {translate('방어 시스템 제어 컴포넌트가 여기에 구현될 예정입니다.', 
                   'Defense system control component will be implemented here.', 
                   language)}
        </Typography>
      </Paper>
    </Box>
  );
};

export default DefenseControl; 