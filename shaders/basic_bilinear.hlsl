Texture2D<float4> InputTexture : register(t0);
SamplerState LinearSampler : register(s0);
RWTexture2D<float4> OutputTexture : register(u0);

struct Constants {
    float2 InputSize;
    float2 OutputSize;
    float Sharpness;
    float Padding;
};

ConstantBuffer<Constants> CB : register(b0);

[numthreads(8, 8, 1)]
void CS_Main(uint3 dtid : SV_DispatchThreadID) {
    if (dtid.x >= (uint)CB.OutputSize.x || dtid.y >= (uint)CB.OutputSize.y) return;

    float2 uv = (float2(dtid.xy) + 0.5) / CB.OutputSize;
    
    // Simple Bilinear Sampling
    float4 color = InputTexture.SampleLevel(LinearSampler, uv, 0);
    
    OutputTexture[dtid.xy] = color;
}
