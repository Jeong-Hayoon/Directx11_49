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

// 레지스터 바인딩
void HYMaterial::UpdateData()
{
	// 지정된 Shader가 없다면 상수 데이터, Texture 바인딩이 의미가 X
	if (nullptr == m_pShader.Get())
		return;

	// 사용할 쉐이더 바인딩(Rasterizer State, Depth Stencil State, Blend State, Topology, Shader Update)
	m_pShader->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
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
		// 인덱스는 0부터 시작하니까
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::INT_0;

		// 배열의 인덱스 위치 return
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

	// 쓰기 모드(wb)
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// 재질 상수값 저장
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	// 재질이 참조하는 텍스쳐 정보를 저장
	// (텍스처를 저장하는 것이 아닌 텍스처를 다시 참조할 수 있을만한 정보를 저장)
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef<HYTexture>(m_arrTex[i], pFile);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	SaveAssetRef<HYGraphicsShader>(m_pShader, pFile);

	return 0;
}

int HYMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	
	// 읽기 모드(rb)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	// 재질 상수값 저장
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);


	// 재질이 참조하는 텍스쳐 정보를 로드
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LoadAssetRef<HYTexture>(m_arrTex[i], pFile);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	LoadAssetRef<HYGraphicsShader>(m_pShader, pFile);

	return 0;
}