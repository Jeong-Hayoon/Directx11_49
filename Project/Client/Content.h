#pragma once
#include "UI.h"

// AssetMgr �� �ε��Ǿ� �ִ� ��� Asset���� ����

class TreeUI;

// ���뿡 ���ؼ��� TreeUI���� ����
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

