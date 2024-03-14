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

    HYFSM*                       m_Master;              // 원본 FSM(m_Master == nullptr -> Asset으로 등록되어 있는 최상위 FSM)

    HYStateMachine*              m_StateMachine;        // FSM 을 사용하는 StateMachine(원본 FSM인 경우에는 여러 오브젝트가 나를 가리키기 때문에 가리키는게 의미가 없음
                                                        // Master는 하나의 StateMachine을 가리키면 안 됨)

    HYState*                     m_CurState;            // 현재 상태

public:
    void AddState(const wstring& _StateName, HYState* _State);
    HYState* FindState(const wstring& _StateName);
    void SetState(const wstring& _strState);
    void SetStateMachine(HYStateMachine* _SM)
    {
        // m_Master가 존재한다면(사본 FSM인 경우)
        if (m_Master)
        {
            m_StateMachine = _SM;
        }
    }
    HYFSM* GetFSMIstance();

    // 나를 사용하고 있는 StateMachine을 반환해주는 함수
    HYStateMachine* GetStateMachine() { return m_StateMachine; }

    void ChangeState(const wstring& _strStateName);

public:
    void finaltick();

public:
    CLONE_DISABLE(HYFSM);

    HYFSM(bool _bEngine = false);
    ~HYFSM();
};

