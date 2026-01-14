#include "api.h"
#include <d3d11.h>
#include <dxgi1_2.h>
#include <windows.h>

typedef struct {
    HWND hwnd;
    IDXGISwapChain1* swap_chain;
    ID3D11RenderTargetView* rtv;
} PresentState;

static PresentState g_present = {0};

bool Present_Initialize(ID3D11Device* device, uint32_t width, uint32_t height) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = DefWindowProc;
    wc.lpszClassName = "NovaScaleOutput";
    RegisterClass(&wc);

    g_present.hwnd = CreateWindowEx(WS_EX_TOPMOST, "NovaScaleOutput", "NovaScale", WS_POPUP | WS_VISIBLE, 0, 0, width, height, NULL, NULL, NULL, NULL);

    IDXGIDevice* dxgi_device = NULL;
    device->lpVtbl->QueryInterface(device, &IID_IDXGIDevice, (void**)&dxgi_device);
    IDXGIAdapter* adapter = NULL;
    dxgi_device->lpVtbl->GetParent(dxgi_device, &IID_IDXGIAdapter, (void**)&adapter);
    IDXGIFactory2* factory = NULL;
    adapter->lpVtbl->GetParent(adapter, &IID_IDXGIFactory2, (void**)&factory);

    DXGI_SWAP_CHAIN_DESC1 sd = {0};
    sd.Width = width;
    sd.Height = height;
    sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    factory->lpVtbl->CreateSwapChainForHwnd(factory, (IUnknown*)device, g_present.hwnd, &sd, NULL, NULL, &g_present.swap_chain);
    
    ID3D11Texture2D* bb = NULL;
    g_present.swap_chain->lpVtbl->GetBuffer(g_present.swap_chain, 0, &IID_ID3D11Texture2D, (void**)&bb);
    device->lpVtbl->CreateRenderTargetView(device, (ID3D11Resource*)bb, NULL, &g_present.rtv);
    bb->lpVtbl->Release(bb);

    factory->lpVtbl->Release(factory);
    adapter->lpVtbl->Release(adapter);
    dxgi_device->lpVtbl->Release(dxgi_device);
    return true;
}

void Present_Cleanup() {
    if (g_present.rtv) g_present.rtv->lpVtbl->Release(g_present.rtv);
    if (g_present.swap_chain) g_present.swap_chain->lpVtbl->Release(g_present.swap_chain);
    if (g_present.hwnd) DestroyWindow(g_present.hwnd);
    g_present = (PresentState){0};
}
