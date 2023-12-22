#ifndef _STD2D
#define _STD2D

// #ifndef _STD2D, #define _STD2D, #endif -> c ��Ÿ���� #pragma once
// _STD2D�� ���ǵǾ� ���� �ʴٸ� 
// #pragma once : ���漱�� ��ø�� ������(c++)

// 4096 ũ������
// b0 : ��� ������ ����
cbuffer TRANSFORM : register(b0)
{
    // row_major : gpu�� �޸𸮸� �о���̴� ����� �� ����̶� ��ġ�� ����� ��
    // ��켱���� ��ġ���Ѽ� �츮�� ���ϴ� ������ ������ ��
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
    
}
// �� �ؽ�ó�� ��� �ִ� �̹��� ���� ���� = Sample
Texture2D g_tex_0 : register(t0);

// Sampling : �ؽ�ó ����
// Sampler : �ؽ�ó ������ ���� ����..?
SamplerState g_sam_0 : register(s0);

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
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    // mul : hlsl���� ��ǥ�� ���Ϳ� ����� �����ִ� �Լ�
    // float4(_in.vPos, 1.f) : float3�� ���� ���� ���� ��ǥ��� ��ȯ(��Ÿ ������)
    // ���� ��ǥ�� -> ���� ��ǥ��� �� ���� ��ȯ(���� ��ȯ)
    // output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // ����(��) ��ǥ�� -> ���� -> �� -> ���� ��ǥ��� ���������� ��ȯ

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
 
    return output;
}

// VS_Std2D���� ��ȯ�� ���� ���ڷ� ����
// ���� ���ο� �ȼ��� ä������ �Ŷ�
// � ���̵� �������� ���Ͻ�Ų ���� �ȼ��� ���� ���� 
// ������ ��ġ(�Ÿ�0�� ���� ���� ������ �� ���� 
// Pixel Shader�� �Է����� ����
// SV_Target : ��ȯ Ÿ���� �������ִ� �ø�ƽ -> Ÿ���� �ǹ̴� ���� Ÿ��
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    //if (vColor.a <= 0.1f)
    //{
    //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
    //}
    
    //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    //vColor.rgb = float3(Aver, Aver, Aver);    
    //vColor.a = 1.f;
    
    return vColor;
    
}

#endif
