#pragma once
#include "AssetUI.h"

class TextureUI :
    public AssetUI
{
private:

public:
    virtual void render_update() override;

public:
    TextureUI();
    ~TextureUI();
};

