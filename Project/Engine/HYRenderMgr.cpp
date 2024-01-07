#include "pch.h"
#include "HYRenderMgr.h"

#include "HYDevice.h"
#include "HYCamera.h"
#include "HYMeshRender.h"
#include "HYAssetMgr.h"
#include "HYTransform.h"
#include "HYTimeMgr.h"

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
	// �� �۾��� ����� Main Camera�� View ���, Projection ����� ����� �� ����
	g_Transform.matView = m_vecCam[0]->GetViewMat();
	g_Transform.matProj = m_vecCam[0]->GetProjMat();

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end();)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CrossMesh"));
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
		// vColor�ε� VEC4_0�� ���� �� �ִ� ������ Vec3�� ���ڷ� �޴� �����ڰ� �����ϱ� ����
		m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// Cross Shape�� ������� ��쿡�� �켱 Topology�� �����س��ٰ� ��� ���Ŀ� ���� �ǵ����ֱ�
		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			// Cross�� ���� Topology�� LINELIST�� �� 
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();

		// ������ �Ŀ� ���� Topology�� �ǵ�����
		m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);


		// ���� �ð��� ���� �ð��� �Ѿ�� Erase
		(*iter).fLifeTime += DT;
		if ((*iter).fDuration <= (*iter).fLifeTime)
		{
			// erase�� �ϰ� �Ǹ� �ش� iterator�� �����ǰ� ������ iterator�� ��ȯ���ִϱ�
			// iter�� �޾Ƴ��� ���� ++�� ������� ��
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void HYRenderMgr::RegisterCamera(HYCamera* _Cam, int _Idx)
{
	// ����� Idx���� �۰ų� ���ٸ�
	if (m_vecCam.size() <= _Idx + 1)
	{
		// resize
		m_vecCam.resize(_Idx + 1);
	}

	// ������ �켱������ ī�޶� �̹� �����ϸ� assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}