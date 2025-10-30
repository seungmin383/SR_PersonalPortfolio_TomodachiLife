
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;
vector g_vCamPosition;


vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = vector(1.0f, 1.0f, 1.0f, 1.f);

texture2D g_DiffuseTexture;

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
    
    // 나중에 diffuse에서 a 값 분리
    float4 vMtrlDiffuse = 1.f;
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    
    float4 vLook = In.vWorldPosition - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 50);
    
    Out.vColor = 
    (g_vLightDiffuse * vMtrlDiffuse)* saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
        
    return Out;
}


technique11 DefaultTechnique
{
    pass Model
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}

