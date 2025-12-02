
RasterizerState RS_Default
{
    FillMode = SOLID;
    CullMode = BACK;
};


RasterizerState RS_ccwCull
{
    FillMode = SOLID;
    CullMode = FRONT;
};

RasterizerState RS_NoneCull
{
    FillMode = SOLID;
    CullMode = NONE;
};

BlendState BS_Default
{

};

DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_NoneDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = ALWAYS;
};