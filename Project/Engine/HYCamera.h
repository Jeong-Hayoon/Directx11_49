#pragma once
#include "HYComponent.h"

// ���� ���
enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // ���� ����
    PERSPECTIVE,  // ���� ����
};


// ī�޶� �Ŵ����� �ƴ� ī�޶� ���� �ϴ� ������Ʈ�� ī�޶�� ���
// ������ ������ ����� ī�޶� ��
class HYCamera :
    public HYComponent
{
private:
    PROJ_TYPE   m_ProjType;     // ���� ���

    // ��������(Perspective)�� �� ���
    float   m_FOV;          // �þ� ��

    // ��������(Orthographic)�� �� ���
    float   m_Width;        // �������� ���� ����
    float   m_Scale;        // �������� ����

    // Both(���������� �������� �� �� �ʿ��� ������)
    float   m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����(ȭ�� �ػ󵵿� ���� ���εǴ� �ȼ��� �޶���)
    float   m_Far;          // ���� �ִ� �Ÿ�

    // ��ȯ ���
    Matrix  m_matView;      // �� ���
    Matrix  m_matProj;      // ���� ���

public:
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

    float GetScale() { return m_Scale; }
    void SetScale(float _Scale) { m_Scale = _Scale; }

    float GetFOV() { return m_FOV; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

public:
    virtual void finaltick() override;

public:
    HYCamera();
    ~HYCamera();
};

