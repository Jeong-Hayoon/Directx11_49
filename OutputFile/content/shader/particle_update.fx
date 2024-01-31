#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

StructuredBuffer<tParticleModule>       g_Module : register(t20);
RWStructuredBuffer<tParticle>           g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount>         g_SpawnCount : register(u1);


#define MAX_COUNT   g_int_0             // ��ƼŬ�� �ִ� ����
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]
#define CenterPos   g_vec4_0.xyz        // ��ƼŬ�� �߽� ��ġ��

// �׷� �ϳ��� 1024���� ���η� �Ϸķ� �� ������� ����
[numthreads(32, 1, 1)]
// �ڵ� ���� �ʿ�
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    // Thread�� ���ϰ� �������� �ʵ��� ����ó��
    if (MAX_COUNT <= id.x)
        return;
    
    // ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (0 == Particle.Active)
    {
        // ���� ��� Ȱ��ȭ üũ
        if (0 == Module.arrModuleCheck[0])
            return;
        
        while (0 < SpawnCount)
        {
            // Atomic �Լ� 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
           
            // InterlockedExchange �Լ��� �Ἥ SpawnCount �� ��ü, �����ϸ�
            // �ʱ� �õ��� ���尡 ���� �����尡 ������ ���Ŀ� �����ϴ� ��찡 �ִ�. 
            // �̶� SpawnCount �� ������ �÷������� ������ �߻��� �� �ִ�. 
            // InterlockedCompareExchange �� ���ؼ� ������ ���� ��ġ�� ��쿡�� 
            // ��ü�� �ϵ��� �ϴ� �Լ��� ����Ѵ�.
            InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            
            // InterlockedExchange(SpawnCount, Exchange, Origin);
            
            // ��Ȱ��ȭ ���� -> Ȱ��ȭ�� ��ȯ
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // ����
                // MAX_COUNT - 1 : ���
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                // g_time�� ������ �ְ� �Ǹ� Texture ������ �����ϴ� �ӵ��� �ʹ� ����������
                vUV.x += g_time * 0.2f;
                //                 ( ���ļ� )    (����)  (V �� offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                // SampleLevel : Sample�� PS������ ����, ������ Compute Shader �����̶� SampleLevel ���
                // Noise Texture 1���� ��ä���̶� RGB�� �Ȱ��� -> ������ �÷��� x,y,z � ���� ����ص� �Ȱ��� 
                // LOD�� ���� 2D�� ����� ���ϱ⿡ ���� Level 0 �ۿ� ����
                // Noise Texture 3���� RGB�� �� �ٸ� -> vRand[0], vRand[1], vRand[2] Ȱ�� ����
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                        
                // SpawnShape �� Sphere Ÿ���̶��
                if (0 == Module.SpawnShape)
                {
                    // vRand.x�� 0~1 ���̰�
                    // RandomRadius�� Module.Radius�� 100�̴ϱ� 0~100 ������ ������
                    float RandomRadius = vRand[0] * Module.Radius;
                    // ȸ�� ���� ������(2 * PI = 360��)
                    float RandomAngle = vRand[1] * 2 * PI;
                    
                    // Particle ������Ʈ(��ü) �� �߽���ġ(����) ����
                    // ���� ����, ���� �������� �ش��ϴ� ��ġ�� ����ؼ� ��ƼŬ�� �ʱ� ��ġ�� �ش�.
                    Particle.vLocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
      
                }
                // SpawnShape �� Box Ÿ���̶��
                else
                {
                    Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale.x - (Module.vSpawnBoxScale.x / 2.f);
                    Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale.y - (Module.vSpawnBoxScale.y / 2.f);
                    Particle.vLocalPos.z = 0.f;
                }
                
                // Worldpos�� Sphere, Box Ÿ�� ��� ����
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // ���� �÷� ����
                Particle.vColor = Module.vSpawnColor;
                
                // ���� �ʱ� ũ�� ����                
                Particle.vWorldInitScale = Particle.vWorldScale = (Module.vSpawnMaxScale - Module.vSpawnMinScale) * vRand[2] + Module.vSpawnMinScale;
     
                // Age �ʱ�ȭ
                Particle.Age = 0.f;
                
                // ���� Life ����
                Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MaxLife;
                         
                 // ���� Mass ����
                Particle.Mass = clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                
                // Add VelocityModule
                if (Module.arrModuleCheck[3])
                {
                    // 0 : From Center
                    // Local ��ǥ�谡 �θ�κ��� ������� ��ǥ��ϱ� Local Pos�� �� �̵��ؾ� �� ����
                    if (0 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Particle.vLocalPos.xyz);
                        // clamp(vRand[2]�� ���� MinSpeed�� MaxSpeed ������ ������ return)
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
                    // �ӵ� �ʱ�ȭ
                     Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                }
                
                break;
            }
        }
    }
  
    
    // ��ƼŬ�� Ȱ��ȭ ���¶��
    else
    {
       Particle.Age += g_dt;
        // ������ ���̰� ������ Ŀ���� ��Ȱ��ȭ
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = 0;
            return;
        }
        
         // ������ ����(1) - ���ø� �ڵ� ���� ������ ����
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
         // ������ ����(2)
        //vUV.x += g_time * 0.2f;
        //vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        //float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        // �̹� �����ӿ��� �޴� ���� �ʱ�ȭ
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // Normalize Age(0~1 ������ ������ ����ȭ�� ����) ���
        Particle.NomalizedAge = Particle.Age / Particle.Life;
        
        // Scale ���
        if (Module.arrModuleCheck[2])
        {
            // NomalizedAge�� 1�� �������� �� ���� scale ������ �Ǿ�� ��
            Particle.vWorldScale = Particle.vWorldInitScale * (1.f + (Module.vScaleRatio - 1.f) * Particle.NomalizedAge);
        }
        
          // Noise Force
        if (Module.arrModuleCheck[4])
        {
            // NoiseForce�� �� ���� ���ù��� ���� ���ٸ�
            if (Particle.NoiseForceTime == 0.f)
            {
                // normalize(Rand.xyz * 2.f - 1.f) : ������ �־��� ���� �������� ����
                // * 2.f - 1.f : ���������� ����θ� �Ǵٺ��� ��ƼŬ�� ������ ��ü������ �������� ������ ������ ������ 0~1 -> -0.5~0.5�� ����
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                
                // NoiseForce�� ���ù��� �ð��� ���
                Particle.NoiseForceTime = g_time;
            }
            // �Ҹ�ŭ ���̰� ���� ������ NoiseForce�� ���� ���� X
            // ���� �Ѿ�� ���Ӱ� NoiseForce ����
            else if (Module.NoiseForceTerm < g_time - Particle.NoiseForceTime)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
        }
        
        // Calculate Force
        if (Module.arrModuleCheck[5])
        {
            // ������ �� ����
            Particle.vForce.xyz += Particle.vNoiseForce.xyz;
            
            // Force ����
            // F = M x A
            // A = F / M
            // ���ӵ� ���
            float3 vAccel = Particle.vForce.xyz / Particle.Mass;
            
            // Accel ����(�ӵ��� ���ӵ��� ����)
            Particle.vVelocity.xyz += vAccel * g_dt;
            
            // Velocity ����
            // Local Space���� ���
            if (0 == Module.SpaceType)
            {
                Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_dt;
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
            }
            // World Space���� ���
            else if (1 == Module.SpaceType)
            {
                Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_dt;
            }
        }
      
    }
    
}

#endif