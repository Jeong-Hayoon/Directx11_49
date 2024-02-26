#pragma once
#include <Engine/HYScript.h>

class HYMissileScript :
    public HYScript
{
private:
    float   m_fSpeed;

public:
    virtual void tick() override;

    CLONE(HYMissileScript);

public:
    HYMissileScript();
    ~HYMissileScript();
};


