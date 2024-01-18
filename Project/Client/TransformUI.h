#pragma once
#include "ComponentUI.h"

#include <Engine/HYGameObject.h>

class TransformUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

