#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

// ==========================
// TileMap Shader

// Parameter
#define vLeftTopUV g_vec2_0
#define vSliceUV   g_vec2_1
// ==========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
        
    if (g_btex_0)
    {
        float2 vUV = vLeftTopUV + (vSliceUV * _in.vUV);
        vColor = g_tex_0.Sample(g_sam_0, vUV);
    }
    
    return vColor;
}







#endif