#pragma once
#include "HYComputeShader.h"

// Texture를 특정 컬러로 색칠하는 Shader
class HYSetColorShader :
    public HYComputeShader
{
private:
    Ptr<HYTexture>   m_TargetTex;
    Vec4             m_Color;

public:
    // 무슨 Texture한테 색칠을 할건지
    void SetTargetTexture(Ptr<HYTexture> _tex) { m_TargetTex = _tex; }
    // 무슨 색상으로 색칠할건지
    void SetColor(Vec3 _Color) { m_Color = _Color; }

public:
    // Shader가 필요한 데이터들을 업데이트 시켜 놓아라
    virtual int UpdateData() override;
    virtual void Clear() override;

public:
    HYSetColorShader();
    ~HYSetColorShader();
};

