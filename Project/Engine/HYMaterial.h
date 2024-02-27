#pragma once
#include "HYAsset.h"

#include "HYTexture.h"
#include "HYGraphicsShader.h"

// �Ķ���͸� ������ ���� Ŭ����
// 1. Shader���� �ʿ���ϴ� ����(��� ������)�� ����
// 2. Shader���� �ʿ���ϴ� �ؽ�ó ����
class HYMaterial :
    public HYAsset
{
public:
    // Material�� Shader���� �����ִ� ��� ������ ����
    tMtrlConst                  m_Const;
    // Shader���� �ʿ���ϴ� �ؽ�ó
    Ptr<HYTexture>              m_arrTex[(UINT)TEX_PARAM::END];
    // ������ Shader�� �����ؾ� �ϹǷ� ��� ������ ������ ���� 
    Ptr<HYGraphicsShader>       m_pShader;


public:

    void SetShader(Ptr<HYGraphicsShader> _Shader) { m_pShader = _Shader; }
    Ptr<HYGraphicsShader> GetShader() { return m_pShader; }

    template<typename T>
    void SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value);
    void SetTexParam(TEX_PARAM _Param, Ptr<HYTexture> _pTex);
	void* GetScalarParam(SCALAR_PARAM _ParamType);
	Ptr<HYTexture> GetTexParam(TEX_PARAM _ParamType) { return m_arrTex[(UINT)_ParamType]; }

    // Material�� ��� �ִ� �����͸� GPU�� ������ �Լ�
    void UpdateData();

	// ������ �Ķ���� ����(���� ����)
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

	// ������ �������� ��ȯ���ִ� �Լ�
	CLONE(HYMaterial);

public:
    HYMaterial(bool _bEngine = false);
    ~HYMaterial();
};


// Template�� h�� �����ؾ� ��
template<typename T>
void HYMaterial::SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value)
{
	const T* pValue = &_Value;

	switch (_ParamType)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
		/*	if constexpr (std::is_same_v<T, int>) {

			}*/
		m_Const.iArr[(UINT)_ParamType] = *((int*)pValue);
		break;

	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
		m_Const.fArr[(UINT)_ParamType - (UINT)SCALAR_PARAM::FLOAT_0] = *((float*)pValue);
		break;

	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
		m_Const.v2Arr[(UINT)_ParamType - (UINT)SCALAR_PARAM::VEC2_0] = *((Vec2*)pValue);

		break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
		m_Const.v4Arr[(UINT)_ParamType - (UINT)SCALAR_PARAM::VEC4_0] = *((Vec4*)pValue);

		break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
		m_Const.matArr[(UINT)_ParamType - (UINT)SCALAR_PARAM::MAT_0] = *((Matrix*)pValue);
		break;
	}
}