#pragma once
#include "HYEntity.h"

class HYGameObject;

class HYComponent :
    public HYEntity
{
private:
    const COMPONENT_TYPE    m_Type;
    HYGameObject* m_Owner;


public:
    virtual void begin() {}
    virtual void tick() {}
    virtual void finaltick() = 0;
    virtual void UpdateData() {};

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    HYGameObject* GetOwner() { return m_Owner; }

public:
    HYComponent(COMPONENT_TYPE _Type);
    ~HYComponent();

    friend class HYGameObject;
};

