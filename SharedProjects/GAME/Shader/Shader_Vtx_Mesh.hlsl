#include "Shader_Defines.hlsli"

matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;
vector g_vCamPosition;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(0.f, 0.f, 0.f, 0.f);

vector g_vMtrlAmbient = vector(0.7f, 0.7f, 0.7f, 1.f);
vector g_vMtrlSpecular = vector(1.0f, 1.0f, 1.0f, 1.f);

float4 g_BaseColor;
float g_Brightness;

texture2D g_DiffuseTexture;


sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = mirror;
    AddressV = mirror;
};

struct VS_IN
{
    float3 vPosition    : POSITION;
    float3 vNormal      : NORMAL;
    float3 vTangent     : TANGENT;
    float3 vBinormal    : BINORMAL;
    float2 vTexcoord    : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition        : SV_POSITION;
    float4 vNormal          : NORMAL;
    float2 vTexcoord        : TEXCOORD0;
    float4 vWorldPosition   : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float4 vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

struct PS_IN
{
    float4 vPosition        : SV_POSITION;
    float4 vNormal          : NORMAL;
    float2 vTexcoord        : TEXCOORD0;
    float4 vWorldPosition   : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    
    float4 vLook = In.vWorldPosition - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 1000);
    
    float3 vFinalColor = 
    (g_vLightDiffuse.rgb * vNoneAlphaDiffuse) * saturate(fShade + (g_vLightAmbient.rgb * g_vMtrlAmbient.rgb)) + (g_vLightSpecular.rgb * g_vMtrlSpecular.rgb) * fSpecular;
    
    if(fAlpha < 0.1f)
        discard;
    
    Out.vColor = float4(vFinalColor, fAlpha);
    
    return Out;
}

/* Diffuse 없이 색으로만 */
PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out;
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    
    float4 vLook = In.vWorldPosition - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 10);
    
    float4 vFinalColor =
    (g_vLightDiffuse * g_BaseColor) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    
    Out.vColor = vFinalColor;
    return Out;
}

/* 밝기 조절 */
PS_OUT PS_ADJUST_BRIGHTNESS(PS_IN In)
{
    PS_OUT Out;
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

    float3 vColor = vMtrlDiffuse.rgb;
    vColor += g_Brightness;
    
    if(vMtrlDiffuse.a < 0.1f)
        discard;
    
    Out.vColor = float4(vColor, vMtrlDiffuse.a);
    return Out;
}

PS_OUT PS_SHADOW(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;
 
    Out.vColor = vMtrlDiffuse;
    Out.vColor = float4(vNoneAlphaDiffuse * 2.f, fAlpha);
    
    return Out;
}

/* 그림자 역 */
PS_OUT PS_LIGHT(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;
 
    Out.vColor = vMtrlDiffuse;
    Out.vColor = float4(1.f - (vNoneAlphaDiffuse * 2.f), fAlpha / 3.f);
    
    return Out;
}

PS_OUT PS_BRIGHTNESS(PS_IN In)
{
    PS_OUT Out;
    
    // 나중에 diffuse에서 a 값 분리
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    
    float4 vLook = In.vWorldPosition - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 1000);
    
    float3 vFinalColor =
    (g_vLightDiffuse.rgb * vNoneAlphaDiffuse) * saturate(fShade + (g_vLightAmbient.rgb * g_vMtrlAmbient.rgb)) + (g_vLightSpecular.rgb * g_vMtrlSpecular.rgb) * fSpecular;
    
    Out.vColor = float4(vFinalColor += 0.12f, fAlpha);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Defualt
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Color
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }

    pass ccwColor
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }
    
    pass Brightness
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ADJUST_BRIGHTNESS();
    }

    pass Shadow
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Light
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LIGHT();
    }

    pass Brightness2
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BRIGHTNESS();
    }
}

