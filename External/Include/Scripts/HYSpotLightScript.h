#pragma once
#include <Engine/HYScript.h>

class HYSpotLightScript :
    public HYScript
{
private:
    Vec3   m_Dir;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYSpotLightScript);

public:
    HYSpotLightScript();
    ~HYSpotLightScript();
};


