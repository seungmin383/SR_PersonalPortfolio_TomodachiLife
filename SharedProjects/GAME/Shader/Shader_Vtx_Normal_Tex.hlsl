#include "Shader_Defines.hlsli"

//float4 == vector
matrix g_WorldMatrix; // 월드 매트릭스
matrix g_ViewMatrix; // 뷰 매트릭스
matrix g_ProjMatrix; // 투영 매트릭스

vector g_vLightDir = vector(-1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D g_DiffuseTexture[2]; //디퓨즈 텍스쳐를 받아
Texture2D g_MaskTexture; //마스크 텍스쳐를 받아
vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

float4 g_vCamPosition;

float2 g_vData = 1.f;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION; 
    float4 vNormal : NORMAL;
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
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
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
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


 PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float4 vGroundDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 50.f);
    vector vGrassDiffuse = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 50.f);
    
    vector vMaskDiffuse = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);
    
    //검정색에 Ground // 흰색에 Grass
    vector vMtrlDiffuse = vMaskDiffuse * vGrassDiffuse + (1.f - vMaskDiffuse) * vGroundDiffuse;
    
    
    
    // 빛의 색과 물체의 색만 곱함
    // Out.vColor = g_vLightDiffuse * vMtrlDiffuse;
    
  //  // float fShade = max(dot((normalize(g_vLightAmbient) * -1.f), In.vNormal), 0);
  //   float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
  //  
  //   
  //          
  //   float3 vLook = In.vWorldPos - g_CameraPos;
  //   float3 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
  //   float fSpecular = pow(max(dot((normalize(vLook) * -1.f), vReflect), 0.f), 2);
  // 
  //   Out.vColor =  g_vLightDiffuse * vMtrlDiffuse 
  //   * (fShade + (g_vLightAmbient * g_vMtrlAmbient))
  //   + (fSpecular * (g_vLightSpecular * g_vMtrlSpecular));
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    
    float4 vLook = In.vWorldPos - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 50);
    
    Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
    + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
        
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}