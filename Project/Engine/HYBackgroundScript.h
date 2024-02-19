#pragma once
#include "HYScript.h"

class HYBackgroundScript :
    public HYScript
{
public:
    virtual void tick() override;

    CLONE(HYBackgroundScript);

public:
    HYBackgroundScript();
    ~HYBackgroundScript();
};

