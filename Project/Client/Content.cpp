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
	// ContentUI �ڽ����� Tree �� ����
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// AssetMgr �� ���»��¸� Ʈ���� �����Ѵ�.
	ResetContent();

	// Ʈ���� Delegate �� ����Ѵ�.
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


// Content ������ Reset�� �� �ٽ� ���� ��� Asset�� �̸����� Tree�� ���� ä���ִ� �Լ�
void Content::ResetContent()
{
	// Tree Clear
	m_Tree->ClearNode();

	// ��Ʈ ��� �߰�
	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// �߰��� ���
		TreeNode* CategoryNode = m_Tree->AddTreeNode(RootNode, ASSET_TYPE_STRING[i], 0);
		// ī�װ��� �Ǵ� ������ Frame ������ Node�� �ǵ��� ����
		CategoryNode->SetFrame(true);

		const map<wstring, Ptr<HYAsset>>& mapAsset = HYAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			// CategoryNode�� �θ�� �����Ͽ� �� ������ Asset Ÿ�� ����
			// wstring���� �Ǿ� �ִ� ��ü�� string���� ��ȯ�Ͽ� �־���
			// data�δ� Asset�� �ּҰ� �־��ֱ�
			m_Tree->AddTreeNode(CategoryNode
				, string(pair.first.begin(), pair.first.end())
				, (DWORD_PTR)pair.second.Get());
		}
	}
}

// _Node : ���õ� Node�� �ּ�
void Content::SelectAsset(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	if (nullptr == pNode)
		return;

	// ���� Asset�� �ּҸ� pAsset�� ��´�.
	Ptr<HYAsset> pAsset = (HYAsset*)pNode->GetData();

	// ī�װ� ������ Node���� �����͸� �ȳ־���� ������ nullptr�� �����Ƿ� return
	if (nullptr == pAsset)
		return;

	// ������ ������ Inspector ���� �˷��ش�.
	Inspector* pInspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");

	// Ÿ���� �Ǵ� Asset ����
	pInspector->SetTargetAsset(pAsset);
}