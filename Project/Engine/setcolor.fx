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
[numthreads(32, 32, 1)]
// id : ������ ���� �׷��̰� 
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (TEX_WIDTH <= id.x || TEX_HEIGHT <= id.y)
    {
        return;
    }
    
    // ������ id ��ü�� �ȼ� �� ��ü�� ��(�츮 ���� ��)
    g_TargetTex[id.xy] = float4(g_vec4_0.rgb, 1.f);
}


#endif
