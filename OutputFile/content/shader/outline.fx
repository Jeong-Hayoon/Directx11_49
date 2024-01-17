#ifndef _OUTLINE
#define _OUTLINE

#include "value.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vColor : COLOR;
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;

    float3 vWorldPos : POSITION;
};


VS_OUT VS_Outline(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}


float4 PS_Outline(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 1.f, 0.f, 1.f);
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // 주변 픽셀 샘플링
    float4 center = g_postprocess.Sample(g_sam_1, vScreenUV);
    float4 left = g_postprocess.Sample(g_sam_1, vScreenUV + float2(-0.001f, 0));
    float4 right = g_postprocess.Sample(g_sam_1, vScreenUV + float2(0.001f, 0));
    float4 top = g_postprocess.Sample(g_sam_1, vScreenUV + float2(0, -0.001f));
    float4 bottom = g_postprocess.Sample(g_sam_1, vScreenUV + float2(0, 0.001f));

    // 주변 픽셀과의 차이 계산
    float threshold = 0.15f; // 외곽선 감지 임계값
    float4 outlineColor = float4(0, 1, 0, 1); // 외곽선 색상 (초록)
    
    // 외곽선이 있다면
    if (length(center - left) > threshold || length(center - right) > threshold ||
        length(center - top) > threshold || length(center - bottom) > threshold ||
        length(left - center) > threshold || length(right - center) > threshold ||
        length(top - center) > threshold || length(bottom - center) > threshold)
    {
        return outlineColor;
    }
    else
    {
        vColor = g_postprocess.Sample(g_sam_1, vScreenUV);
        return vColor;
    }
}

#endif