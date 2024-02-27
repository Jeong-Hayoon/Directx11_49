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
	// UI����
	//if (KEY_TAP(KEY::P))
	//{
	//	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	//		Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	//	else
	//	{
	//		Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//		// ������ �ٶ���� �ϱ� ������ ���� ����
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

// ���������� ���� Front�� z������ ���� 
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

	// ���� ���� �� �ٶ󺸴� ������ z���ϰŶ�� ������ ����
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	
	// WS �յ� AD �¿�
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

	// ȸ��
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = HYKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT_ENGINE * XM_PI * 4.f;
		vRot.x += vDrag.y * DT_ENGINE * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}

