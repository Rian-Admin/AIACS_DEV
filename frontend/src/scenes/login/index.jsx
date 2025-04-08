import React, { useState } from 'react';
import { Box, Typography, Paper, TextField, Button, IconButton, InputAdornment, Alert } from '@mui/material';
import { Visibility, VisibilityOff } from '@mui/icons-material';
import { translate } from '../../utils/i18n';
import rianLogo from '../../assets/images/rian_40px.png';

const Login = ({ handleLogin, language }) => {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [showPassword, setShowPassword] = useState(false);
  const [error, setError] = useState('');
  
  const handleSubmit = (e) => {
    e.preventDefault();
    if (!username || !password) {
      setError(translate('사용자 이름과 비밀번호를 모두 입력해주세요.', 'Please enter both username and password.', language));
      return;
    }
    
    // 실제 구현에서는 API 호출로 대체
    if (username === 'admin' && password === 'admin') {
      handleLogin('fake-jwt-token');
    } else {
      setError(translate('사용자 이름 또는 비밀번호가 잘못되었습니다.', 'Invalid username or password.', language));
    }
  };
  
  return (
    <Box
      sx={{
        height: '100vh',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        background: 'linear-gradient(135deg, #0a1929 0%, #0d47a1 100%)',
      }}
    >
      <Paper
        elevation={3}
        sx={{
          p: 4,
          width: '100%',
          maxWidth: 400,
          backgroundColor: '#0a1929',
          border: '1px solid #1e3a5a',
          borderRadius: 2,
          position: 'relative'
        }}
      >
        <Box 
          component="img"
          src={rianLogo}
          alt="RIAN 로고"
          sx={{ 
            height: 20, 
            position: 'absolute',
            top: 15,
            left: 15
          }}
        />
        <Box sx={{ textAlign: 'center', mb: 3, mt: 1 }}>
          <Typography variant="h5" fontWeight="bold" color="primary" sx={{ mb: 0.5 }}>
            AIACS
          </Typography>
          <Typography variant="h6" color="text.secondary">
            {translate('조류충돌방지시스템', 'Bird Collision Prevention System', language)}
          </Typography>
        </Box>
        
        {error && (
          <Alert severity="error" sx={{ mb: 2 }}>
            {error}
          </Alert>
        )}
        
        <form onSubmit={handleSubmit}>
          <TextField
            fullWidth
            label={translate('사용자 이름', 'Username', language)}
            variant="outlined"
            margin="normal"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            sx={{ 
              mb: 2,
              '& .MuiOutlinedInput-root': {
                '& fieldset': {
                  borderColor: '#1e3a5a',
                },
                '&:hover fieldset': {
                  borderColor: '#2196f3',
                },
              }
            }}
          />
          
          <TextField
            fullWidth
            label={translate('비밀번호', 'Password', language)}
            type={showPassword ? 'text' : 'password'}
            variant="outlined"
            margin="normal"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            InputProps={{
              endAdornment: (
                <InputAdornment position="end">
                  <IconButton
                    onClick={() => setShowPassword(!showPassword)}
                    edge="end"
                  >
                    {showPassword ? <VisibilityOff /> : <Visibility />}
                  </IconButton>
                </InputAdornment>
              ),
            }}
            sx={{ 
              mb: 3,
              '& .MuiOutlinedInput-root': {
                '& fieldset': {
                  borderColor: '#1e3a5a',
                },
                '&:hover fieldset': {
                  borderColor: '#2196f3',
                },
              }
            }}
          />
          
          <Button
            type="submit"
            fullWidth
            variant="contained"
            color="primary"
            size="large"
            sx={{ mb: 2, py: 1.5 }}
          >
            {translate('로그인', 'Login', language)}
          </Button>
        </form>
      </Paper>
    </Box>
  );
};

export default Login; 