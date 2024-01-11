#include "pch.h"
#include "HYTexture.h"

#include "HYDevice.h"

HYTexture::HYTexture()
	: HYAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
{
}

HYTexture::~HYTexture()
{
}

int HYTexture::Load(const wstring& _strFilePath)
{
	// ============== 시스템 메모리에 로딩 ==============
	// 알고 싶은 문자열을 담을 배열
	wchar_t szExt[20] = {};

	// _wsplitpath_s : 들어온 문자열을 쪼개주는 함수
	// 확장자만 나오도록
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	// wcscmp : 문자열 비교해서 일치하면 0이 나옴
	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		// 로딩된 이미지를 인자로 보낸 스크래치 이미지 m_Image에 담아줌
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	// 위의 두 개 확장자를 제외한 잘 알려진 포맷은 LoadFromWICFile로 통용
	else
	{
		// png, bmp, jpg, jpeg
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ============== GPU 메모리에 로딩 ==============

	// 1. ID3D11Texture2D 객체 생성
	// 2. ScratchImage 에 있는 데이터를 ID3D11Texture2D 객체 로 전달
	// 3. ID3D11Texture2D 를 이용한 ShaderResourceView 만들기
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// 만들어진 ShaderResourceView 를 이용해서 생성된 ID3D11Texture2D 객체를 알아낸다.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ID3D11Texture2D 객체를 통해서 생성될때 사용된 옵션(DESC) 정보를 가져온다.
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}


int HYTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	// Texture 생성
	// 픽셀 포맷
	m_Desc.Format = _Format;

	// 텍스쳐 해상도
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;

	// 텍스쳐 용도
	m_Desc.BindFlags = _BindFlag;

	// CPU 접근
	m_Desc.Usage = _Usage;
	if (_Usage == D3D11_USAGE_DYNAMIC)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	// 샘플링
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	// 저퀄리티 버전의 사본 생성여부
	m_Desc.MipLevels = 1;
	m_Desc.MiscFlags = 0;
	m_Desc.ArraySize = 1;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// View 생성
	// 만약 입력된 BindFlags 안의 비트 값이 D3D11_BIND_DEPTH_STENCIL이 있었다면 
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	// Depth Stencil은 다른 것과 조합이 안되므로 else
	else
	{
		// 만약 입력된 BindFlags 안의 비트 값이 D3D11_BIND_RENDER_TARGET이 있었다면 
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		// 만약 입력된 BindFlags 안의 비트 값이 D3D11_BIND_SHADER_RESOURCE이 있었다면 
		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		// 만약 입력된 BindFlags 안의 비트 값이 D3D11_BIND_UNORDERED_ACCESS이 있었다면 
		// UNORDERED_ACCESS : Compute Shader에서만 사용할 수 있는 텍스처 타입
		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

// RegisterNum을 알려주면 그 번호로 보냄
void HYTexture::UpdateData(int _RegisterNum)
{
	// ShaderResource : 랜더링에 필요한 자원
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

// Texture 레지스터를 nullptr로 깔끔하게 비워주는 함수
void HYTexture::Clear(int _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}