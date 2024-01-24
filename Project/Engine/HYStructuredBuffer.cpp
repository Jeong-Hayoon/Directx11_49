#include "pch.h"
#include "HYStructuredBuffer.h"

#include "HYDevice.h"

HYStructuredBuffer::HYStructuredBuffer()
	: m_ElementSize(0)
	, m_ElementCount(0)
	, m_Type(SB_TYPE::READ_ONLY)
	, m_bSysMemMove(false)
{
}

HYStructuredBuffer::~HYStructuredBuffer()
{
}

int HYStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _pSysMem)
{
	// 구조화버퍼 크기가 16의 배수인지 체크(16배수 정렬이 되어 있어야 함)
	assert(!(_ElementSize % 16));

	// GPU에 메모리를 재할당하려고 할 때 m_SB, m_SRV, m_SB_Read, m_SB_Write가 최초 호출이라면
	// 아무 문제가 없고 추가 호출인 경우 Comptr이니까 RefCount가 줄어들고 delete 같은 효과를 냄
	m_SB = nullptr;
	m_SRV = nullptr;
	m_SB_Read = nullptr;
	m_SB_Write = nullptr;

	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;
	m_Type = _Type;
	m_bSysMemMove = _bSysMemMove;

	D3D11_BUFFER_DESC tDesc = {};
	// 버퍼의 크기
	tDesc.ByteWidth = m_ElementSize * m_ElementCount;
	// 원래 Texture의 BindFlags가 SHADER_RESOURCE인데 구조화 버퍼 또한 텍스처 레지스터에 바인딩을
	// 시킬거니까 SHADER_RESOURCE로 세팅해주면 됨
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// RW 옵션으로 생성된 구조화버퍼는 UnorderedAccessView 생성도 가능하게 만든다.
	if (SB_TYPE::READ_WRITE == m_Type)
	{
		tDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	// 버퍼의 요소 단위
	tDesc.StructureByteStride = m_ElementSize;

	// 구조화 버퍼에서는 CPUAccessFlags와 Usage를 Dynamtic 세팅이 불가능
	tDesc.CPUAccessFlags = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = E_FAIL;

	// 특정 주소가 초기값으로 들어왔다면
	if (nullptr == _pSysMem)
	{
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB.GetAddressOf());
	}
	else
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pSysMem;
		hr = DEVICE->CreateBuffer(&tDesc, &tSub, m_SB.GetAddressOf());
	}

	if (FAILED(hr)) return E_FAIL;

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	// ViewDimension : ShaderResource View에 전달되는 Buffer라는 정보
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	// 요소 개수
	SRVDesc.Buffer.NumElements = m_ElementCount;

	hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// Unordered Access View 생성
	if (SB_TYPE::READ_WRITE == m_Type)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = 1;

		hr = DEVICE->CreateUnorderedAccessView(m_SB.Get(), &UAVDesc, m_UAV.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;
	}

	// 구조화 버퍼는 CPUAccessFlags와 Usage를 수정할 수 없기 때문에
	// 데이터를 입력받는 용도로 버퍼를 하나 더 만듦
	if (m_bSysMemMove)
	{
		// 쓰기용 버퍼
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Write.GetAddressOf());

		// 읽기용 버퍼 - System mem로 다시 가져올 수 있음
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Read.GetAddressOf());
	}

	return S_OK;
}

// _RegisterNum 레지스터에 바인딩
void HYStructuredBuffer::UpdateData(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

// Buffer를 맵핑해서 언맵해주면 GPU로 보내짐(바뀐 데이터를 보내는 기능)
void HYStructuredBuffer::SetData(void* _SysMem, UINT _ElementCount)
{
	assert(m_bSysMemMove);

	// 기본 Default면 최대 카운트로
	if (0 == _ElementCount)
		_ElementCount = m_ElementCount;

	// 입력 데이터가 구조화버퍼보다 더 큰 경우
	if (m_ElementCount < _ElementCount)
	{
		Create(m_ElementSize, _ElementCount, m_Type, m_bSysMemMove, nullptr);
	}

	// 이 과정이 가능하려면 이 옵션이었어야 함
	// CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// Usage = D3D11_USAGE_DYNAMIC;
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	// m_SB_Write를 맵핑
	CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _SysMem, m_ElementSize * _ElementCount);
	CONTEXT->Unmap(m_SB_Write.Get(), 0);

	// Write Buffer -> Main Buffer
	CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

// GPU로 보낸 데이터를 읽어야할 때
// _Dest에 _ElementCount만큼 읽어들여서 데이터 복사 후 가져오는 함수
void HYStructuredBuffer::GetData(void* _Dest, UINT _ElementCount)
{
	assert(m_bSysMemMove);

	if (0 == _ElementCount)
		_ElementCount = m_ElementCount;

	// Main Buffer -> Read Buffer
	CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
	memcpy(_Dest, tSub.pData, m_ElementSize * _ElementCount);
	CONTEXT->Unmap(m_SB_Read.Get(), 0);
}
