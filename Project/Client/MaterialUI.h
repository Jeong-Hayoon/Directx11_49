#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:

public:
    virtual void render_update() override;

public:
    MaterialUI();
    ~MaterialUI();
};

