#include "pch.h"
#include "HYComputeShader.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"
		  
#include "HYPathMgr.h"

// ������ �� �ݵ�� �������� ����(�׷� �ϳ��� �� ���� �����带 �����ϰ� �ִ���)�� �Է¹޾ƾ� ��
HYComputeShader::HYComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ)
	: HYShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_ThreadX(_ThreadX)
	, m_ThreadY(_ThreadY)
	, m_ThreadZ(_ThreadZ)
{
}

HYComputeShader::~HYComputeShader()
{
}

int HYComputeShader::Create(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = HYPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// ��ǻƮ ���̴� ����
	// ��ǻƮ ���̴� �������� �� ���� m_CSBlob�� ����
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
		, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	// ���������� m_CSBlob�� ��� �޾���
	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize(), nullptr
		, m_CS.GetAddressOf());

	return S_OK;
}

void HYComputeShader::Execute()
{
	if (FAILED(UpdateData()))
		return;

	// ��� ������ ���ε�
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData_CS();


	// ����� ��ǻƮ ���̴� �˷��ְ� ��ǻƮ ���̴� ����
	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	// �׷��� ���� ����
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	Clear();
}
