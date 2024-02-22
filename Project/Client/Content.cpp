#include "pch.h"
#include "Content.h"

#include <Engine/HYAssetMgr.h>
#include <Engine/HYTaskMgr.h>

#include "HYImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

Content::Content()
	: UI("Content", "##Content")
{
	// ContentUI 자식으로 Tree 를 지정
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// AssetMgr 의 에셋상태를 트리에 적용한다.
	ResetContent();

	// 트리에 Delegate 를 등록한다.
	m_Tree->AddSelectDelegate(this, (Delegate_1)&Content::SelectAsset);
}

Content::~Content()
{
}

void Content::render_update()
{
	if (HYTaskMgr::GetInst()->GetAssetEvent())
	{
		ResetContent();
	}
}


// Content 내용을 Reset한 후 다시 현재 모든 Asset의 이름으로 Tree를 가득 채워주는 함수
void Content::ResetContent()
{
	// Tree Clear
	m_Tree->ClearNode();

	// 루트 노드 추가
	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// 중간층 노드
		TreeNode* CategoryNode = m_Tree->AddTreeNode(RootNode, ASSET_TYPE_STRING[i], 0);
		// 카테고리가 되는 노드들은 Frame 형태의 Node가 되도록 세팅
		CategoryNode->SetFrame(true);

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

// _Node : 선택된 Node의 주소
void Content::SelectAsset(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	if (nullptr == pNode)
		return;

	// 실제 Asset의 주소를 pAsset에 담는다.
	Ptr<HYAsset> pAsset = (HYAsset*)pNode->GetData();

	// 카테고리 형태의 Node들은 데이터를 안넣어놨기 때문에 nullptr이 들어오므로 return
	if (nullptr == pAsset)
		return;

	// 선택한 에셋을 Inspector 에게 알려준다.
	Inspector* pInspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");

	// 타겟이 되는 Asset 지정
	pInspector->SetTargetAsset(pAsset);
}