#include "pch.h"
#include "HYMonsterScript.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>


HYMonsterScript::HYMonsterScript()
	: HYScript(MONSTERSCRIPT)
	, m_DetectRange(400.f)
	, m_Speed(200.f)
{
}

HYMonsterScript::~HYMonsterScript()
{
}

void HYMonsterScript::begin()
{
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		// 플레이어를 찾아서 Object 타입으로 블랙보드에 기록한다.
		HYGameObject* pPlayer = HYLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");

		if (pPlayer)
		{
			StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, pPlayer);
		}

		if (nullptr != StateMachine()->GetFSM())
		{
			// 현재 상태 설정
			StateMachine()->GetFSM()->SetState(L"IdleState");
		}
	}
}

void HYMonsterScript::tick()
{
}

void HYMonsterScript::SaveToFile(FILE* _File)
{
}

void HYMonsterScript::LoadFromFile(FILE* _File)
{
}