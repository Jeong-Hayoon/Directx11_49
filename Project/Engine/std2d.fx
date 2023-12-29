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

// GPU�޸𸮴� �ּ� ������ 16byte�̱� ������ �ϳ��ϳ��� ������� ��
// �迭�� ��������� �迭�� �ּ� ������ 16byte 4���� ������
// �ϳ��ϳ� ������ 16byte�ȿ� int 4���� ����ִٰ� ��
cbuffer MATERIAL_CONST : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    // �������Ϳ� ���ε��� Texture�� �ִ��� check�ϴ� �뵵�� bool��
    // (�ּҴ��� ������ bool ������ ��� �Ұ���)
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btexcube_0;
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;
}

// �� �ؽ�ó�� ��� �ִ� �̹��� ���� ���� = Sample
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

// Sampling : �ؽ�ó ����
// Sampler : ���ø��� ���� ����
// ���ػ� �ؽ�ó�� ����� �� ������ ��� ����
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


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
    // GetDimensions : ���ε��Ǿ� �ִ� ������ ���� �� �ִ� �Լ�
    // uint width = 0;
    // uint height = 0;
    // g_tex_1.GetDimensions(width, height) == 0�̸� ���ε��� Texture�� ���ٰ� �Ǵ�
    // ��� �� �Լ��� ��� �ȼ��� ���� ȣ���� �Ǽ� Shader�� ���� �ӵ��� �������ϱ�
    // Texture�� ���ε��Ǿ��ִ��� Check�ϴ� bool������ �ּ� �����ϴ°� ����
    
    // Texture�� nullptr�� ��
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // tex_0�� true�� �ؽ�ó ���, �װ� �ƴ϶�� �⺻ �÷� ����Ÿ�� ���
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        // Clamp �Լ�(���� Ư�� ���� ���� ���ֳ��� ���� �� ����ϴ� �Լ�)
        // saturate : 0 ~ 1 �� ���� �ʰ� ����(0~1 �Ѵ� ���� �߶������ �Լ�)
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        // ���ø��� ������ ����Ÿ�� ����� �����̶�� ������ ��������
        if (fAlpha < 0.1f) 
        {
            // discard : �ȼ� ���̴��� �߰��� ��� ó�� -> ���̻� PipeLine OM�ܰ���� ���� ������ ���� ���嵵 �ȵ�
            discard; //clip(-1); - ���� ���, -1�� ���޵Ǹ� discard ȣ��           
        }
    }
   
    return vColor;
}

#endif


 //if (vColor.a <= 0.1f)
 //{
 //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
 //}
 
 //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
 //vColor.rgb = float3(Aver, Aver, Aver);    
 //vColor.a = 1.f;
 
 //vColor.rgb *= 1.5f; => Blend State ������ ������ ���� �� ���ϰ�