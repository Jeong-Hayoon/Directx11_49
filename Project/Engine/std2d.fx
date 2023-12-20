#ifndef _STD2D
#define _STD2D

// #ifndef _STD2D, #define _STD2D, #endif -> c 스타일의 #pragma once
// _STD2D가 정의되어 있지 않다면 
// #pragma once : 전방선언 중첩을 막아줌(c++)

// 4096 크기제한
// b0 : 상수 버퍼의 네임
cbuffer TRANSFORM : register(b0)
{
    // row_major : gpu가 메모리를 읽어들이는 방식이 열 기반이라 전치를 해줘야 함
    // 행우선으로 전치시켜서 우리가 원하는 값으로 보내는 것
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    
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
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    // mul : hlsl에서 좌표와 벡터에 행렬을 곱해주는 함수
    // float4(_in.vPos, 1.f) : float3로 들어온 값을 동차 좌표계로 변환(기타 생성자)
    // 로컬 좌표계 -> 월드 좌표계로 한 번에 변환(월드 변환)
    // output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    // g_matWorld : 월드 좌표계로 변환
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
            
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
 
    return output;
}

// VS_Std2D에서 반환된 값이 인자로 들어옴
// 정점 내부에 픽셀이 채워지는 거라서
// 어떤 값이든 정점에서 리턴시킨 값이 픽셀로 들어올 떄에 
// 정점의 위치(거리0에 따른 선형 보간이 된 값이 
// Pixel Shader의 입력으로 들어옴
// SV_Target : 반환 타입을 설명해주는 시멘틱 -> 타겟의 의미는 랜더 타겟
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f); -> 빨간색으로 세팅
    
    _in.vColor.a = 0.5f;
    return _in.vColor;    
    
}

#endif
