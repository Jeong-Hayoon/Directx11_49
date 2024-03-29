#pragma once
#include "UI.h"

#include <Engine/HYGameObject.h>
#include <Engine/HYAsset.h>

class ComponentUI;
class AssetUI;

// 타겟된 물체의 정보를 띄우는 역할
// 1. GameObject 정보를 표현하는 기능
//  - 컴포넌트
//  - 스크립트

// 2. Asset 정보
//  - 각 Asset의 정보를 표현하는 기능
class Inspector :
    public UI
{
private:
    // 정보를 출력해야하는 Object
    HYGameObject*   m_TargetObject;
    // 정보를 보여줘야 하는 Asset
    Ptr<HYAsset>    m_TargetAsset;

    ComponentUI*    m_arrComUI[(UINT)COMPONENT_TYPE::END];

    AssetUI*        m_arrAssetUI[(UINT)ASSET_TYPE::END];


public:
    virtual void tick() override;

    virtual void render_update() override;


public:
    void SetTargetObject(HYGameObject* _Object);
    void SetTargetAsset(Ptr<HYAsset> _Asset);

    // 세팅된 타겟을 반환해주는 함수
    HYGameObject* GetTargetObject() { return m_TargetObject; }

private:
    void CreateChildUI();
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

