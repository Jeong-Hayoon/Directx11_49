#pragma once
#include "UI.h"

#include <Engine/HYGameObject.h>
#include <Engine/HYAsset.h>

class TransformUI;

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

    TransformUI*    m_TransformUI;

public:
    virtual void tick() override;

    virtual void render_update() override;


public:
    void SetTargetObject(HYGameObject* _Object);
    void SetTargetAsset(Ptr<HYAsset> _Asset);

public:
    Inspector();
    ~Inspector();
};

