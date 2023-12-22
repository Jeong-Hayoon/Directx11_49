#pragma once
#include "singleton.h"
#include "HYAsset.h"

#include "HYPathMgr.h"
         
#include "HYTexture.h"
#include "HYMesh.h"
#include "HYGraphicsShader.h"


class HYAssetMgr :
    public HYSingleton<HYAssetMgr>
{
    SINGLE(HYAssetMgr);
private:
    // 관리할 Asset의 type이 많기 때문에 template으로 만듦
    // map 배열로 관리를 하는데 한 map에 Asset 1개씩 배치해야 함
    // map의 개수가 Asset type 개수만큼 배열이 있어야 함
    map<wstring, HYAsset*>   m_mapAsset[(UINT)ASSET_TYPE::END];


public:

    void init();


public:
    // template은 무조건 헤더에 구현해야 함
    // 만들어진 Asset 등록
    // T*로 한 이유는 그래야 알맞은 Asset type에 맞는 map 배열로
    // 넣을 수 있기 때문
    template<typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    // Asset을 찾아오는 기능
    template<typename T>
    T* FindAsset(const wstring& _strKey);

    template<typename T>
    T* Load(const wstring& _strKey, const wstring& _strRelativePath);

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

    return Type;
}

template<typename T>
inline void HYAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    // 중복될수도 있으니까 우선 find
    map<wstring, HYAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    // end가 아니라면 이미 존재한다는 의미이므로 assert 처리
    assert(iter == m_mapAsset[(UINT)Type].end());

    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

template<typename T>
inline T* HYAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, HYAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    // Asset이 없다면
    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    return (T*)iter->second;
}

template<typename T>
inline T* HYAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    HYAsset* pAsset = FindAsset<T>(_strKey);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
    if (nullptr != pAsset)
    {
        return (T*)pAsset;
    }

    wstring strFilePath = HYPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;

    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        delete pAsset;
        return nullptr;
    }

    // 에셋이 에셋 매니저에 무슨 키로 등록되었고, 어느 경로로부터 로딩되었는지
    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);

    AddAsset<T>(_strKey, (T*)pAsset);

    return (T*)pAsset;
}
