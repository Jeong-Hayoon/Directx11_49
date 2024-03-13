#pragma once
#include <Engine/HYScript.h>

class HYMonsterScript :
    public HYScript
{
private:
    float                m_DetectRange;         // 탐지 범위
    float                m_Speed;               // 몬스터의 이동 속도

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

