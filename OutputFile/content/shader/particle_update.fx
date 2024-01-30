#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tParticleModule> g_Module : register(t20);
RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_SpawnCount : register(u1);

// 파티클의 최대 개수
#define MAX_COUNT g_int_0 
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]

// 그룹 하나가 1024개가 가로로 일렬로 된 스레드로 구성
[numthreads(32, 1, 1)]
// 코드 수정 필요
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;
    // 파티클이 비활성화 상태라면
    if (0 == Particle.Active)
    {
        // 스폰 모듈 활성화 체크
        if (Module.arrModuleCheck[0])
            return;
        
        while (0 < SpawnCount)
        {
            // Atomic 함수 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
           
            // InterlockedExchange 함수를 써서 SpawnCount 를 교체, 수정하면
            // 초기 시도인 스드가 여러 스레드가 성공한 이후에 진입하는 경우가 있다. 
            // 이때 SpawnCount 를 오히려 늘려버리는 현상이 발생할 수 있다. 
            // InterlockedCompareExchange 를 통해서 예상한 값과 일치할 경우에만 
            // 교체를 하도록 하는 함수를 사용한다.
            InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                
                // 랜덤
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                                
                float4 vColor = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                        
                // SpawnShape 가 Sphere 타입이라면
                if (0 == Module.SpawnShape)
                {
                    float RandomRadius = vColor.x * Module.Radius;
                    
                    // Particle 컴포넌트(본체) 의 중심위치(월드) 값을 알아야한다.
                    
                    // Space 타입에 따라서 생성방식을 다르게 해야한다.
                    // 0 : LocalSpace
                    // 1 : WorldSpalce
                    Module.SpaceType;
                }
                
                break;
            }
        }
    }
  
    
    // 파티클이 활성화 상태라면
    else
    {
        Particle.vWorldPos.y += g_dt * 100.f;
    }
    
}

#endif