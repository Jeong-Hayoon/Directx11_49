// Debug Shape ���� Shader
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
    // ���� ��ı��� ���� ���� NDC ��ǥ�踦 ������
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_DebugShape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // ���� ��� 4�࿡ ��ġ ������ �־�� �ϹǷ� 1�� ���� ��ǥ Ȯ��
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

// �׵θ� ������ �β� ����
static float g_Thickness = 0.1f;

float4 PS_DebugShape(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif