Texture2D colorMap_ : register( t0 );
SamplerState colorSampler_ : register( s0 );


cbuffer cbChangesPerFrame : register( b0 )
{
    matrix mvp_;
};

float blur = 0.0015; 
struct VS_Input
{
    float4 pos  : POSITION;
    float2 tex0 : TEXCOORD0;
    float4 color: COLOR0;
    
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float4 color: COLOR0;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    vsOut.pos = mul( vertex.pos,mvp_ );
    vsOut.tex0 = vertex.tex0;
    vsOut.color = vertex.color;

    return vsOut;
}


float4 PS_Main( PS_Input input ) : SV_TARGET
{
    float4 col =colorMap_.Sample( colorSampler_, input.tex0);
   // float4 c = tex2D(colorMap_.Sample( colorSampler_, input.tex0),float2(input.tex0.x+blur, //input.tex0.y+blur));
// 0.3, 0.59, 0.11
// 0.439, 0.259, 0.078
//    col.rgb=(dot(col.rgb, float3(0.439, 0.259, 0.078)));

     
    return (input.color*col);
}