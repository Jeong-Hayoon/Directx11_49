#pragma once
#include "UI.h"

#include <Engine/HYGameObject.h>

class ComponentUI :
    public UI
{
private:
    HYGameObject*        m_TargetObject;
    COMPONENT_TYPE       m_Type;

    string               m_ComponentTitle;

public:
    virtual void render_update() override;

public:
    HYGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(HYGameObject* _Target);

    void SetComopnentTitle(const string& _title) { m_ComponentTitle = _title; }

    COMPONENT_TYPE GetType() { return m_Type; }

public:
    ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type);
    ~ComponentUI();
};

