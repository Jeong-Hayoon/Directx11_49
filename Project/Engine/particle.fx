#ifndef _PARTICLE
#define _PARTICLe

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);

// g_int_0 인덱스에 해당하는 그 파티클 자체를 매크로로
#define Particle g_ParticleBuffer[_in.iInstID]

struct VS_IN
{
    // 로컬 pos
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    // 인스턴싱으로 랜더링할 때 몇번째를 랜더링하고 있는지 들어옴
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vWorldPos = (_in.vPos * Particle.vWorldScale.xyz) + Particle.vWorldPos.xyz;
    
    // 월드 행렬은 파티클 자체가 아닌 오브젝트의 월드 행렬이기 때문에 Pass
    output.vPosition = mul(mul(float4(vWorldPos, 1.f), g_matView), g_matProj);
    output.vUV = _in.vUV;
    output.InstID = _in.iInstID;
    
    return output;
}

// Geometry Shader
// 1. 담당 파티클이 비활성화 상태인 경우, 렌더링을 정점연산 단계에서 중단시키기
// 2. 빌보드 구현의 편의성

float4 PS_Particle(VS_OUT _in) : SV_Target
{
    // 비활성화 상태라면
    if (!g_ParticleBuffer[(uint) _in.InstID].Active)
    {
        discard;
    }
    
    return float4(1.f, 0.f, 0.f, 1.f);
}




#endif