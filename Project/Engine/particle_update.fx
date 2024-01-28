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

// 그룹 하나가 1024개가 가로로 일렬로 된 스레드로 구성
[numthreads(1024, 1, 1)]
// 코드 수정 필요
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
     SpawnCount = 100;
    
    // 파티클이 비활성화 상태라면
    if (0 == Particle.Active)
    {
        if (0 < SpawnCount)
        {
            // Atomic 함수 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 1;
                        
            //InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            InterlockedExchange(SpawnCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
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