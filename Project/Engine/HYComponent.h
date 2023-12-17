#pragma once
#include "HYEntity.h"

// GET_OTHER_COMPONENT 매크로를 위해 참조
#include "HYGameObject.h"

// Owner에 접근하여 돌고 돌아 Component에 접근하는 일이 많기 때문에 매크로 처리
#define GET_OTHER_COMPONENT(Type) HY##Type* Type() { return m_Owner->Type(); }

class HYGameObject;

// Component와 GameObject는 서로를 알고 있어야 함
class HYComponent :
    public HYEntity
{
private:
    // const이기 때문에 생성자에서 무조건 넣어줘야 함
    const COMPONENT_TYPE    m_Type;
    HYGameObject*           m_Owner;


public:
    virtual void begin() {}
    virtual void tick() {}
    // 순수가상함수기 때문에 자식클래스에서는 무조건 재정의해야함
    virtual void finaltick() = 0;
    // 구현할 애들만 할것이기 때문에 순수가상함수 x
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

