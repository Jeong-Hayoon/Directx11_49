#pragma once
#include "singleton.h"

class HYLevel;

class HYLevelMgr :
    public HYSingleton<HYLevelMgr>
{
    SINGLE(HYLevelMgr);
private:
    HYLevel*        m_CurLevel;

public:
    void init();

    void tick();

public:
    void ChangeLevelState(LEVEL_STATE _State);

public:
    HYLevel* GetCurrentLevel() { return m_CurLevel; }
};

