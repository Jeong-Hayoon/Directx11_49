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