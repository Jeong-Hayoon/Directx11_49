#pragma once
#include "HYComponent.h"

class HYMesh;
class HYGraphicsShader;

class HYRenderComponent :
    public HYComponent
{
private:
    HYMesh* m_Mesh;
    HYGraphicsShader* m_Shader;


public:
    void SetMesh(HYMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetShader(HYGraphicsShader* _Shader) { m_Shader = _Shader; }

    HYMesh* GetMesh() { return m_Mesh; }
    HYGraphicsShader* GetShader() { return m_Shader; }


public:
    virtual void finaltick() {};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    HYRenderComponent(COMPONENT_TYPE _Type);
    ~HYRenderComponent();
};

