#include "pch.h"
#include "HYFSM.h"

HYFSM::HYFSM(bool _bEngine)
	: HYAsset(ASSET_TYPE::FSM, _bEngine)
{
}

HYFSM::~HYFSM()
{
}

void HYFSM::finaltick(HYStateMachine* _StateMachine)
{

}

void HYFSM::AddState(const wstring& _StateName, HYState* _State)
{
	// State�� �߰��Ϸ��� �ϴµ� �̸��� �̹� ����
	assert(!(FindState(_StateName)));

	_State->m_FSM = this;
	m_mapState.insert(make_pair(_StateName, _State));
}

HYState* HYFSM::FindState(const wstring& _StateName)
{
	map<wstring, HYState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}
