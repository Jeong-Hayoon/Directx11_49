#pragma once
#include "HYComponent.h"

#include "HYFSM.h"
#include "HYBlackboard.h"

class HYStateMachine :
    public HYComponent
{
private:
    Ptr<HYFSM>       m_FSM;
    HYBlackboard     m_Blackboard;

public:
    void SetFSM(Ptr<HYFSM> _FSM);
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData);

public:
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(HYStateMachine);

    HYStateMachine();
    ~HYStateMachine();
};
