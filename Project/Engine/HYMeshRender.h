#pragma once
#include "HYRenderComponent.h"
class HYMeshRender :
    public HYRenderComponent
{
public:
    virtual void UpdateData() override;
    virtual void render() override;

public:
    HYMeshRender();
    ~HYMeshRender();
};

