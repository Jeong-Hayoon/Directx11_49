#pragma once
#include "HYComponent.h"

enum class COLLIDER2D_TYPE
{
    RECT,
    CIRCLE,
};


class HYCollider2D :
    public HYComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    int             m_CollisionCount;   // �ٸ� �浹ü�� �浹���� Ƚ��
    bool            m_bAbsolute;        // �ڽ��� �����ϰ� �ִ� ������Ʈ�� ������ ������

    Matrix          m_matColWorld;      // �浹ü�� ���� ���� ���
    COLLIDER2D_TYPE m_Type;

public:
    void SetAbsolute(bool _bAbsol) { m_bAbsolute = _bAbsol; }
    void SetOffsetPos(Vec2 _vOffset) { m_vOffsetPos = Vec3(_vOffset.x, _vOffset.y, 0.f); }     // 0.f : ������ �׵��(�ִ��� ������ ���ֱ� ����)
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }      // 1.f : ������ �׵��
    void SetColliderType(COLLIDER2D_TYPE _Type) { m_Type = _Type; }

    bool IsAbsolute() { return m_bAbsolute; }
    Vec2 GetOffsetPos() { return Vec2(m_vOffsetPos.x, m_vOffsetPos.y); }
    Vec2 GetOffsetScale() { return Vec2(m_vOffsetScale.x, m_vOffsetScale.y); }
    COLLIDER2D_TYPE GetType() { return m_Type; }

public:
    virtual void finaltick() override;

public:
    HYCollider2D();
    ~HYCollider2D();
};


