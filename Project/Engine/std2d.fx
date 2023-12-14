#ifndef _STD2D
#define _STD2D

// #ifndef _STD2D, #define _STD2D, #endif -> c ��Ÿ���� #pragma once
// _STD2D�� ���ǵǾ� ���� �ʴٸ� 
// #pragma once : ���漱�� ��ø�� ������(c++)

// 4096 ũ������
// b0 : ��� ������ ����
cbuffer TRANSFORM : register(b0)
{
    float4 g_vWorldPos;
    float4 g_vWorldScale;
}

// Shader �ڵ�� ���ҽ�ó�� Ȱ���� ��(��Ÿ�� ���� �ǽð����� �������ϹǷ�0
// �ؽ�ó ��ǥ - �»���� (0,0)�̰� ���ϴ��� (1,1)�� ��ǥ��
// hlsl�� �ۼ��� �ڵ带 �����Ͻ��Ѽ� GPU�� ������ �� �ִ� ���� �ٲ���� ��   
// ����ȭ�� ���簢���� �̹����� ���缭 ������ ������ 
// ���ҽ��� ��׷��� ���µ� ����� ����, ȸ���� ������
// ȣ��Ǵ� �ȼ����� UV�� ���缭 ���� �������� �����̱⶧����
struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// VS_IN : �Է� ����ü / VS_OUT : ��� ����ü
// ���� �� �״�� ���
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    // ���� ��ǥ�� Scale ������ �����ϰ� ��ǥ �̵��� �ϰ� ��
    float2 vFinalPos = _in.vPos.xy * g_vWorldScale.xy + g_vWorldPos.xy;
    
    output.vPosition = float4(vFinalPos, 0.f, 1.f);
    
    // output.vPosition = float4(_in.vPos.xy, 0.f, 1.f);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    

    return output;
}

// VS_Std2D���� ��ȯ�� ���� ���ڷ� ����
// ���� ���ο� �ȼ��� ä������ �Ŷ�
// � ���̵� �������� ���Ͻ�Ų ���� �ȼ��� ���� ���� 
// ������ ��ġ(�Ÿ�0�� ���� ���� ������ �� ���� 
// Pixel Shader�� �Է����� ����
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f); -> ���������� ����
    
    return _in.vColor;    
    
}

#endif