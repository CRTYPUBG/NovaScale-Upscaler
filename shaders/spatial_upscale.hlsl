Texture2D<float4> InputTexture : register(t0);
RWTexture2D<float4> OutputTexture : register(u0);

struct Constants {
    float2 InputSize;
    float2 OutputSize;
    float Sharpness;
    float Padding;
};

ConstantBuffer<Constants> CB : register(b0);

float3 EASU(float2 uv) {
    float2 pos = uv * CB.InputSize - 0.5;
    float2 f = frac(pos);
    int2 i = int2(floor(pos));

    float3 c00 = InputTexture.Load(int3(i + int2(0,0), 0)).rgb;
    float3 c10 = InputTexture.Load(int3(i + int2(1,0), 0)).rgb;
    float3 c01 = InputTexture.Load(int3(i + int2(0,1), 0)).rgb;
    float3 c11 = InputTexture.Load(int3(i + int2(1,1), 0)).rgb;

    float dL = dot(c00, float3(0.299, 0.587, 0.114));
    float dR = dot(c11, float3(0.299, 0.587, 0.114));
    float grad = abs(dL - dR);

    float w = 1.0 / (1.0 + grad * 10.0);
    return lerp(c00, c11, f.x * w);
}

[numthreads(8, 8, 1)]
void CS_Main(uint3 dtid : SV_DispatchThreadID) {
    if (dtid.x >= (uint)CB.OutputSize.x || dtid.y >= (uint)CB.OutputSize.y) return;
    float2 uv = (float2(dtid.xy) + 0.5) / CB.OutputSize;
    OutputTexture[dtid.xy] = float4(EASU(uv), 1.0);
}
