#pragma once
#include "singleton.h"
#include "HYAsset.h"


class HYMesh;
class HYGraphicsShader;

class HYAssetMgr :
    public HYSingleton<HYAssetMgr>
{
    SINGLE(HYAssetMgr);
private:
    // ������ Asset�� type�� ���� ������ template���� ����
    // map �迭�� ������ �ϴµ� �� map�� Asset 1���� ��ġ�ؾ� ��
    // map�� ������ Asset type ������ŭ �迭�� �־�� ��
    map<wstring, HYAsset*>   m_mapAsset[(UINT)ASSET_TYPE::END];


public:

    void init();


public:
    // template�� ������ ����� �����ؾ� ��
    // ������� Asset ���
    // T*�� �� ������ �׷��� �˸��� Asset type�� �´� map �迭��
    // ���� �� �ֱ� ����
    template<typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    // Asset�� ã�ƿ��� ���
    template<typename T>
    T* FindAsset(const wstring& _strKey);

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
    else if (&info == &typeid(HYGraphicsShader))
        Type = ASSET_TYPE::GRAPHICS_SHADER;

    return Type;
}

template<typename T>
inline void HYAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    // �ߺ��ɼ��� �����ϱ� �켱 find
    map<wstring, HYAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    // end�� �ƴ϶�� �̹� �����Ѵٴ� �ǹ��̹Ƿ� assert ó��
    assert(iter == m_mapAsset[(UINT)Type].end());

    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

template<typename T>
inline T* HYAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, HYAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    // Asset�� ���ٸ�
    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    return (T*)iter->second;
}
