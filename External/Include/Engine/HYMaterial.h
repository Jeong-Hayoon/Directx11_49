#pragma once
#include "HYAsset.h"

#include "HYTexture.h"
#include "HYGraphicsShader.h"

// 파라미터를 가지고 있을 클래스
// 1. Shader에서 필요로하는 변수(상수 데이터)를 제공
// 2. Shader에서 필요로하는 텍스처 전달
class HYMaterial :
    public HYAsset
{
public:
    // Material이 Shader한테 보내주는 상수 데이터 묶음
    tMtrlConst                  m_Const;
    // Shader에서 필요로하는 텍스처
    Ptr<HYTexture>              m_arrTex[(UINT)TEX_PARAM::END];
    // 재질이 Shader를 참조해야 하므로 멤버 변수로 가지고 있음 
    Ptr<HYGraphicsShader>       m_pShader;


public:

    void SetShader(Ptr<HYGraphicsShader> _Shader) { m_pShader = _Shader; }
    Ptr<HYGraphicsShader> GetShader() { return m_pShader; }

    template<typename T>
    void SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value);
    void SetTexParam(TEX_PARAM _Param, Ptr<HYTexture> _pTex);
	void* GetScalarParam(SCALAR_PARAM _ParamType);
	Ptr<HYTexture> GetTexParam(TEX_PARAM _ParamType) { return m_arrTex[(UINT)_ParamType]; }

    // Material이 들고 있는 데이터를 GPU로 보내는 함수
    void UpdateData();

	// 재질의 파라미터 대입(재질 복사)
	void operator = (const HYMaterial& _OtherMtrl)
	{
		SetName(_OtherMtrl.GetName());

		m_Const = _OtherMtrl.m_Const;

		for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
		{
			m_arrTex[i] = _OtherMtrl.m_arrTex[i];
		}

		m_pShader = _OtherMtrl.m_pShader;
	}

	virtual int Save(const wstring& _strRelativePath);
	virtual int Load(const wstring& _strFilePath);

	// 재질의 복제본을 반환해주는 함수
	CLONE(HYMaterial);

public:
    HYMaterial(bool _bEngine = false);
    ~HYMaterial();
};


// Template는 h에 구현해야 함
template<typename T>
void HYMaterial::SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value)
{
	const T* pValue = &_Value;

	switch (_ParamType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		/*	if constexpr (std::is_same_v<T, int>) {

			}*/
		m_Const.iArr[_ParamType] = *((int*)pValue);
		break;

	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_Const.fArr[_ParamType - FLOAT_0] = *((float*)pValue);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_Const.v2Arr[_ParamType - VEC2_0] = *((Vec2*)pValue);

		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_Const.v4Arr[_ParamType - VEC4_0] = *((Vec4*)pValue);

		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_Const.matArr[_ParamType - MAT_0] = *((Matrix*)pValue);
		break;
	}
}