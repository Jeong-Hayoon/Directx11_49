#include "pch.h"
#include "HYMaterial.h"

#include "HYGraphicsShader.h"
		  
#include "HYDevice.h"
#include "HYConstBuffer.h"
#include "HYTexture.h"


HYMaterial::HYMaterial(bool _bEngine)
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

void* HYMaterial::GetScalarParam(SCALAR_PARAM _ParamType)
{
	switch (_ParamType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
	{
		// 인덱스는 0부터 시작하니까
		int idx = _ParamType - INT_0;

		// 배열의 인덱스 위치 return
		return m_Const.iArr + idx;
	}
	break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
	{
		int idx = _ParamType - FLOAT_0;
		return m_Const.fArr + idx;
	}
	break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
	{
		int idx = _ParamType - VEC2_0;
		return m_Const.v2Arr + idx;
	}
	break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
	{
		int idx = _ParamType - VEC4_0;
		return m_Const.v4Arr + idx;
	}
	break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
	{
		int idx = _ParamType - MAT_0;
		return m_Const.matArr + idx;
	}
	break;
	}

	return nullptr;
}