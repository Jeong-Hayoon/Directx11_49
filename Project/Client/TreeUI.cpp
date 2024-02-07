#include "pch.h"
#include "TreeUI.h"

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}


void TreeNode::render_update()
{
	string strID = m_Name + m_ID;

	if (ImGui::TreeNode(strID.c_str()))
	{
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

	return pNewNode;
}
