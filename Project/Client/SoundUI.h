#pragma once
#include "AssetUI.h"


class SoundUI :
    public AssetUI
{
private:

public:
    virtual void render_update() override;

public:
    SoundUI();
    ~SoundUI();
};

