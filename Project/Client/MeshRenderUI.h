#pragma once
#include "ComponentUI.h"

// 무슨 Mesh와 Shdaer를 참조하고 있는지
class MeshRenderUI :
    public ComponentUI
{

public:
    virtual void render_update() override;

public:
    void MeshSelect(DWORD_PTR _ptr);
    void MaterialSelect(DWORD_PTR _ptr);

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

