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

	// Master FSM인 경우에만 delete, 다 같은 주소를 참조하기 있기 때문에
	if(!m_Master)
	{
		Delete_Map(m_mapState);
	}
}

void HYFSM::finaltick()
{
	// 현재 상태에 finaltick 호출
	if (m_CurState)
	{
		m_CurState->m_FSM = this;
		m_CurState->finaltick();
	}
}

void HYFSM::AddState(const wstring& _StateName, HYState* _State)
{
	// State를 추가하려고 하는데 이름이 이미 존재
	assert(!(FindState(_StateName)));

	// 상태를 소유하고 있는 FSM이 누군지 지정
	_State->m_FSM = this;
	m_mapState.insert(make_pair(_StateName, _State));
}

// 상태를 찾는 함수
HYState* HYFSM::FindState(const wstring& _StateName)
{
	map<wstring, HYState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

// 현재 상태를 지정하는 함수
void HYFSM::SetState(const wstring& _strState)
{
	m_CurState = FindState(_strState);
}

// 사본을 주는 함수(얕은 복사)
HYFSM* HYFSM::GetFSMIstance()
{
	HYFSM* pFSMInst = new HYFSM;

	// m_mapState는 원본과 사본 주소 동일
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
