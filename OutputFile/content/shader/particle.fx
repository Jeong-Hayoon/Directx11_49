#ifndef _PARTICLE
#define _PARTICLe

#include "value.fx"
#include "struct.fx"

// Geometry Shader : 정점을 생성시킬 수 있는 Shader == 정점을 만들지 않게 되면 아무 정점도 없는 상태로 만들 수도 있음
// 1. 담당 파티클이 비활성화 상태인 경우, 렌더링을 정점연산 단계에서 중단시키기
// 2. 빌보드 구현의 편의성

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule> g_ParticleModule : register(t21);

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
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

// 정점에 대한 연산은 Geometry Shader를 통해 진행할 것이기 때문에 
// Vertex Shader 단계에서는 입력으로 들어온 것을 그대로 보내주기만 하면 됨
VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.InstID = _in.iInstID;
    
    return output;
}


struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

// Geometry Shader에서는 설명 정보를 적어줘야 함, maxvertexcount(6) = 필요한 정점의 개수가 6개
[maxvertexcount(6)]
// _OutStream : Geometry Shader가 최종 결과물을 담을 출력 스트림
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    // 배열은 4개를 선언하고 최종 생성하는 정점은 6개(중첩되는 위치가 있을테니까)
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    GS_OUT output_cross[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // GS 가 담당하는 파티클 정보를 가져온다.
    tParticle particle = g_ParticleBuffer[(int) _in[0].InstID];
    
    // 파티클이 비활성화 상태라면 return
    if (false == particle.Active)
    {
        return;
    }
    
    // 월드 행렬은 파티클 자체가 아닌 오브젝트의 월드 행렬이기 때문에 Pass
    // 파티클의 중심점을 월드로 이동(중심점의 월드 좌표 == 파티클의 월드 좌표)
    // Local 좌표계에서 원점인 0,0이라 Scale을 곱해봤자 의미가 없기 때문에 일단 월드 위치로 이동
    float3 vWorldPos = particle.vWorldPos.xyz;
    
    // 파티클의 ViewSpace 상에서의 중심 포지션 구하기
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // 0 -- 1	     
	// |    |	     
	// 3 -- 2
    // View Space 공간에서 중심점을 기쥰으로 4개의 점을 만듦(vViewPos를 더해버리면 공전이 되어버려서 나중에 계산) - 빌보드 적용
    output[0].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    
    output_cross[0].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[1].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[2].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    output_cross[3].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    
    output_cross[0].vUV = output[0].vUV = float2(0.f, 0.f);
    output_cross[1].vUV = output[1].vUV = float2(1.f, 0.f);
    output_cross[2].vUV = output[2].vUV = float2(1.f, 1.f);
    output_cross[3].vUV = output[3].vUV = float2(0.f, 1.f);
    
     // 렌더모듈 기능
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능 - 회전
        if (g_ParticleModule[0].VelocityAlignment)
        {
            // 진행방향을 view space로 가져와서 정규화한 것이 우벡터
            float3 vR = normalize(mul(float4(particle.vVelocity.xyz, 0.f), g_matView).xyz);
            // cross에 사용한 두 인자가 단위 벡터여도 외적의 결과값이 단위벡터는 아님 -> 정규화를 별도로 해줘야 함
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));
            
            // 3*3 행렬
            float3x3 vRot =
            {
                vR,
                vU,
                vF
            };
            
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
                output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
            }
        }
    }
       
    // View 좌표로 이동, 투영행렬 적용
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
        
        output_cross[i].vPosition.xyz += vViewPos.xyz;
        output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        
        output[i].InstID = _in[0].InstID;
        output_cross[i].InstID = _in[0].InstID;
    }
   
    
    // Append : 3개씩 끊어서 하나의 삼각형이라는 것을 알려줌
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[2]);
            _OutStream.Append(output_cross[3]);
            _OutStream.RestartStrip();
    
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[1]);
            _OutStream.Append(output_cross[2]);
            _OutStream.RestartStrip();
        }
    }
}


// PS에 들어왔다면 이미 Active가 true
float4 PS_Particle(GS_OUT _in) : SV_Target
{
    
    tParticle particle = g_ParticleBuffer[_in.InstID];
    tParticleModule module = g_ParticleModule[0];
    
    // 출력 색상
    float4 vOutColor = particle.vColor;
    
    if (g_btex_0)
    {
        float4 vSampleColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        vOutColor.rgb *= vSampleColor.rgb;
        vOutColor.a = vSampleColor.a;
    }
    
    // 렌더모듈이 켜져 있으면
    if (module.arrModuleCheck[6])
    {
        if (1 == module.AlphaBasedLife)
        {
            vOutColor.a = saturate(1.f - clamp(particle.NomalizedAge, 0.f, 1.f));
        }
        else if (2 == module.AlphaBasedLife)
        {
            float fRatio = particle.Age / module.AlphaMaxAge;
            vOutColor.a = saturate(1.f - clamp(fRatio, 0.f, 1.f));
        }
    }
    
    return vOutColor;
    
}

#endif
