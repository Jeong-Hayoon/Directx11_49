#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/HYAsset.h>

// Asset의 정보를 출력시켜주는 UI들의 부모 클래스
class AssetUI :
    public UI
{
private:
    // 담당 Asset
    Ptr<HYAsset>        m_Asset;
    
    // 담당하고 있는 Asset이 무슨 Type인지
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

