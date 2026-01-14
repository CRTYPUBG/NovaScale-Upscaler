# NovaScale Performance Optimization Strategies

## 1. Frame Capture (Zero-Copy)
- **DirectX Interop**: Ensure frames never leave the GPU. `IDXGIOutputDuplication` provides a texture that can be accessed directly by D3D11.
- **Dirty Rectangles**: Use `DXGI_OUTDUPL_FRAME_INFO.TotalMetadataBufferSize` to only update regions of the screen that changed (useful for strategy games/emulators).

## 2. Compute Shader Optimization
- **Thread Group Tiling**: Use 8x8 or 16x16 thread groups to maximize occupancy.
- **Shared Memory (LDS)**: Store input pixel neighborhoods in Local Data Store (LDS) to reduce global memory bandwidth during RCAS/EASU sampling.
- **Instruction Balancing**: Use `mad` (multiply-add) and bitwise operations for edge detection to keep integer and float units busy.

## 3. Latency Management
- **Flip Model**: Use `DXGI_SWAP_EFFECT_FLIP_DISCARD` to allow the OS to bypass the compositor (Desktop Window Manager) when in fullscreen.
- **Waitable Swap Chain**: Use `IDXGISwapChain2::GetFrameLatencyWaitableObject` to sync the CPU thread exactly with the display's VBlank, minimizing the "back-pressure" lag.
- **Spin-Waiting**: For ultra-low latency, use a hybrid spin-wait instead of `Sleep(1)` when approaching the target frame time.

## 4. CNN Optimization (Pascal)
- **Kernel Fusion**: Combine activation layers (PReLU) into the convolution kernel to save memory round-trips.
- **Winograd Convolutions**: For 3x3 kernels, Winograd can significantly reduce the number of multiplications.
- **FP16 Storage**: Store weights and intermediate activations in FP16 to halve memory bandwidth requirements.

## 5. UI/Core Communication
- **Shared Memory Buffer**: Status and stats are written to a shared memory block that Python reads every 100ms, avoiding expensive context switches.
