import React from 'react';
import { 
  Paper, 
  Typography, 
  Box,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow
} from '@mui/material';

// 신뢰도 레벨에 따른 색상 반환
const getConfidenceColor = (confidence) => {
  if (confidence >= 90) return '#4caf50';
  if (confidence >= 75) return '#8bc34a';
  if (confidence >= 60) return '#ffeb3b';
  if (confidence >= 45) return '#ff9800';
  return '#f44336';
};

/**
 * 최근 탐지 목록 테이블 컴포넌트
 * @param {Object} props
 * @param {Array} props.detections - 탐지 데이터 배열
 * @returns {JSX.Element}
 */
const DetectionTable = ({ detections }) => {
  return (
    <Paper sx={{ p: 2, mb: 2, bgcolor: '#0a1929', border: '1px solid #1e3a5a' }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 2 }}>
        최근 탐지 기록
      </Typography>
      <TableContainer>
        <Table size="small">
          <TableHead>
            <TableRow sx={{ '& th': { fontWeight: 'bold', color: 'text.secondary' } }}>
              <TableCell>ID</TableCell>
              <TableCell>시간</TableCell>
              <TableCell>종류</TableCell>
              <TableCell>신뢰도</TableCell>
              <TableCell>거리(m)</TableCell>
              <TableCell>객체 수</TableCell>
              <TableCell>위험도</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {detections.map((detection) => (
              <TableRow key={detection.id} sx={{ '&:hover': { bgcolor: 'rgba(30, 58, 138, 0.08)' } }}>
                <TableCell>{detection.id}</TableCell>
                <TableCell>{detection.timestamp}</TableCell>
                <TableCell>{detection.species}</TableCell>
                <TableCell>
                  <Box sx={{ display: 'flex', alignItems: 'center' }}>
                    <Box
                      sx={{
                        width: 40,
                        height: 8,
                        borderRadius: 4,
                        bgcolor: getConfidenceColor(detection.confidence),
                        mr: 1
                      }}
                    />
                    {detection.confidence}%
                  </Box>
                </TableCell>
                <TableCell>{detection.distance}m</TableCell>
                <TableCell>{detection.objectCount}</TableCell>
                <TableCell>
                  <Box 
                    component="span" 
                    sx={{ 
                      px: 1, 
                      py: 0.5, 
                      borderRadius: 1, 
                      fontSize: '0.8rem',
                      bgcolor: detection.riskLevel === '고위험' ? 'rgba(244, 67, 54, 0.1)' : 'rgba(76, 175, 80, 0.1)',
                      color: detection.riskLevel === '고위험' ? '#f44336' : '#4caf50',
                      border: `1px solid ${detection.riskLevel === '고위험' ? '#f44336' : '#4caf50'}`
                    }}
                  >
                    {detection.riskLevel}
                  </Box>
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </TableContainer>
    </Paper>
  );
};

export default DetectionTable; 