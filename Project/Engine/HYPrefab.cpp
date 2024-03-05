#include "pch.h"
#include "HYPrefab.h"

#include "HYGameObject.h"

SAVE_TYPE HYPrefab::GAMEOBJECT_SAVE = nullptr;
LOAD_TYPE HYPrefab::GAMEOBJECT_LOAD = nullptr;

HYPrefab::HYPrefab()
	: HYAsset(ASSET_TYPE::PREFAB)
	, m_ProtoObj(nullptr)
{
}

// Prefab을 처음부터 등록하는 버전의 생성자
HYPrefab::HYPrefab(HYGameObject* _Proto)
	: HYAsset(ASSET_TYPE::PREFAB)
	, m_ProtoObj(_Proto)
{
}

HYPrefab::~HYPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

// Prefab 등록하는 함수
void HYPrefab::SetGameObject(HYGameObject* _Object)
{
	// 기존에 등록되어 있던 Prefab이 있다면 delete
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;

	m_ProtoObj = _Object;
}

// 원본 오브젝트의 복제본을 반환해주는 함수
HYGameObject* HYPrefab::Instantiate()
{
	return m_ProtoObj->Clone();
}

int HYPrefab::Save(const wstring& _strRelativePath)
{
	assert(GAMEOBJECT_SAVE);

	wstring strContentPath = HYPathMgr::GetContentPath();
	strContentPath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strContentPath.c_str(), L"wb");

	GAMEOBJECT_SAVE(m_ProtoObj, pFile);

	fclose(pFile);
	return S_OK;
}

int HYPrefab::Load(const wstring& _strFilePath)
{
	assert(GAMEOBJECT_LOAD);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	m_ProtoObj = GAMEOBJECT_LOAD(pFile);

	fclose(pFile);
	return S_OK;
}