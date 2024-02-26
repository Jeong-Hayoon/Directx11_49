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
    void ChangeLevel(HYLevel* _NextLevel, LEVEL_STATE _NextStartState);
    void ChangeLevelState(LEVEL_STATE _State);

private:
    void ChangeLevel_Task(HYLevel* _NextLevel, LEVEL_STATE _NextLevelState);

public:
    HYLevel* GetCurrentLevel() { return m_CurLevel; }


    friend class HYTaskMgr;
};

