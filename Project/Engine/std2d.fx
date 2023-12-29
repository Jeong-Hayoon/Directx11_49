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
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
    
}

// GPU메모리는 최소 단위가 16byte이기 때문에 하나하나씩 적어줘야 함
// 배열로 묶어버리면 배열의 최소 단위가 16byte 4개로 생각함
// 하나하나 적으면 16byte안에 int 4개가 들어있다고 봄
cbuffer MATERIAL_CONST : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    // 레지스터에 바인딩된 Texture가 있는지 check하는 용도의 bool값
    // (최소단위 때문에 bool 변수는 사용 불가능)
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btexcube_0;
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;
}

// 이 텍스처에 들어 있는 이미지 색상 정보 = Sample
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

// Sampling : 텍스처 추출
// Sampler : 샘플링을 위한 도구
// 저해상도 텍스처를 사용할 때 보간의 방법 설정
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


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

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
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
    // GetDimensions : 바인딩되어 있는 정보를 구할 수 있는 함수
    // uint width = 0;
    // uint height = 0;
    // g_tex_1.GetDimensions(width, height) == 0이면 바인딩된 Texture가 없다고 판단
    // 대신 이 함수는 모든 픽셀에 대해 호출이 되서 Shader의 연산 속도가 느려지니까
    // Texture가 바인딩되어있는지 Check하는 bool변수를 둬서 관리하는게 나음
    
    // Texture가 nullptr일 때
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // tex_0가 true면 텍스처 출력, 그게 아니라면 기본 컬러 마젠타로 출력
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        // Clamp 함수(값을 특정 범위 내에 가둬놓고 싶을 때 사용하는 함수)
        // saturate : 0 ~ 1 을 넘지 않게 보정(0~1 넘는 값을 잘라버리는 함수)
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        // 샘플링한 색상이 마젠타에 가까운 색상이라면 색상을 날려버림
        if (fAlpha < 0.1f) 
        {
            // discard : 픽셀 쉐이더를 중간에 폐기 처리 -> 더이상 PipeLine OM단계까지 가지 않으며 깊이 저장도 안됨
            discard; //clip(-1); - 유사 용어, -1이 전달되면 discard 호출           
        }
    }
   
    return vColor;
}

#endif


 //if (vColor.a <= 0.1f)
 //{
 //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
 //}
 
 //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
 //vColor.rgb = float3(Aver, Aver, Aver);    
 //vColor.a = 1.f;
 
 //vColor.rgb *= 1.5f; => Blend State 때문에 약해진 색을 더 진하게