#pragma once
#include "UI.h"

// Tree의 Node 1개
class TreeNode
{
private:
    // 소속되어 있는 TreeUI
    TreeUI*             m_Owner;

    // Node의 노출되는 이름
    string              m_Name;

    // 숨겨져 있는 고유 ID
    string              m_ID;

    // Node 안에 자식으로 Node를 들고 있을 수 있기 때문에 vector로 관리
    vector<TreeNode*>   m_vecChildNode;

    TreeNode*           m_ParentNode;

    // Node 하나가 가지고 있을 데이터(ex) Asset의 실제 주소)
    DWORD_PTR           m_Data;

    // Node의 형태를 Frame으로 할지 
    bool                m_bFrame;

    bool                m_bSelected;

public:
    // Node의 이름 지정하는 함수
    void SetName(string& _Name) { m_Name = _Name; } 

    void SetFrame(bool _Frame) { m_bFrame = _Frame; }

    const string& GetName() { return m_Name; }
    DWORD_PTR GetData() { return m_Data; }


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

    // 선택된 Node(멀티 셀렉트를 구현하고 싶다면 단일 포인터가 아닌 vector로 만들고 선택될 때마다 pushback)
    TreeNode*               m_Selected;

    // Drag & Drop된 Node 기록
    TreeNode*               m_DragNode;
    TreeNode*               m_DropNode;

    // Root Name 노출 여부
    bool                    m_bShowRoot;

    // Drag & Drop 기능 사용 여부
    bool                    m_bDragDrop;

    // Select가 발생했을 때 호출시킬 객체의 주소
    UI*                     m_SelectInst;

    // Select가 발생했을 때 호출시킬 함수 포인터
    Delegate_1              m_SelectFunc;

    // 이번 프레임에 특정 Node가 선택되는 일이 발생했는지 여부
    bool                    m_bSelectEvent;

    // =============== Drag & Drop =============== //
    // Drag & Drop 발생 시 호출시킬 객체와 멤버 함수
    UI*                     m_DragDropInst;
    Delegate_2              m_DragDropFunc;

    // Drag & Drop이 발생했는지 Check
    bool                    m_bDragDropEvent;


public:
    virtual void render_update() override;


public:
    // _Inst는 Content, Outliner, Inspector와 같은 UI 자체가 들어감
    void AddSelectDelegate(UI* _Inst, Delegate_1 _pFunc) { m_SelectInst = _Inst; m_SelectFunc = _pFunc; }

    void AddDragDropDelegate(UI* _Inst, Delegate_2 _pFunc) { m_DragDropInst = _Inst; m_DragDropFunc = _pFunc; }

    // Root Name 노출 여부 설정 함수
    void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }

    void UseDragDrop(bool _Use) { m_bDragDrop = _Use; }

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

private:
    // TreeNode가 본인이 선택되었다는 것을 TreeUI에게 알려주는 함수
    void SetSelectedNode(TreeNode* _SelectedNode);
    void SetDragNode(TreeNode* _DragNode);
    void SetDropNode(TreeNode* _DropNode);

public:
    TreeUI(const string& _ID);
    ~TreeUI();

    friend class TreeNode;
};

