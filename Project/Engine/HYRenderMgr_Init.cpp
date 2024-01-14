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

	m_PostProcessTex = HYAssetMgr::GetInst()->CreateTexture(L"PostProcessTex"
															,(UINT)vRenderResolution.x
															, (UINT)vRenderResolution.y
															, DXGI_FORMAT_R8G8B8A8_UNORM
															, D3D11_BIND_SHADER_RESOURCE);
}

void HYRenderMgr::CopyRenderTargetToPostProcessTarget()
{
	// AssetMgr�� ��ϵ� ����Ÿ�� �ؽ�ó�� �ҷ���
	Ptr<HYTexture> pRTTex = HYAssetMgr::GetInst()->FindAsset<HYTexture>(L"RenderTargetTex");
	// ������ - m_PostProcessTex�� ���� - ����Ÿ�� �ؽ�ó�� �����(CopyResource�� ����Ͽ� �����ϱ� ���ؼ��� �� �ؽ�ó�� �ػ󵵰� ��ġ�ؾ� ��)
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}