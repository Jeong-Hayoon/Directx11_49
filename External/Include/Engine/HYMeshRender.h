#pragma once
#include "HYRenderComponent.h"
class HYMeshRender :
    public HYRenderComponent
{
public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(HYMeshRender);

public:
    HYMeshRender();
    ~HYMeshRender();
};

