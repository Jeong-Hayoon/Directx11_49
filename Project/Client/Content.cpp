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

	// 컨텐츠 폴더에 있는 에셋을 로딩한다.
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

void Content::ReloadContent()
{
	// Content 폴더에 있는 모든 에셋 파일명(상대경로)을 찾아낸다.
	wstring strContentPath = HYPathMgr::GetContentPath();
	FindFileName(strContentPath);

	// 찾은 파일이름으로 에셋들을 로드
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

	// 삭제된 에셋이 있으면, AssetMgr 에서도 메모리 해제 

	ResetContent();
}

// 파일명을 추출하는 함수
void Content::FindFileName(const wstring& _Directory)
{
	// 파일 탐색 결과 저장할 구조체
	WIN32_FIND_DATA FIND_DATA = {};

	// 탐색을 시도할 디렉터리 경로 + 찾을 파일 타입 포맷(*.* : 무엇이든 올 수 있음) 지정
	wstring strDirectory = _Directory + L"*.*";

	// 탐색 핸들 생성 후 첫번재 찾은 파일을 저장(얻어낸 핸들을 통해 다음 파일을 계속 탐색)
	HANDLE hFindHandle = FindFirstFile(strDirectory.c_str(), &FIND_DATA);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// 탐색 핸들을 이용해서, 파일을 다음 파일을 탐색, 더이상 없으면 false 반환
	while (FindNextFile(hFindHandle, &FIND_DATA))
	{
		// 찾은 파일이 Directory(폴더) 타입인지 확인
		if (FIND_DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 폴더를 탐색하면 .. 라는 폴더가 숨겨져 있음 -> 부모(상위) 폴더로 나가게 해주는 폴더
			if (!wcscmp(L"..", FIND_DATA.cFileName))
				continue;

			// 재귀함수로 하위 폴더내에 있는 파일이름을 탐색
			FindFileName(_Directory + FIND_DATA.cFileName + L"\\");
		}
		// 찾은 파일이 폴더 타입
		else
		{
			// 파일 타입인 경우, 디렉터리까지 붙여서 최종 경로를 만들고, 상대경로만 추출해서 m_vecAssetFileName 에 취합
			wstring strRelativePath = HYPathMgr::GetRelativePath(_Directory + FIND_DATA.cFileName);
			m_vecAssetFileName.push_back(strRelativePath);
		}
	}

	// 탐색 핸들 해제
	FindClose(hFindHandle);
}

// Asset의 확장자를 추출해서 반환해주는 함수
ASSET_TYPE Content::GetAssetTypeByExt(const path& _relativePath)
{
	// extension : 확장자를 추출해주는 함수
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