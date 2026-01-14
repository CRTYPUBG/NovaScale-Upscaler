# NovaScale CNN Architektur Proposal (Experimental "Ultra" Mode)

## 1. Goal
Provide a spatial reconstruction that outperforms standard EASU by using learned features, while maintaining Pascal (GTX 10xx) compatibility and sub-8ms latency.

## 2. Architecture: NovaSR-Tiny
A lightweight FSR-CNN inspired architecture (4-6 layers).

| Layer | Type | Configuration | Activation | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| 1 | Cov2D | 3x3, 1 -> 16 channels | PReLU | Feature Extraction |
| 2-4 | Conv2D | 3x3, 16 -> 16 channels | PReLU | Residual blocks / Shrinking |
| 5 | Conv2D | 3x3, 16 -> 4 channels | - | Mapping to pixel shuffle |
| 6 | PixelShuffle| 2x Upscale | - | Reconstruction |

## 3. Pascal Optimization
- **Data Type**: Use FP16 (Half Precision) on Pascal. While Pascal lacks Tensor Cores, it supports FP16 arithmetic (at 1/64 or 2x rate depending on specific instruction/model, usually 1:1 on GTX 1080 Ti for some ops).
- **DP4a**: Use `__dp4a` (Dot Product and Accumulate) for INT8 quantization. This instruction is available on Pascal (SM 6.1) and is highly effective for accelerating convolutions without dedicated hardware.
- **Tiling**: Process the 1080p output in 256x256 tiles to keep memory usage low and cache hits high.

## 4. Training Strategy
- **Dataset**: DIV2K or Flickr2K datasets.
- **Degradation**: Bicubic downsampling + Gaussian Noise + Slight Compression artifacts.
- **Loss**: Charbonnier Loss + MS-SSIM to ensure perceptual quality over raw PSNR.

## 5. Integration
- The model is exported to **ONNX** or a raw weights file.
- The `upscale_cnn.c` module uses a custom CUDA kernel or **TensorRT** (optional) to run the inference.
- GPU textures are shared using `cudaGraphicsD3D11RegisterResource`.
