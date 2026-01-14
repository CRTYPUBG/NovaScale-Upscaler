---
description: Build and Run NovaScale
---

# Build and Run Workflow

## Prerequisites
- Visual Studio 2022 (with C++ Desktop Development)
- CMake 3.16+
- Python 3.10+
- NVIDIA CUDA Toolkit (for CNN mode)

## 1. Build C Core Engine
// turbo
```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## 2. Setup Python Environment
```powershell
python -m venv venv
.\venv\Scripts\activate
pip install -r requirements.txt
```

## 3. Run Application
```powershell
# Ensure novascale.dll is in the same directory as main.py
python main.py
```
