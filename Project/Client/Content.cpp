#include "pch.h"
#include "Content.h"

#include <Engine/HYAssetMgr.h>

#include "TreeUI.h"

Content::Content()
	: UI("Content", "##Content")
{
	// ContentUI �ڽ����� Tree �� ����
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// Tree ����
	TreeNode* pNode = m_Tree->AddTreeNode(nullptr, "RootNode", 0);

	// pNode�� �θ�� �ϴ� Node �߰�
	m_Tree->AddTreeNode(pNode, "Child1", 0);
	m_Tree->AddTreeNode(pNode, "Child2", 0);

	pNode = m_Tree->AddTreeNode(pNode, "Child3", 0);

	// Child3�� �θ�� �ϴ� Node �߰�
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


// Content ������ Reset�� �� �ٽ� ���� ��� Asset�� �̸����� Tree�� ���� ä���ִ� �Լ�
void Content::ResetContent()
{
	m_Tree->ClearNode();

	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// �߰��� ���
		TreeNode* CategoryNode = m_Tree->AddTreeNode(RootNode, ASSET_TYPE_STRING[i], 0);

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