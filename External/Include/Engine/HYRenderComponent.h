#pragma once
#include "HYComponent.h"

#include "HYMesh.h"
#include "HYMaterial.h"

class HYRenderComponent :
    public HYComponent
{
private:
    // 물체의 모양
    Ptr<HYMesh>      m_Mesh;

    Ptr<HYMaterial>  m_SharedMtrl;
    Ptr<HYMaterial>  m_DynamicMtrl;
    Ptr<HYMaterial>  m_CurMtrl;

public:
    void SetMesh(Ptr<HYMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<HYMaterial> _Mtrl);

    Ptr<HYMesh> GetMesh() { return m_Mesh; }
    Ptr<HYMaterial> GetMaterial() { return m_CurMtrl; }
    Ptr<HYMaterial> GetSharedMaterial() { return m_SharedMtrl; }
    Ptr<HYMaterial> GetDynamicMaterial();

    void RestoreMaterial();

public:
    virtual void finaltick() {};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    virtual HYRenderComponent* Clone() = 0;

public:
    HYRenderComponent(COMPONENT_TYPE _Type);
    HYRenderComponent(const HYRenderComponent& _OriginRenderCom);
    ~HYRenderComponent();
};

