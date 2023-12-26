#pragma once
#include "HYScript.h"

class HYMissileScript :
    public HYScript
{
private:
    float   m_fSpeed;

public:
    virtual void tick() override;

public:
    HYMissileScript();
    ~HYMissileScript();
};


