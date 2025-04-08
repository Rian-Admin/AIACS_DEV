from django.shortcuts import render
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .rf_core_wrapper import RFCoreWrapper
from django.conf import settings
from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt
from .utils import get_rf_core
import json
import os
from datetime import datetime
import time
from rest_framework.decorators import api_view, permission_classes
from django.http import JsonResponse

# Create your views here.

class RadarConnectionView(APIView):
    """레이더 시스템 연결 관리 API"""
    
    def get(self, request):
        """현재 연결 상태를 조회합니다."""
        rf_core = RFCoreWrapper.get_instance()
        state_info = rf_core.get_state()
        return Response({
            'connected': state_info['connected'],
            'last_update': state_info['last_update']
        })
    
    def post(self, request):
        """레이더 시스템에 연결합니다."""
        ip = request.data.get('ip', '')
        port = request.data.get('port', 4102)
        
        if not ip:
            return Response({
                'success': False,
                'message': 'IP 주소가 필요합니다.'
            }, status=status.HTTP_400_BAD_REQUEST)
        
        rf_core = RFCoreWrapper.get_instance()
        success, message = rf_core.connect(ip, port)
        
        if success:
            return Response({
                'success': True,
                'message': message
            }, status=status.HTTP_200_OK)
        else:
            return Response({
                'success': False,
                'message': message
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
    
    def delete(self, request):
        """레이더 시스템과의 연결을 종료합니다."""
        rf_core = RFCoreWrapper.get_instance()
        success, message = rf_core.disconnect()
        
        if success:
            return Response({
                'success': True,
                'message': message
            }, status=status.HTTP_200_OK)
        else:
            return Response({
                'success': False,
                'message': message
            }, status=status.HTTP_400_BAD_REQUEST)

class RadarCommandView(APIView):
    """레이더 시스템 명령 API"""
    
    def get(self, request):
        """사용 가능한 명령어 목록을 조회합니다."""
        rf_core = RFCoreWrapper.get_instance()
        commands = rf_core.get_available_commands()
        return Response({'commands': commands})
    
    def post(self, request):
        """지정된 명령어를 전송합니다."""
        command_id = request.data.get('command_id')
        params = request.data.get('params', {})
        
        if not command_id:
            return Response({'error': '명령어 ID가 필요합니다.'}, status=status.HTTP_400_BAD_REQUEST)
        
        try:
            command_id = int(command_id)
        except ValueError:
            return Response({'error': '명령어 ID는 숫자여야 합니다.'}, status=status.HTTP_400_BAD_REQUEST)
        
        rf_core = RFCoreWrapper.get_instance()
        success, message = rf_core.send_command(command_id, params)
        
        if success:
            return Response({'message': message}, status=status.HTTP_200_OK)
        else:
            return Response({'error': message}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarStateView(APIView):
    """레이더 시스템 상태 API"""
    
    def get(self, request):
        """현재 레이더 시스템 상태를 조회합니다."""
        rf_core = RFCoreWrapper.get_instance()
        state_info = rf_core.get_state()
        return Response(state_info)
    
    def delete(self, request):
        """상태 정보를 초기화합니다."""
        rf_core = RFCoreWrapper.get_instance()
        success, message = rf_core.clear_state()
        
        if success:
            return Response({'message': message}, status=status.HTTP_200_OK)
        else:
            return Response({'error': message}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarMessageView(APIView):
    """레이더 메시지 조회 API - 4001 메시지와 기타 메시지 구분"""
    
    def get(self, request, message_type=None):
        """메시지 타입에 따른 데이터 조회
        message_type: '4001' 또는 'other'
        """
        rf_core = RFCoreWrapper.get_instance()
        state_info = rf_core.get_state()
        
        if message_type == '4001':
            return Response({
                'message': state_info['last_4001_state'],
                'last_update': state_info['last_update'],
                'has_new_data': True  # 항상 새 데이터가 있다고 처리
            })
        elif message_type == 'other':
            return Response({
                'message': state_info['last_other_state'],
                'last_update': state_info['last_update'],
                'has_new_data': state_info['has_new_data'] and not state_info['is_4001']
            })
        else:
            # 전체 상태 반환
            return Response({
                'is_4001': state_info['is_4001'],
                'has_new_data': state_info['has_new_data'],
                'last_4001_state': state_info['last_4001_state'],
                'last_other_state': state_info['last_other_state'],
                'last_update': state_info['last_update']
            })

class CommandDetailView(APIView):
    """명령어 상세 정보 API"""
    
    def get(self, request, command_id):
        """특정 명령어의 상세 정보를 조회합니다."""
        try:
            command_id = int(command_id)
        except ValueError:
            return Response({'error': '명령어 ID는 숫자여야 합니다.'}, status=status.HTTP_400_BAD_REQUEST)
        
        rf_core = RFCoreWrapper.get_instance()
        command_json = rf_core.get_command_details(command_id)
        
        if command_json:
            return Response({'command': command_json})
        else:
            return Response({'error': '해당 명령어를 찾을 수 없습니다.'}, status=status.HTTP_404_NOT_FOUND)

class RadarSimulationView(APIView):
    """
    에뮬레이터와의 시뮬레이션 시나리오 통신을 위한 API 뷰
    
    에뮬레이터는 T_Scenario.json 파일을 통해 시나리오를 로드합니다.
    시뮬레이션 요청 시 파일을 생성하고, 상태 요청 시 현재 실행 상태를 확인합니다.
    """
    
    def get(self, request, *args, **kwargs):
        """
        현재 시뮬레이션 상태를 반환합니다.
        """
        rf_core = get_rf_core()
        
        # 에뮬레이터 연결 상태 확인
        if not rf_core.is_connected():
            return Response({
                "status": "error", 
                "message": "에뮬레이터에 연결되어 있지 않습니다",
                "connected": False
            })
        
        # 플롯/트랙 정보 확인
        try:
            # 레이더 상태 정보 가져오기
            state_data = rf_core.get_state()
            
            # 플롯/트랙 데이터 처리
            plots = []
            tracks = []
            
            # 플롯 데이터 처리
            for plot_state in state_data.get('last_plot_states', []):
                if isinstance(plot_state, dict):
                    # 플롯 ID와 위치 정보 확인
                    plot_id = plot_state.get('plotId')
                    if plot_id is not None:
                        # 위치가 [0,0,0]이고 RCS가 0인 플롯만 제외
                        position = plot_state.get('position', [])
                        rcs = plot_state.get('rcs', 0)
                        
                        if position == [0, 0, 0] and rcs == 0:
                            print(f"플롯 필터링: ID={plot_id}, 위치={position}, RCS={rcs}")
                            continue
                            
                        plot_info = {
                            'id': plot_id,
                            'position': position,
                            'rcs': rcs,
                            'angle': plot_state.get('angle', 0),
                            'range': plot_state.get('range', 0),
                            'time': plot_state.get('header', {}).get('tv_sec', 0)
                        }
                        plots.append(plot_info)
            
            # 트랙 데이터 처리
            for track_state in state_data.get('last_track_states', []):
                if isinstance(track_state, dict):
                    # 트랙 ID와 위치 정보 확인
                    track_id = track_state.get('trackId')
                    if track_id is not None:
                        # 유효한 position 데이터가 있는지 확인
                        position = track_state.get('position', [])
                        if not position or len(position) < 2 or (position[0] == 0 and position[1] == 0 and position[2] == 0):
                            # position 정보가 없으면 Range/Azimuth에서 변환
                            if 'Range' in track_state and 'Azimuth' in track_state:
                                import math
                                range_val = float(track_state.get('Range', 0))
                                azimuth_val = float(track_state.get('Azimuth', 0))
                                elevation_val = float(track_state.get('Elevation', 0))
                                
                                # 직교좌표 계산 (방위각이 북쪽이 0도, 시계방향으로 증가하는 경우)
                                azimuth_rad = math.radians(azimuth_val)
                                elevation_rad = math.radians(elevation_val)
                                
                                # 북쪽(y+), 동쪽(x+) 좌표계 변환 
                                x = range_val * math.sin(azimuth_rad) * math.cos(elevation_rad)
                                y = range_val * math.cos(azimuth_rad) * math.cos(elevation_rad)
                                z = range_val * math.sin(elevation_rad)
                                
                                position = [x, y, z]
                        
                        # 유효한 velocity 데이터가 있는지 확인
                        velocity = track_state.get('velocity', [])
                        if not velocity or len(velocity) < 2:
                            # velocity 정보가 없으면 vEast/vNorth에서 변환
                            if 'vEast' in track_state and 'vNorth' in track_state:
                                velocity = [
                                    float(track_state.get('vEast', 0)),
                                    float(track_state.get('vNorth', 0)),
                                    float(track_state.get('vUp', 0))
                                ]
                            else:
                                velocity = [0.1, 0.1, 0]  # 기본값 설정
                        
                        track_info = {
                            'id': track_id,
                            'position': position,
                            'velocity': velocity,
                            'type': track_state.get('trackType', track_state.get('type', 0)),
                            'rcs': track_state.get('RCS', 0),
                            'time': track_state.get('tv_sec', track_state.get('header', {}).get('tv_sec', 0))
                        }
                        tracks.append(track_info)
            
            # 4001 메시지로부터 레이더 상태 정보 추출
            radar_status = {}
            last_4001 = state_data.get('last_4001_state')
            if isinstance(last_4001, dict) and 'Rdp' in last_4001:
                radar_status = {
                    'state': last_4001['Rdp'].get('State', 0),
                    'serial_no': last_4001['Rdp'].get('RdpSerialNo', 0),
                    'op_time': last_4001['Rdp'].get('OpTime', 0),
                    'scan': last_4001['Rdp'].get('Scan', 0),
                    'track_num': last_4001['Rdp'].get('TrackNum', 0),
                    'plot_num': last_4001['Rdp'].get('PlotNum', 0),
                    'panel_num': last_4001.get('RadarPanelNum', 0)
                }
            
            # 응답 구성
            simulation_status = {
                "status": "success",
                "connected": True,
                "operation_mode": "운용중" if rf_core.is_operating() else "대기중",
                "plots": plots,
                "plots_count": len(plots),
                "tracks": tracks,
                "tracks_count": len(tracks),
                "plot_track_enabled": rf_core.is_plot_track_enabled(),
                "radar_status": radar_status,
                "timestamp": int(time.time())
            }
            
            print(f"플롯 수: {len(plots)}, 트랙 수: {len(tracks)}")
            
            return Response(simulation_status)
            
        except Exception as e:
            print(f"시뮬레이션 상태 확인 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"상태 확인 중 오류 발생: {str(e)}",
                "connected": rf_core.is_connected()
            })
    
    def post(self, request, *args, **kwargs):
        """
        시뮬레이션 시나리오를 제출합니다.
        에뮬레이터는 T_Scenario.json 파일을 사용하여 시나리오를 로드합니다.
        
        여러 형식을 지원합니다:
        1. 배열 형식: 직접 시나리오 배열 (에뮬레이터 네이티브 형식)
        [
            {
                "Valid": 1,
                "Time": 0,
                "Type": "Linear",
                "RCS": 0.01,
                "Pd": 1.0, 
                "StartPos": [1000,2500,300],
                "StopPos": [-1000,2500,300],
                "Velocity": 10
            }, 
            ...
        ]
        
        2. 객체 형식: 템플릿 기반 시나리오 등
        {
            "scenario_type": "drone_swarm",
            "scenario_data": { ... },
            "duration": 300
        }
        
        3. 이전 형식 호환성: scenario 필드 내 배열
        {
            "scenario": [ ... ]
        }
        """
        try:
            print(f"수신된 시나리오 요청 타입: {type(request.data)}")
            print(f"수신된 시나리오 요청 내용: {json.dumps(request.data, indent=2)}")
            
            # 요청 데이터 가져오기
            request_data = request.data
            
            # 시나리오 데이터 형식 확인 및 표준화
            if isinstance(request_data, list):
                # 직접 배열 형식 - 에뮬레이터 네이티브 형식
                print("네이티브 배열 형식 시나리오 수신")
                scenario_data = request_data
            elif isinstance(request_data, dict):
                # 객체 형식 처리
                if "scenario" in request_data and isinstance(request_data["scenario"], list):
                    # 이전 형식 - scenario 필드 내 배열
                    print("scenario 필드 포함 객체 형식 수신")
                    scenario_data = request_data["scenario"]
                elif "scenario_type" in request_data and "scenario_data" in request_data:
                    # 템플릿 형식 - 그대로 전달
                    print(f"{request_data['scenario_type']} 템플릿 형식 수신")
                    scenario_data = request_data
                elif "targets" in request_data and isinstance(request_data["targets"], list):
                    # targets 배열이 있는 객체
                    print("targets 배열 포함 객체 형식 수신")
                    scenario_data = request_data["targets"]
                else:
                    # 그 외 객체 형식 - 그대로 전달
                    print("기타 객체 형식 수신")
                    scenario_data = request_data
            else:
                return Response(
                    {"status": "error", "message": "지원되지 않는 데이터 형식입니다"},
                    status=status.HTTP_400_BAD_REQUEST
                )
            
            # JSON 형식 출력
            print(f"처리된 시나리오 데이터: {json.dumps(scenario_data, indent=2)}")
            
            if not scenario_data:
                return Response(
                    {"status": "error", "message": "유효한 시나리오 데이터가 없습니다"},
                    status=status.HTTP_400_BAD_REQUEST
                )
            
            # RFCoreWrapper를 통해 시나리오 전송
            rf_core = get_rf_core()
            success, message = rf_core.send_simulation_scenario(scenario_data)
            
            if success:
                return Response({
                    "status": "success",
                    "message": message,
                    "scenario_received": True
                })
            else:
                return Response({
                    "status": "error",
                    "message": message,
                    "scenario_received": False
                }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
                
        except Exception as e:
            print(f"시뮬레이션 API 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"시뮬레이션 처리 중 오류 발생: {str(e)}",
                "scenario_received": False
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
    
    def delete(self, request, *args, **kwargs):
        """
        현재 실행 중인 시뮬레이션을 중지합니다.
        """
        rf_core = get_rf_core()
        success, message = rf_core.stop_simulation()
        
        if success:
            return Response({
                "status": "success",
                "message": message
            })
        else:
            return Response({
                "status": "error",
                "message": message
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarOperationView(APIView):
    """
    레이더 운용 모드를 설정하는 API 뷰
    """
    
    def post(self, request, *args, **kwargs):
        try:
            operation_mode = request.data.get('OperationMode')
            
            if operation_mode not in [1, 2]:  # 1: Standby, 2: Operation
                return Response(
                    {"status": "error", "message": "유효하지 않은 운용 모드입니다. 1(Standby) 또는 2(Operation)를 사용하세요."},
                    status=status.HTTP_400_BAD_REQUEST
                )
            
            rf_core = get_rf_core()
            success, message = rf_core.send_command(3002, {"OperationMode": operation_mode})
            
            if success:
                mode_text = "대기" if operation_mode == 1 else "운용"
                return Response({
                    "status": "success",
                    "message": f"레이더가 {mode_text} 모드로 설정되었습니다.",
                    "operation_mode": operation_mode
                })
            else:
                return Response({
                    "status": "error",
                    "message": f"운용 모드 설정 실패: {message}"
                }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
                
        except Exception as e:
            print(f"운용 모드 설정 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"운용 모드 설정 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarPlotTrackView(APIView):
    """
    레이더 플롯/트랙 정보 전송 설정을 위한 API 뷰
    """
    
    def post(self, request, *args, **kwargs):
        try:
            plot_send_flag = request.data.get('PlotSendFlag', 1)
            plot_type = request.data.get('PlotType', 3)
            track_send_flag = request.data.get('TrackSendFlag', 1)
            track_type = request.data.get('TrackType', 3)
            
            rf_core = get_rf_core()
            success, message = rf_core.send_command(3005, {
                "PlotSendFlag": plot_send_flag,
                "PlotType": plot_type,
                "TrackSendFlag": track_send_flag,
                "TrackType": track_type
            })
            
            if success:
                return Response({
                    "status": "success",
                    "message": "플롯/트랙 데이터 설정이 완료되었습니다.",
                    "settings": {
                        "plot_send_flag": plot_send_flag,
                        "plot_type": plot_type,
                        "track_send_flag": track_send_flag,
                        "track_type": track_type
                    }
                })
            else:
                return Response({
                    "status": "error",
                    "message": f"플롯/트랙 설정 실패: {message}"
                }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
                
        except Exception as e:
            print(f"플롯/트랙 설정 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"플롯/트랙 설정 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarPerformanceView(APIView):
    """
    레이더 성능 파라미터 설정을 위한 API 뷰
    """
    
    def get(self, request, *args, **kwargs):
        """현재 성능 파라미터 정보를 조회합니다."""
        try:
            rf_core = get_rf_core()
            success, message = rf_core.send_command(3011, {})  # 성능 파라미터 요청 명령
            
            return Response({
                "status": "success" if success else "error",
                "message": message,
                "performance_params": rf_core.get_state()
            })
                
        except Exception as e:
            print(f"성능 파라미터 조회 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"성능 파라미터 조회 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
    
    def post(self, request, *args, **kwargs):
        """성능 파라미터를 설정합니다."""
        try:
            params = request.data
            
            rf_core = get_rf_core()
            success, message = rf_core.send_command(3012, params)  # 성능 파라미터 설정 명령
            
            if success:
                return Response({
                    "status": "success",
                    "message": "성능 파라미터 설정이 완료되었습니다.",
                    "params": params
                })
            else:
                return Response({
                    "status": "error",
                    "message": f"성능 파라미터 설정 실패: {message}"
                }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
                
        except Exception as e:
            print(f"성능 파라미터 설정 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"성능 파라미터 설정 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarStatusView(APIView):
    """
    레이더 상태 정보 조회를 위한 API 뷰
    """
    
    def get(self, request, *args, **kwargs):
        """현재 레이더 상태 정보를 조회합니다."""
        try:
            rf_core = get_rf_core()
            state = rf_core.get_state()
            
            # 4001 상태 메시지를 추출하여 반환
            status_data = {}
            if isinstance(state, dict) and 'Rdp' in state:
                status_data = {
                    'operating': rf_core.is_operating(),
                    'rdp_state': state['Rdp'].get('State', 0),
                    'rdp_serial': state['Rdp'].get('RdpSerialNo', ''),
                    'connected': rf_core.is_connected()
                }
            
            return Response({
                "status": "success",
                "message": "레이더 상태 정보 조회 완료",
                "radar_status": status_data
            })
                
        except Exception as e:
            print(f"상태 정보 조회 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"상태 정보 조회 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

class RadarTrackView(APIView):
    """
    레이더 트랙 정보 관리를 위한 API 뷰
    """
    
    def get(self, request, *args, **kwargs):
        """현재 트랙 정보를 조회합니다."""
        try:
            rf_core = get_rf_core()
            state = rf_core.get_state()
            
            # 트랙 정보 추출
            tracks = []
            if isinstance(state, dict) and 'trackId' in state:
                tracks.append({
                    'id': state.get('trackId'),
                    'position': state.get('position', []),
                    'velocity': state.get('velocity', []),
                    'type': state.get('trackType', 0)
                })
            
            return Response({
                "status": "success",
                "message": "트랙 정보 조회 완료",
                "tracks": tracks
            })
                
        except Exception as e:
            print(f"트랙 정보 조회 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"트랙 정보 조회 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
    
    def post(self, request, *args, **kwargs):
        """트랙 설정 명령을 전송합니다."""
        try:
            track_id = request.data.get('trackId')
            track_command = request.data.get('command')
            
            if not track_id or not track_command:
                return Response({
                    "status": "error",
                    "message": "트랙 ID와 명령이 필요합니다"
                }, status=status.HTTP_400_BAD_REQUEST)
            
            rf_core = get_rf_core()
            
            # 트랙 명령 종류에 따라 적절한 API 호출
            if track_command == 'end':
                # 트랙 종료 명령 (3044)
                success, message = rf_core.send_command(3044, {"trackId": track_id})
            elif track_command == 'highrate':
                # 고속 갱신 설정 명령 (3042)
                success, message = rf_core.send_command(3042, {"trackId": track_id})
            elif track_command == 'identify':
                # 트랙 식별 명령 (3041)
                track_type = request.data.get('trackType', 3)  # 기본값: 드론
                success, message = rf_core.send_command(3041, {
                    "trackId": track_id,
                    "trackType": track_type
                })
            else:
                return Response({
                    "status": "error",
                    "message": f"지원되지 않는 트랙 명령: {track_command}"
                }, status=status.HTTP_400_BAD_REQUEST)
            
            if success:
                return Response({
                    "status": "success",
                    "message": f"트랙 명령 전송 완료: {track_command}",
                    "track_id": track_id
                })
            else:
                return Response({
                    "status": "error",
                    "message": f"트랙 명령 전송 실패: {message}"
                }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)
                
        except Exception as e:
            print(f"트랙 명령 전송 중 오류: {str(e)}")
            import traceback
            traceback.print_exc()
            
            return Response({
                "status": "error",
                "message": f"트랙 명령 전송 중 오류 발생: {str(e)}"
            }, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

# 트랙 종료 요청 API 엔드포인트 추가 
@api_view(['POST'])
@permission_classes([])
def track_delete_request(request):
    """특정 트랙의 추적을 종료하도록 요청합니다 (ID=3044)"""
    try:
        # 요청 데이터 파싱
        data = request.data
        track_id = data.get('track_id')
        
        # track_id 유효성 검사
        if track_id is None:
            return JsonResponse({'success': False, 'message': '트랙 ID가 제공되지 않았습니다'}, status=400)
        
        # RF Core 인스턴스 가져오기
        rf_core = RFCoreWrapper.get_instance()
        
        # 트랙 종료 명령 (3044) 전송
        success, message = rf_core.send_command(3044, {"trackId": track_id})
        
        if success:
            return JsonResponse({'success': True, 'message': f'트랙 {track_id} 종료 요청이 성공적으로 전송되었습니다'})
        else:
            return JsonResponse({'success': False, 'message': f'트랙 종료 요청 실패: {message}'}, status=500)
    
    except Exception as e:
        import traceback
        traceback.print_exc()
        return JsonResponse({'success': False, 'message': f'트랙 종료 요청 중 오류 발생: {str(e)}'}, status=500)

# 트랙 종료 정보 API 엔드포인트 추가 (4017 메시지 수신 시 사용)
@api_view(['GET'])
@permission_classes([])
def get_deleted_tracks(request):
    """레이더로부터 종료된 트랙 정보를 가져옵니다 (ID=4017)"""
    try:
        # RF Core 인스턴스 가져오기
        rf_core = RFCoreWrapper.get_instance()
        
        # 현재 상태 가져오기
        state_info = rf_core.get_state()
        
        # 4017 메시지 (트랙 종료 정보) 확인
        deleted_tracks = []
        
        if state_info and state_info.get('last_other_state'):
            last_state = state_info.get('last_other_state')
            
            # 4017 메시지인지 확인
            if last_state.get('header', {}).get('id') == 4017:
                # 트랙 종료 정보 추출
                n_track = last_state.get('nTrack', 0)
                tracks = last_state.get('track', [])
                
                if n_track > 0 and tracks:
                    deleted_tracks = [track.get('trackId') for track in tracks if 'trackId' in track]
        
        return JsonResponse({
            'success': True, 
            'deleted_tracks': deleted_tracks,
            'count': len(deleted_tracks)
        })
        
    except Exception as e:
        import traceback
        traceback.print_exc()
        return JsonResponse({'success': False, 'message': f'종료된 트랙 정보 조회 중 오류 발생: {str(e)}'}, status=500)
