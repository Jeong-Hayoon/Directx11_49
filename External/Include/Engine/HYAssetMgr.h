#pragma once
#include "singleton.h"

#include "HYPathMgr.h"
         
#include "HYTexture.h"
#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYComputeShader.h"
#include "HYMaterial.h"


class HYAssetMgr :
    public HYSingleton<HYAssetMgr>
{
    SINGLE(HYAssetMgr);
private:
    // 관리할 Asset의 type이 많기 때문에 template으로 만듦
    // map 배열로 관리를 하는데 한 map에 Asset 1개씩 배치해야 함
    // map의 개수가 Asset type 개수만큼 배열이 있어야 함
    // second가 이제 Ptr이므로 .Get함수를 통해 Asset* 주소를 받을 수 있음
    map<wstring, Ptr<HYAsset>>   m_mapAsset[(UINT)ASSET_TYPE::END];

public:
    void init();
private:
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultMaterial();
    void CreateDefaultComputeShader();

public:
    // template은 무조건 헤더에 구현해야 함

    // 각 Asset 타입별로 항목의 이름을 반환해주는 함수
    template<typename T>
    const map<wstring, Ptr<HYAsset>>& GetAssets();
    const map<wstring, Ptr<HYAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }

    // 만들어진 Asset 등록
    // T*로 한 이유는 그래야 알맞은 Asset type에 맞는 map 배열로
    // 넣을 수 있기 때문
    template<typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    void AddAsset(const wstring& _strKey, HYAsset* _Asset);

    // Asset을 찾아오는 기능
    template<typename T>
    Ptr<T> FindAsset(const wstring& _strKey);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

    // _Flag : D3D11_BIND_FLAG
    Ptr<HYTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    // 이미 SwapChain이 생성될 때 RenderTarget에 Texture2D 객체가 만들어지므로 그 객체를 통해 Texture로 전환하여 우리 자체 Texture 클래스로 만들어서 AssetMgr가 들고 있음
    Ptr<HYTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D);

public: 
    // 특정 Asset 타입의 이름들을 알려주는 함수
    void GetAssetName(ASSET_TYPE _Type, vector<string>& _Out);

};

// template의 타입을 알아내는 함수(전역함수)
template<typename T>
ASSET_TYPE GetAssetType()
{
    const type_info& info = typeid(T);

    ASSET_TYPE Type = ASSET_TYPE::END;
    
    // 두 주소가 일치한다면 해당 타입으로 땅땅
    // 아래와 같이 hash_code로도 확인 가능
    // if (info.hash_code() == &typeid(HYMesh).hash_code())
    if (&info == &typeid(HYMesh))
        Type = ASSET_TYPE::MESH;
    else if (&info == &typeid(HYTexture))
        Type = ASSET_TYPE::TEXTURE;
    else if (&info == &typeid(HYGraphicsShader))
        Type = ASSET_TYPE::GRAPHICS_SHADER;
    else if (&info == &typeid(HYComputeShader))
        Type = ASSET_TYPE::COMPUTE_SHADER;
    else if (&info == &typeid(HYMaterial))
        Type = ASSET_TYPE::MATERIAL;

    return Type;
}

template<typename T>
inline const map<wstring, Ptr<HYAsset>>& HYAssetMgr::GetAssets()
{
    ASSET_TYPE Type = GetAssetType<T>();

    return m_mapAsset[(UINT)Type];
}

template<typename T>
inline void HYAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    // 중복될수도 있으니까 우선 find
    map<wstring, Ptr<HYAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    // end가 아니라면 이미 존재한다는 의미이므로 assert 처리
    assert(iter == m_mapAsset[(UINT)Type].end());

    _Asset->SetKey(_strKey);
    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}



template<typename T>
Ptr<T> HYAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<HYAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    // Asset이 없다면
    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    // 부모 타입이 아니라 상속받아간 자식 클래스를 find 할수도 있으니까 
    return (T*)iter->second.Get();
}

template<typename T>
Ptr<T> HYAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<T> pAsset = FindAsset<T>(_strKey);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
    if (nullptr != pAsset)
    {
        return (T*)pAsset.Get();
    }

    wstring strFilePath = HYPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;

    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    // 에셋이 에셋 매니저에 무슨 키로 등록되었고, 어느 경로로부터 로딩되었는지
    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);  

    AddAsset<T>(_strKey, (T*)pAsset.Get());

    return (T*)pAsset.Get();
}
