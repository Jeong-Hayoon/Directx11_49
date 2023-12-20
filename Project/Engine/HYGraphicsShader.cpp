#include "pch.h"
#include "HYGraphicsShader.h"

#include "HYDevice.h"
#include "HYPathMgr.h"

HYGraphicsShader::HYGraphicsShader()
	: HYShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
{
}

HYGraphicsShader::~HYGraphicsShader()
{
}


int HYGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// 버텍스 쉐이더
	// HLSL 버텍스 쉐이더 함수 컴파일
	wstring strContentPath = HYPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), nullptr
		, m_VS.GetAddressOf());


	// 정점 구조정보(Layout) 생성
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	// Layout 생성
	DEVICE->CreateInputLayout(arrElement, 3
		, m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf());

	return S_OK;
}

int HYGraphicsShader::CreateHullShader()
{

	return 0;
}

int HYGraphicsShader::CreateGeometryShader()
{
	return 0;
}

int HYGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = HYPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// 픽셀 쉐이더 생성
	// 픽셀 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize(), nullptr
		, m_PS.GetAddressOf());

	return S_OK;
}

void HYGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->RSSetState(HYDevice::GetInst()->GetRSState(m_RSType).Get());
	CONTEXT->OMSetDepthStencilState(HYDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
	CONTEXT->OMSetBlendState(HYDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);

	// 이렇게 세팅해놓은 이유는 이렇게 해놔야 파이프라인 단계별로
	// 할 것들이 잘 전달이 되고 만약 안쓰는 Shader더라도 nullptr로
	// 밀어놔야 다음에 쓸 때 잔재가 안남음
	
	// Vertex Shader : Input Assembler에 입력된 정점 버퍼에 들어있던
	// 정점 정보를 입력으로 받아들임
	// 입력으로 들어오는 정점을 제대로 분석하기 위해서는 레이아웃 필요

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}
