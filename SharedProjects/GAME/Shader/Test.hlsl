
struct VS_OUT
{
    float4 vPos : SV_Position;
};

VS_OUT VS_MAIN(uint id : SV_VERTEXID)
{
    float2 verts[3] = { float2(-1, 1), float2(1, 1), float2(1, -1) }; // fullscreen triangle
    VS_OUT Out;
    Out.vPos = float4(verts[id], 0.5, 1);
    return Out;
}

struct PS_IN
{
    float4 vPos : SV_Position;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = float4(1, 0.5, 0, 1);
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}