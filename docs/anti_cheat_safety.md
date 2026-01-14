# Anti-Cheat Safety Protocol

NovaScale is designed to be **Zero-Entry**. This means it never interacts with the internal memory or execution flow of a game process.

## 1. No Hooking
We do not use `SetWindowsHookEx`, VTable swapping, or internal function detouring. The game runs exactly as the developer intended.

## 2. No Injection
No DLLs are injected into the game's address space. NovaScale is a completely separate process.

## 3. No Memory Access
NovaScale does not call `ReadProcessMemory` or `WriteProcessMemory` on game handles. 

## 4. Desktop Capture (DXGI)
We use the **DXGI Desktop Duplication API**. This API is provided by Windows for utilities like:
- OBS Studio
- Discord Screen Sharing
- Remote Desktop
- Windows Game Bar

Because we use legitimate system APIs, NovaScale maintains a safety profile identical to common, whitelisted streaming software.

## 5. Independent Rendering
NovaScale renders its upscaled image to a separate, overlaying borderless fullscreen window. It does not tamper with the game's `SwapChain::Present` call.
