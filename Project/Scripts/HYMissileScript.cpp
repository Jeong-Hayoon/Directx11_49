#include "pch.h"
#include "HYMissileScript.h"


HYMissileScript::HYMissileScript()
	: m_fSpeed(1500.f)
	, HYScript(SCRIPT_TYPE::MISSILESCRIPT)
{
}

HYMissileScript::~HYMissileScript()
{
}

void HYMissileScript::tick()
{
	// ��ġ�� �� Script Component�� �����ϰ� �ִ� GameObject�� ��ġ
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);
} 

void HYMissileScript::SaveToFile(FILE* _File)
{
}

void HYMissileScript::LoadFromFile(FILE* _File)
{
}
