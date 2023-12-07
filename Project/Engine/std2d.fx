#ifndef _STD2D
#define _STD2D

// #ifndef _STD2D, #define _STD2D, #endif -> c 스타일의 #pragma once
// _STD2D가 정의되어 있지 않다면 
// #pragma once : 전방선언 중첩을 막아줌(c++)

// 4096 크기제한
// b0 : 상수 버퍼의 네임
cbuffer TRANSFORM : register(b0)
{
    float4 g_vWorldPos;
    float4 g_vWorldScale;
}

// Shader 코드는 리소스처럼 활용이 됨(런타임 도중 실시간으로 컴파일하므로0
// 텍스처 좌표 - 좌상단이 (0,0)이고 우하단이 (1,1)인 좌표계
// hlsl로 작성한 코드를 컴파일시켜서 GPU가 이해할 수 있는 언어로 바꿔줘야 함   
// 정규화된 정사각형에 이미지가 맞춰서 들어오기 떄문에 
// 리소스가 찌그러진 형태든 상관이 없음, 회전도 쉬워짐
// 호출되는 픽셀마다 UV에 맞춰서 색을 가져오는 개념이기때문에
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
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    // 최종 좌표는 Scale 조절을 먼저하고 좌표 이동을 하게 됨
    float2 vFinalPos = _in.vPos.xy * g_vWorldScale.xy + g_vWorldPos.xy;
    
    output.vPosition = float4(vFinalPos, 0.f, 1.f);
    
    // output.vPosition = float4(_in.vPos.xy, 0.f, 1.f);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    

    return output;
}

// VS_Std2D에서 반환된 값이 인자로 들어옴
// 정점 내부에 픽셀이 채워지는 거라서
// 어떤 값이든 정점에서 리턴시킨 값이 픽셀로 들어올 떄에 
// 정점의 위치(거리0에 따른 선형 보간이 된 값이 
// Pixel Shader의 입력으로 들어옴
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f); -> 빨간색으로 세팅
    
    return _in.vColor;    
    
}

#endif
