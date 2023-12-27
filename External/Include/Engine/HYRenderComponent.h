#pragma once
#include "HYComponent.h"

class HYMesh;
class HYMaterial;

class HYRenderComponent :
    public HYComponent
{
private:
    // 물체의 모양
    HYMesh* m_Mesh;
    HYMaterial* m_Mtrl;
 

public:
    void SetMesh(HYMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(HYMaterial* _Mtrl) { m_Mtrl = _Mtrl; }

    HYMesh* GetMesh() { return m_Mesh; }
    HYMaterial* GetMaterial() { return m_Mtrl; }


public:
    virtual void finaltick() {};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    HYRenderComponent(COMPONENT_TYPE _Type);
    ~HYRenderComponent();
};

