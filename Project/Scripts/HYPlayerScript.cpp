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
	Ptr<HYTexture> pAltasTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\playerFrame_small.png", L"texture\\playerFrame_small.png");

	Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(100.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 1, 10);
	Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 1, 10);
	Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(300.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 1, 10);
	Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(200.f, 0.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 1, 10);

	Animator2D()->Create(L"MOVE_UP", pAltasTex, Vec2(0.f, 200.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 10, 10);
	Animator2D()->Create(L"MOVE_DOWN", pAltasTex, Vec2(0.f, 100.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 10, 10);
	Animator2D()->Create(L"MOVE_LEFT", pAltasTex, Vec2(0.f, 400.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 10, 10);
	Animator2D()->Create(L"MOVE_RIGHT", pAltasTex, Vec2(0.f, 300.f), Vec2(100.f, 100.f), Vec2(0.f, 0.f), Vec2(70.f, 150.f), 10, 10);

	GetRenderComponent()->GetDynamicMaterial();

	// Prefab Test
	//m_Missile = HYAssetMgr::GetInst()->FindAsset<HYPrefab>(L"MissilePrefab");
	// Save한 Prefab 로딩해서 사용
	//m_Missile = HYAssetMgr::GetInst()->Load<HYPrefab>(L"prefab\\missile.pref", L"prefab\\missile.pref");
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
		Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
		//GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
		GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		Ptr<HYMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
	}
	else if (KEY_RELEASED(KEY::SPACE))
	{
		Ptr<HYMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}
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

void HYPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void HYPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}