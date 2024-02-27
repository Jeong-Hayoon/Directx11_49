#pragma once
#include <Engine/HYScript.h>

class HYSpotLightScript :
    public HYScript
{
private:
    Vec3   m_Dir;

public:
    virtual void tick() override;

    CLONE(HYSpotLightScript);

public:
    HYSpotLightScript();
    ~HYSpotLightScript();
};

