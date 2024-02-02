#ifndef _PARTICLE
#define _PARTICLe

#include "value.fx"
#include "struct.fx"

// Geometry Shader : ������ ������ų �� �ִ� Shader == ������ ������ �ʰ� �Ǹ� �ƹ� ������ ���� ���·� ���� ���� ����
// 1. ��� ��ƼŬ�� ��Ȱ��ȭ ������ ���, �������� �������� �ܰ迡�� �ߴܽ�Ű��
// 2. ������ ������ ���Ǽ�

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule> g_ParticleModule : register(t21);

// g_int_0 �ε����� �ش��ϴ� �� ��ƼŬ ��ü�� ��ũ�η�
#define Particle g_ParticleBuffer[_in.iInstID]

struct VS_IN
{
    // ���� pos
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    // �ν��Ͻ����� �������� �� ���°�� �������ϰ� �ִ��� ����
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

// ������ ���� ������ Geometry Shader�� ���� ������ ���̱� ������ 
// Vertex Shader �ܰ迡���� �Է����� ���� ���� �״�� �����ֱ⸸ �ϸ� ��
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
    float InstID : FOG;
};

// Geometry Shader������ ���� ������ ������� ��, maxvertexcount(6) = �ʿ��� ������ ������ 6��
[maxvertexcount(6)]
// _OutStream : Geometry Shader�� ���� ������� ���� ��� ��Ʈ��
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    // �迭�� 4���� �����ϰ� ���� �����ϴ� ������ 6��(��ø�Ǵ� ��ġ�� �����״ϱ�)
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // GS �� ����ϴ� ��ƼŬ ������ �����´�.
    tParticle particle = g_ParticleBuffer[(int) _in[0].InstID];
    
    // ��ƼŬ�� ��Ȱ��ȭ ���¶�� return
    if (false == particle.Active)
    {
        return;
    }
    
    // ���� ����� ��ƼŬ ��ü�� �ƴ� ������Ʈ�� ���� ����̱� ������ Pass
    // ��ƼŬ�� �߽����� ����� �̵�(�߽����� ���� ��ǥ == ��ƼŬ�� ���� ��ǥ)
    // Local ��ǥ�迡�� ������ 0,0�̶� Scale�� ���غ��� �ǹ̰� ���� ������ �ϴ� ���� ��ġ�� �̵�
    float3 vWorldPos = particle.vWorldPos.xyz;
    
    // ��ƼŬ�� ViewSpace �󿡼��� �߽� ������ ���ϱ�
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // 0 -- 1	     
	// |    |	     
	// 3 -- 2
    // View Space �������� �߽����� �������� 4���� ���� ����(vViewPos�� ���ع����� ������ �Ǿ������ ���߿� ���) - ������ ����
    output[0].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    
     // ������� ���
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // �ӵ��� ���� ���� ��� - ȸ��
        if (g_ParticleModule[0].VelocityAlignment)
        {
            // ��������� view space�� �����ͼ� ����ȭ�� ���� �캤��
            float3 vR = normalize(mul(float4(particle.vVelocity.xyz, 1.f), g_matView).xyz);
            // cross�� ����� �� ���ڰ� ���� ���Ϳ��� ������ ������� �������ʹ� �ƴ� -> ����ȭ�� ������ ����� ��
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));
            
            // 3*3 ���
            float3x3 vRot =
            {
                vR,
                vU,
                vF
            };
            
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
            }
        }
    }
        
    for (int i = 0; i < 4; ++i)
    {
        // view space�� �߽� ������ ��
        output[i].vPosition.xyz += vViewPos.xyz;
    }
    
    // ������� ����
    output[0].vPosition = mul(output[0].vPosition, g_matProj);
    output[1].vPosition = mul(output[1].vPosition, g_matProj);
    output[2].vPosition = mul(output[2].vPosition, g_matProj);
    output[3].vPosition = mul(output[3].vPosition, g_matProj);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    // Append : 3���� ��� �ϳ��� �ﰢ���̶�� ���� �˷���
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
}


// PS�� ���Դٸ� �̹� Active�� true
float4 PS_Particle(GS_OUT _in) : SV_Target
{
    
    float4 vOutColor = g_ParticleBuffer[(uint) _in.InstID].vColor;
    
    if (g_btex_0)
    {
        // ��ƼŬ �ؽ�ó���� �� ��ä���̶� ���� ��ƼŬ ���� �ؽ�ó�� ������ �����ָ� ��ƼŬ ������ ������ �ؽ�ó�� ��µ�
        vOutColor *= g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    return vOutColor;
    
}

#endif
