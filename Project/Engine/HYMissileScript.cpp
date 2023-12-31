#include "pch.h"
#include "HYMissileScript.h"


HYMissileScript::HYMissileScript()
	: m_fSpeed(1500.f)
{
}

HYMissileScript::~HYMissileScript()
{
}

void HYMissileScript::tick()
{
	// 위치는 이 Script Component를 소유하고 있는 GameObject의 위치
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);
} 