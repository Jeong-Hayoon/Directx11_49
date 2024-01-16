#pragma once
#pragma once
#include "UI.h"

#include <Engine/HYGameObject.h>

class TransformUI :
    public UI
{
private:
    HYGameObject*       m_TargetObject;


public:
    void SetTargetObject(HYGameObject* _Target)
    {
        m_TargetObject = _Target;
    }

public:
    virtual void render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

