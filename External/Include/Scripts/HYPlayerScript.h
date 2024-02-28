#pragma once
#include <Engine/HYScript.h>

class HYPlayerScript :
    public HYScript
{
private:
    float       m_Speed;

public:

    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);
    virtual void Overlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);
    virtual void EndOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYPlayerScript);

public:
    HYPlayerScript();
    ~HYPlayerScript();
};

