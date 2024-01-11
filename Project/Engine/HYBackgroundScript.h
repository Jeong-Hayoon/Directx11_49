#pragma once
#include "HYScript.h"

class HYBackgroundScript :
    public HYScript
{
public:
    virtual void tick() override;

public:
    HYBackgroundScript();
    ~HYBackgroundScript();
};

