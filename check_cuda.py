import sys
import torch
import os
import platform

def check_cuda():
    print(f"Python version: {platform.python_version()}")
    print(f"Python executable: {sys.executable}")
    print(f"PyTorch installation path: {torch.__file__}")
    print(f"PyTorch version: {torch.__version__}")
    
    # Check if CUDA is in the PyTorch version string
    if '+cu' not in torch.__version__ and '+cuda' not in torch.__version__:
        print("WARNING: You appear to be using a CPU-only PyTorch build!")
    
    # Environment variables
    print(f"CUDA_HOME environment variable: {os.environ.get('CUDA_HOME', 'Not set')}")
    print(f"CUDA_PATH environment variable: {os.environ.get('CUDA_PATH', 'Not set')}")
    print(f"PATH environment variable contains CUDA: {'CUDA' in os.environ.get('PATH', '')}")
    
    # 모든 환경 변수 출력
    print("\n환경 변수 목록:")
    for key, value in os.environ.items():
        if 'CUDA' in key or 'GPU' in key:
            print(f"{key}: {value}")
    
    # CUDA 라이브러리 경로 확인
    print("\nCUDA 라이브러리 경로 확인:")
    cuda_paths = [path for path in os.environ.get('PATH', '').split(os.pathsep) if 'CUDA' in path]
    for path in cuda_paths:
        print(f"- {path}")
    
    # CUDA availability
    cuda_available = torch.cuda.is_available()
    print(f"\nCUDA available according to PyTorch: {cuda_available}")
    
    if cuda_available:
        # Only run these if CUDA is available to avoid errors
        device_count = torch.cuda.device_count()
        print(f"Number of CUDA devices: {device_count}")
        
        if device_count > 0:
            for i in range(device_count):
                print(f"CUDA Device {i}: {torch.cuda.get_device_name(i)}")
                print(f"CUDA Device {i} capability: {torch.cuda.get_device_capability(i)}")
        
        print(f"Current CUDA device: {torch.cuda.current_device()}")
    else:
        print("Cannot query CUDA devices as CUDA is not available in this PyTorch build.")
    
    # Try to query CUDA version
    try:
        cuda_version = torch.version.cuda
        print(f"CUDA version according to PyTorch: {cuda_version}")
    except AttributeError:
        print("Could not determine CUDA version from PyTorch.")

if __name__ == "__main__":
    try:
        check_cuda()
    except Exception as e:
        print(f"Error during CUDA check: {e}")
        import traceback
        traceback.print_exc()