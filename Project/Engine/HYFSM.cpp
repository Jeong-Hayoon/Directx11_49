#include "pch.h"
#include "HYFSM.h"

HYFSM::HYFSM(bool _bEngine)
	: HYAsset(ASSET_TYPE::FSM, _bEngine)
	, m_Master(nullptr)
	, m_Blackboard(nullptr)
{
}

HYFSM::~HYFSM()
{
	if(m_Blackboard)
		delete m_Blackboard;

	// Master FSM�� ��쿡�� delete, �� ���� �ּҸ� �����ϱ� �ֱ� ������
	if(!m_Master)
	{
		Delete_Map(m_mapState);
	}
}

void HYFSM::finaltick()
{
	// ���� ���¿� finaltick ȣ��
	if (m_CurState)
	{
		m_CurState->m_FSM = this;
		m_CurState->finaltick();
	}
}

void HYFSM::AddState(const wstring& _StateName, HYState* _State)
{
	// State�� �߰��Ϸ��� �ϴµ� �̸��� �̹� ����
	assert(!(FindState(_StateName)));

	// ���¸� �����ϰ� �ִ� FSM�� ������ ����
	_State->m_FSM = this;
	m_mapState.insert(make_pair(_StateName, _State));
}

// ���¸� ã�� �Լ�
HYState* HYFSM::FindState(const wstring& _StateName)
{
	map<wstring, HYState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

// ���� ���¸� �����ϴ� �Լ�
void HYFSM::SetState(const wstring& _strState)
{
	m_CurState = FindState(_strState);
}

// �纻�� �ִ� �Լ�(���� ����)
HYFSM* HYFSM::GetFSMIstance()
{
	HYFSM* pFSMInst = new HYFSM;

	// m_mapState�� ������ �纻 �ּ� ����
	pFSMInst->m_mapState = m_mapState;

	pFSMInst->m_Master = this;
	pFSMInst->m_Blackboard = nullptr;
	pFSMInst->m_CurState = nullptr;

	return pFSMInst;
}

void HYFSM::ChangeState(const wstring& _strStateName)
{
	if (nullptr != m_CurState)
		m_CurState->Exit();

	m_CurState = FindState(_strStateName);

	assert(m_CurState);

	m_CurState->Enter();
}
