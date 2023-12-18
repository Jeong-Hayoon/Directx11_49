#pragma once
#include "HYEntity.h"

// GET_OTHER_COMPONENT ��ũ�θ� ���� ����
#include "HYGameObject.h"

// Owner�� �����Ͽ� ���� ���� Component�� �����ϴ� ���� ���� ������ ��ũ�� ó��
#define GET_OTHER_COMPONENT(Type) HY##Type* Type() { return m_Owner->Type(); }

class HYGameObject;

// Component�� GameObject�� ���θ� �˰� �־�� ��
class HYComponent :
    public HYEntity
{
private:
    // const�̱� ������ �����ڿ��� ������ �־���� ��
    const COMPONENT_TYPE    m_Type;
    HYGameObject*           m_Owner;


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

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);

public:
    HYComponent(COMPONENT_TYPE _Type);
    ~HYComponent();

    friend class HYGameObject;
};

