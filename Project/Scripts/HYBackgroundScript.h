#pragma once
#include <Engine/HYScript.h>

class HYBackgroundScript :
    public HYScript
{
public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYBackgroundScript);

public:
    HYBackgroundScript();
    ~HYBackgroundScript();
};

