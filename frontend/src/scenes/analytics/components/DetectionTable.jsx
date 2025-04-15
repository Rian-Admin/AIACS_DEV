import React, { useState } from 'react';
import { 
  Paper, 
  Typography, 
  Box,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Collapse,
  IconButton,
  Chip,
  CircularProgress,
  TablePagination
} from '@mui/material';
import KeyboardArrowDownIcon from '@mui/icons-material/KeyboardArrowDown';
import KeyboardArrowUpIcon from '@mui/icons-material/KeyboardArrowUp';
import axios from 'axios';

/**
 * ISO 형식 날짜 문자열을 한국어 형식으로 변환
 * @param {string} isoString - ISO 형식 날짜 문자열 (예: 2025-04-15T04:09:50.981099+00:00)
 * @returns {string} 한국어 형식 날짜 문자열 (예: 2025년 04월 15일 16:03:46)
 */
const formatDateTimeKorean = (isoString) => {
  if (!isoString) return '';
  
  try {
    const date = new Date(isoString);
    
    // 유효한 날짜인지 확인
    if (isNaN(date.getTime())) {
      return isoString;
    }
    
    // 년, 월, 일, 시, 분, 초 추출
    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');
    
    // 한국어 형식으로 포맷팅
    return `${year}년 ${month}월 ${day}일 ${hours}:${minutes}:${seconds}`;
  } catch (error) {
    console.error('날짜 변환 오류:', error);
    return isoString;
  }
};

/**
 * 바운딩 박스 세부 정보가 포함된 확장 가능한 행
 */
const ExpandableRow = ({ row }) => {
  const [open, setOpen] = useState(false);
  const [loading, setLoading] = useState(false);
  const [bbData, setBbData] = useState([]);

  // 바운딩 박스 데이터 로드
  const loadBoundingBoxes = async () => {
    if (open) {
      setOpen(false);
      return;
    }
    
    if (bbData.length > 0) {
      setOpen(true);
      return;
    }
    
    setLoading(true);
    try {
      const response = await axios.get(`/api/detection/bb-info/${row.id}/`);
      setBbData(response.data.bb_info || []);
      setOpen(true);
    } catch (error) {
      console.error('바운딩 박스 데이터 로드 오류:', error);
    } finally {
      setLoading(false);
    }
  };

  return (
    <>
      <TableRow sx={{ '&:hover': { bgcolor: 'rgba(30, 58, 138, 0.08)' } }}>
        <TableCell>
          <IconButton
            aria-label="expand row"
            size="small"
            onClick={loadBoundingBoxes}
            disabled={loading}
          >
            {loading ? (
              <CircularProgress size={20} />
            ) : open ? (
              <KeyboardArrowUpIcon />
            ) : (
              <KeyboardArrowDownIcon />
            )}
          </IconButton>
        </TableCell>
        <TableCell>{row.id}</TableCell>
        <TableCell>{formatDateTimeKorean(row.timestamp)}</TableCell>
        <TableCell>{row.species || '다양한 조류'}</TableCell>
        <TableCell align="center">
          <Chip
            label={row.objectCount}
            color={row.objectCount > 5 ? "error" : "primary"}
            size="small"
            variant={row.objectCount > 5 ? "default" : "outlined"}
          />
        </TableCell>
        <TableCell align="center">{row.camera_id}</TableCell>
      </TableRow>
      <TableRow>
        <TableCell style={{ paddingBottom: 0, paddingTop: 0 }} colSpan={6}>
          <Collapse in={open} timeout="auto" unmountOnExit>
            <Box sx={{ margin: 2 }}>
              <Typography variant="h6" gutterBottom component="div">
                바운딩 박스 상세 정보
              </Typography>
              
              {loading ? (
                <Box sx={{ display: 'flex', justifyContent: 'center', py: 2 }}>
                  <CircularProgress size={24} />
                </Box>
              ) : bbData.length > 0 ? (
                <TableContainer component={Paper} variant="outlined" sx={{ bgcolor: 'rgba(10, 25, 41, 0.8)' }}>
                  <Table size="small" aria-label="바운딩 박스 정보">
                    <TableHead>
                      <TableRow>
                        <TableCell>ID</TableCell>
                        <TableCell>조류 종류</TableCell>
                        <TableCell align="right">왼쪽(%)</TableCell>
                        <TableCell align="right">위(%)</TableCell>
                        <TableCell align="right">오른쪽(%)</TableCell>
                        <TableCell align="right">아래(%)</TableCell>
                        <TableCell align="right">너비(%)</TableCell>
                        <TableCell align="right">높이(%)</TableCell>
                        <TableCell align="right">면적(%²)</TableCell>
                      </TableRow>
                    </TableHead>
                    <TableBody>
                      {bbData.map((bb) => {
                        const width = bb.bb_right - bb.bb_left;
                        const height = bb.bb_bottom - bb.bb_top;
                        const area = width * height;
                        
                        return (
                          <TableRow key={bb.record_id} hover>
                            <TableCell>{bb.record_id}</TableCell>
                            <TableCell>{bb.class_name}</TableCell>
                            <TableCell align="right">{(bb.bb_left * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(bb.bb_top * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(bb.bb_right * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(bb.bb_bottom * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(width * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(height * 100).toFixed(1)}%</TableCell>
                            <TableCell align="right">{(area * 100).toFixed(1)}%²</TableCell>
                          </TableRow>
                        );
                      })}
                    </TableBody>
                  </Table>
                </TableContainer>
              ) : (
                <Typography variant="body2" color="text.secondary">
                  바운딩 박스 데이터가 없습니다.
                </Typography>
              )}
            </Box>
          </Collapse>
        </TableCell>
      </TableRow>
    </>
  );
};

/**
 * 최근 탐지 목록 테이블 컴포넌트
 * @param {Object} props
 * @param {Array} props.detections - 탐지 데이터 배열
 * @returns {JSX.Element}
 */
const DetectionTable = ({ detections }) => {
  const [page, setPage] = useState(0);
  const [rowsPerPage, setRowsPerPage] = useState(10);

  // 페이지 변경 핸들러
  const handleChangePage = (event, newPage) => {
    setPage(newPage);
  };

  // 페이지당 행 수 변경 핸들러
  const handleChangeRowsPerPage = (event) => {
    setRowsPerPage(parseInt(event.target.value, 10));
    setPage(0);
  };

  // 시간 기준 내림차순으로 정렬된 데이터 (최신 데이터가 먼저 표시)
  const sortedData = React.useMemo(() => {
    // 원본 배열을 변경하지 않기 위해 복사본 사용
    return [...detections].sort((a, b) => {
      // 시간 문자열 비교 (내림차순)
      return new Date(b.timestamp) - new Date(a.timestamp);
    });
  }, [detections]);
  
  // 현재 페이지 데이터
  const currentPageData = sortedData.slice(
    page * rowsPerPage,
    page * rowsPerPage + rowsPerPage
  );

  return (
    <Paper sx={{ 
      p: 2, 
      bgcolor: '#0a1929', 
      border: '1px solid #1e3a5a',
      height: '100%',
      display: 'flex',
      flexDirection: 'column'
    }}>
      <Typography variant="subtitle1" color="primary" sx={{ mb: 1 }}>
        최근 탐지 기록 ({detections.length}개)
      </Typography>
      
      <TableContainer sx={{ mb: 1, flex: 1, overflow: 'auto' }}>
        <Table size="small" stickyHeader>
          <TableHead>
            <TableRow sx={{ '& th': { fontWeight: 'bold', color: 'text.secondary' } }}>
              <TableCell width={50}></TableCell>
              <TableCell>ID</TableCell>
              <TableCell>시간</TableCell>
              <TableCell>종류</TableCell>
              <TableCell align="center">객체 수</TableCell>
              <TableCell align="center">카메라 ID</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {currentPageData.map((detection) => (
              <ExpandableRow key={detection.id} row={detection} />
            ))}
          </TableBody>
        </Table>
      </TableContainer>
      
      <TablePagination
        rowsPerPageOptions={[5, 10, 25]}
        component="div"
        count={detections.length}
        rowsPerPage={rowsPerPage}
        page={page}
        onPageChange={handleChangePage}
        onRowsPerPageChange={handleChangeRowsPerPage}
        labelRowsPerPage="페이지당 행 수:"
        labelDisplayedRows={({ from, to, count }) => `${from}-${to} / ${count}`}
      />
    </Paper>
  );
};

export default DetectionTable; 