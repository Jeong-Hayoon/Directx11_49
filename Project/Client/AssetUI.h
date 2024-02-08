#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/HYAsset.h>

// Asset�� ������ ��½����ִ� UI���� �θ� Ŭ����
class AssetUI :
    public UI
{
private:
    // ��� Asset
    Ptr<HYAsset>        m_Asset;
    
    // ����ϰ� �ִ� Asset�� ���� Type����
    ASSET_TYPE          m_Type;

public:
    Ptr<HYAsset> GetAsset() { return m_Asset; }
    void SetAsset(Ptr<HYAsset> _Asset);

    ASSET_TYPE GetType() { return m_Type; }


public:
    virtual void render_update() override;


public:
    AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _Type);
    ~AssetUI();
};

