#include "pch.h"
#include "Outliner.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>

#include "TreeUI.h"

Outliner::Outliner()
	: UI("Outliner", "##Outliner")
{
	m_Tree = new TreeUI("OutlinerTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	ResetCurrentLevel();
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
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
