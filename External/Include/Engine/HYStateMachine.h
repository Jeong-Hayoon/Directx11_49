#pragma once
#include "HYComponent.h"

#include "HYFSM.h"
#include "HYBlackboard.h"

class HYStateMachine :
    public HYComponent
{
private:
    Ptr<HYFSM>                   m_FSM;
    Ptr<HYFSM>                   m_FSM_Inst;

    HYBlackboard                 m_Blackboard;

public:
    void SetFSM(Ptr<HYFSM> _FSM);
    Ptr<HYFSM> GetFSM()
    {
        if (m_FSM_Inst.Get())
            return m_FSM_Inst;
        else
            m_FSM;
    };

    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData);
    void* GetBlackboardData(const wstring& _strKey);

public:
    virtual void begin() override;
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(HYStateMachine);

    HYStateMachine();
    ~HYStateMachine();
};
