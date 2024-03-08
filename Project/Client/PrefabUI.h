#pragma once
#include "AssetUI.h"

class PrefabUI :
    public AssetUI
{
public:
    virtual void render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

