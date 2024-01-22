#pragma once
#include "ComponentUI.h"
class TilemapUI :
    public ComponentUI
{
    virtual void render_update() override;
public:
    TilemapUI();
    ~TilemapUI();
};

