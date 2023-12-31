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
		GetOwner()->Destroy();
		
		// GameObject 생성
		HYGameObject* pObj = nullptr;

		pObj = new HYGameObject;
		pObj->SetName(L"Missile");
		pObj->AddComponent(new HYTransform);
		pObj->AddComponent(new HYMeshRender);
		pObj->AddComponent(new HYMissileScript);

		// 위치는 이 Script Component를 소유하고 있는 GameObject의 위치
		pObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));

		GamePlayStatic::SpawnGameObject(pObj, 0);
	}

	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
	//GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);

}

void HYPlayerScript::BeginOverlap(HYCollider2D* _Collider
	, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider)
{
	_OtherObj->Destroy();
}

void HYPlayerScript::Overlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider)
{

}

void HYPlayerScript::EndOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider)
{
}