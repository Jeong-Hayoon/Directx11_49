#ifndef _VALUE
#define _VALUE

// #ifndef _STD2D, #define _STD2D, #endif -> c 스타일의 #pragma once
// _STD2D가 정의되어 있지 않다면 
// #pragma once : 전방선언 중첩을 막아줌(c++)

// 4096 크기제한
// b0 : 상수 버퍼의 네임

// 4096 크기제한
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


#endif