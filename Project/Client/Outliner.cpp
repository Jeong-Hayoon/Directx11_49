#include "pch.h"
#include "Outliner.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>
#include <Engine/HYKeyMgr.h>
#include <Engine/HYTaskMgr.h>

#include "HYImGuiMgr.h"
#include "Inspector.h"

#include "TreeUI.h"

Outliner::Outliner()
	: UI("Outliner", "##Outliner")
{
	m_Tree = new TreeUI("OutlinerTree");
	m_Tree->ShowRootNode(false);
	// Drag & Drop 기능 사용
	m_Tree->UseDragDrop(true);

	AddChildUI(m_Tree);

	// 트리에 클릭 이벤트 등록(this 객체로 Outliner::SelectObject 함수 호출)
	m_Tree->AddSelectDelegate(this, (Delegate_1)&Outliner::SelectObject);

	// 트리에 Drag & Drop 이벤트 등록
	m_Tree->AddDragDropDelegate(this, (Delegate_2)&Outliner::DragDropObject);

	// 트리 내용을 현재 레벨의 물체들로 구성
	ResetCurrentLevel();
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
	if (HYTaskMgr::GetInst()->GetObjectEvent())
	{
		ResetCurrentLevel();
	}

	if (KEY_TAP(KEY::DEL))
	{
		// 선택된 노드의 데이터에 접근하여 게임 오브젝트 삭제
		TreeNode* pNode = m_Tree->GetSelectedNode();
		if (nullptr != pNode)
		{
			HYGameObject* pSelectObj = (HYGameObject*)pNode->GetData();
			GamePlayStatic::DestroyGameObject(pSelectObj);
		}
	}
}

// 현재 레벨의 상태를 확인할 수 있는 함수
void Outliner::ResetCurrentLevel()
{
	// 트리 내용을 삭제
	m_Tree->ClearNode();

	// 트리에 루트 추가
	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

	// 현재 레벨을 가져온다.
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

// Tree에 Object를 집어 넣는 함수
void Outliner::AddObjectToTree(TreeNode* _Node, HYGameObject* _Object)
{
	TreeNode* pNode = m_Tree->AddTreeNode(_Node, string(_Object->GetName().begin(), _Object->GetName().end()), (DWORD_PTR)_Object);

	const vector<HYGameObject*>& vecChild = _Object->GetChild();

	// 자식이 있었으면 자식도 Node화
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddObjectToTree(pNode, vecChild[i]);
	}
}

void Outliner::SelectObject(DWORD_PTR _Node)
{
	// _Node : Node의 주소값이 들어옴
	TreeNode* pNode = (TreeNode*)_Node;

	// GetData : GameObject의 주소를 넣어놓았음
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

	HYGameObject* pDestObj = nullptr;
	if (nullptr != pDestNode)
		pDestObj = (HYGameObject*)pDestNode->GetData();

	HYGameObject* pSourceObj = (HYGameObject*)pSourceNode->GetData();

	// 부모 오브젝트가 자신의 자식오브젝트의 자식으로 들어가려는 경우는 방지
	if (pDestObj != nullptr && pDestObj->IsAncestor(pSourceObj))
		return;

	if (nullptr == pDestNode)
	{
		int LayerIdx = pSourceObj->DisconnectWithParent();

		// 원래 부모가 없었다면
		if (LayerIdx == -1)
			return;

		HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(pSourceObj, LayerIdx, false);
	}
	else
	{
		pDestObj->AddChild(pSourceObj);
	}

	ResetCurrentLevel();
}

