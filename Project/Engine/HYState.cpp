#include "pch.h"
#include "HYState.h"

#include "HYStateMachine.h"

HYState::HYState()
	: m_FSM(nullptr)
{
}

HYState::~HYState()
{
}

// Blackboard에서 데이터를 꺼내주는 함수(void*라 받아가는 쪽에서 캐스팅을 해줘야 함)
void* HYState::GetBlackboardData(const wstring& _strKey)
{
	// StateMachine에 접근해서 StateMachine으로부터 데이터를 꺼냄
	HYStateMachine* pSM = m_FSM->GetStateMachine();
	return pSM->GetBlackboardData(_strKey);
}

void HYState::ChangeState(const wstring& _strStateName)
{
	assert(m_FSM);
	m_FSM->ChangeState(_strStateName);
}