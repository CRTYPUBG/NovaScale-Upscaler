![ana](https://github.com/user-attachments/assets/c37012a5-ffff-4e1f-a2c9-71edcb8c4c6c)

# NovaScale

Professional-grade real-time upscaling application for Windows.

## Features
- **Anti-Cheat Safe**: No DLL injection or API hooking.
- **High Performance**: Optimized C core with DirectX 11.
- **Custom Upscaling**: FSR-inspired spatial upscaling.
- **Experimental CNN**: Ultra-quality mode for Pascal GPUs and newer.

## Structure
- `core/`: C engine for capture and upscaling.
- `shaders/`: HLSL compute shaders.
- `ui/`: PySide6 modern user interface.
- `cnn/`: CNN model training and export scripts.
- `docs/`: Technical documentation.

## License
MIT License

Copyright (c) 2026 NovaScale Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
