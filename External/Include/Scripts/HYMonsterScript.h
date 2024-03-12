#pragma once
#include <Engine/HYScript.h>

class HYMonsterScript :
    public HYScript
{
private:
    float       m_DetectRange;
    float       m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYMonsterScript);

public:
    HYMonsterScript();
    ~HYMonsterScript();
};

