#pragma once
#include "UI.h"

#include <Engine/HYGameObject.h>
#include <Engine/HYAsset.h>

class ComponentUI;
class AssetUI;

// Ÿ�ٵ� ��ü�� ������ ���� ����
// 1. GameObject ������ ǥ���ϴ� ���
//  - ������Ʈ
//  - ��ũ��Ʈ

// 2. Asset ����
//  - �� Asset�� ������ ǥ���ϴ� ���
class Inspector :
    public UI
{
private:
    // ������ ����ؾ��ϴ� Object
    HYGameObject*   m_TargetObject;
    // ������ ������� �ϴ� Asset
    Ptr<HYAsset>    m_TargetAsset;

    ComponentUI*    m_arrComUI[(UINT)COMPONENT_TYPE::END];

    AssetUI*        m_arrAssetUI[(UINT)ASSET_TYPE::END];


public:
    virtual void tick() override;

    virtual void render_update() override;


public:
    void SetTargetObject(HYGameObject* _Object);
    void SetTargetAsset(Ptr<HYAsset> _Asset);

    // ���õ� Ÿ���� ��ȯ���ִ� �Լ�
    HYGameObject* GetTargetObject() { return m_TargetObject; }

private:
    void CreateChildUI();
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

