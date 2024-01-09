#include "pch.h"
#include "HYRenderMgr.h"
		  
#include "HYGameObject.h"
#include "HYTransform.h"
#include "HYMeshRender.h"

#include "HYStructuredBuffer.h"


void HYRenderMgr::init()
{
	m_Light2DBuffer = new HYStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

	m_pDebugObj = new HYGameObject;
	m_pDebugObj->AddComponent(new HYTransform);
	m_pDebugObj->AddComponent(new HYMeshRender);
}