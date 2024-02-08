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
	// �ڽ�Node�� ������(���� ���) ȭ��ǥ�� ������ �ʵ��� ����
	if (m_vecChildNode.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;
	// ���õ� Node���� ���̶���Ʈ ȿ��
	if (m_bSelected)
		Flag |= ImGuiTreeNodeFlags_Selected;

	// ������ �����̸鼭 ���� ����� ��쿡�� ������ �༭ ���� ���� ���߱�
	if (m_bFrame && m_vecChildNode.empty())
		strID = "   " + strID;


	// TreeNodeEx - Flag ���� �� �� ���� / TreeNode - �⺻��
	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		// Ŭ���� �Ǹ� 
		if (ImGui::IsItemClicked())
		{
			// Node�� ���� TreeUI���� �����ؼ� ���õ� ���� �����̶�� �˷���
			m_Owner->SetSelectedNode(this);
		}

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

	// Delegate ȣ��
	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			// m_SelectFunc�� �ּ��� �������� ȣ���ؾ��ϱ� ������ * �ʼ�
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}

	m_bSelectEvent = false;
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

	// � TreeUI�� �����ִ��� Owner�� ��������
	pNewNode->m_Owner = this;

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	// �̹� ������ ���õ� Node�� �־����� ���� Node�� ���������ְ�
	if (m_Selected)
	{
		m_Selected->m_bSelected = false;
	}

	// ���ο� Node�� ��ü
	m_Selected = _SelectedNode;

	if (nullptr != m_Selected)
	{
		m_Selected->m_bSelected = true;
	}

	// Select Event �߻�
	m_bSelectEvent = true;
}