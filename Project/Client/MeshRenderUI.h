#pragma once
#include "ComponentUI.h"

// ���� Mesh�� Shdaer�� �����ϰ� �ִ���
class MeshRenderUI :
    public ComponentUI
{

public:
    virtual void render_update() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

