#pragma once
#include "HYAsset.h"

#include "HYState.h"
#include "HYBlackboard.h"

class HYStateMachine;

class HYFSM :
    public HYAsset
{
private:
    map<wstring, HYState*>       m_mapState;
    HYBlackboard                 m_Blackboard;

public:
    void AddState(const wstring& _StateName, HYState* _State);
    HYState* FindState(const wstring& _StateName);

public:
    // 사용하는 StateMachine Component를 인자로 넣어줌
    void finaltick(HYStateMachine* _StateMachine);

public:
    CLONE_DISABLE(HYFSM);

    HYFSM(bool _bEngine = false);
    ~HYFSM();
};

