#include "pch.h"
#include "HYPlayerScript.h"


HYPlayerScript::HYPlayerScript()
	: m_Speed(2.f)
{
}

HYPlayerScript::~HYPlayerScript()
{
}

void HYPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_Speed;
	}

	Transform()->SetRelativePos(vPos);
}
