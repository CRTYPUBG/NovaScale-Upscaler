#include "api.h"
#include <d3d11.h>
#include <dxgi1_2.h>
#include <stdio.h>

typedef struct {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGIOutputDuplication* duplication;
    DXGI_OUTPUT_DESC output_desc;
} CaptureState;

static CaptureState g_capture = {0};

bool Capture_Initialize(ID3D11Device* device, ID3D11DeviceContext* context) {
    g_capture.device = device;
    g_capture.context = context;

    IDXGIDevice* dxgi_device = NULL;
    device->lpVtbl->QueryInterface(device, &IID_IDXGIDevice, (void**)&dxgi_device);

    IDXGIAdapter* adapter = NULL;
    dxgi_device->lpVtbl->GetParent(dxgi_device, &IID_IDXGIAdapter, (void**)&adapter);

    IDXGIOutput* output = NULL;
    adapter->lpVtbl->EnumOutputs(adapter, 0, &output);

    IDXGIOutput1* output1 = NULL;
    output->lpVtbl->QueryInterface(output, &IID_IDXGIOutput1, (void**)&output1);

    HRESULT hr = output1->lpVtbl->DuplicateOutput(output1, (IUnknown*)device, &g_capture.duplication);
    
    output1->lpVtbl->GetDesc(output1, &g_capture.output_desc);
    output1->lpVtbl->Release(output1);
    output->lpVtbl->Release(output);
    adapter->lpVtbl->Release(adapter);
    dxgi_device->lpVtbl->Release(dxgi_device);

    return SUCCEEDED(hr);
}

ID3D11Texture2D* Capture_AcquireFrame(uint32_t* width, uint32_t* height) {
    if (!g_capture.duplication) return NULL;

    IDXGIResource* resource = NULL;
    DXGI_OUTDUPL_FRAME_INFO frame_info;
    
    HRESULT hr = g_capture.duplication->lpVtbl->AcquireNextFrame(g_capture.duplication, 10, &frame_info, &resource);
    if (FAILED(hr)) return NULL;

    ID3D11Texture2D* frame = NULL;
    resource->lpVtbl->QueryInterface(resource, &IID_ID3D11Texture2D, (void**)&frame);
    
    D3D11_TEXTURE2D_DESC desc;
    frame->lpVtbl->GetDesc(frame, &desc);
    *width = desc.Width;
    *height = desc.Height;

    resource->lpVtbl->Release(resource);
    return frame;
}

void Capture_ReleaseFrame() {
    if (g_capture.duplication) g_capture.duplication->lpVtbl->ReleaseFrame(g_capture.duplication);
}

void Capture_Cleanup() {
    if (g_capture.duplication) {
        g_capture.duplication->lpVtbl->Release(g_capture.duplication);
        g_capture.duplication = NULL;
    }
}
