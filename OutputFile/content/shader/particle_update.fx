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
#define CenterPos   g_vec4_0.xyz

// 그룹 하나가 1024개가 가로로 일렬로 된 스레드로 구성
[numthreads(32, 1, 1)]
// 코드 수정 필요
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    // Thread가 과하게 배정되지 않도록 예외처리
    if (MAX_COUNT <= id.x)
        return;
    
    // 파티클이 비활성화 상태라면
    if (0 == Particle.Active)
    {
        // 스폰 모듈 활성화 체크
        if (0 == Module.arrModuleCheck[0])
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
            
            // InterlockedExchange(SpawnCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                
                // 랜덤
                // MAX_COUNT - 1 : 등분
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUV.x += g_time * 0.2f;
                //                 ( 주파수 )    (진폭)  (V 축 offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                // SampleLevel : Sample은 PS에서만 가능, 지금은 Compute Shader 시점이라 SampleLevel 사용
                // Noise Texture 1번은 무채색이라 RGB가 똑같음 -> 추출한 컬러의 x,y,z 어떤 것을 사용해도 똑같음 
                // LOD는 현재 2D라 사용을 안하기에 원본 Level 0 밖에 없음
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                        
                // SpawnShape 가 Sphere 타입이라면
                if (0 == Module.SpawnShape)
                {
                    // vRand.x는 0~1 사이값
                    // RandomRadius는 Module.Radius가 100이니까 0~100 사이의 랜덤값
                    float RandomRadius = vRand[0] * Module.Radius;
                    float RandomAngle = vRand[1] * 2 * PI;
                    
                    // Particle 컴포넌트(본체) 의 중심위치(월드) 에서
                    // 랜덤 각도, 랜덤 반지름에 해당하는 위치를 계산해서 파티클의 초기 위치로 준다.
                    Particle.vLocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
                    
                    // Particle 컴포넌트(본체) 의 중심위치(월드) 값을 알아야한다.
                    
                    // Space 타입에 따라서 생성방식을 다르게 해야한다.
                    // 0 : LocalSpace
                    // 1 : WorldSpalce
                    Module.SpaceType;
                }
                else
                {
                    Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale.x - (Module.vSpawnBoxScale.x / 2.f);
                    Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale.y - (Module.vSpawnBoxScale.y / 2.f);
                    Particle.vLocalPos.z = 0.f;
                }
                
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // 스폰 컬러 설정
                Particle.vColor = Module.vSpawnColor;
                
                // 스폰 크기 설정                
                Particle.vWorldScale = (Module.vSpawnMaxScale - Module.vSpawnMinScale) * vRand[2] + Module.vSpawnMinScale;
                
                // 스폰 Life 설정
                Particle.Age = 0.f;
                Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MaxLife;
                         
                // Add VelocityModule
                if (Module.arrModuleCheck[3])
                {
                    // 0 : From Center
                    if (0 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    if (1 == Module.AddVelocityType)
                    {
                        float3 vDir = -normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                }
                
                break;
            }
        }
    }
  
    
    // 파티클이 활성화 상태라면
    else
    {
       Particle.Age += g_dt;
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = 0;
            return;
        }
        
        // 
        if (0 == Module.SpaceType)
        {
            Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_dt;
            Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
        }
        else if (1 == Module.SpaceType)
        {
            Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_dt;
        }
    }
    
}

#endif