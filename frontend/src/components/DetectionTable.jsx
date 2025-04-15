// 바운딩 박스 데이터 로드
const loadBoundingBoxes = async () => {
  if (bbData.length > 0) {
    setOpen(!open);
    return;
  }

  setLoading(true);
  try {
    // API 경로 수정 - 백엔드 실제 URL과 일치하도록 수정
    const response = await axios.get(`/api/detection/bb-info/${row.id}/`);
    setBbData(response.data.bb_info || []);
    setOpen(true);
  } catch (error) {
    console.error('바운딩 박스 데이터 로드 오류:', error);
    // 개발 환경에서는 더미 데이터 사용
    if (process.env.NODE_ENV === 'development') {
      console.log('백엔드 API 연결 실패, 임시 데이터 사용');
      const dummyBBData = Array(row.objectCount || 3).fill(0).map((_, idx) => ({
        record_id: idx + 1,
        class_name: '미상',
        bb_left: 0.1 + Math.random() * 0.3,
        bb_top: 0.1 + Math.random() * 0.3,
        bb_right: 0.6 + Math.random() * 0.3,
        bb_bottom: 0.6 + Math.random() * 0.3
      }));
      setBbData(dummyBBData);
      setOpen(true);
    }
  } finally {
    setLoading(false);
  }
}; 