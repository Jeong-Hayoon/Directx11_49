#pragma once
#include "UI.h"

// Tree의 Node 1개
class TreeNode
{
private:
    // Node의 노출되는 이름
    string              m_Name;
    // 숨겨져 있는 고유 ID
    string              m_ID;
    // Node 안에 자식으로 Node를 들고 있을 수 있기 때문에 vector로 관리
    vector<TreeNode*>   m_vecChildNode;

    TreeNode*           m_ParentNode;
    // Node 하나가 가지고 있을 데이터(ex) Asset의 실제 주소)
    DWORD_PTR           m_Data;

public:
    // Node의 이름 지정하는 함수
    void SetName(string& _Name) { m_Name = _Name; } 

private:
    // ID 설정해주는 함수(고유한 아이디를 가져야 하기 때문에 Private)
    void SetID(const string& _ID) { m_ID = _ID; }

    // 부모도 알고 자식도 vector로 관리하기 때문에 서로 알고 있음
    void AddChildNode(TreeNode* _Node)
    {
        m_vecChildNode.push_back(_Node);
        // 부모를 알 수 있도록
        _Node->m_ParentNode = this;
    }


public:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};


// 단독으로 창이 뜨는 UI가 아니라 다른 UI 밑에 내부 영역을 Child로 지정하여 사용하는 용도
class TreeUI :
    public UI
{
private:
    // Node별로 이름이 겹치면 안되니까 NodeID를 하나씩 증가시키면서 ID값을 줌
    static  UINT            NodeID;

private:
    // Node들 중에서 가장 최상위 Node(밑의 자식들은 이미 최상위 Node가 vector로 관리하고 있기 때문에 RootNode만 알면 됨)
    TreeNode*               m_Root;
    // Root Name 노출 여부
    bool                    m_bShowRoot;

public:
    virtual void render_update() override;


public:
    // Root Name 노출 여부 설정 함수
    void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }

    TreeNode* AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData);

    // 넣어놓은 Node들을 다 Clear 해주는 함수
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

