//float4 == vector
matrix g_WorldMatrix; // 월드 매트릭스
matrix g_ViewMatrix; // 뷰 매트릭스
matrix g_ProjMatrix; // 투영 매트릭스

Texture2D g_Texture;

float2 g_vData = 1.f;

//Vertex Shader ====================================== //
// VS_IN -> Vertex Shader 단계에서 받아올 정점 데이터
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

// VS_OUT -> Vertex Shader 단계가 끝날 때 내보내는 정점 데이터
// 이 데이터를 다음 파이프라인 단계에서 IN으로 사용함
struct VS_OUT
{
    float4 vPosition : SV_POSITION; //sv -> 내가 할건 끝났다. 장치가 해야될 일을 해
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

//Pixel Shader ================================= //

//텍스쳐에서 색을 빼오는걸 샘플링이라고 함
sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
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

// 픽셀의 최종적인 색을 결정함
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    float4 col = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    if(col.a < 0.1f)
        discard;
    
    Out.vColor = col;
    return Out;
}

//그래픽 카드 사면 이 그래픽 카드는 쉐이더 버전 스펙이 있다 지원해주는
//패스 : 렌더링 기법의 캡슐화 (모아둘께요)
/*
일반적인 투패스 렌더링은 모델을 두번 그리는 것 -> 미친짓

*/

technique11 DefaultTechnique
{
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}