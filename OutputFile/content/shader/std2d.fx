#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"


// ����ȭ ���۴� �ؽ�ó �������Ϳ� �ٷ� ���ε��� �� �ִ� ������ �ڷ����� ���� ��������� �ʾƵ� ��
// 14�� �������Ϳ� float4 Ÿ�� ����ȭ ����
StructuredBuffer<float4> g_Data : register(t14);


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
    
    float3 vWorldPos : POSITION;
    
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
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
 
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
    
    // Shader Parameter Test�� 1
    // if (g_int_0)
        // return float4(1.f, 1.f, 0.f, 1.f);
    
    // Shader Parameter Test�� 2
    if (g_vec4_1.w == 3.14f)
        return float4(1.f, 1.f, 0.f, 1.f);
    
    if (g_UseAnim2D)
    {
        // g_vLeftTop + (g_vSlizeSize / 2.f) : Center
        // center���� Background�� ���ݸ�ŭ ���� �Ǹ� vBackgroundLeftTop�� ����
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        
        // Background ��ü�� �̵�
        // UV ������ +�� ���� �����´ٴ� �� +�� ��ŭ ���� ��µǴ� �̹����� �������� �ݴ�� ���� ����
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        // Background ������ ���� �ʿ���� �κе��� discard
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSlizeSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSlizeSize.y) < vUV.y)
        {
           // vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
        
        // ����Ÿ ���� ����
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            discard;           
        }
        
        
    }
    else
    {
          // tex_0�� true�� �ؽ�ó ���, �װ� �ƴ϶�� �⺻ �÷� ����Ÿ�� ���
        if (g_btex_0)
        {
            // WRAP ���ø� ����� ���� ���� �帣�� ���� ȿ���� �� �� ����(���� �ð� ���� ���޵Ǿ��� ������ ����)
            // vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
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
    }
    
    // ���� ó��
    // ������ Ÿ�Ժ� ó��
    // ������ �������� �� ó��
    // g_Light2DCount;    
    
    tLightColor LightColor = (tLightColor) 0.f;
    
    // �ݺ����� ���鼭 �޴� ���� �� ����
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    // �ּҷ� ����Ǵ� ȯ�汤�� ���� �� �����ϱ� 
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    // ���� ����� alpha���� 0�̸�
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}


// EffectShader(VS�� ����, PS�� alpha �� ����)
float4 PS_Std2D_Effect(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSlizeSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSlizeSize.y) < vUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
    }
    else
    {
        if (g_btex_0)
        {
            //vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
                    
            //saturate 0 ~ 1 �� ���� �ʰ� ����
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
            if (fAlpha < 0.1f)
            {
            // �ȼ� ���̴��� �߰��� ���ó��
                discard; //clip(-1);            
            }
        }
    }
        
    // ���� ó��
    // ������ Ÿ�Ժ� ó��
    // ������ �������� �� ó��
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    return vColor;
}


#endif



