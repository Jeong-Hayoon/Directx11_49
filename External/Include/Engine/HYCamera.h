#pragma once
#include "HYComponent.h"

// 카메라를 매니저가 아닌 카메라 역할 하는 오브젝트가 카메라로 취급
// 앞으로 랜더링 기능은 카메라가 함
class HYCamera :
    public HYComponent
{
private:
    // 원근투영(Perspective)을 할 경우
    float   m_FOV;          // 시야 각

    // 직교투영(Orthographic)을 할 경우
    float   m_Width;        // 직교투영 가로 길이
    float   m_Scale;        // 직교투영 배율

    // Both(원근투영과 직교투영 둘 다 필요한 데이터)
    float   m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float   m_Far;          // 투영 최대 거리

    // 변환 행렬
    Matrix  m_matView;      // 뷰 행렬
    Matrix  m_matProj;      // 투영 행렬

public:
    virtual void finaltick() override;

public:
    HYCamera();
    ~HYCamera();
};

