#include "pch.h"
#include "HYMaterial.h"

#include "HYGraphicsShader.h"
		  
#include "HYDevice.h"
#include "HYConstBuffer.h"
#include "HYTexture.h"


HYMaterial::HYMaterial()
	: HYAsset(ASSET_TYPE::MATERIAL)
	, m_Const{}
{
}

HYMaterial::~HYMaterial()
{
}

// 레지스터 바인딩
void HYMaterial::UpdateData()
{
	// 지정된 Shader가 없다면 상수 데이터, Texture 바인딩이 의미가 X
	if (nullptr == m_pShader.Get())
		return;

	// 사용할 쉐이더 바인딩(Rasterizer State, Depth Stencil State, Blend State, Topology, Shader Update)
	m_pShader->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->UpdateData(i);
			m_Const.bTex[i] = 1;
		}
		// 레지스터에 전달할 값이 없다는 것
		else
		{
			// 레지스터에 전달할 값이 없더라도 앞전에 업데이트된 재질,
			// 바인딩되었던 텍스트 레지스터를 빼주는 Clear함수를 호출
			HYTexture::Clear(i);
			m_Const.bTex[i] = 0;
		}
	}

	// 상수 데이터 업데이트
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();
}

// 텍스처 전달
void HYMaterial::SetTexParam(TEX_PARAM _Param, Ptr<HYTexture>_pTex)
{
	m_arrTex[_Param] = _pTex;
}