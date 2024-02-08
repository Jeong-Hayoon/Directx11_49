#pragma once
#include "HYEntity.h"

#include "Ptr.h"

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
        // Reference Count�� 0�� �Ǹ� �ڱ� �ڽ��� �Ҹ��Ű��
        if (0 >= m_RefCount)
        {
            delete this;
        }
    }

    virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
    // ��ӹ��� Ŭ�������� ������ Ÿ���� �Է������ ��
    HYAsset(ASSET_TYPE _Type);
    ~HYAsset();

    // friend ���� ���ִ� Ŭ�������� �ش� Ŭ���� private�� ���� ����
    friend class HYAssetMgr;

    // template<typename T>�� friend ������ ���ְ� �Ǹ�
    // Ptr�� ��������� ��� Ŭ�������� friend ����
    template<typename T>
    friend class Ptr;
};

