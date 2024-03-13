#pragma once
#include "HYAsset.h"

#include "HYState.h"
#include "HYBlackboard.h"

class HYStateMachine;

class HYFSM :
    public HYAsset
{
private:    
    map<wstring, HYState*>       m_mapState;            // 보유하고 있는 State

    HYBlackboard*                m_Blackboard;

    HYFSM*                       m_Master;

    HYStateMachine*              m_StateMachine;        // FSM 을 사용하는 StateMachine;

    HYState*                     m_CurState;


public:
    void AddState(const wstring& _StateName, HYState* _State);
    HYState* FindState(const wstring& _StateName);
    void SetState(const wstring& _strState);
    void SetStateMachine(HYStateMachine* _SM)
    {
        if (m_Master)
        {
            m_StateMachine = _SM;
        }
    }
    HYFSM* GetFSMIstance();

    HYStateMachine* GetStateMachine() { return m_StateMachine; }
    void ChangeState(const wstring& _strStateName);

public:
    void finaltick();

public:
    CLONE_DISABLE(HYFSM);

    HYFSM(bool _bEngine = false);
    ~HYFSM();
};

