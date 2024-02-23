Texture2D tex : register(t0);

sampler smp : register(s0);

cbuffer Transform : register(b0)
{
    matrix model;
};

cbuffer Camera : register(b1)
{
    matrix view;
    matrix proj;
};

cbuffer DrawMode : register(b2)
{
    // 0: ゲームオブジェクトとして描画
    // 1: UIとして描画
    uint drawMode;
};

cbuffer Material : register(b3)
{
    float4 color;
};

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    uint instanceID : SV_InstanceID;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    
    // UIとして描画する場合は、ビュー行列を適用しない
    matrix viewProj = (drawMode == 1) ? proj : mul(proj, view);
    matrix modelViewProj = mul(viewProj, model);
    
    output.position = mul(modelViewProj, float4(input.position, 1.f));
    output.uv = input.uv;
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target0
{
    float4 texColor = tex.Sample(smp, input.uv);
    texColor *= color;
    return texColor;
}