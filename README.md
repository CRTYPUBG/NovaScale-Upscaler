
# NovaScale![ana](https://github.com/user-attachments/assets/c37012a5-ffff-4e1f-a2c9-71edcb8c4c6c)


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
![ana](https://github.com/user-attachments/assets/d1867367-6e2f-4e05-9dcb-d687886f31ef)
<svg width="1200" height="350" viewBox="0 0 1200 350" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="g" x1="0%" y1="0%" x2="100%" y2="0%">
      <stop offset="0%" stop-color="#4FC3F7"/>
      <stop offset="100%" stop-color="#9C6BFF"/>
    </linearGradient>
    <filter id="glow">
      <feGaussianBlur stdDeviation="5"/>
    </filter>
  </defs>

  <rect width="100%" height="100%" fill="#070B1A"/>

  <!-- Icon -->
  <g transform="translate(90 110)">
    <g fill="url(#g)">
      <rect x="0" y="0" width="18" height="18" rx="3"/>
      <rect x="26" y="0" width="18" height="18" rx="3"/>
      <rect x="52" y="0" width="18" height="18" rx="3"/>
      <rect x="0" y="26" width="18" height="18" rx="3"/>
      <rect x="26" y="26" width="18" height="18" rx="3"/>
      <rect x="52" y="26" width="18" height="18" rx="3"/>
      <rect x="0" y="52" width="18" height="18" rx="3"/>
      <rect x="26" y="52" width="18" height="18" rx="3"/>
      <rect x="52" y="52" width="18" height="18" rx="3"/>
    </g>

    <polygon points="90,35 135,35 135,20 165,50 135,80 135,65 90,65"
             fill="url(#g)"/>

    <rect x="185" y="10" width="55" height="55" rx="8" fill="url(#g)"/>
    <rect x="250" y="10" width="55" height="55" rx="8" fill="url(#g)"/>
  </g>

  <!-- Text -->
  <text x="420" y="205"
        font-size="110"
        font-family="Segoe UI, Inter, Arial, sans-serif"
        font-weight="700"
        fill="url(#g)"
        filter="url(#glow)">
    NovaScale
  </text>
</svg>
