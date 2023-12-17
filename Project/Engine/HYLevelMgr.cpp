#include "pch.h"
#include "HYLevelMgr.h"
		  
#include "HYDevice.h"
#include "HYAssetMgr.h"
		  
#include "HYLevel.h"
#include "HYGameObject.h"
#include "components.h"
#include "HYPlayerScript.h"
		  
#include "HYMesh.h"
#include "HYGraphicsShader.h"

HYLevelMgr::HYLevelMgr()
	: m_CurLevel(nullptr)
{

}

HYLevelMgr::~HYLevelMgr()
{

}

void HYLevelMgr::init()
{
	// �ʱ� ���� �����ϱ�
	m_CurLevel = new HYLevel;

	// GameObject ����
	HYGameObject* pObj = nullptr;

	pObj = new HYGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(-0.5f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetShader(HYAssetMgr::GetInst()->FindAsset<HYGraphicsShader>(L"Std2DShader"));

	m_CurLevel->AddObject(pObj, 0);
}

void HYLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

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
