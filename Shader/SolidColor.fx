

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;

};

VOut VS_Main(float4 position : POSITION, float4 color : COLOR0)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}

float4 PS_Main( float4 pos : SV_POSITION, float4 color : COLOR0) : SV_TARGET
{
    return color;
}