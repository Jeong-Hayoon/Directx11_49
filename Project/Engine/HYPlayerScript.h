#pragma once
#include "HYScript.h"

class HYPlayerScript :
    public HYScript
{
private:
    float       m_Speed;

public:
    virtual void tick() override;

public:
    HYPlayerScript();
    ~HYPlayerScript();
};

