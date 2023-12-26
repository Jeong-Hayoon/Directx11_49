#include "pch.h"
#include "HYLevelMgr.h"
		  
#include "HYDevice.h"
#include "HYAssetMgr.h"
		  
#include "HYLevel.h"
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

	// 텍스처 생성
	HYTexture* pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"Effect Texture", L"texture\\AirBossSpawn4.png");

	if (nullptr != pTex)
	{
		pTex->UpdateData(0);
	}

	// Camera Object 생성 -> 카메라 기능 수행
	HYGameObject* pCamObj = new HYGameObject;
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);
	pCamObj->AddComponent(new HYCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

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
	pObj->MeshRender()->SetShader(HYAssetMgr::GetInst()->FindAsset<HYGraphicsShader>(L"Std2DShader"));

	HYGameObject* pChildObj = new HYGameObject;
	pChildObj->SetName(L"Child");

	pChildObj->AddComponent(new HYTransform);
	pChildObj->AddComponent(new HYMeshRender);

	pChildObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 0.f));
	pChildObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
	pChildObj->Transform()->SetAbsolute(true);

	pChildObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pChildObj->MeshRender()->SetShader(HYAssetMgr::GetInst()->FindAsset<HYGraphicsShader>(L"Std2DShader"));

	// Level은 제일 최상위 부모만 부르고 그 부모가 tick을 받으면
	// 부모가 자식에게 tick을 줌(벡터롤 관리)
	pObj->AddChild(pChildObj);

	m_CurLevel->AddObject(pObj, 0);
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

void HYLevelMgr::render()
{
	if (nullptr == m_CurLevel)
		return;

	// 0~255 <-> 0~1 Normalize 
	// 배경색 초기화
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	m_CurLevel->render();


	HYDevice::GetInst()->Present();
}
