#include "Shader_Defines.hlsli"

matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;
vector g_vCamPosition;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(0.f, 0.f, 0.f, 0.f);

vector g_vMtrlAmbient = vector(0.8f, 0.8f, 0.8f, 1.f);
vector g_vMtrlSpecular = vector(1.0f, 1.0f, 1.0f, 1.f);

texture2D g_DiffuseTexture;

float4x4 g_BoneMatrices[128];

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_IN
{
    float3 vPosition    : POSITION;
    float3 vNormal      : NORMAL;
    float3 vTangent     : TANGENT;
    float3 vBinormal    : BINORMAL;
    float2 vTexcoord    : TEXCOORD0;
    uint4 vBlendIndex   : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
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
    
    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
        g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
        g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
        g_BoneMatrices[In.vBlendIndex.w] * fWeightW;
    
    float4 vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    
    float4x4 wvMatrix;
    float4x4 wvpMatrix;
    wvMatrix = mul(g_WorldMatrix, g_ViewMatrix);
    wvpMatrix = mul(wvMatrix, g_ProjMatrix);
    
    Out.vPosition       = mul(vPosition, wvpMatrix);
    Out.vNormal         = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord       = In.vTexcoord;
    Out.vWorldPosition  = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
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
    
    Out.vColor = float4(vFinalColor += 0.1f, fAlpha);
    
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
    Out.vColor = float4(1.f - (vNoneAlphaDiffuse * 2.f), fAlpha/3.f);
    
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
    pass Model
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Shadow
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Light
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_LIGHT();
    }

    pass Brightness
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_BRIGHTNESS();
    }

}

