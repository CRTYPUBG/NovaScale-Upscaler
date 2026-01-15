#include "api.h"
#include <windows.h>
#include <d3d11.h>
#include <process.h>
#include <stdio.h>
#include <time.h>

void Engine_Log(const char* message) {
    FILE* f = fopen("novascale_engine.log", "a");
    if (f) {
        time_t now = time(NULL);
        char* t = ctime(&now);
        if (t) t[24] = '\0'; // Remove newline
        fprintf(f, "[%s] %s\n", t ? t : "Unknown", message);
        fclose(f);
    }
}

// Forward declarations for internal modules
bool Capture_Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
void Capture_Cleanup();
ID3D11Texture2D* Capture_AcquireFrame(uint32_t* width, uint32_t* height);
void Capture_ReleaseFrame();

bool Present_Initialize(ID3D11Device* device, uint32_t width, uint32_t height);
void Present_Cleanup();
void Present_Frame(ID3D11DeviceContext* context, ID3D11Texture2D* upscaled_texture);

bool Upscale_Basic_Init(ID3D11Device* device);
void Upscale_Basic_Process(ID3D11DeviceContext* context, ID3D11ShaderResourceView* input_srv, ID3D11UnorderedAccessView* output_uav, uint32_t in_w, uint32_t in_h, uint32_t out_w, uint32_t out_h);
void Upscale_Basic_Cleanup();

static struct {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    ID3D11Texture2D* upscale_texture;
    ID3D11UnorderedAccessView* upscale_uav;
    ID3D11ShaderResourceView* upscale_srv;
    Config config;
    Stats stats;
    bool is_running;
    HANDLE worker_thread;
    uint32_t out_w, out_h;
} g_engine = {0};

void Engine_Worker(void* arg) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    while (g_engine.is_running) {
        QueryPerformanceCounter(&start);

        uint32_t in_w, in_h;
        ID3D11Texture2D* frame = Capture_AcquireFrame(&in_w, &in_h);
        
        if (frame) {
            ID3D11ShaderResourceView* input_srv = NULL;
            g_engine.device->lpVtbl->CreateShaderResourceView(g_engine.device, (ID3D11Resource*)frame, NULL, &input_srv);

            // Processing (Alpha: Basic Bilinear)
            Upscale_Basic_Process(g_engine.context, input_srv, g_engine.upscale_uav, in_w, in_h, g_engine.out_w, g_engine.out_h);

            // Presentation
            Present_Frame(g_engine.context, g_engine.upscale_texture);

            if (input_srv) input_srv->lpVtbl->Release(input_srv);
            Capture_ReleaseFrame();
            
            QueryPerformanceCounter(&end);
            g_engine.stats.frame_time_ms = (float)(end.QuadPart - start.QuadPart) * 1000.0f / (float)freq.QuadPart;
            g_engine.stats.fps = (uint32_t)(1000.0f / g_engine.stats.frame_time_ms);
        } else {
            Sleep(1);
        }
    }
}

EXPORT bool NovaScale_Initialize() {
    Engine_Log("NovaScale_Initialize called");
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &g_engine.device, NULL, &g_engine.context);
    if (FAILED(hr)) {
        char buf[128];
        sprintf(buf, "D3D11CreateDevice failed: 0x%08X", (unsigned int)hr);
        Engine_Log(buf);
        return false;
    }

    if (!Capture_Initialize(g_engine.device, g_engine.context)) {
        Engine_Log("Capture_Initialize failed");
        return false;
    }
    
    if (!Upscale_Basic_Init(g_engine.device)) {
        Engine_Log("Upscale_Basic_Init failed");
        return false;
    }

    Engine_Log("NovaScale_Initialize succeeded");
    return true;
}

EXPORT bool NovaScale_Start(Config config) {
    Engine_Log("NovaScale_Start called");
    if (g_engine.is_running) return true;
    
    g_engine.config = config;
    g_engine.out_w = 1920; 
    g_engine.out_h = 1080;

    D3D11_TEXTURE2D_DESC td = {0};
    td.Width = g_engine.out_w;
    td.Height = g_engine.out_h;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    
    HRESULT hr = g_engine.device->lpVtbl->CreateTexture2D(g_engine.device, &td, NULL, &g_engine.upscale_texture);
    if (FAILED(hr)) {
        Engine_Log("CreateTexture2D failed");
        return false;
    }
    g_engine.device->lpVtbl->CreateUnorderedAccessView(g_engine.device, (ID3D11Resource*)g_engine.upscale_texture, NULL, &g_engine.upscale_uav);
    g_engine.device->lpVtbl->CreateShaderResourceView(g_engine.device, (ID3D11Resource*)g_engine.upscale_texture, NULL, &g_engine.upscale_srv);

    if (!Present_Initialize(g_engine.device, g_engine.out_w, g_engine.out_h)) {
        Engine_Log("Present_Initialize failed");
        return false;
    }

    g_engine.is_running = true;
    g_engine.worker_thread = (HANDLE)_beginthread(Engine_Worker, 0, NULL);
    Engine_Log("Engine worker thread started");
    return true;
}

EXPORT void NovaScale_Shutdown() {
    NovaScale_Stop();
    
    if (g_engine.worker_thread) {
        WaitForSingleObject(g_engine.worker_thread, 1000);
        g_engine.worker_thread = NULL;
    }

    Upscale_Basic_Cleanup();
    Capture_Cleanup();
    Present_Cleanup();

    if (g_engine.upscale_srv) g_engine.upscale_srv->lpVtbl->Release(g_engine.upscale_srv);
    if (g_engine.upscale_uav) g_engine.upscale_uav->lpVtbl->Release(g_engine.upscale_uav);
    if (g_engine.upscale_texture) g_engine.upscale_texture->lpVtbl->Release(g_engine.upscale_texture);
    
    if (g_engine.context) g_engine.context->lpVtbl->Release(g_engine.context);
    if (g_engine.device) g_engine.device->lpVtbl->Release(g_engine.device);
    
    memset(&g_engine, 0, sizeof(g_engine));
}

EXPORT void NovaScale_Stop() {
    Engine_Log("NovaScale_Stop called");
    g_engine.is_running = false;
}

EXPORT void NovaScale_UpdateConfig(Config config) {
    g_engine.config = config;
}

EXPORT Stats NovaScale_GetStats() {
    return g_engine.stats;
}
