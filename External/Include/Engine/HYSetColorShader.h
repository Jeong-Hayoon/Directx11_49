#pragma once
#include "HYComputeShader.h"

// Texture를 특정 컬러로 색칠하는 Shader
class HYSetColorShader :
    public HYComputeShader
{
private:
    Ptr<HYTexture>   m_TargetTex;
    Vec4            m_Color;

public:
    // 무슨 Texture를
    void SetTargetTexture(Ptr<HYTexture> _tex) { m_TargetTex = _tex; }
    // 무슨 색상으로
    void SetColor(Vec3 _Color) { m_Color = _Color; }

public:
    virtual int UpdateData() override;
    virtual void Clear() override;

public:
    HYSetColorShader();
    ~HYSetColorShader();
};

