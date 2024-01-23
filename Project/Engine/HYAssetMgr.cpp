#include "pch.h"
#include "HYAssetMgr.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYMaterial.h"

HYAssetMgr::HYAssetMgr()
{
}

HYAssetMgr::~HYAssetMgr()
{
}

Ptr<HYTexture> HYAssetMgr::CreateTexture(const wstring& _strKey, 
										UINT _Width, UINT _Height, DXGI_FORMAT _Format,
										UINT _Flag, D3D11_USAGE _Usage)
{
	// �����Ϸ��� �ؽ��Ŀ� ������ Ű�� �ؽ��İ� �̹� AssetMgr �� �ִٸ�
	Ptr<HYTexture> pTex = FindAsset<HYTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new HYTexture;

	if (FAILED(pTex->Create(_Width, _Height, _Format, _Flag, _Usage)))
	{
		MessageBox(nullptr, L"�ؽ��� ���� ����", L"���ҽ� ���� ����", MB_OK);
		return nullptr;
	}

	// AssetMgr�� ���
	AddAsset<HYTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<HYTexture> HYAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D)
{
	// �����Ϸ��� �ؽ��Ŀ� ������ Ű�� �ؽ��İ� �̹� AssetMgr �� �ִٸ�
	Ptr<HYTexture> pTex = FindAsset<HYTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new HYTexture;

	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, L"�ؽ��� ���� ����", L"���ҽ� ���� ����", MB_OK);
		return nullptr;
	}

	AddAsset<HYTexture>(_strKey, pTex.Get());

	return pTex;
}

void HYAssetMgr::GetAssetName(ASSET_TYPE _Type, vector<string>& _Out)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		// first�� key���� string���� �ٲ㼭 ���� ���Ϳ� �����
		_Out.push_back(ToString(pair.first));
	}
}