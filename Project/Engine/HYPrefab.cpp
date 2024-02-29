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

void HYPrefab::SetGameObject(HYGameObject* _Object)
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;

	m_ProtoObj = _Object;
}

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