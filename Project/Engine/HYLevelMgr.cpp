#include "pch.h"
#include "HYLevelMgr.h"
		  
#include "HYDevice.h"
#include "HYAssetMgr.h"
		  
#include "HYLevel.h"
#include "HYLayer.h"
#include "HYGameObject.h"
#include "components.h"
#include "HYPlayerScript.h"
#include "HYCameraMoveScript.h"
		  
#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYTexture.h"


HYLevelMgr::HYLevelMgr()
	: m_CurLevel(nullptr)
{

}

HYLevelMgr::~HYLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void HYLevelMgr::init()
{
	// 초기 레벨 구성하기
	m_CurLevel = new HYLevel;
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"Monster");
	m_CurLevel->GetLayer(31)->SetName(L"UI");

	// Main Camera Object 생성 -> 카메라 기능 수행
	HYGameObject* pCamObj = new HYGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);
	pCamObj->AddComponent(new HYCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	// Main Camera는 UI를 안보게 세팅
	pCamObj->Camera()->LayerCheck(L"UI", false);

	m_CurLevel->AddObject(pCamObj, 0);

	// Sub Camera
	pCamObj = new HYGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	// Sub Camera는 원점에서 UI를 바라보도록
	pCamObj->Camera()->LayerCheck(L"UI", true);

	m_CurLevel->AddObject(pCamObj, 0);

	// GameObject 생성
	HYGameObject* pObj = nullptr;

	pObj = new HYGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);

	// 텍스처 생성
	Ptr<HYTexture> pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"PlayerTexture", L"texture\\AirBossSpawn4.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);

	m_CurLevel->AddObject(pObj, L"Default", false);

	pObj = new HYGameObject;
	pObj->SetName(L"UI");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));

	m_CurLevel->AddObject(pObj, L"UI", false);


	GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
}

void HYLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// 이전 프레임에 등록된 오브젝트들 clear
	// 매 프레임마다 어차피 자기 자신을 실시간으로 등록을 할 것이기 때문에
	// clear을 해줘야 함(이전 프레임이 다 모여있을 거니까)
	m_CurLevel->clear();

	m_CurLevel->tick();
	m_CurLevel->finaltick();
}

