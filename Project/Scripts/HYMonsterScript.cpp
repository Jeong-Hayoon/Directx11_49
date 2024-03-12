#include "pch.h"
#include "HYMonsterScript.h"

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

		// �÷��̾ ã�Ƽ� Object Ÿ������ �����忡 ����Ѵ�.
		//StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, );
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

