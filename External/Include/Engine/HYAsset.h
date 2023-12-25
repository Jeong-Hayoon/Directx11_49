#pragma once
#include "HYEntity.h"

class HYAsset :
    public HYEntity
{
private:
    wstring             m_Key;
    wstring             m_RelativePath;
    // ���ҽ��� Feference Count ���, Feference Count�� 0�� �Ǹ� delete
    int                 m_RefCount;
    // �� �� �����ϸ� Ÿ���� �ٲ��� ���ϵ��� const
    const ASSET_TYPE    m_Type;

public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }

private:
  
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = _RelativePath; }

    void AddRef() { ++m_RefCount; }
    void SubRef() { --m_RefCount; }

    int GetRefCount() { return m_RefCount; }

    ASSET_TYPE GetType() { return m_Type; }

    virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
    // ��ӹ��� Ŭ�������� ������ Ÿ���� �Է������ ��
    HYAsset(ASSET_TYPE _Type);
    ~HYAsset();

    friend class HYAssetMgr;
};

