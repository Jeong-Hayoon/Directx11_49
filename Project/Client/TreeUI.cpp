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

	// 화살표를 누르거나 더블 클릭하는 경우에만 열리도록
	UINT Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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
	// true -> 자식이 없는 경우면서 Flag에 해당하는 경우
	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		// Drag 했을 때 가장 직전에 열렸던 것에 대해 판정(Drop하기 전까지 계속 호출됨)
		if (ImGui::BeginDragDropSource())
		{
			// SetDragDropPayload(키값(m_Owner - Content, Outliner, Inspector...), 데이터의 주소, 데이터의 크기) - Drag를 통해 이동시킬 데이터를 Set하는 함수
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));

			// 잡아끌리는 Object의 이름이 Drag할 때 보임
			ImGui::Text(m_Name.c_str());

			// BeginDragDropSource와 짝으로 꼭 해줘야 함
			ImGui::EndDragDropSource();

			// Tree 에 자신이 Drag 된 노드임을 알린다.
			m_Owner->SetDragNode(this);
		}
		// Drop된 경우
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
			// 클릭 판정
			if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				// Node가 속한 TreeUI에게 접근해서 선택된 것이 본인이라고 알려줌
				m_Owner->SetSelectedNode(this);
			}
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
	// false -> 자식이 있는 경우면서 Flag에 해당하는 경우
	else
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// Tree 에 자신이 Drag 된 노드임을 알린다.
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

// 정적 변수 초기화
UINT TreeUI::NodeID = 0;

// 여러 곳에서 범용적으료 사용될 것이기 때문에 아이디가 겹치지 않도록 인자로 ID를 받음
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

	// Delegate 호출
	if(m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			// m_SelectFunc는 주소의 원형으로 호출해야하기 때문에 * 필수
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}

	// 드래그 대상을 특정 노드가 아닌 공중드랍 시킨 경우
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

void TreeUI::SetDragNode(TreeNode* _DragNode)
{
	m_DragNode = _DragNode;
}

void TreeUI::SetDropNode(TreeNode* _DropNode)
{
	m_DropNode = _DropNode;
	m_bDragDropEvent = true;
}