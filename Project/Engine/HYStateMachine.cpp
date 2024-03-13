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
	// ���� FSM ������ Instance �� �����صд�.
	if (m_FSM.Get())
	{
		m_FSM_Inst = m_FSM->GetFSMIstance();
		m_FSM_Inst->SetStateMachine(this);
	}
}

void HYStateMachine::finaltick()
{
	// �����ϰ� �ִ� ���� FSM(�纻)�� finaltick
	if (m_FSM_Inst.Get())
		m_FSM_Inst->finaltick();
}

// ������ �����ϴ� �Լ�
void HYStateMachine::SetFSM(Ptr<HYFSM> _FSM)
{
	m_FSM = _FSM;
	m_FSM_Inst = nullptr;
}

// �����͸� �����忡 �ִ� �Լ�(Ű��, ������ Ÿ��, ������ �ּ�) - begin �Լ����� ȣ��
void HYStateMachine::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData)
{
	m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

void* HYStateMachine::GetBlackboardData(const wstring& _strKey)
{
	// ����� ������ �ִ� BlackBoard�� ����ִ� �����͸� ��ȯ
	return m_Blackboard.GetBlackboardData(_strKey);
}

void HYStateMachine::SaveToFile(FILE* _File)
{
}

void HYStateMachine::LoadFromFile(FILE* _File)
{
}

