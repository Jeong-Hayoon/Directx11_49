#pragma once
#include "HYEntity.h"

class HYGameObject;

// Component�� GameObject�� ���θ� �˰� �־�� ��
class HYComponent :
    public HYEntity
{
private:
    // const�̱� ������ �����ڿ��� ������ �־���� ��
    const COMPONENT_TYPE    m_Type;
    HYGameObject* m_Owner;


public:
    virtual void begin() {}
    virtual void tick() {}
    // ���������Լ��� ������ �ڽ�Ŭ���������� ������ �������ؾ���
    virtual void finaltick() = 0;
    // ������ �ֵ鸸 �Ұ��̱� ������ ���������Լ� x
    virtual void UpdateData() {};

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    HYGameObject* GetOwner() { return m_Owner; }

public:
    HYComponent(COMPONENT_TYPE _Type);
    ~HYComponent();

    friend class HYGameObject;
};

