#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// 화면 전체에 PS가 호출되는 것이 목적
// RectMesh를 가져와서 x,y,z를 2배로 해주면 투영 스페이스 
// 기준 전체 영역으로 늘려서 강제 출력

// mesh : RectMesh 고정
struct VS_IN
{
    // Mesh의 로컬 좌표
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ==========
// GrayFilter
// ==========
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

// 흑백 필터(화면 전체)
float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    
    // 회색
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}

// ==========
// Distortion 
// ==========
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // PixelShader에 SV_Position 으로 입력된 값 == PixelShader를 호출한 해당 픽셀의 좌표
    // _in.vPosition.xy : 호출된 픽셀의 좌표(SV_Position만 예외)
    // 해상도로 나눠주게 되면 전체 대비 위치가 비율로 나옴 -> 이를 UV로 사용
    // vScreenUV - 전체 화면을 기준으로 한 UV 좌표
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // ===== 출렁거리는 효과 ===== 
    // g_time : 고정되지 않고 움직이도록
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    // ===========================
    if (g_btex_0)
    {
        ///////////////////////////////////
        // 미세하게 엇갈리게 오려 붙이기 // -> 캐릭터 은신 효과, 스킬 이펙트, 물 일렁임, 거울 반사 등 가능해짐
        ///////////////////////////////////
        float2 vUV = _in.vUV;
        
        // 추출하는 부위가 시간에 따라 달라짐
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        // vNoise.xy == vNoise.rg
        // Noise의 랜덤 범위 : 0~1
        vNoise = (vNoise.xy - 0.5f) * 0.1f;     // 랜덤 범위 -0.5~0.5의 10퍼센트
        
        vScreenUV += vNoise;
    }
        
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}

#endif