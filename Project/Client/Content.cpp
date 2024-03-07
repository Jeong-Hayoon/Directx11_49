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

	// ������ ������ �ִ� ������ �ε��Ѵ�.
	ReloadContent();
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

void Content::ReloadContent()
{
	// Content ������ �ִ� ��� ���� ���ϸ�(�����)�� ã�Ƴ���.
	wstring strContentPath = HYPathMgr::GetContentPath();
	FindFileName(strContentPath);

	// ã�� �����̸����� ���µ��� �ε�
	for (size_t i = 0; i < m_vecAssetFileName.size(); ++i)
	{
		ASSET_TYPE Type = GetAssetTypeByExt(m_vecAssetFileName[i]);

		switch (Type)
		{
		case ASSET_TYPE::END:
		default:
			continue;
			break;
		case ASSET_TYPE::MESH:
			HYAssetMgr::GetInst()->Load<HYMesh>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::MESHDATA:
			//CAssetMgr::GetInst()->Load<CMeshData>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::PREFAB:
			HYAssetMgr::GetInst()->Load<HYPrefab>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::TEXTURE:
			HYAssetMgr::GetInst()->Load<HYTexture>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::MATERIAL:
			HYAssetMgr::GetInst()->Load<HYMaterial>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::SOUND:
			//HYAssetMgr::GetInst()->Load<HYSound>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		}
	}

	// ������ ������ ������, AssetMgr ������ �޸� ���� 

	ResetContent();
}

// ���ϸ��� �����ϴ� �Լ�
void Content::FindFileName(const wstring& _Directory)
{
	// ���� Ž�� ��� ������ ����ü
	WIN32_FIND_DATA FIND_DATA = {};

	// Ž���� �õ��� ���͸� ��� + ã�� ���� Ÿ�� ����(*.* : �����̵� �� �� ����) ����
	wstring strDirectory = _Directory + L"*.*";

	// Ž�� �ڵ� ���� �� ù���� ã�� ������ ����(�� �ڵ��� ���� ���� ������ ��� Ž��)
	HANDLE hFindHandle = FindFirstFile(strDirectory.c_str(), &FIND_DATA);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// Ž�� �ڵ��� �̿��ؼ�, ������ ���� ������ Ž��, ���̻� ������ false ��ȯ
	while (FindNextFile(hFindHandle, &FIND_DATA))
	{
		// ã�� ������ Directory(����) Ÿ������ Ȯ��
		if (FIND_DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// ������ Ž���ϸ� .. ��� ������ ������ ���� -> �θ�(����) ������ ������ ���ִ� ����
			if (!wcscmp(L"..", FIND_DATA.cFileName))
				continue;

			// ����Լ��� ���� �������� �ִ� �����̸��� Ž��
			FindFileName(_Directory + FIND_DATA.cFileName + L"\\");
		}
		// ã�� ������ ���� Ÿ��
		else
		{
			// ���� Ÿ���� ���, ���͸����� �ٿ��� ���� ��θ� �����, ����θ� �����ؼ� m_vecAssetFileName �� ����
			wstring strRelativePath = HYPathMgr::GetRelativePath(_Directory + FIND_DATA.cFileName);
			m_vecAssetFileName.push_back(strRelativePath);
		}
	}

	// Ž�� �ڵ� ����
	FindClose(hFindHandle);
}

// Asset�� Ȯ���ڸ� �����ؼ� ��ȯ���ִ� �Լ�
ASSET_TYPE Content::GetAssetTypeByExt(const path& _relativePath)
{
	// extension : Ȯ���ڸ� �������ִ� �Լ�
	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;
	if (_relativePath.extension() == L".mtrl")
		return ASSET_TYPE::MATERIAL;
	if (_relativePath.extension() == L".mdat")
		return ASSET_TYPE::MESHDATA;
	if (_relativePath.extension() == L".pref")
		return ASSET_TYPE::PREFAB;

	if (_relativePath.extension() == L".png"
		|| _relativePath.extension() == L".bmp"
		|| _relativePath.extension() == L".jpg"
		|| _relativePath.extension() == L".jpeg"
		|| _relativePath.extension() == L".dds"
		|| _relativePath.extension() == L".tga")
		return ASSET_TYPE::TEXTURE;

	if (_relativePath.extension() == L".wav"
		|| _relativePath.extension() == L".mp3"
		|| _relativePath.extension() == L".ogg")
		return ASSET_TYPE::SOUND;

	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;
	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;

	return ASSET_TYPE::END;
}