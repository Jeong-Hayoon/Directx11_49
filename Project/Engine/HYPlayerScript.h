#pragma once
#include "HYScript.h"

class HYPlayerScript :
    public HYScript
{
private:
    float       m_Speed;

public:
    virtual void tick() override;
    virtual void BeginOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);
    virtual void Overlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);
    virtual void EndOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);

public:
    HYPlayerScript();
    ~HYPlayerScript();
};

