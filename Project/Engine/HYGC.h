#pragma once
#include "singleton.h"

class HYEntity;

class HYGC :
    public HYSingleton<HYGC>
{
    SINGLE(HYGC);
private:
    vector<HYEntity*>    m_vecEntity;

public:
    void tick();

    void Add(HYEntity* _Entity)
    {
        m_vecEntity.push_back(_Entity);
    }
};

