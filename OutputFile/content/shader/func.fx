#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

// inout : 참조자 같은 역할 -> 빛을 누적시킬 수 있음
void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    // 빛을 적용시킬 광원의 정보
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (0 == info.LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    
    // Point Light
    else if (1 == info.LightType)
    {
        // 감쇠(1이면 최대치 빛)
        float fAttenu = 1.f;
        
        // distance : 피타고라스 정리에 의해 두 좌표의 거리값을 계산해주는 함수
        // xy만 사용하는 것은 2D이기 때문
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        // 반경보다 fDist가 작으면 광원안에 들어옴
        if (fDist < info.fRadius)
        {
            // g_int_0 true -> 점진적으로 색 감쇠, false -> 선형 감쇠(BackgroundScript T키로 전환)
            if (g_int_0)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                // 전체 길이 대비 둘 사이의 거리를 분모 분자로 나누면 나로부터 내 최대 거리 대비 어느 정도 위치해 있는지 위치값이 나옴
                // saturate 사용한 이유 : 음수로 되게 되면 나중에 다른 광원이 빛을 줘도 음수를 복구시키느라 빛 적용이 안됨
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
        float2 toPoint = normalize(info.vWorldPos.xy - _WorldPos.xy); // 현재위치-> 광원위치로 향하는 방향벡터
        float cosTheta = dot(normalize(info.vWorldDir.xy), toPoint); // 빛이 향하는 방향과 빛을 받는 지점까지 벡터간의 각도
        
        if (fDist < info.fRadius)
        {
            // cosTheta > info.fAngle / 2.f : 현재 픽셀이 발산하는 빛의 각도 범위 내에 있음
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

// 중앙이 가장 가중치를 높게 받도록
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
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_time /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    // frac : 소수점만 남기는 함수
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    // 픽셀에 직접 접근을 해야하기 떄문에 텍스처의 해상도와 UV를 곱해서 해당 픽셀을 구함
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