#include "pch.h"
#include "HYRenderMgr.h"
		  
#include "HYDevice.h"
#include "HYAssetMgr.h" 

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

	Vec2 vRenderResolution = HYDevice::GetInst()->GetRenderResolution();

	m_PostProcessTex = HYAssetMgr::GetInst()->CreateTexture((UINT)vRenderResolution.x
		, (UINT)vRenderResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE);
}