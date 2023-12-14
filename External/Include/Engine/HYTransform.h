#pragma once
#include "HYComponent.h"

class HYTransform :
    public HYComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRealtiveRotation;
    Vec3    m_arrDir[3]; // Right, Up, Front

    Matrix  m_matWorld;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRealtiveRotation = _Rotation; }

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRealtiveRotation; }

    const Matrix& GetWorldMat() { return m_matWorld; }

    Vec3 GetDir(DIR_TYPE _type) { return m_arrDir[(UINT)_type]; }

public:
    HYTransform();
    ~HYTransform();
};

