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

	// �ؽ�ó ����
	HYTexture* pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"Effect Texture", L"texture\\AirBossSpawn4.png");

	if (nullptr != pTex)
	{
		pTex->UpdateData(0);
	}

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

	// Level�� ���� �ֻ��� �θ� �θ��� �� �θ� tick�� ������
	// �θ� �ڽĿ��� tick�� ��(���ͷ� ����)
	pObj->AddChild(pChildObj);

	m_CurLevel->AddObject(pObj, 0);
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
	// ���� �ʱ�ȭ
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	m_CurLevel->render();


	HYDevice::GetInst()->Present();
}
