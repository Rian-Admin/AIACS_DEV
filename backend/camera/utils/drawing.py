import cv2
import numpy as np
import logging

# 로그 비활성화
logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())
logger.propagate = False

def draw_detections(frame, results, model_names):
    """감지된 객체에 대한 바운딩 박스를 그립니다."""
    if frame is None:
        logger.warning("프레임이 None입니다")
        return frame
        
    if results is None:
        logger.debug("결과가 None입니다")
        return frame
    
    # 결과에 boxes 속성이 있는지 확인
    if not hasattr(results, 'boxes') or results.boxes is None:
        logger.debug("boxes 속성이 없거나 None입니다")
        return frame
    
    # 박스 개수 확인
    try:
        box_count = len(results.boxes)
        if box_count == 0:
            logger.debug("감지된 객체가 없습니다")
            return frame
    except:
        logger.warning("boxes 객체가 iterable이 아닙니다")
        return frame
    
    # 프레임 크기
    frame_height, frame_width = frame.shape[:2]
    
    # 클래스별 색상
    colors = {}
    # 각 박스에 대해 처리
    for i, box in enumerate(results.boxes):
        try:
            # xyxy 속성 확인
            if not hasattr(box, 'xyxy') or box.xyxy is None or len(box.xyxy) == 0:
                logger.debug(f"박스 {i}에 xyxy 속성이 없습니다")
                continue
            
            # 좌표 가져오기 (CPU로 이동)
            xyxy = box.xyxy[0].cpu().numpy()
            x1, y1, x2, y2 = map(int, xyxy)
            
            # 클래스 정보 가져오기
            cls_id = 0
            if hasattr(box, 'cls') and box.cls is not None and len(box.cls) > 0:
                cls_id = int(box.cls[0].cpu().numpy())
            
            # 클래스 이름 가져오기
            class_name = "알 수 없음"
            if model_names and cls_id in model_names:
                class_name = model_names[cls_id]
            
            # 신뢰도 가져오기
            conf = 0.0
            if hasattr(box, 'conf') and box.conf is not None and len(box.conf) > 0:
                conf = float(box.conf[0].cpu().numpy())
            
            # 색상 선택
            color = colors.get(cls_id, (0, 0, 0))
            
            # 바운딩 박스 그리기
            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 1)
            
            # 클래스 이름과 신뢰도를 분리
            class_name_text = f"{class_name}"
            confidence_text = f"({int(conf*100)}%)"  # 소수점 없이 퍼센트로 변환

            # 텍스트 크기 계산
            (name_width, name_height), _ = cv2.getTextSize(class_name_text, cv2.FONT_HERSHEY_COMPLEX, 0.4, 1)
            (conf_width, conf_height), _ = cv2.getTextSize(confidence_text, cv2.FONT_HERSHEY_COMPLEX, 0.4, 1)

            # 텍스트 위치 설정 - 박스 아래에 표시
            name_x = x1  # 바운딩 박스의 왼쪽 경계
            name_y = y2 + name_height + 5  # 바운딩 박스 아래쪽
            conf_x = name_x + name_width + 5  # 클래스 이름 바로 오른쪽
            conf_y = name_y  # 클래스 이름과 같은 높이

            # 텍스트 그리기
            cv2.putText(frame, class_name_text, (name_x, name_y), cv2.FONT_HERSHEY_COMPLEX, 0.4, (0, 0, 0), 1)
            cv2.putText(frame, confidence_text, (conf_x, conf_y), cv2.FONT_HERSHEY_COMPLEX, 0.4, (0, 0, 0), 1)
            
            logger.debug(f"박스 {i}: {class_name}, 신뢰도={conf:.2f}, 좌표=({x1},{y1},{x2},{y2})")
            
        except Exception as e:
            logger.error(f"박스 {i} 처리 중 오류: {e}")
    
    return frame
