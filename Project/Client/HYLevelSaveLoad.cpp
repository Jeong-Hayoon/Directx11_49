#include "pch.h"
#include "HYLevelSaveLoad.h"

#include <Engine/HYPathMgr.h>
#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>
#include <Engine/components.h>
#include <Engine/HYAssetMgr.h>

void HYLevelSaveLoad::SaveLevel(HYLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);

	// Level �� ������ ���
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// ������ �̸�
	SaveWString(_Level->GetName(), pFile);

	// ������ ���̾� ����
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void HYLevelSaveLoad::SaveLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer �� �̸� ����
	SaveWString(_Layer->GetName(), _File);

	// Layer �� �����ϰ� �ִ� GameObject ���� ����
	const vector<HYGameObject*>& vecObject = _Layer->GetParentObjects();

	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _File);
	}
}

void HYLevelSaveLoad::SaveGameObject(HYGameObject* _Obj, FILE* _File)
{

}

HYLevel* HYLevelSaveLoad::LoadLevel(const wstring& _strLevelPath)
{
	HYLevel* pLevel = nullptr;

	// Level �� �ҷ��� ���
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	if (!exists(strLevelPath))
	{
		MessageBox(nullptr, L"���� ��ΰ� �ùٸ��� �ʰų�, ������ ����", L"���� �ε� ����", MB_OK);
		return nullptr;
	}

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"rb");

	// ������ �̸��� �д´�.
	pLevel = new HYLevel;
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	pLevel->SetName(strLevelName);

	// Layer �ε�
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		LoadLayer(pLevel->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void HYLevelSaveLoad::LoadLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer �� �̸� ����
	wstring strLayerName;
	LoadWString(strLayerName, _File);
	_Layer->SetName(strLayerName);


	// Layer �� �����ϴ� GameObject ���� �ҷ��´�.
	size_t ObjCount = 0;
	fread(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		HYGameObject* pObject = LoadGameObject(_File);
		_Layer->AddObject(pObject, false);
	}
}

HYGameObject* HYLevelSaveLoad::LoadGameObject(FILE* _File)
{


	return nullptr;
}