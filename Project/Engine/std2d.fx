#ifndef _STD2D
#define _STD2D


struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
};

// VS_IN : 입력 구조체 / VS_OUT : 출력 구조체
// 들어온 값 그대로 출력
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPosition = float4(_in.vPos.xy, 0.f, 1.f);
    output.vColor = _in.vColor;

    return output;
}

// PS - 빨간색으로 세팅
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}

#endif
