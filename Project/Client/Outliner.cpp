#include "pch.h"
#include "Outliner.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>

#include "HYImGuiMgr.h"
#include "Inspector.h"

#include "TreeUI.h"

Outliner::Outliner()
	: UI("Outliner", "##Outliner")
{
	m_Tree = new TreeUI("OutlinerTree");
	m_Tree->ShowRootNode(false);
	// Drag & Drop ��� ���
	m_Tree->UseDragDrop(true);

	AddChildUI(m_Tree);

	// Ʈ���� Ŭ�� �̺�Ʈ ���(this ��ü�� Outliner::SelectObject �Լ� ȣ��)
	m_Tree->AddSelectDelegate(this, (Delegate_1)&Outliner::SelectObject);

	// Ʈ���� Drag & Drop �̺�Ʈ ���
	m_Tree->AddDragDropDelegate(this, (Delegate_2)&Outliner::DragDropObject);

	// Ʈ�� ������ ���� ������ ��ü��� ����
	ResetCurrentLevel();
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
}

// ���� ������ ���¸� Ȯ���� �� �ִ� �Լ�
void Outliner::ResetCurrentLevel()
{
	// Ʈ�� ������ ����
	m_Tree->ClearNode();

	// Ʈ���� ��Ʈ �߰�
	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

	// ���� ������ �����´�.
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		HYLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<HYGameObject*>& vecParent = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			AddObjectToTree(pRootNode, vecParent[i]);
		}
	}
}

// Tree�� Object�� ���� �ִ� �Լ�
void Outliner::AddObjectToTree(TreeNode* _Node, HYGameObject* _Object)
{
	TreeNode* pNode = m_Tree->AddTreeNode(_Node, string(_Object->GetName().begin(), _Object->GetName().end()), (DWORD_PTR)_Object);

	const vector<HYGameObject*>& vecChild = _Object->GetChild();

	// �ڽ��� �־����� �ڽĵ� Nodeȭ
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddObjectToTree(pNode, vecChild[i]);
	}
}

void Outliner::SelectObject(DWORD_PTR _Node)
{
	// _Node : Node�� �ּҰ��� ����
	TreeNode* pNode = (TreeNode*)_Node;

	// GetData : GameObject�� �ּҸ� �־������
	HYGameObject* pObject = (HYGameObject*)pNode->GetData();

	if (nullptr == pObject)
		return;

	Inspector* pInspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pObject);
}

void Outliner::DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source)
{
	TreeNode* pDestNode = (TreeNode*)_Dest;
	TreeNode* pSourceNode = (TreeNode*)_Source;

	if (nullptr == pDestNode)
	{
		HYGameObject* pSourceObj = (HYGameObject*)pSourceNode->GetData();

		int LayerIdx = pSourceObj->DisconnectWithParent();

		// ���� �θ� �����ٸ�
		if (LayerIdx == -1)
			return;

		HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(pSourceObj, LayerIdx, false);
	}
	else
	{
		HYGameObject* pDestObj = (HYGameObject*)pDestNode->GetData();
		HYGameObject* pSourceObj = (HYGameObject*)pSourceNode->GetData();
		pDestObj->AddChild(pSourceObj);
	}

	ResetCurrentLevel();
}

