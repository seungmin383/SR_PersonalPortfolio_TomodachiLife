//float4 == vector
matrix g_WorldMatrix; // 월드 매트릭스
matrix g_ViewMatrix; // 뷰 매트릭스
matrix g_ProjMatrix; // 투영 매트릭스

float4 g_EyebrowColor;

Texture2D g_Texture : register(t0);
Texture2D g_OutlineTexture : register(t1);

Texture2D g_IconTexture : register(t2);

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

    if (col.a < 0.1f)
        discard;
  
    Out.vColor = col;
    return Out;
}

PS_OUT PS_PLACEMOVEBUTTON(PS_IN In)
{
    PS_OUT Out;
    
    float4 OutLineCol = g_OutlineTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 col = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    float4 Color = (1.f - OutLineCol) * col + OutLineCol * float4(1.0f, 0.423f, 0.051f, 1.0f);

    if (Color.a < 0.1f)
        discard;
  
    Out.vColor = Color;
    return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
    PS_OUT Out;
    
    float4 col = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (col.a < 0.1f)
        discard;
  
    float Darkness = 1.2f;
    
    col.rgb = col.rgb * Darkness;
    
    Out.vColor = col;
    return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
    PS_OUT Out;
    
    float4 col = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    float4 iconCol = g_IconTexture.Sample(DefaultSampler, In.vTexcoord);

    float Darkness = 1.2f;
    col.rgb = col.rgb * Darkness; // 배경만 살짝 어둡게

    // 아이콘 알파 기반으로 배경 위에 아이콘 얹기
    float iconAlpha = saturate(iconCol.a);

    float3 finalRgb = lerp(col.rgb, iconCol.rgb, iconAlpha);
    float finalA = max(col.a, iconAlpha); // 필요에 따라 col.a로 해도 됨

    if(finalA < 0.1f)
        discard;
    
    Out.vColor = float4(finalRgb, finalA);
    return Out;
}

PS_OUT PS_MIIEYEBROW(PS_IN In)
{
    PS_OUT Out;

    float4 tex = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    float3 finalColor = g_EyebrowColor.rgb; // 갈색 같은 최종 색
    float finalAlpha = tex.a * g_EyebrowColor.a; // 눈썹 텍스쳐의 알파 그라데이션

    if(finalAlpha < 0.1f)
        discard;
    
    Out.vColor = float4(finalColor, finalAlpha);
    return Out;
}

PS_OUT SELECTEDCMBUTTON(PS_IN In)
{
    PS_OUT Out;
    
    float4 col = g_Texture.Sample(DefaultSampler, In.vTexcoord); // 버튼 배경
    float4 iconCol = g_IconTexture.Sample(DefaultSampler, In.vTexcoord); // 아이콘
    
    // 아이콘 알파 (아이콘 영역: 1, 아닌 곳: 0)
    float iconAlpha = saturate(iconCol.a);
    float bgMask = 1.0f - iconAlpha; // 아이콘 없는 곳: 1

    // 곱해줄 파란색 계열 (0, 222, 165)
    float3 blueMul = float3(0.0f, 222.0f / 255.0f, 165.0f / 255.0f);

    float3 bgColor = lerp(col.rgb, col.rgb * blueMul, bgMask);
    
    float3 finalRgb = lerp(bgColor, iconCol.rgb, iconAlpha);

    float finalA = max(col.a, iconAlpha);

    if (finalA < 0.1f)
        discard;

    Out.vColor = float4(finalRgb, finalA);
    return Out;
}

technique11 DefaultTechnique
{
    pass PlaceMoveButton
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_PLACEMOVEBUTTON();
    }

    pass BackGround
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN1();
    }

    pass CMButton
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN2();
    }

    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass MiiEyebrowPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MIIEYEBROW();
    }

    pass SelectedCMButton
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 SELECTEDCMBUTTON();
    }
}