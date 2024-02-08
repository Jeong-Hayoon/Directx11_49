#pragma once
#include "HYEntity.h"

#include "Ptr.h"

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
    int GetRefCount() { return m_RefCount; }
    ASSET_TYPE GetType() { return m_Type; }

private:
  
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = _RelativePath; }

    void AddRef() 
    { 
        ++m_RefCount; 
    }
    void Release() 
    {
        --m_RefCount;
        // Reference Count가 0이 되면 자기 자신을 소멸시키기
        if (0 >= m_RefCount)
        {
            delete this;
        }
    }

    virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
    // 상속받은 클래스들은 무조건 타입을 입력해줘야 함
    HYAsset(ASSET_TYPE _Type);
    ~HYAsset();

    // friend 선언 해주는 클래스들은 해당 클래스 private에 접근 가능
    friend class HYAssetMgr;

    // template<typename T>로 friend 선언을 해주게 되면
    // Ptr로 만들어지는 모든 클래스에게 friend 선언
    template<typename T>
    friend class Ptr;
};

