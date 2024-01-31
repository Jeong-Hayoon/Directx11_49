#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

// inout : ������ ���� ���� -> ���� ������ų �� ����
void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    // ���� �����ų ������ ����
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (0 == info.LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    
    // Point Light
    else if (1 == info.LightType)
    {
        // ����(1�̸� �ִ�ġ ��)
        float fAttenu = 1.f;
        
        // distance : ��Ÿ��� ������ ���� �� ��ǥ�� �Ÿ����� ������ִ� �Լ�
        // xy�� ����ϴ� ���� 2D�̱� ����
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        // �ݰ溸�� fDist�� ������ �����ȿ� ����
        if (fDist < info.fRadius)
        {
            // g_int_0 true -> ���������� �� ����, false -> ���� ����(BackgroundScript TŰ�� ��ȯ)
            if (g_int_0)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                // ��ü ���� ��� �� ������ �Ÿ��� �и� ���ڷ� ������ ���κ��� �� �ִ� �Ÿ� ��� ��� ���� ��ġ�� �ִ��� ��ġ���� ����
                // saturate ����� ���� : ������ �ǰ� �Ǹ� ���߿� �ٸ� ������ ���� �൵ ������ ������Ű���� �� ������ �ȵ�
                fAttenu = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
            
            _output.vColor += info.Color.vColor * fAttenu;
        }
    }
    
    // Spot Light
    else
    {
        float fAttenu = 1.f;

        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        float2 toPoint = normalize(info.vWorldPos.xy - _WorldPos.xy); // ������ġ-> ������ġ�� ���ϴ� ���⺤��
        float cosTheta = dot(normalize(info.vWorldDir.xy), toPoint); // ���� ���ϴ� ����� ���� �޴� �������� ���Ͱ��� ����
        
        if (fDist < info.fRadius)
        {
            // cosTheta > info.fAngle / 2.f : ���� �ȼ��� �߻��ϴ� ���� ���� ���� ���� ����
            if (cosTheta > cos(radians(info.fAngle) / 2.f))
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            
                _output.vColor += info.Color.vColor * fAttenu;
            }
        }
    
    }
}

// ======
// Random
// ======

// �߾��� ���� ����ġ�� ���� �޵���
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_time * 0.5f;
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (g_time /*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
    // frac : �Ҽ����� ����� �Լ�
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    // �ȼ��� ���� ������ �ؾ��ϱ� ������ �ؽ�ó�� �ػ󵵿� UV�� ���ؼ� �ش� �ȼ��� ����
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)].xyz * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

#endif