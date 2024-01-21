#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
    virtual void render_update() override;
public:
    Animator2DUI();
    ~Animator2DUI();
};

