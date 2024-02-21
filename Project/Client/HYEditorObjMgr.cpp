#include "pch.h"
#include "HYEditorObjMgr.h"

#include "HYGameObjectEx.h"
#include <Engine/components.h>
#include "HYCameraMoveScript.h"

#include <Engine/HYRenderMgr.h>

HYEditorObjMgr::HYEditorObjMgr()
{

}

HYEditorObjMgr::~HYEditorObjMgr()
{
	Delete_Vec(m_vecEditorObj);
}

void HYEditorObjMgr::init()
{
	// �����Ϳ� ī�޶� ������Ʈ ����
	HYGameObjectEx* pEditorCam = new HYGameObjectEx;
	pEditorCam->AddComponent(new HYTransform);
	pEditorCam->AddComponent(new HYCamera);
	pEditorCam->AddComponent(new HYCameraMoveScript);

	pEditorCam->Camera()->LayerCheckAll();
	pEditorCam->Camera()->LayerCheck(31, false);	// UI�� Viewport ���� ���� �ϹǷ� false
	pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCam->Camera()->SetFOV(XM_PI / 2.f);
	pEditorCam->Camera()->SetFar(100000.f);

	m_vecEditorObj.push_back(pEditorCam);

	// Editor �� ī�޶�μ� �����Ŵ����� ���
	HYRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());
}

void HYEditorObjMgr::progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}
