Texture2D<float4> InputTexture : register(t0);
RWTexture2D<float4> OutputTexture : register(u0);

struct Constants {
    float2 InputSize;
    float2 OutputSize;
    float Sharpness;
    float Padding;
};

ConstantBuffer<Constants> CB : register(b0);

float3 RCAS(int2 pos, float3 color) {
    float3 m = InputTexture.Load(int3(pos + int2(0,-1), 0)).rgb;
    float3 l = InputTexture.Load(int3(pos + int2(-1,0), 0)).rgb;
    float3 r = InputTexture.Load(int3(pos + int2(1,0), 0)).rgb;
    float3 b = InputTexture.Load(int3(pos + int2(0,1), 0)).rgb;

    float3 min_c = min(color, min(m, min(l, min(r, b))));
    float3 max_c = max(color, max(m, max(l, max(r, b))));

    float3 contrast = max_c - min_c;
    float sharp = CB.Sharpness * (1.0 / (max_c.g + 0.0001));
    
    return color + (color - (m+l+r+b)*0.25) * sharp;
}

[numthreads(8, 8, 1)]
void CS_Main(uint3 dtid : SV_DispatchThreadID) {
    if (dtid.x >= (uint)CB.OutputSize.x || dtid.y >= (uint)CB.OutputSize.y) return;
    float3 color = InputTexture.Load(int3(dtid.xy, 0)).rgb;
    OutputTexture[dtid.xy] = float4(RCAS(dtid.xy, color), 1.0);
}
