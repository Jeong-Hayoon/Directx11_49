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

// AddAsset 일반 함수 버전
void HYAssetMgr::AddAsset(const wstring& _strKey, HYAsset* _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();

	map<wstring, Ptr<HYAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
	assert(iter == m_mapAsset[(UINT)Type].end());

	_Asset->SetKey(_strKey);
	_Asset->SetRelativePath(_strKey);
	m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

Ptr<HYTexture> HYAssetMgr::CreateTexture(const wstring& _strKey, 
										UINT _Width, UINT _Height, DXGI_FORMAT _Format,
										UINT _Flag, D3D11_USAGE _Usage)
{
	// 생성하려는 텍스쳐와 동일한 키의 텍스쳐가 이미 AssetMgr 에 있다면
	Ptr<HYTexture> pTex = FindAsset<HYTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new HYTexture;

	if (FAILED(pTex->Create(_Width, _Height, _Format, _Flag, _Usage)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	// AssetMgr에 등록
	AddAsset<HYTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<HYTexture> HYAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D)
{
	// 생성하려는 텍스쳐와 동일한 키의 텍스쳐가 이미 AssetMgr 에 있다면
	Ptr<HYTexture> pTex = FindAsset<HYTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new HYTexture;

	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset<HYTexture>(_strKey, pTex.Get());

	return pTex;
}

void HYAssetMgr::GetAssetName(ASSET_TYPE _Type, vector<string>& _Out)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		// first가 key값을 string으로 바꿔서 들어온 벡터에 담아줌
		_Out.push_back(ToString(pair.first));
	}
}