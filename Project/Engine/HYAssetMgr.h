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
    // ������ Asset�� type�� ���� ������ template���� ����
    // map �迭�� ������ �ϴµ� �� map�� Asset 1���� ��ġ�ؾ� ��
    // map�� ������ Asset type ������ŭ �迭�� �־�� ��
    // second�� ���� Ptr�̹Ƿ� .Get�Լ��� ���� Asset* �ּҸ� ���� �� ����
    map<wstring, Ptr<HYAsset>>   m_mapAsset[(UINT)ASSET_TYPE::END];

public:
    void init();
private:
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultMaterial();
    void CreateDefaultComputeShader();

public:
    // template�� ������ ����� �����ؾ� ��

    // �� Asset Ÿ�Ժ��� �׸��� �̸��� ��ȯ���ִ� �Լ�
    template<typename T>
    const map<wstring, Ptr<HYAsset>>& GetAssets();
    const map<wstring, Ptr<HYAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }

    // ������� Asset ���
    // T*�� �� ������ �׷��� �˸��� Asset type�� �´� map �迭��
    // ���� �� �ֱ� ����
    template<typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    void AddAsset(const wstring& _strKey, HYAsset* _Asset);

    // Asset�� ã�ƿ��� ���
    template<typename T>
    Ptr<T> FindAsset(const wstring& _strKey);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

    // _Flag : D3D11_BIND_FLAG
    Ptr<HYTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    // �̹� SwapChain�� ������ �� RenderTarget�� Texture2D ��ü�� ��������Ƿ� �� ��ü�� ���� Texture�� ��ȯ�Ͽ� �츮 ��ü Texture Ŭ������ ���� AssetMgr�� ��� ����
    Ptr<HYTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D);

public: 
    // Ư�� Asset Ÿ���� �̸����� �˷��ִ� �Լ�
    void GetAssetName(ASSET_TYPE _Type, vector<string>& _Out);

};

// template�� Ÿ���� �˾Ƴ��� �Լ�(�����Լ�)
template<typename T>
ASSET_TYPE GetAssetType()
{
    const type_info& info = typeid(T);

    ASSET_TYPE Type = ASSET_TYPE::END;
    
    // �� �ּҰ� ��ġ�Ѵٸ� �ش� Ÿ������ ����
    // �Ʒ��� ���� hash_code�ε� Ȯ�� ����
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

    // �ߺ��ɼ��� �����ϱ� �켱 find
    map<wstring, Ptr<HYAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    // end�� �ƴ϶�� �̹� �����Ѵٴ� �ǹ��̹Ƿ� assert ó��
    assert(iter == m_mapAsset[(UINT)Type].end());

    _Asset->SetKey(_strKey);
    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}



template<typename T>
Ptr<T> HYAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<HYAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    // Asset�� ���ٸ�
    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    // �θ� Ÿ���� �ƴ϶� ��ӹ޾ư� �ڽ� Ŭ������ find �Ҽ��� �����ϱ� 
    return (T*)iter->second.Get();
}

template<typename T>
Ptr<T> HYAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<T> pAsset = FindAsset<T>(_strKey);

    // �ε��� �� ����� Ű�� �̹� �ٸ� ������ �ִٸ�
    if (nullptr != pAsset)
    {
        return (T*)pAsset.Get();
    }

    wstring strFilePath = HYPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;

    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"���� �ε� ����", L"���� �ε� ����", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    // ������ ���� �Ŵ����� ���� Ű�� ��ϵǾ���, ��� ��ηκ��� �ε��Ǿ�����
    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);  

    AddAsset<T>(_strKey, (T*)pAsset.Get());

    return (T*)pAsset.Get();
}
