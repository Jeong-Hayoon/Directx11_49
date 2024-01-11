#pragma once
#include "HYComponent.h"

// 광원 역할을 하는 오브젝트를 만들어서 Light2D Component 부착
class HYLight2D :
    public HYComponent
{
private:
    tLightInfo      m_Info;

public:
    virtual void finaltick() override;

public:
    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightColor(Vec3 _vColor) { m_Info.vColor = _vColor; }
    void SetSpecular(Vec3 _vSpec) { m_Info.vSpecular = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_Info.vAmbient = _vAmb; }

    Vec4 GetLightColor(Vec3 _vColor) { return m_Info.vColor; }
    Vec4 GetSpecular(Vec3 _vSpec) { return m_Info.vSpecular; }
    Vec4 GetAmbient(Vec3 _vAmb) { return m_Info.vAmbient; }

    void SetLightType(LIGHT_TYPE _type);
    void SetRadius(float _Radius);
    void SetAngle(float _Angle);

    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_Info.LightType; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }


    void SetWorldDir(Vec3 WorldDir) { m_Info.vWorldDir = WorldDir; }


public:
    HYLight2D();
    ~HYLight2D();
};

