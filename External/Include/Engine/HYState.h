#pragma once
#include "HYEntity.h"

class HYFSM;
class HYStateMachine;

class HYState :
    public HYEntity
{
private:
    // State를 소유하고 있는 FSM(PTR로 하면 서로 가리키면서 삭제가 안되기 때문에 native pointer 사용)
    HYFSM*                   m_FSM;

public:
    virtual void finaltick() = 0;
   
    virtual void Enter() = 0;           // 상태 진입 함수
    virtual void Exit() = 0;

protected:
    void* GetBlackboardData(const wstring& _strKey);
    HYFSM* GetFSM() { return m_FSM; }

    void ChangeState(const wstring& _strStateName);

public:
    CLONE_DISABLE(HYState);

    HYState();
    ~HYState();

    friend class HYFSM;
};
