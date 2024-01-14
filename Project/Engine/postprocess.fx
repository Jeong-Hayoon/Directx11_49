#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// ȭ�� ��ü�� PS�� ȣ��Ǵ� ���� ����
// RectMesh�� �����ͼ� x,y,z�� 2��� ���ָ� ���� �����̽� 
// ���� ��ü �������� �÷��� ���� ���

// mesh : RectMesh ����
struct VS_IN
{
    // Mesh�� ���� ��ǥ
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ==========
// GrayFilter
// ==========
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

// ��� ����(ȭ�� ��ü)
float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    
    // ȸ��
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}

// ==========
// Distortion 
// ==========
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // PixelShader�� SV_Position ���� �Էµ� �� == PixelShader�� ȣ���� �ش� �ȼ��� ��ǥ
    // _in.vPosition.xy : ȣ��� �ȼ��� ��ǥ(SV_Position�� ����)
    // �ػ󵵷� �����ְ� �Ǹ� ��ü ��� ��ġ�� ������ ���� -> �̸� UV�� ���
    // vScreenUV - ��ü ȭ���� �������� �� UV ��ǥ
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // ===== �ⷷ�Ÿ��� ȿ�� ===== 
    // g_time : �������� �ʰ� �����̵���
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (�ӵ�) )) * (���ļ�)) * (����);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    // ===========================
    if (g_btex_0)
    {
        ///////////////////////////////////
        // �̼��ϰ� �������� ���� ���̱� // -> ĳ���� ���� ȿ��, ��ų ����Ʈ, �� �Ϸ���, �ſ� �ݻ� �� ��������
        ///////////////////////////////////
        float2 vUV = _in.vUV;
        
        // �����ϴ� ������ �ð��� ���� �޶���
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        // vNoise.xy == vNoise.rg
        // Noise�� ���� ���� : 0~1
        vNoise = (vNoise.xy - 0.5f) * 0.1f;     // ���� ���� -0.5~0.5�� 10�ۼ�Ʈ
        
        vScreenUV += vNoise;
    }
        
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}

#endif