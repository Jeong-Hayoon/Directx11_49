#include "pch.h"
#include "TreeUI.h"

TreeNode::TreeNode()
	: m_bFrame(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}


void TreeNode::render_update()
{
	string strID = m_Name + m_ID;

	UINT Flag = 0;

	if (m_bFrame)
		Flag |= ImGuiTreeNodeFlags_Framed;
	// 자식Node가 없으면(말단 노드) 화살표가 보이지 않도록 세팅
	if (m_vecChildNode.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;
	// 선택된 Node에게 하이라이트 효과
	if (m_bSelected)
		Flag |= ImGuiTreeNodeFlags_Selected;

	// 프레임 형태이면서 말단 노드인 경우에는 공백을 줘서 글자 간격 맞추기
	if (m_bFrame && m_vecChildNode.empty())
		strID = "   " + strID;


	// TreeNodeEx - Flag 값을 줄 수 있음 / TreeNode - 기본형
	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		// 클릭이 되면 
		if (ImGui::IsItemClicked())
		{
			// Node가 속한 TreeUI에게 접근해서 선택된 것이 본인이라고 알려줌
			m_Owner->SetSelectedNode(this);
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}

// 정적 변수 초기화
UINT TreeUI::NodeID = 0;

// 여러 곳에서 범용적으료 사용될 것이기 때문에 아이디가 겹치지 않도록 인자로 ID를 받음
TreeUI::TreeUI(const string& _ID)
	: UI("", _ID)
	, m_bShowRoot(true)
{
}

TreeUI::~TreeUI()
{
	delete m_Root;
}

void TreeUI::render_update()
{
	if (nullptr == m_Root)
		return;


	if (m_bShowRoot)
	{
		m_Root->render_update();
	}

	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
		{
			m_Root->m_vecChildNode[i]->render_update();
		}
	}

	// Delegate 호출
	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			// m_SelectFunc는 주소의 원형으로 호출해야하기 때문에 * 필수
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}

	m_bSelectEvent = false;
}

// Node를 추가하는 함수(부모 Node, 출력될 이름, Node에 집어 넣은 데이터)
TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode;
	pNewNode->m_Data = _dwData;
	pNewNode->SetName(_strName);

	// 노드마다 겹치지않는 숫자를 ## 뒤에 ID 로 붙인다.(NodeID : 정적 변수)
	UINT id = NodeID++;

	char buff[50] = {};
	// 숫자를 문자로 바꿔주는 함수(버퍼주소, 만들어질 문자열의 형태, 숫자)
	sprintf_s(buff, "##%d", id);
	pNewNode->SetID(buff);

	// Parent가 nullptr이라는 것은 생성할 Node가 Root가 되길 바라는 경우인데
	if (nullptr == _Parent)
	{
		// 만약 이미 Root가 있다면 assert
		assert(!m_Root);

		m_Root = pNewNode;
	}
	else
	{
		_Parent->AddChildNode(pNewNode);
	}

	// 어떤 TreeUI에 속해있는지 Owner를 지정해줌
	pNewNode->m_Owner = this;

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	// 이미 이전에 선택된 Node가 있었으면 기존 Node는 해제시켜주고
	if (m_Selected)
	{
		m_Selected->m_bSelected = false;
	}

	// 새로운 Node로 교체
	m_Selected = _SelectedNode;

	if (nullptr != m_Selected)
	{
		m_Selected->m_bSelected = true;
	}

	// Select Event 발생
	m_bSelectEvent = true;
}
