#include "api.h"
#include <windows.h>
#include <d3d11.h>
#include <process.h>

bool Capture_Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
void Capture_Cleanup();
bool Present_Initialize(ID3D11Device* device, uint32_t width, uint32_t height);
void Present_Cleanup();

static struct {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    bool is_running;
    Config config;
} g_engine = {0};

void Engine_Worker(void* arg) {
    while (g_engine.is_running) {
        Sleep(1);
    }
}

EXPORT bool NovaScale_Initialize() {
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &g_engine.device, NULL, &g_engine.context);
    if (FAILED(hr)) return false;
    return Capture_Initialize(g_engine.device, g_engine.context);
}

EXPORT void NovaScale_Shutdown() {
    NovaScale_Stop();
    Capture_Cleanup();
    Present_Cleanup();
    if (g_engine.context) g_engine.context->lpVtbl->Release(g_engine.context);
    if (g_engine.device) g_engine.device->lpVtbl->Release(g_engine.device);
}

EXPORT bool NovaScale_Start(Config config) {
    g_engine.config = config;
    g_engine.is_running = true;
    Present_Initialize(g_engine.device, 1920, 1080);
    _beginthread(Engine_Worker, 0, NULL);
    return true;
}

EXPORT void NovaScale_Stop() {
    g_engine.is_running = false;
}

EXPORT void NovaScale_UpdateConfig(Config config) {
    g_engine.config = config;
}

EXPORT Stats NovaScale_GetStats() {
    Stats s = {0};
    return s;
}
