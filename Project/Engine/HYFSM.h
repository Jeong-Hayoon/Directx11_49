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
    // ����ϴ� StateMachine Component�� ���ڷ� �־���
    void finaltick(HYStateMachine* _StateMachine);

public:
    CLONE_DISABLE(HYFSM);

    HYFSM(bool _bEngine = false);
    ~HYFSM();
};

