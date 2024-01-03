// Debug Shape 전용 Shader
#ifndef _DEBUG
#define _DEBUG

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    // 투영 행렬까지 곱한 최종 NDC 좌표계를 보내줌
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_DebugShape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 상태 행렬 4행에 위치 정보가 있어야 하므로 1로 동차 좌표 확장
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

// 테두리 라인의 두께 설정
static float g_Thickness = 0.1f;

float4 PS_DebugShape(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif