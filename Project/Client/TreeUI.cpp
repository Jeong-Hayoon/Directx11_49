#include "pch.h"
#include "TreeUI.h"

#include <Engine/HYKeyMgr.h>

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

	// ȭ��ǥ�� �����ų� ���� Ŭ���ϴ� ��쿡�� ��������
	UINT Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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
	// true -> �ڽ��� ���� ���鼭 Flag�� �ش��ϴ� ���
	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		// Drag ���� �� ���� ������ ���ȴ� �Ϳ� ���� ����(Drop�ϱ� ������ ��� ȣ���)
		if (ImGui::BeginDragDropSource())
		{
			// SetDragDropPayload(Ű��(m_Owner - Content, Outliner, Inspector...), �������� �ּ�, �������� ũ��) - Drag�� ���� �̵���ų �����͸� Set�ϴ� �Լ�
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));

			// ��Ʋ����� Object�� �̸��� Drag�� �� ����
			ImGui::Text(m_Name.c_str());

			// BeginDragDropSource�� ¦���� �� ����� ��
			ImGui::EndDragDropSource();

			// Tree �� �ڽ��� Drag �� ������� �˸���.
			m_Owner->SetDragNode(this);
		}
		// Drop�� ���
		else if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());

			if (payload)
			{
				m_Owner->SetDropNode(this);
			}

			ImGui::EndDragDropTarget();
		}
		else
		{
			// Ŭ�� ����
			if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				// Node�� ���� TreeUI���� �����ؼ� ���õ� ���� �����̶�� �˷���
				m_Owner->SetSelectedNode(this);
			}
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
	// false -> �ڽ��� �ִ� ���鼭 Flag�� �ش��ϴ� ���
	else
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// Tree �� �ڽ��� Drag �� ������� �˸���.
			m_Owner->SetDragNode(this);
		}
		else
		{
			if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				m_Owner->SetSelectedNode(this);
			}
		}
	}

}

// ���� ���� �ʱ�ȭ
UINT TreeUI::NodeID = 0;

// ���� ������ ���������� ���� ���̱� ������ ���̵� ��ġ�� �ʵ��� ���ڷ� ID�� ����
TreeUI::TreeUI(const string& _ID)
	: UI("", _ID)
	, m_bShowRoot(true)
	, m_bDragDrop(false)
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
	if(m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			// m_SelectFunc�� �ּ��� �������� ȣ���ؾ��ϱ� ������ * �ʼ�
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}

	// �巡�� ����� Ư�� ��尡 �ƴ� ���ߵ�� ��Ų ���
	if(KEY_RELEASED(KEY::LBTN) && m_DragNode && !m_DropNode)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}
		m_DragNode = nullptr;
	}
	else if (m_bDragDropEvent)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}

		m_DropNode = nullptr;
		m_DragNode = nullptr;
	}

	m_bSelectEvent = false;
	m_bDragDropEvent = false;
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

void TreeUI::SetDragNode(TreeNode* _DragNode)
{
	m_DragNode = _DragNode;
}

void TreeUI::SetDropNode(TreeNode* _DropNode)
{
	m_DropNode = _DropNode;
	m_bDragDropEvent = true;
}