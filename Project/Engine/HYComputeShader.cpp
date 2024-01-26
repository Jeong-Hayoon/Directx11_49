#include "pch.h"
#include "HYComputeShader.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"
		  
#include "HYPathMgr.h"

// 생성할 때 반드시 스레드의 개수(그룹 하나가 몇 개의 스레드를 보유하고 있는지)를 입력받아야 함
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

	// 컴퓨트 쉐이더 생성
	// 컴퓨트 쉐이더 컴파일이 된 것은 m_CSBlob에 저장
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

	// 성공했으면 m_CSBlob에 결과 받아짐
	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize(), nullptr
		, m_CS.GetAddressOf());

	return S_OK;
}

void HYComputeShader::Execute()
{
	if (FAILED(UpdateData()))
		return;

	// 상수 데이터 바인딩
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData_CS();


	// 사용할 컴퓨트 쉐이더 알려주고 컴퓨트 쉐이더 실행
	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	// 그룹의 개수 지정
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	Clear();
}
