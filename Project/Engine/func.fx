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


#endif