#pragma once
#include "HYComputeShader.h"

// Texture�� Ư�� �÷��� ��ĥ�ϴ� Shader
class HYSetColorShader :
    public HYComputeShader
{
private:
    Ptr<HYTexture>   m_TargetTex;
    Vec4            m_Color;

public:
    // ���� Texture��
    void SetTargetTexture(Ptr<HYTexture> _tex) { m_TargetTex = _tex; }
    // ���� ��������
    void SetColor(Vec3 _Color) { m_Color = _Color; }

public:
    virtual int UpdateData() override;
    virtual void Clear() override;

public:
    HYSetColorShader();
    ~HYSetColorShader();
};

