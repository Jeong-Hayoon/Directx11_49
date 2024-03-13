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

// Blackboard���� �����͸� �����ִ� �Լ�(void*�� �޾ư��� �ʿ��� ĳ������ ����� ��)
void* HYState::GetBlackboardData(const wstring& _strKey)
{
	// StateMachine�� �����ؼ� StateMachine���κ��� �����͸� ����
	HYStateMachine* pSM = m_FSM->GetStateMachine();
	return pSM->GetBlackboardData(_strKey);
}

void HYState::ChangeState(const wstring& _strStateName)
{
	assert(m_FSM);
	m_FSM->ChangeState(_strStateName);
}