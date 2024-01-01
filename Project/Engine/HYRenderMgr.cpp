#include "pch.h"
#include "HYRenderMgr.h"

#include "HYDevice.h"
#include "HYCamera.h"
#include "HYMeshRender.h"
#include "HYAssetMgr.h"
#include "HYTransform.h"

HYRenderMgr::HYRenderMgr()
	: m_pDebugObj(nullptr)
{

}

HYRenderMgr::~HYRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;
}

void HYRenderMgr::tick()
{
	Vec4 vClearColor = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	HYDevice::GetInst()->ClearRenderTarget(vClearColor);

	render();

	render_debug();

	HYDevice::GetInst()->Present();
}

void HYRenderMgr::render()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
	}
}

void HYRenderMgr::render_debug()
{
	// 이 작업을 해줘야 Main Camera의 View 행렬, Projection 행렬이 적용될 수 있음
	g_Transform.matView = m_vecCam[0]->GetViewMat();
	g_Transform.matProj = m_vecCam[0]->GetProjMat();

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end(); ++iter)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CircleMesh"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"SphereMesh"));
			break;
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"DebugShapeMtrl"));
		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();
	}
}

void HYRenderMgr::RegisterCamera(HYCamera* _Cam, int _Idx)
{
	// 사이즈가 Idx보다 작거나 같다면
	if (m_vecCam.size() <= _Idx + 1)
	{
		// resize
		m_vecCam.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}