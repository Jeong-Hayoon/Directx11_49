#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

StructuredBuffer<tParticleModule>       g_Module : register(t20);
RWStructuredBuffer<tParticle>           g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount>         g_SpawnCount : register(u1);


#define MAX_COUNT   g_int_0             // 파티클의 최대 개수
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]
#define CenterPos   g_vec4_0.xyz        // 파티클의 중심 위치값

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
            
            // 비활성화 상태 -> 활성화로 전환
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // 랜덤
                // MAX_COUNT - 1 : 등분
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                // g_time을 온전히 넣게 되면 Texture 한장을 전진하는 속도가 너무 빨라져버림
                vUV.x += g_time * 0.2f;
                //                 ( 주파수 )    (진폭)  (V 축 offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                // SampleLevel : Sample은 PS에서만 가능, 지금은 Compute Shader 시점이라 SampleLevel 사용
                // Noise Texture 1번은 무채색이라 RGB가 똑같음 -> 추출한 컬러의 x,y,z 어떤 것을 사용해도 똑같음 
                // LOD는 현재 2D라 사용을 안하기에 원본 Level 0 밖에 없음
                // Noise Texture 3번은 RGB가 다 다름 -> vRand[0], vRand[1], vRand[2] 활용 가능
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                        
                // SpawnShape 가 Sphere 타입이라면
                if (0 == Module.SpawnShape)
                {
                    // vRand.x는 0~1 사이값
                    // RandomRadius는 Module.Radius가 100이니까 0~100 사이의 랜덤값
                    float RandomRadius = vRand[0] * Module.Radius;
                    // 회전 각도 랜덤값(2 * PI = 360도)
                    float RandomAngle = vRand[1] * 2 * PI;
                    
                    // Particle 컴포넌트(본체) 의 중심위치(월드) 에서
                    // 랜덤 각도, 랜덤 반지름에 해당하는 위치를 계산해서 파티클의 초기 위치로 준다.
                    Particle.vLocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
      
                }
                // SpawnShape 가 Box 타입이라면
                else
                {
                    Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale.x - (Module.vSpawnBoxScale.x / 2.f);
                    Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale.y - (Module.vSpawnBoxScale.y / 2.f);
                    Particle.vLocalPos.z = 0.f;
                }
                
                // Worldpos는 Sphere, Box 타입 모두 동일
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // 스폰 컬러 설정
                Particle.vColor = Module.vSpawnColor;
                
                // 스폰 초기 크기 설정                
                Particle.vWorldInitScale = Particle.vWorldScale = (Module.vSpawnMaxScale - Module.vSpawnMinScale) * vRand[2] + Module.vSpawnMinScale;
     
                // Age 초기화
                Particle.Age = 0.f;
                
                // 스폰 Life 설정
                Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MaxLife;
                         
                 // 스폰 Mass 설정
                Particle.Mass = clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                
                // Add VelocityModule
                if (Module.arrModuleCheck[3])
                {
                    // 0 : From Center
                    // Local 좌표계가 부모로부터 상대적인 좌표계니까 Local Pos가 곧 이동해야 할 방향
                    if (0 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Particle.vLocalPos.xyz);
                        // clamp(vRand[2]의 값을 MinSpeed과 MaxSpeed 사이의 값으로 return)
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    // 1 : To Center
                    if (1 == Module.AddVelocityType)
                    {
                        float3 vDir = -normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                }
                else
                {
                    // 속도 초기화
                     Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                }
                
                break;
            }
        }
    }
  
    
    // 파티클이 활성화 상태라면
    else
    {
       Particle.Age += g_dt;
        // 본인의 나이가 수명보다 커지면 비활성화
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = 0;
            return;
        }
        
         // 랜덤값 추출(1) - 샘플링 코드 직접 구현한 버전
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
         // 랜덤값 추출(2)
        //vUV.x += g_time * 0.2f;
        //vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        //float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        // 이번 프레임에서 받는 힘을 초기화
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // Normalize Age(0~1 사이의 비율로 정규화된 나이) 계산
        Particle.NomalizedAge = Particle.Age / Particle.Life;
        
        // Scale 모듈
        if (Module.arrModuleCheck[2])
        {
            // NomalizedAge가 1에 도달했을 때 최종 scale 비율이 되어야 함
            Particle.vWorldScale = Particle.vWorldInitScale * (1.f + (Module.vScaleRatio - 1.f) * Particle.NomalizedAge);
        }
        
          // Noise Force
        if (Module.arrModuleCheck[4])
        {
            // NoiseForce를 한 번도 세팅받은 적이 없다면
            if (Particle.NoiseForceTime == 0.f)
            {
                // normalize(Rand.xyz * 2.f - 1.f) : 나한테 주어질 힘의 방향으로 생각
                // * 2.f - 1.f : 랜덤범위가 양수로만 되다보니 파티클의 방향이 대체적으로 우상단으로 몰리기 때문에 범위를 0~1 -> -0.5~0.5로 변경
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                
                // NoiseForce를 세팅받은 시간을 기록
                Particle.NoiseForceTime = g_time;
            }
            // 텀만큼 차이가 나지 않으면 NoiseForce를 새로 주지 X
            // 텀을 넘어가면 새롭게 NoiseForce 세팅
            else if (Module.NoiseForceTerm < g_time - Particle.NoiseForceTime)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
        }
        
        // Calculate Force
        if (Module.arrModuleCheck[5])
        {
            // 누적된 힘 연산
            Particle.vForce.xyz += Particle.vNoiseForce.xyz;
            
            // Force 연산
            // F = M x A
            // A = F / M
            // 가속도 계산
            float3 vAccel = Particle.vForce.xyz / Particle.Mass;
            
            // Accel 연산(속도에 가속도를 더함)
            Particle.vVelocity.xyz += vAccel * g_dt;
            
            // Velocity 연산
            // Local Space였을 경우
            if (0 == Module.SpaceType)
            {
                Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_dt;
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
            }
            // World Space였을 경우
            else if (1 == Module.SpaceType)
            {
                Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_dt;
            }
        }
      
    }
    
}

#endif