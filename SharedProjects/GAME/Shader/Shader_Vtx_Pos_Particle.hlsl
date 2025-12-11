#include "Shader_Defines.hlsli"

matrix g_WorldMatrix;
matrix g_ViewMatrix; 
matrix g_ProjMatrix; 
float4 g_CamPosition;

Texture2D g_Texture         : register(t0);

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    
    row_major float4x4 TransformationMatrix : WORLD;
    
    float2 vLifeTime : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float4 vTransPosition = mul(float4(In.vPosition, 1.f), In.TransformationMatrix);
    vector vPosition = mul(vTransPosition, g_WorldMatrix);
    
    Out.vPosition = vPosition;
    Out.vPSize = float2(length(In.TransformationMatrix._11_12_13), length(In.TransformationMatrix._21_22_23));;
    Out.vLifeTime = In.vLifeTime;
    
    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD1;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
    
    float3 vLook = normalize(g_CamPosition - In[0].vPosition).xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x;
    float3 vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y;

    float4x4 vpMat = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out[0].vPosition = mul(float4((In[0].vPosition.xyz + vRight + vUp), 1.f), vpMat);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    Out[1].vPosition = mul(float4((In[0].vPosition.xyz - vRight + vUp), 1.f), vpMat);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    
    Out[2].vPosition = mul(float4((In[0].vPosition.xyz - vRight - vUp), 1.f), vpMat);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    
    Out[3].vPosition = mul(float4((In[0].vPosition.xyz + vRight - vUp), 1.f), vpMat);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;

    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* 텍스쳐 색상을 그대로 사용, A 값이 낮으면 discard */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    if (TextureColor.a < 0.3f)
        discard;
  
    TextureColor.a = saturate(In.vLifeTime.y - In.vLifeTime.x);
    
    TextureColor.rgb = TextureColor.rgb + (1.f - TextureColor.a);
    Out.vColor = TextureColor;
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}