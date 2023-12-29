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

// �������� ���ε�
void HYMaterial::UpdateData()
{
	// ������ Shader�� ���ٸ� ��� ������, Texture ���ε��� �ǹ̰� X
	if (nullptr == m_pShader.Get())
		return;

	// ����� ���̴� ���ε�(Rasterizer State, Depth Stencil State, Blend State, Topology, Shader Update)
	m_pShader->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->UpdateData(i);
			m_Const.bTex[i] = 1;
		}
		// �������Ϳ� ������ ���� ���ٴ� ��
		else
		{
			// �������Ϳ� ������ ���� ������ ������ ������Ʈ�� ����,
			// ���ε��Ǿ��� �ؽ�Ʈ �������͸� ���ִ� Clear�Լ��� ȣ��
			HYTexture::Clear(i);
			m_Const.bTex[i] = 0;
		}
	}

	// ��� ������ ������Ʈ
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();
}

// �ؽ�ó ����
void HYMaterial::SetTexParam(TEX_PARAM _Param, Ptr<HYTexture>_pTex)
{
	m_arrTex[_Param] = _pTex;
}