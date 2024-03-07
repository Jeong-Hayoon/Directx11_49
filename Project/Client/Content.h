#pragma once
#include "UI.h"

// AssetMgr �� �ε��Ǿ� �ִ� ��� Asset���� ����

class TreeUI;

// ���뿡 ���ؼ��� TreeUI���� ����
class Content :
    public UI
{
private:
    TreeUI*             m_Tree;

    // Content ���� ���� ���� �̸��� �����ϴ� ����
    vector<wstring>     m_vecAssetFileName;

public:
    void ReloadContent();

    void ResetContent();

    // Asset�� ���õǾ��� �� ȣ��Ǵ� �Լ�
    void SelectAsset(DWORD_PTR _Node);

public:
    virtual void render_update() override;

private:
    void FindFileName(const wstring& _Directory);
    ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

public:
    Content();
    ~Content();
};

