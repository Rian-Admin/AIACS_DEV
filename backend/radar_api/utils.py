from .rf_core_wrapper import RFCoreWrapper

def get_rf_core():
    """
    RFCoreWrapper의 싱글톤 인스턴스를 반환합니다.
    이 함수는 모든 뷰에서 레이더 코어 인스턴스에 접근할 때 사용해야 합니다.
    
    Returns:
        RFCoreWrapper: RFCoreWrapper의 싱글톤 인스턴스
    """
    return RFCoreWrapper.get_instance() 