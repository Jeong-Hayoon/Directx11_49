#include "pch.h"
#include "HYStateMachine.h"

HYStateMachine::HYStateMachine()
	: HYComponent(COMPONENT_TYPE::STATEMACHINE)
{
}

HYStateMachine::~HYStateMachine()
{
}

void HYStateMachine::finaltick()
{
	// ������ �����ϰ� �ִ� FSM�� �ִ� ��� finaltick ȣ��
	m_FSM->finaltick(this);
}

void HYStateMachine::SetFSM(Ptr<HYFSM> _FSM)
{
	m_FSM = _FSM;
}

// ���¸� �����忡 �ִ� �Լ�
void HYStateMachine::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData)
{
	m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

void HYStateMachine::SaveToFile(FILE* _File)
{
}

void HYStateMachine::LoadFromFile(FILE* _File)
{
}

