#pragma once
#include "ComponentUI.h"

class Light2DUI :
    public ComponentUI
{
public:
    void LightTypeSelect(DWORD_PTR _ptr);


public:
    virtual void render_update() override;
public:
    Light2DUI();
    ~Light2DUI();
};

