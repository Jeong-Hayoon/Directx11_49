#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
    virtual void render_update() override;
public:
    Collider2DUI();
    ~Collider2DUI();
};

