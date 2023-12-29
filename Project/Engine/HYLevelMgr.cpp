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
	// �ʱ� ���� �����ϱ�
	m_CurLevel = new HYLevel;

	// Camera Object ���� -> ī�޶� ��� ����
	HYGameObject* pCamObj = new HYGameObject;
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);
	pCamObj->AddComponent(new HYCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	m_CurLevel->AddObject(pCamObj, 0);

	// GameObject ����
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

	// �ؽ�ó ����
	Ptr<HYTexture> pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"PlayerTexture", L"texture\\AirBossSpawn4.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);


	m_CurLevel->AddObject(pObj, 0, false);
}

void HYLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// ���� �����ӿ� ��ϵ� ������Ʈ�� clear
	// �� �����Ӹ��� ������ �ڱ� �ڽ��� �ǽð����� ����� �� ���̱� ������
	// clear�� ����� ��(���� �������� �� ������ �Ŵϱ�)
	m_CurLevel->clear();

	m_CurLevel->tick();
	m_CurLevel->finaltick();
}

void HYLevelMgr::render()
{
	if (nullptr == m_CurLevel)
		return;

	// 0~255 <-> 0~1 Normalize 
	// ���� �ʱ�ȭ.
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	m_CurLevel->render();


	HYDevice::GetInst()->Present();
}
