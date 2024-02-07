#pragma once
#include "UI.h"

// Tree�� Node 1��
class TreeNode
{
private:
    // Node�� ����Ǵ� �̸�
    string              m_Name;
    // ������ �ִ� ���� ID
    string              m_ID;
    // Node �ȿ� �ڽ����� Node�� ��� ���� �� �ֱ� ������ vector�� ����
    vector<TreeNode*>   m_vecChildNode;

    TreeNode*           m_ParentNode;
    // Node �ϳ��� ������ ���� ������(ex) Asset�� ���� �ּ�)
    DWORD_PTR           m_Data;

public:
    // Node�� �̸� �����ϴ� �Լ�
    void SetName(string& _Name) { m_Name = _Name; } 

private:
    // ID �������ִ� �Լ�(������ ���̵� ������ �ϱ� ������ Private)
    void SetID(const string& _ID) { m_ID = _ID; }

    // �θ� �˰� �ڽĵ� vector�� �����ϱ� ������ ���� �˰� ����
    void AddChildNode(TreeNode* _Node)
    {
        m_vecChildNode.push_back(_Node);
        // �θ� �� �� �ֵ���
        _Node->m_ParentNode = this;
    }


public:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};


// �ܵ����� â�� �ߴ� UI�� �ƴ϶� �ٸ� UI �ؿ� ���� ������ Child�� �����Ͽ� ����ϴ� �뵵
class TreeUI :
    public UI
{
private:
    // Node���� �̸��� ��ġ�� �ȵǴϱ� NodeID�� �ϳ��� ������Ű�鼭 ID���� ��
    static  UINT            NodeID;

private:
    // Node�� �߿��� ���� �ֻ��� Node(���� �ڽĵ��� �̹� �ֻ��� Node�� vector�� �����ϰ� �ֱ� ������ RootNode�� �˸� ��)
    TreeNode*               m_Root;
    // Root Name ���� ����
    bool                    m_bShowRoot;

public:
    virtual void render_update() override;


public:
    // Root Name ���� ���� ���� �Լ�
    void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }

    TreeNode* AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData);

    // �־���� Node���� �� Clear ���ִ� �Լ�
    void ClearNode()
    {
        if (nullptr != m_Root)
        {
            delete m_Root;
            m_Root = nullptr;
        }
    }

public:
    TreeUI(const string& _ID);
    ~TreeUI();
};

