import socket
import json
import threading

class RFCoreConnector:
    def __init__(self, ip, port=4102):
        self.ip = ip
        self.port = port  # 포트 번호를 파라미터로 받음
        self.socket = None
        self.running = False
        self.state = None  # 수신 데이터 저장
        self.lock = threading.Lock()  # 데이터 동기화를 위한 Lock
        self.connect()
        self.start_receiving()

    def connect(self):
        """서버에 연결을 시도합니다."""
        if self.socket:
            self.disconnect()
        
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.socket.connect((self.ip, self.port))
            print(f"Connected to {self.ip}:{self.port}")
        except socket.error as e:
            print(f"Connection failed: {e}")
            self.socket = None

    def disconnect(self):
        """서버와의 연결을 종료합니다."""
        self.running = False
        if self.socket:
            self.socket.close()
            self.socket = None
            print("Disconnected")

    def send_command(self, id_value):
        """지정된 ID로 JSON 명령을 생성하여 전송합니다."""
        if not self.socket:
            print("Not connected to the server.")
            return
        
        command = {
            "header": {
                "id": id_value,
                "tv_sec":147804,
                "tv_usec":0
            }
        }
        
        try:
            json_data = json.dumps(command) + "\n"  # 개행 문자 추가 (서버에서 구분 가능하도록)
            self.socket.sendall(json_data.encode('utf-8'))
        except socket.error as e:
            print(f"Failed to send command: {e}")
            self.reconnect()

    def start_receiving(self):
        """서버로부터 데이터를 실시간으로 수신하는 스레드를 시작합니다."""
        self.running = True
        thread = threading.Thread(target=self.receive_data, daemon=True)
        thread.start()

    def receive_data(self):
        """서버로부터 데이터를 수신하여 동기화합니다."""
        while self.running:
            if self.socket:
                try:
                    data = self.socket.recv(1024)
                    if data:
                        parsed_data = json.loads(data.decode('utf-8'))
                        with self.lock:
                            self.state = parsed_data  # 데이터 동기화
                except (socket.error, json.JSONDecodeError) as e:
                    print(f"Receive error: {e}")
                    self.reconnect()

    def get_state(self):
        """현재 저장된 상태 데이터를 반환합니다."""
        with self.lock:
            return self.state
