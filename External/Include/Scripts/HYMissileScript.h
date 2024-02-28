#pragma once
#include <Engine/HYScript.h>

class HYMissileScript :
    public HYScript
{
private:
    float   m_fSpeed;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYMissileScript);

public:
    HYMissileScript();
    ~HYMissileScript();
};


