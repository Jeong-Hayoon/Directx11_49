#include "pch.h"
#include "HYRenderMgr.h"
		  
#include "HYGameObject.h"
#include "HYTransform.h"
#include "HYMeshRender.h"

void HYRenderMgr::init()
{
	m_pDebugObj = new HYGameObject;
	m_pDebugObj->AddComponent(new HYTransform);
	m_pDebugObj->AddComponent(new HYMeshRender);
}