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
	Vec3 vRot = Transform()->GetRelativeRotation();

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

	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}
