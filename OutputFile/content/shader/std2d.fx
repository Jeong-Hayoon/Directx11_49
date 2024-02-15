#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"


// 구조화 버퍼는 텍스처 레지스터에 바로 바인딩할 수 있는 구조라서 자료형을 따로 명시해주지 않아도 됨
// 14번 레지스터에 float4 타입 구조화 버퍼
StructuredBuffer<float4> g_Data : register(t14);


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
    
    float3 vWorldPos : POSITION;
    
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
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
 
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
    
    // Shader Parameter Test용 1
    // if (g_int_0)
        // return float4(1.f, 1.f, 0.f, 1.f);
    
    // Shader Parameter Test용 2
    if (g_vec4_1.w == 3.14f)
        return float4(1.f, 1.f, 0.f, 1.f);
    
    if (g_UseAnim2D)
    {
        // g_vLeftTop + (g_vSlizeSize / 2.f) : Center
        // center에서 Background의 절반만큼 빼게 되면 vBackgroundLeftTop가 나옴
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        
        // Background 자체를 이동
        // UV 값보다 +한 곳을 가져온다는 건 +한 만큼 실제 출력되는 이미지가 왼쪽으로 반대로 가는 개념
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        // Background 때문에 생긴 필요없는 부분들은 discard
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSlizeSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSlizeSize.y) < vUV.y)
        {
           // vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
        
        // 마젠타 색상 빼기
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            discard;           
        }
        
        
    }
    else
    {
          // tex_0가 true면 텍스처 출력, 그게 아니라면 기본 컬러 마젠타로 출력
        if (g_btex_0)
        {
            // WRAP 샘플링 방식을 통해 물이 흐르는 듯한 효과를 낼 수 있음(누적 시간 값이 전달되었기 때문에 가능)
            // vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
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
    }
    
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    // g_Light2DCount;    
    
    tLightColor LightColor = (tLightColor) 0.f;
    
    // 반복문을 돌면서 받는 빛의 양 누적
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    // 최소로 보장되는 환경광은 있을 수 있으니까 
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    // 최종 출력할 alpha값이 0이면
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}


// EffectShader(VS는 동일, PS는 alpha 값 적용)
float4 PS_Std2D_Effect(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSlizeSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSlizeSize.y) < vUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
    }
    else
    {
        if (g_btex_0)
        {
            //vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
                    
            //saturate 0 ~ 1 을 넘지 않게 보정
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
            if (fAlpha < 0.1f)
            {
            // 픽셀 쉐이더를 중간에 폐기처리
                discard; //clip(-1);            
            }
        }
    }
        
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    return vColor;
}


#endif



