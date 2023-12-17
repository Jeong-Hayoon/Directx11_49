#pragma once
#include "HYComponent.h"

// ī�޶� �Ŵ����� �ƴ� ī�޶� ���� �ϴ� ������Ʈ�� ī�޶�� ���
class HYCamera :
    public HYComponent
{
private:
    // ��������(Perspective)�� �� ���
    float   m_FOV;          // �þ� ��

    // ��������(Orthographic)�� �� ���
    float   m_Width;        // �������� ���� ����
    float   m_Scale;        // �������� ����

    // Both(���������� �������� �� �� �ʿ��� ������)
    float   m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
    float   m_Far;          // ���� �ִ� �Ÿ�

    // ��ȯ ���
    Matrix  m_matView;      // �� ���
    Matrix  m_matProj;      // ���� ���

public:
    HYCamera();
    ~HYCamera();
};

