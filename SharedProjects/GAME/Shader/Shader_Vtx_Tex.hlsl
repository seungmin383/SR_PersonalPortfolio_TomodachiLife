#include "Shader_Defines.hlsli"

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
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
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

    if (TextureColor.a < 0.1f)
        discard;
  
    Out.vColor = TextureColor;
    return Out;
}

/* 텍스쳐와 그 위에 올라올 텍스쳐를 사용 */
PS_OUT PS_MIX_TEXTURE(PS_IN In)
{
    PS_OUT Out;
    
    float4 UpperTexture         = g_UpperTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 TextureColor         = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    float Mask = saturate(UpperTexture.a);
    
    float4 Color = lerp(TextureColor, UpperTexture, Mask);

    if (Color.a < 0.1f)
        discard;
  
    Out.vColor = Color;
    return Out;
}

/* 텍스쳐를 좀 더 밝게 */
PS_OUT PS_ADJUST_BRIGHTNESS(PS_IN In)
{
    PS_OUT Out;
    
    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    TextureColor.rgb = TextureColor.rgb * g_Brightness;
    
    if (TextureColor.a < 0.1f)
        discard;
    
    Out.vColor = TextureColor;
    return Out;
}

/* 텍스쳐 색을 고정색으로 변경 */
PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out;

    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    float3 FinalColor = g_FixColor.rgb; 
    float FinalAlpha = TextureColor.a * g_FixColor.a; 

    if (FinalAlpha < 0.1f)
        discard;
    
    Out.vColor = float4(FinalColor, FinalAlpha);
    return Out;
}

/* 텍스쳐와 색상을 조화롭게 섞기 */
PS_OUT PS_MIX_COLOR(PS_IN In)
{
    PS_OUT Out;

    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    float3 FinalColor = TextureColor.rgb * g_FixColor.rgb;
    
    float FinalAlpha = TextureColor.a * g_FixColor.a;

    if (FinalAlpha < 0.1f)
        discard;
    
    Out.vColor = float4(FinalColor, FinalAlpha);
    return Out;
}

/* Place Move Button을 위한 PS */
PS_OUT PS_FOR_PLACEMOVEBUTTON(PS_IN In)
{
    PS_OUT Out;
    
    float4 UpperTexture = g_UpperTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    
    float Mask = saturate(UpperTexture.a);
    
    if (UpperTexture.r < 0.1f && UpperTexture.g < 0.1f && UpperTexture.b < 0.1f)
        Mask = 0.f;
    else
        UpperTexture = UpperTexture * float4(1.0f, 0.423f, 0.051f, 1.0f);

    float4 Color = lerp(TextureColor, UpperTexture, Mask);

    if (Color.a < 0.1f)
        discard;
  
    Out.vColor = Color;
    return Out;
}

/* TexCoord 늘려서 확대하기 */
PS_OUT PS_TEXCOORD(PS_IN In)
{
    PS_OUT Out;
    
    float4 TextureColor = g_Texture.Sample(DefaultSampler, In.vTexcoord * g_Pow);

    if (TextureColor.a < 0.1f)
        discard;

// 0.0 ~ 1.0 사이 값. 클수록 더 연해짐(더 하얗게).
    float lighten = 0.4f;

// RGB를 흰색 쪽으로 섞어서 연하게
    TextureColor.rgb = lerp(TextureColor.rgb, float3(1.0f, 1.0f, 1.0f), lighten);

    Out.vColor = TextureColor;
    return Out;
}



technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass MixTexture
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MIX_TEXTURE();
    }

    pass Brighten
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ADJUST_BRIGHTNESS();
    }

    pass Color
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }

    pass MixColor
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MIX_COLOR();
    }

    pass PlaceMoveButton
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FOR_PLACEMOVEBUTTON();
    }

    pass Texcoord
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TEXCOORD();
    }

    pass DefaultBlending
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}