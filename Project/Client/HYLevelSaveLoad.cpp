#include "pch.h"
#include "HYLevelSaveLoad.h"

#include <Engine/HYPathMgr.h>
#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>
#include <Engine/components.h>
#include <Engine/HYAssetMgr.h>

#include <Scripts/HYScriptMgr.h>
#include <Engine/HYScript.h>

// SaveLevel(저장할 레벨, 경로)
void HYLevelSaveLoad::SaveLevel(HYLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);

	// Level 을 저장할 경로
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// 레벨의 이름 저장
	SaveWString(_Level->GetName(), pFile);

	// 레벨의 레이어 저장
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), pFile);
	}

	fclose(pFile);
}

// 레이어를 저장하는 함수
void HYLevelSaveLoad::SaveLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer 의 이름 저장
	SaveWString(_Layer->GetName(), _File);

	// Layer 가 보유하고 있는 GameObject 들을 저장
	const vector<HYGameObject*>& vecObject = _Layer->GetParentObjects();

	// 오브젝트의 개수 저장
	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _File);
	}
}

void HYLevelSaveLoad::SaveGameObject(HYGameObject* _Obj, FILE* _File)
{
	// GameObject 의 이름을 저장
	SaveWString(_Obj->GetName(), _File);

	UINT i = 0;

	// 보유하고 있는 모든 컴포넌트 정보 저장
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		HYComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// 컴포넌트 타입 정보 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 해당 컴포넌트가 저장할 데이터 저장(가상 함수)
		pCom->SaveToFile(_File);
	}
	fwrite(&i, sizeof(UINT), 1, _File);

	// 스크립트 정보 저장
	const vector<HYScript*>& vecScripts = _Obj->GetScripts();
	size_t ScriptCount = vecScripts.size();

	// 스크립트 개수 저장
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		SaveWString(HYScriptMgr::GetScriptName(vecScripts[i]), _File);
		vecScripts[i]->SaveToFile(_File);
	}

	// 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
	const vector<HYGameObject*>& vecChild = _Obj->GetChild();
	size_t childcount = vecChild.size();
	fwrite(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
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

	return pLevel;
}

// Layer를 하나씩 생성해서 주는 개념이 아닌 Layer를 받아서 입력으로 들어온 레이어를 파일에 저장된 걸로 채움
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

	HYGameObject* pObject = new HYGameObject;

	// GameObject 의 이름을 로드
	wstring strName;
	LoadWString(strName, _File);
	pObject->SetName(strName);

	// 컴포넌트 정보를 불러오기
	COMPONENT_TYPE type = COMPONENT_TYPE::END;

	while (true)
	{
		fread(&type, sizeof(UINT), 1, _File);
		if (COMPONENT_TYPE::END == type)
			break;

		HYComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new HYTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new HYCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new HYAnimator2D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new HYLight2D;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new HYCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new HYMeshRender;
			break;
		case COMPONENT_TYPE::TILEMAP:
			pComponent = new HYTileMap;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new HYParticleSystem;
			break;
		default:

			// 새로운 Component 를 추가했는데 case 추가를 안했을 경우 알 수 있도록 assert
			assert(nullptr);
			break;
		}

		// 해당 컴포넌트가 저장한 데이터를 로드
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// 스크립트 개수 읽기
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWString(strScriptName, _File);

		HYScript* pScript = HYScriptMgr::GetScript(strScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromFile(_File);
	}

	// 자식 오브젝트가 있으면 자식 오브젝트 정보 저장	
	size_t childcount = 0;
	fread(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		pObject->AddChild(LoadGameObject(_File));
	}

	return pObject;
}
