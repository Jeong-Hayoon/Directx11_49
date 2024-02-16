#pragma once
#include "UI.h"

class MenuUI :
    public UI
{

public:
    virtual void render() override;
    virtual void render_update() override;

public:
    MenuUI();
    ~MenuUI();
};

