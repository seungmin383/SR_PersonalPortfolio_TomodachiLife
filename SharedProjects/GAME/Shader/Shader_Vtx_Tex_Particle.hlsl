#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix;
matrix g_ViewMatrix; 
matrix g_ProjMatrix; 

Texture2D g_Texture         : register(t0);
Texture2D g_UpperTexture    : register(t1);
Texture2D g_IconTexture     : register(t2);

float4 g_FixColor;
float g_Brightness;
float g_Pow;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    
    float4 vRight : TEXCOORD1;
    float4 vUp : TEXCOORD2;
    float4 vLook : TEXCOORD3;
    float4 vTranslation : TEXCOORD4;
    
    float2 vLifeTime : TEXCOORD5;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float4x4 TransformationMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    
    float4 vTransPosition = mul(float4(In.vPosition, 1.f), TransformationMatrix);
    float4x4 wvpMatrix = mul(mul(g_WorldMatrix, g_ViewMatrix), g_ProjMatrix);
    
    vector vPosition = mul(vTransPosition, wvpMatrix);
    
    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vLifeTime = In.vLifeTime;
    
    return Out;
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
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}