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

    // Asset�� ���õǾ��� �� ȣ��Ǵ� �Լ�
    void SelectAsset(DWORD_PTR _Node);



public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

