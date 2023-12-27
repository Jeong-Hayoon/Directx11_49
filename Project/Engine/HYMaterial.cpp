#include "pch.h"
#include "HYMaterial.h"

#include "HYGraphicsShader.h"
		  
#include "HYDevice.h"
#include "HYConstBuffer.h"

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
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();

	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData();
	}
}