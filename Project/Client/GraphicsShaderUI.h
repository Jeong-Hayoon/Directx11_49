#pragma once
#include "AssetUI.h"
class GraphicsShader :
    public AssetUI
{
private:

public:
    virtual void render_update() override;

public:
    GraphicsShader();
    ~GraphicsShader();
};

