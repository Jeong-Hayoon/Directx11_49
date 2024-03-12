#pragma once
#include "HYEntity.h"

class HYFSM;

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

public:
    CLONE_DISABLE(HYState);

    HYState();
    ~HYState();

    friend class HYFSM;
};
