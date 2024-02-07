#include "pch.h"
#include "Content.h"

#include <Engine/HYAssetMgr.h>

#include "TreeUI.h"

Content::Content()
	: UI("Content", "##Content")
{
	// ContentUI 자식으로 Tree 를 지정
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// Tree 구성
	TreeNode* pNode = m_Tree->AddTreeNode(nullptr, "RootNode", 0);

	// pNode를 부모로 하는 Node 추가
	m_Tree->AddTreeNode(pNode, "Child1", 0);
	m_Tree->AddTreeNode(pNode, "Child2", 0);

	pNode = m_Tree->AddTreeNode(pNode, "Child3", 0);

	// Child3을 부모로 하는 Node 추가
	m_Tree->AddTreeNode(pNode, "Child3-1", 0);
	m_Tree->AddTreeNode(pNode, "Child3-2", 0);

	ResetContent();
}

Content::~Content()
{
}

void Content::render_update()
{
}


// Content 내용을 Reset한 후 다시 현재 모든 Asset의 이름으로 Tree를 가득 채워주는 함수
void Content::ResetContent()
{
	m_Tree->ClearNode();

	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// 중간층 노드
		TreeNode* CategoryNode = m_Tree->AddTreeNode(RootNode, ASSET_TYPE_STRING[i], 0);

		const map<wstring, Ptr<HYAsset>>& mapAsset = HYAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			// CategoryNode를 부모로 지정하여 그 밑으로 Asset 타입 나열
			// wstring으로 되어 있는 객체를 string으로 변환하여 넣어줌
			// data로는 Asset의 주소값 넣어주기
			m_Tree->AddTreeNode(CategoryNode
				, string(pair.first.begin(), pair.first.end())
				, (DWORD_PTR)pair.second.Get());
		}
	}
}