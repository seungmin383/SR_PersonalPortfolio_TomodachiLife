//float4 == vector
matrix g_WorldMatrix;   // 월드 매트릭스
matrix g_ViewMatrix;    // 뷰 매트릭스
matrix g_ProjMatrix;    // 투영 매트릭스

Texture2D g_Texture;

float2 g_vData = 1.f;

//Vertex Shader ====================================== //
// VS_IN -> Vertex Shader 단계에서 받아올 정점 데이터
struct VS_IN
{
    float3 vPos         : POSITION;
    float2 vTexcoord    : TEXCOORD0;
};

// VS_OUT -> Vertex Shader 단계가 끝날 때 내보내는 정점 데이터
// 이 데이터를 다음 파이프라인 단계에서 IN으로 사용함
struct VS_OUT
{
    float4 vPos         : SV_POSITION; //sv -> 내가 할건 끝났다. 장치가 해야될 일을 해
    float2 vTexcoord    : TEXCOORD0;
    float4 vWorldPos    : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    matrix wvMatrix;
    matrix wvpMatrix;
    
    wvMatrix        = mul(g_WorldMatrix, g_ViewMatrix);     // World랑 View
    wvpMatrix       = mul(wvMatrix, g_ProjMatrix);          // 최종 행렬
    
    Out.vPos        = mul(float4(In.vPos, 1.f), wvpMatrix);
    Out.vTexcoord   = In.vTexcoord;
    Out.vWorldPos   = mul(float4(In.vPos, 1.f), g_WorldMatrix);
    
    return Out;
}

//Pixel Shader ================================= //

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
};

/*
HLSL:
SamplerState DefaultSampler : register(s0);

CPP:
D3D11_SAMPLER_DESC sd{};
sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
ComPtr<ID3D11SamplerState> sampler;
device->CreateSamplerState(&sd, &sampler);
context->PSSetSamplers(0, 1, sampler.GetAddressOf()); // s0에 바인딩
*/

struct PS_IN
{
    float4 vPos         : SV_POSITION;
    float2 vTexcoord    : TEXCOORD0;
    float4 vWorldPos    : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor       : SV_TARGET0;
};

// 픽셀의 최종적인 색을 결정함
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if(Out.vColor.a < 0.3f )
        discard;
    
    Out.vColor.gb = Out.vColor.r;
    
    return Out;
}

//그래픽 카드 사면 이 그래픽 카드는 쉐이더 버전 스펙이 있다 지원해주는
//패스 : 렌더링 기법의 캡슐화 (모아둘께요)
/*
일반적인 투패스 렌더링은 모델을 두번 그리는 것 -> 미친짓

*/

technique11 DefaultTechnique
{
    pass UI
    {
        VertexShader    = compile vs_5_0 VS_MAIN();
        PixelShader     = compile ps_5_0 PS_MAIN();
    }

    pass Effect
    {
        VertexShader    = compile vs_5_0 VS_MAIN();
        PixelShader     = compile ps_5_0 PS_MAIN();
    }
}