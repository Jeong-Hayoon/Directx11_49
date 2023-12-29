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
	// ============== �ý��� �޸𸮿� �ε� ==============
	// �˰� ���� ���ڿ��� ���� �迭
	wchar_t szExt[20] = {};

	// _wsplitpath_s : ���� ���ڿ��� �ɰ��ִ� �Լ�
	// Ȯ���ڸ� ��������
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	// wcscmp : ���ڿ� ���ؼ� ��ġ�ϸ� 0�� ����
	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		// �ε��� �̹����� ���ڷ� ���� ��ũ��ġ �̹��� m_Image�� �����
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	// ���� �� �� Ȯ���ڸ� ������ �� �˷��� ������ LoadFromWICFile�� ���
	else
	{
		// png, bmp, jpg, jpeg
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ============== GPU �޸𸮿� �ε� ==============

	// 1. ID3D11Texture2D ��ü ����
	// 2. ScratchImage �� �ִ� �����͸� ID3D11Texture2D ��ü �� ����
	// 3. ID3D11Texture2D �� �̿��� ShaderResourceView �����
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// ������� ShaderResourceView �� �̿��ؼ� ������ ID3D11Texture2D ��ü�� �˾Ƴ���.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ID3D11Texture2D ��ü�� ���ؼ� �����ɶ� ���� �ɼ�(DESC) ������ �����´�.
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

// RegisterNum�� �˷��ָ� �� ��ȣ�� ����
void HYTexture::UpdateData(int _RegisterNum)
{
	// ShaderResource : �������� �ʿ��� �ڿ�
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

// Texture �������͸� nullptr�� ����ϰ� ����ִ� �Լ�
void HYTexture::Clear(int _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}