#include "Engine_Shader_Defines.hlsli"

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
    float4 vProjPos         : TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float4 vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    /* 굳이 클립 스페이스상의 위치를 받는 이유 -> Out.vPosition은 System Value로 PS로 넘어가기 전에 
    w 나누기 / 뷰포트 변환을 거친 후 PS에 넘겨질 것이기 때문 */ 
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition        : SV_POSITION;
    float4 vNormal          : NORMAL;
    float2 vTexcoord        : TEXCOORD0;
    float4 vWorldPosition   : TEXCOORD1;
    float4 vProjPos         : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPick : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    /* World 상태의 Normal은 -1 ~ 1의 범위인데, 
    Normal의 Render Target에 픽셀 포맷이 DXGI_FORMAT_R16G16B16A16_UNORM 이라 음수값이 잘린다.
    이를 방지하기 위해 0 ~ 1로 선형보간 해주는 작업 */
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    
    /* x 위치에 뷰포트 공간의 z값을 저장하고 싶다.
    w로 나누게 되면 NDC 좌표계의 z 값이고 /
    뷰포트 변환을 거쳐야 되는데 어짜피 mindepth, maxdepth가 0, 1이므로 따로 변환하지 않아도 됨 / 만약 설정이 바뀌면 변환을 해줘야 됨
    */
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
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
    
    
    Out.vDiffuse = vFinalColor;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
    return Out;
}

/* 밝기 조절 */
PS_OUT PS_ADJUST_BRIGHTNESS(PS_IN In)
{
    PS_OUT Out;
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

    float3 vColor = vMtrlDiffuse.rgb;
    vColor += g_Brightness;

    if (vMtrlDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse = float4(vColor, vMtrlDiffuse.a);
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
    return Out;
}

PS_OUT PS_SHADOW(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexcoord);

    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;

    Out.vDiffuse = float4(vNoneAlphaDiffuse * 2.f, fAlpha);
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
    return Out;
}

/* 그림자 역 */
PS_OUT PS_LIGHT(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexcoord);

    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;

    Out.vDiffuse = float4(1.f - (vNoneAlphaDiffuse * 2.f), fAlpha / 3.f);
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
    return Out;
}

PS_OUT PS_BRIGHTNESS(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexcoord);

    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a;

    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);

    float4 vLook = In.vWorldPosition - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 1000);

    float3 vFinalColor =
    (g_vLightDiffuse.rgb * vNoneAlphaDiffuse) * saturate(fShade + (g_vLightAmbient.rgb * g_vMtrlAmbient.rgb)) + (g_vLightSpecular.rgb * g_vMtrlSpecular.rgb) * fSpecular;

    Out.vDiffuse = float4(vFinalColor += 0.12f, fAlpha);
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
    return Out;
}

PS_OUT PS_PARK(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexcoord);

    float3 vNoneAlphaDiffuse = float3(vMtrlDiffuse.rgb);
    float fAlpha = vMtrlDiffuse.a - 0.5f;
     
    if (vNoneAlphaDiffuse.x > 0.1f) 
        discard;
    
    Out.vDiffuse = float4(vNoneAlphaDiffuse, fAlpha );
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vPick = vector(In.vWorldPosition.xyz, 1.f);
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
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Light
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LIGHT();
    }

    pass Brightness2
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BRIGHTNESS();
    }


    pass Park
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PARK();
    }

}

