#include "Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_WorldMatrixInv, g_ViewMatrixInv, g_ProjMatrixInv;

/* Light */
float4 g_vLightDir;
float4 g_vLightPos;
float g_fLightRange;

float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

float4 g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
float4 g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

float4 g_vCamPosition;

/* Texture */
texture2D g_Texture;
texture2D g_DiffuseTexture;
texture2D g_ShadeTexture;
texture2D g_NormalTexture;
texture2D g_DepthTexture;

texture2D g_SpecularTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float4x4 matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
        
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT_BACKBUFFER
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_BACKBUFFER PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT_BACKBUFFER Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

PS_OUT_BACKBUFFER PS_MAIN_COMBINED(PS_IN In)
{
    PS_OUT_BACKBUFFER Out;
    
    float4 vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if(0.f == vDiffuse.a)
        discard;
    
    float4 vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vShade + vSpecular;
    
    return Out;
}

struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL_LIGHT(PS_IN In)
{
    PS_OUT_LIGHT Out;
 
    float4 vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
   
    /* 0 ~ 1로 보간된 노말을 다시 World 노말로 복원 */
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    float4 vShade = max(dot((normalize(g_vLightDir) * -1.f), vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
    Out.vShade = g_vLightDiffuse * saturate(vShade);
    
    float4 vReflect = reflect(normalize(g_vLightDir), vNormal);
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    
    /* 깊이값으로 월드위치 추론 */
    /* 픽셀들의 Texcoord상의 좌표 즉 뷰포트상의 좌표 -> NDC좌표계로 변환 */
    vector vWorldPos;
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * (-2.f) + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    /* NDC좌표계 -> 클립 스페이스로 변환 */
    float w = vDepthDesc.y * 1000.f;
    vWorldPos *= w;
    
    /* 클립 스페이스 -> 뷰 스페이스 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    /* 뷰 스페이스 -> 월드 스페이스 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float4 vLook = vWorldPos - g_vCamPosition;
    Out.vSpecular = pow(max(dot((normalize(vReflect) * -1.f), normalize(vLook)), 0.f), 50.f) * (g_vLightSpecular * g_vMtrlSpecular);
    return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT_LIGHT(PS_IN In)
{
    PS_OUT_LIGHT Out;
 
    float4 vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
   
    /* 0 ~ 1로 보간된 노말을 다시 World 노말로 복원 */
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
   
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    
    /* 깊이값으로 월드위치 추론 */
    /* 픽셀들의 Texcoord상의 좌표 즉 뷰포트상의 좌표 -> NDC좌표계로 변환 */
    float4 vWorldPos;
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * (-2.f) + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    /* NDC좌표계 -> 클립 스페이스로 변환 */
    float w = vDepthDesc.y * 1000.f;
    vWorldPos *= w;
    
    /* 클립 스페이스 -> 뷰 스페이스 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    /* 뷰 스페이스 -> 월드 스페이스 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float4 vLightDir = vWorldPos - g_vLightPos;
    float fDistance = length(vLightDir);
    
    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);
    
    float4 vShade = max(dot((normalize(vLightDir) * -1.f), vNormal), 0) + (g_vLightAmbient * g_vMtrlAmbient);
    Out.vShade = g_vLightDiffuse * vShade * fAtt;
    
    float4 vReflect = reflect(normalize(vLightDir), vNormal);
    vector vLook = vWorldPos - g_vCamPosition;
    Out.vSpecular = fAtt * ((g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f));

    
    return Out;
}

technique11 DefaultTechnique
{
    pass Debug
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass DirectionalLight
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL_LIGHT();
    }

    pass PointLight
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT_LIGHT();
    }

    pass Combined
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COMBINED();
    }

}