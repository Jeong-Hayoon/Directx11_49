#ifndef _POSTPROCESS
#define _POSTPROCESS

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

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = float4(0.3f, 0.3f, 0.3f, 1.f);
    
    return vColor;
}





#endif