import sys
import os
import platform

print(f"Python 버전: {platform.python_version()}")
print(f"Python 경로: {sys.executable}")
print(f"운영체제: {platform.system()} {platform.release()}")

# CUDA 환경 변수 확인
cuda_path = os.environ.get('CUDA_PATH')
cuda_home = os.environ.get('CUDA_HOME')
path = os.environ.get('PATH')

print(f"\nCUDA_PATH: {cuda_path}")
print(f"CUDA_HOME: {cuda_home}")

# PATH에서 CUDA 관련 경로 찾기
if path:
    paths = path.split(os.pathsep)
    cuda_paths = [p for p in paths if 'cuda' in p.lower()]
    print("\nPATH에 포함된 CUDA 관련 경로:")
    for p in cuda_paths:
        print(f"  - {p}")

# PyTorch 확인
try:
    import torch
    print(f"\nPyTorch 버전: {torch.__version__}")
    print(f"CUDA 사용 가능: {torch.cuda.is_available()}")
    if torch.cuda.is_available():
        print(f"CUDA 버전: {torch.version.cuda}")
        print(f"GPU 수: {torch.cuda.device_count()}")
        for i in range(torch.cuda.device_count()):
            print(f"  - GPU {i}: {torch.cuda.get_device_name(i)}")
    else:
        print("PyTorch가 CUDA를 인식하지 못했습니다.")
except ImportError:
    print("\nPyTorch가 설치되어 있지 않습니다.")
except Exception as e:
    print(f"\nPyTorch 확인 중 오류 발생: {e}")

# 드라이버 파일 존재 확인
cuda_dll_paths = [
    "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v12.6\\bin\\cudart64_126.dll",
    "C:\\Windows\\System32\\cudart64_126.dll"
]

print("\nCUDA 드라이버 파일 확인:")
for path in cuda_dll_paths:
    print(f"  - {path}: {'있음' if os.path.exists(path) else '없음'}")

# 실행 가능한 파일 찾기
nvcc_path = None
try:
    import subprocess
    result = subprocess.run(['where', 'nvcc'], capture_output=True, text=True)
    if result.returncode == 0:
        nvcc_path = result.stdout.strip()
        print(f"\nnvcc 경로: {nvcc_path}")
    else:
        print("\nnvcc를 찾을 수 없습니다.")
except Exception as e:
    print(f"\nnvcc 확인 중 오류 발생: {e}")

# 설치된 패키지 목록
try:
    import subprocess
    result = subprocess.run([sys.executable, '-m', 'pip', 'list'], capture_output=True, text=True)
    if result.returncode == 0:
        print("\n설치된 패키지 목록:")
        print(result.stdout)
    else:
        print("\n패키지 목록을 가져올 수 없습니다.")
except Exception as e:
    print(f"\n패키지 목록 확인 중 오류 발생: {e}") 