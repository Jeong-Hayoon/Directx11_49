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
	// 본인이 소유하고 있는 FSM이 있는 경우 finaltick 호출
	m_FSM->finaltick(this);
}

void HYStateMachine::SetFSM(Ptr<HYFSM> _FSM)
{
	m_FSM = _FSM;
}

// 상태를 블랙보드에 넣는 함수
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

