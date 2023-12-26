#include "pch.h"
#include "HYPlayerScript.h"
#include "HYAssetMgr.h"
		 
#include "HYMissileScript.h"


HYPlayerScript::HYPlayerScript()
	: m_Speed(500.f)
{
}

HYPlayerScript::~HYPlayerScript()
{
}

void HYPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
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

	if (KEY_TAP(KEY::SPACE))
	{
		// GameObject ����
		HYGameObject* pObj = nullptr;

		pObj = new HYGameObject;
		pObj->SetName(L"Missile");
		pObj->AddComponent(new HYTransform);
		pObj->AddComponent(new HYMeshRender);
		pObj->AddComponent(new HYMissileScript);

		// ��ġ�� �� Script Component�� �����ϰ� �ִ� GameObject�� ��ġ
		pObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
		pObj->MeshRender()->SetShader(HYAssetMgr::GetInst()->FindAsset<HYGraphicsShader>(L"Std2DShader"));

		GamePlayStatic::SpawnGameObject(pObj, 0);
	}
}
