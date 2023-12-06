#ifndef _STD2D
#define _STD2D

// #ifndef _STD2D, #define _STD2D, #endif -> C 스타일의 #pragma once
// Shader 코드는 리소스처럼 활용이 됨(런타임 도중 실시간으로 컴파일하므로0
// 텍스처 좌표 - 좌상단이 (0,0)이고 우하단이 (1,1)인 좌표계

struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// VS_IN : 입력 구조체 / VS_OUT : 출력 구조체
// 들어온 값 그대로 출력
//
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPosition = float4(_in.vPos.xy, 0.f, 1.f);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    

    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f); -> 빨간색으로 세팅
    
    return _in.vColor;    
    
}

#endif
