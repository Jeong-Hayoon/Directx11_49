#pragma once
#include "AssetUI.h"
class MeshDataUI :
    public AssetUI
{
private:

public:
    virtual void render_update() override;

public:
    MeshDataUI();
    ~MeshDataUI();

};

