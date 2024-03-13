#include "pch.h"
#include "HYStateMachine.h"

HYStateMachine::HYStateMachine()
	: HYComponent(COMPONENT_TYPE::STATEMACHINE)
{
}

HYStateMachine::~HYStateMachine()
{
}

void HYStateMachine::begin()
{
	// 원본 FSM 에셋의 Instance 를 생성해둔다.
	if (m_FSM.Get())
	{
		m_FSM_Inst = m_FSM->GetFSMIstance();
		m_FSM_Inst->SetStateMachine(this);
	}
}

void HYStateMachine::finaltick()
{
	// 보유하고 있는 동적 FSM(사본)에 finaltick
	if (m_FSM_Inst.Get())
		m_FSM_Inst->finaltick();
}

// 원본을 세팅하는 함수
void HYStateMachine::SetFSM(Ptr<HYFSM> _FSM)
{
	m_FSM = _FSM;
	m_FSM_Inst = nullptr;
}

// 데이터를 블랙보드에 넣는 함수(키값, 데이터 타입, 데이터 주소) - begin 함수에서 호출
void HYStateMachine::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData)
{
	m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

void* HYStateMachine::GetBlackboardData(const wstring& _strKey)
{
	// 멤버로 가지고 있는 BlackBoard에 들어있는 데이터를 반환
	return m_Blackboard.GetBlackboardData(_strKey);
}

void HYStateMachine::SaveToFile(FILE* _File)
{
}

void HYStateMachine::LoadFromFile(FILE* _File)
{
}

