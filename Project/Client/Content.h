#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 Asset들을 나열

class TreeUI;

// 내용에 대해서는 TreeUI에게 일임
class Content :
    public UI
{
private:
    TreeUI*             m_Tree;

    // Content 폴더 내의 파일 이름을 저장하는 벡터
    vector<wstring>     m_vecAssetFileName;

public:
    void ReloadContent();

    void ResetContent();

    // Asset이 선택되었을 때 호출되는 함수
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

