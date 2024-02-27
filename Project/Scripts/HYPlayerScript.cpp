#include "pch.h"
#include "HYPlayerScript.h"

#include <Engine/HYAssetMgr.h>

#include "HYMissileScript.h"
#include <Engine/HYGameObject.h>
#include <Engine/HYMaterial.h>
#include <Engine/HYRenderComponent.h>


HYPlayerScript::HYPlayerScript()
	: HYScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Speed(500.f)
{
}

HYPlayerScript::~HYPlayerScript()
{
}

void HYPlayerScript::begin()
{
	Ptr<HYTexture> pAltasTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"AnimAtlasTex", L"texture\\playerFrame_small.png");

	Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(100.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 1, 10);
	Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 1, 10);
	Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(300.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 1, 10);
	Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(200.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 1, 10);

	Animator2D()->Create(L"MOVE_UP", pAltasTex, Vec2(0.f, 200.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 10, 10);
	Animator2D()->Create(L"MOVE_DOWN", pAltasTex, Vec2(0.f, 100.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 10, 10);
	Animator2D()->Create(L"MOVE_LEFT", pAltasTex, Vec2(0.f, 400.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 10, 10);
	Animator2D()->Create(L"MOVE_RIGHT", pAltasTex, Vec2(0.f, 300.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(350.f, 350.f), 10, 10);
}

void HYPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::UP))
		vPos.y += DT * m_Speed;
	if (KEY_TAP(KEY::UP))
		Animator2D()->Play(L"MOVE_UP");
	if (KEY_RELEASED(UP))
		Animator2D()->Play(L"IDLE_UP");

	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * m_Speed;
	if (KEY_TAP(KEY::DOWN))
		Animator2D()->Play(L"MOVE_DOWN");
	if (KEY_RELEASED(DOWN))
		Animator2D()->Play(L"IDLE_DOWN");

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_Speed;
	if (KEY_TAP(KEY::LEFT))
		Animator2D()->Play(L"MOVE_LEFT");
	if (KEY_RELEASED(LEFT))
		Animator2D()->Play(L"IDLE_LEFT");

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_Speed;
	if (KEY_TAP(KEY::RIGHT))
		Animator2D()->Play(L"MOVE_RIGHT");
	if (KEY_RELEASED(RIGHT))
		Animator2D()->Play(L"IDLE_RIGHT");

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
	// _OtherObj->Destroy();
	
	// 특정 오브젝트에만 특정 효과를 주고 싶은 경우에는 동적 재질을 받아서 효과를 줌
	
	// Ptr<HYMaterial> pMtrl = GetOwner()->GetRenderComponent()->GetDynamicMaterial();

	Ptr<HYMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
}

void HYPlayerScript::Overlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider)
{

}

void HYPlayerScript::EndOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider)
{
}