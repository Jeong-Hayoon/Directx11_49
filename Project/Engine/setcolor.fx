#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

// u 레지스터 : 샘플링 추출도 가능하면서 반대로 색을 입힐 수도 있음(unordered access)
// u 레지스터에 데이터를 보낼 때는 D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS Flag를
// 통해 UnorderedAccessView를 만들고 이를 통해서만 가능(Texture도 마찬가지로 UNORDERED_ACCESS여야 함)
// RWTexture2D : Read/Write 가능한 텍스처
RWTexture2D<float4> g_TargetTex : register(u0);

// numthreads : 그룹 하나의 스레드 차원 수(하나의 그룹에 스레드 x,y,z가 있다)
// hlsl 5.0, 하나의 그룹이 가질수 있는 최대 스레드개수 1024개
// SV_GroupID : 소속 그룹 ID
// SV_GroupThreadID : 소속 그룹 내에서의 ID
// SV_GroupIndex : 소속 그룹내에서의 ID 를 1차원으로 변환
// SV_DispatchThreadID : 모든 그룹과 그룹내의 스레드를 통틀어서 전체 기준, 호출된 해당 스레드의 ID
[numthreads(32, 32, 1)]
// id : 본인이 무슨 그룹이고 
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (TEX_WIDTH <= id.x || TEX_HEIGHT <= id.y)
    {
        return;
    }
    
    // 본인의 id 자체가 픽셀 그 자체가 됨(우리 엔진 상)
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
