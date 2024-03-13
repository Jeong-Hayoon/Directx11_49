#pragma once
#include "HYEntity.h"

class HYFSM;
class HYStateMachine;

class HYState :
    public HYEntity
{
private:
    // State�� �����ϰ� �ִ� FSM(PTR�� �ϸ� ���� ����Ű�鼭 ������ �ȵǱ� ������ native pointer ���)
    HYFSM*                   m_FSM;

public:
    virtual void finaltick() = 0;
   
    virtual void Enter() = 0;           // ���� ���� �Լ�
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
