#include "api.h"
#include <d3d11.h>
#include <d3dcompiler.h>

typedef struct {
    float InputWidth;
    float InputHeight;
    float OutputWidth;
    float OutputHeight;
    float Sharpness;
    float Reserved[3];
} UpscaleConstants;

static ID3D11ComputeShader* g_basic_cs = NULL;
static ID3D11SamplerState* g_linear_sampler = NULL;
static ID3D11Buffer* g_constant_buffer = NULL;

bool Upscale_Basic_Init(ID3D11Device* device) {
    ID3DBlob* shader_blob = NULL;
    ID3DBlob* error_blob = NULL;

    HRESULT hr = D3DCompileFromFile(L"shaders/basic_bilinear.hlsl", NULL, NULL, "CS_Main", "cs_5_0", 0, 0, &shader_blob, &error_blob);
    if (FAILED(hr)) return false;
    
    device->lpVtbl->CreateComputeShader(device, shader_blob->lpVtbl->GetBufferPointer(shader_blob), shader_blob->lpVtbl->GetBufferSize(shader_blob), NULL, &g_basic_cs);
    shader_blob->lpVtbl->Release(shader_blob);

    // Sampler State
    D3D11_SAMPLER_DESC sd = {0};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    device->lpVtbl->CreateSamplerState(device, &sd, &g_linear_sampler);

    // Constant Buffer
    D3D11_BUFFER_DESC bd = {0};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(UpscaleConstants);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->lpVtbl->CreateBuffer(device, &bd, NULL, &g_constant_buffer);

    return true;
}

void Upscale_Basic_Process(ID3D11DeviceContext* context, 
                          ID3D11ShaderResourceView* input_srv, 
                          ID3D11UnorderedAccessView* output_uav,
                          uint32_t in_w, uint32_t in_h,
                          uint32_t out_w, uint32_t out_h) {
    
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (SUCCEEDED(context->lpVtbl->Map(context, (ID3D11Resource*)g_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
        UpscaleConstants* data = (UpscaleConstants*)mapped.pData;
        data->InputWidth = (float)in_w;
        data->InputHeight = (float)in_h;
        data->OutputWidth = (float)out_w;
        data->OutputHeight = (float)out_h;
        context->lpVtbl->Unmap(context, (ID3D11Resource*)g_constant_buffer, 0);
    }

    context->lpVtbl->CSSetConstantBuffers(context, 0, 1, &g_constant_buffer);
    context->lpVtbl->CSSetSamplers(context, 0, 1, &g_linear_sampler);
    context->lpVtbl->CSSetShader(context, g_basic_cs, NULL, 0);
    context->lpVtbl->CSSetShaderResources(context, 0, 1, &input_srv);
    context->lpVtbl->CSSetUnorderedAccessViews(context, 0, 1, &output_uav, NULL);
    
    context->lpVtbl->Dispatch(context, (out_w + 7) / 8, (out_h + 7) / 8, 1);

    ID3D11UnorderedAccessView* null_uav = NULL;
    context->lpVtbl->CSSetUnorderedAccessViews(context, 0, 1, &null_uav, NULL);
}

void Upscale_Basic_Cleanup() {
    if (g_basic_cs) g_basic_cs->lpVtbl->Release(g_basic_cs);
    if (g_linear_sampler) g_linear_sampler->lpVtbl->Release(g_linear_sampler);
    if (g_constant_buffer) g_constant_buffer->lpVtbl->Release(g_constant_buffer);
}
