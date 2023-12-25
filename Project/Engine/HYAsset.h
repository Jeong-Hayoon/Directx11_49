#pragma once
#include "HYEntity.h"

class HYAsset :
    public HYEntity
{
private:
    wstring             m_Key;
    wstring             m_RelativePath;
    // 리소스는 Feference Count 기반, Feference Count이 0이 되면 delete
    int                 m_RefCount;
    // 한 번 세팅하면 타입을 바꾸지 못하도록 const
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
    // 상속받은 클래스들은 무조건 타입을 입력해줘야 함
    HYAsset(ASSET_TYPE _Type);
    ~HYAsset();

    friend class HYAssetMgr;
};

