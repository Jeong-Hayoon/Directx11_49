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

// ���� ���� �ʱ�ȭ
UINT TreeUI::NodeID = 0;

// ���� ������ ���������� ���� ���̱� ������ ���̵� ��ġ�� �ʵ��� ���ڷ� ID�� ����
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

// Node�� �߰��ϴ� �Լ�(�θ� Node, ��µ� �̸�, Node�� ���� ���� ������)
TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode;
	pNewNode->m_Data = _dwData;
	pNewNode->SetName(_strName);

	// ��帶�� ��ġ���ʴ� ���ڸ� ## �ڿ� ID �� ���δ�.(NodeID : ���� ����)
	UINT id = NodeID++;

	char buff[50] = {};
	// ���ڸ� ���ڷ� �ٲ��ִ� �Լ�(�����ּ�, ������� ���ڿ��� ����, ����)
	sprintf_s(buff, "##%d", id);
	pNewNode->SetID(buff);

	// Parent�� nullptr�̶�� ���� ������ Node�� Root�� �Ǳ� �ٶ�� ����ε�
	if (nullptr == _Parent)
	{
		// ���� �̹� Root�� �ִٸ� assert
		assert(!m_Root);

		m_Root = pNewNode;
	}
	else
	{
		_Parent->AddChildNode(pNewNode);
	}

	return pNewNode;
}
