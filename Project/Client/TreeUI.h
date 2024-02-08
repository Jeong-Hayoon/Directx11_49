#pragma once
#include "UI.h"

// Tree�� Node 1��
class TreeNode
{
private:
    // �ҼӵǾ� �ִ� TreeUI
    TreeUI*             m_Owner;

    // Node�� ����Ǵ� �̸�
    string              m_Name;

    // ������ �ִ� ���� ID
    string              m_ID;

    // Node �ȿ� �ڽ����� Node�� ��� ���� �� �ֱ� ������ vector�� ����
    vector<TreeNode*>   m_vecChildNode;

    TreeNode*           m_ParentNode;

    // Node �ϳ��� ������ ���� ������(ex) Asset�� ���� �ּ�)
    DWORD_PTR           m_Data;

    // Node�� ���¸� Frame���� ���� 
    bool                m_bFrame;

    //
    bool                m_bSelected;

public:
    // Node�� �̸� �����ϴ� �Լ�
    void SetName(string& _Name) { m_Name = _Name; } 

    void SetFrame(bool _Frame) { m_bFrame = _Frame; }

    const string& GetName() { return m_Name; }
    DWORD_PTR GetData() { return m_Data; }


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

    // ���õ� Node(��Ƽ ����Ʈ�� �����ϰ� �ʹٸ� ���� �����Ͱ� �ƴ� vector�� ����� ���õ� ������ pushback)
    TreeNode*               m_Selected;

    // Root Name ���� ����
    bool                    m_bShowRoot;

    // Select�� �߻����� �� ȣ���ų ��ü�� �ּ�
    UI*                     m_SelectInst;

    // Select�� �߻����� �� ȣ���ų �Լ� ������
    Delegate_1              m_SelectFunc;

    // �̹� �����ӿ� Ư�� Node�� ���õǴ� ���� �߻��ߴ��� ����
    bool                    m_bSelectEvent;


public:
    virtual void render_update() override;


public:
    // _Inst�� Content, Outliner, Inspector�� ���� UI ��ü�� ��
    void AddSelectDelegate(UI* _Inst, Delegate_1 _pFunc) { m_SelectInst = _Inst; m_SelectFunc = _pFunc; }

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

private:
    // TreeNode�� ������ ���õǾ��ٴ� ���� TreeUI���� �˷��ִ� �Լ�
    void SetSelectedNode(TreeNode* _SelectedNode);

public:
    TreeUI(const string& _ID);
    ~TreeUI();

    friend class TreeNode;
};

