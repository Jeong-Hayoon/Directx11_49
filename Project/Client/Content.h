#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 Asset들을 나열

class TreeUI;

// 내용에 대해서는 TreeUI에게 일임
class Content :
    public UI
{

private:
    TreeUI* m_Tree;

public:
    void ResetContent();



public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

