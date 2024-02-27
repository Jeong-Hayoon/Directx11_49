#include "pch.h"
#include "HYCameraMoveScript.h"

HYCameraMoveScript::HYCameraMoveScript()
	: m_CamSpeed(500.f)
	, HYScript(-1)
{
}

HYCameraMoveScript::~HYCameraMoveScript()
{
}

void HYCameraMoveScript::tick()
{
	// UI에서
	//if (KEY_TAP(KEY::P))
	//{
	//	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	//		Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	//	else
	//	{
	//		Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//		// 정면을 바라봐야 하기 때문에 방향 리셋
	//		Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	//	}
	//}


	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		MoveOrthographic();
	}

	else
	{
		MovePerspective();
	}

	if (KEY_PRESSED(KEY::NUM1))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT_ENGINE * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT_ENGINE * 2.f);
	}

	if (KEY_PRESSED(KEY::NUM2))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT_ENGINE * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT_ENGINE * 2.f);
	}
}

// 직교투영일 때는 Front를 z축으로 고정 
void HYCameraMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// 
	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT_ENGINE * m_CamSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void HYCameraMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// 원근 투영 시 바라보는 방향이 z축일거라는 보장이 없음
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	
	// WS 앞뒤 AD 좌우
	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT_ENGINE * m_CamSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vFront;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vRight;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT_ENGINE * m_CamSpeed * vRight;
	}

	Transform()->SetRelativePos(vPos);

	// 회전
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = HYKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT_ENGINE * XM_PI * 4.f;
		vRot.x += vDrag.y * DT_ENGINE * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}

