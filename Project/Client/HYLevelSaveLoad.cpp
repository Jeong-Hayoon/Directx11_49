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

	// Level 을 저장할 경로
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// 레벨의 이름
	SaveWString(_Level->GetName(), pFile);

	// 레벨의 레이어 저장
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void HYLevelSaveLoad::SaveLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer 의 이름 저장
	SaveWString(_Layer->GetName(), _File);

	// Layer 가 보유하고 있는 GameObject 들을 저장
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

	// Level 을 불러올 경로
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	if (!exists(strLevelPath))
	{
		MessageBox(nullptr, L"레벨 경로가 올바르지 않거나, 레벨이 없음", L"레벨 로드 실패", MB_OK);
		return nullptr;
	}

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"rb");

	// 레벨의 이름을 읽는다.
	pLevel = new HYLevel;
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	pLevel->SetName(strLevelName);

	// Layer 로드
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		LoadLayer(pLevel->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void HYLevelSaveLoad::LoadLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer 의 이름 저장
	wstring strLayerName;
	LoadWString(strLayerName, _File);
	_Layer->SetName(strLayerName);


	// Layer 가 보유하던 GameObject 들을 불러온다.
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
