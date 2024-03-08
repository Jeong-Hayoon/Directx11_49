#include "pch.h"
#include "HYMaterial.h"

#include "HYGraphicsShader.h"
		  
#include "HYDevice.h"
#include "HYConstBuffer.h"
#include "HYTexture.h"

#include "HYPathMgr.h"

#include "Ptr.h"

HYMaterial::HYMaterial(bool _bEngine)
	: HYAsset(ASSET_TYPE::MATERIAL, _bEngine)
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
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
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
	m_arrTex[(UINT)_Param] = _pTex;
}

void* HYMaterial::GetScalarParam(SCALAR_PARAM _ParamType)
{
	switch (_ParamType)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
	{
		// �ε����� 0���� �����ϴϱ�
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::INT_0;

		// �迭�� �ε��� ��ġ return
		return m_Const.iArr + idx;
	}
	break;
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
	{
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::FLOAT_0;
		return m_Const.fArr + idx;
	}
	break;
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
	{
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::VEC2_0;
		return m_Const.v2Arr + idx;
	}
	break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
	{
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::VEC4_0;
		return m_Const.v4Arr + idx;
	}
	break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
	{
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::MAT_0;
		return m_Const.matArr + idx;
	}
	break;
	}

	return nullptr;
}

int HYMaterial::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = HYPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;

	// ���� ���(wb)
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// ���� ����� ����
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	// ������ �����ϴ� �ؽ��� ������ ����
	// (�ؽ�ó�� �����ϴ� ���� �ƴ� �ؽ�ó�� �ٽ� ������ �� �������� ������ ����)
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef<HYTexture>(m_arrTex[i], pFile);
	}

	// ������ �����ϴ� ���̴� ������ ����
	SaveAssetRef<HYGraphicsShader>(m_pShader, pFile);

	return 0;
}

int HYMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	
	// �б� ���(rb)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	// ���� ����� ����
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);


	// ������ �����ϴ� �ؽ��� ������ �ε�
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LoadAssetRef<HYTexture>(m_arrTex[i], pFile);
	}

	// ������ �����ϴ� ���̴� ������ ����
	LoadAssetRef<HYGraphicsShader>(m_pShader, pFile);

	return 0;
}