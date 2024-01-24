#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

// u �������� : ���ø� ���⵵ �����ϸ鼭 �ݴ�� ���� ���� ���� ����(unordered access)
// u �������Ϳ� �����͸� ���� ���� D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS Flag��
// ���� UnorderedAccessView�� ����� �̸� ���ؼ��� ����(Texture�� ���������� UNORDERED_ACCESS���� ��)
// RWTexture2D : Read/Write ������ �ؽ�ó
RWTexture2D<float4> g_TargetTex : register(u0);

// numthreads : �׷� �ϳ��� ������ ���� ��(�ϳ��� �׷쿡 ������ x,y,z�� �ִ�)
// hlsl 5.0, �ϳ��� �׷��� ������ �ִ� �ִ� �����尳�� 1024��
// SV_GroupID : �Ҽ� �׷� ID
// SV_GroupThreadID : �Ҽ� �׷� �������� ID
// SV_GroupIndex : �Ҽ� �׷쳻������ ID �� 1�������� ��ȯ
// SV_DispatchThreadID : ��� �׷�� �׷쳻�� �����带 ��Ʋ� ��ü ����, ȣ��� �ش� �������� ID
[numthreads(32, 32, 1)]
// id : ������ ���� �׷��̰� 
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (TEX_WIDTH <= id.x || TEX_HEIGHT <= id.y)
    {
        return;
    }
    
    // ������ id ��ü�� �ȼ� �� ��ü�� ��(�츮 ���� ��)
    if (5 == id.x && 1 == id.y)
    {
        g_TargetTex[id.xy] = float4(0.f, 1.f, 1.f, 1.f);
    }
    else
    {
        g_TargetTex[id.xy] = float4(g_vec4_0.rgb, 1.f);
    }
}


#endif
