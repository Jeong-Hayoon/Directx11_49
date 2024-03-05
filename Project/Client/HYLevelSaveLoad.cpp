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

// SaveLevel(������ ����, ���)
void HYLevelSaveLoad::SaveLevel(HYLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);

	// Level �� ������ ���
	wstring strLevelPath = HYPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// ������ �̸� ����
	SaveWString(_Level->GetName(), pFile);

	// ������ ���̾� ����
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), pFile);
	}

	fclose(pFile);
}

// ���̾ �����ϴ� �Լ�
void HYLevelSaveLoad::SaveLayer(HYLayer* _Layer, FILE* _File)
{
	// Layer �� �̸� ����
	SaveWString(_Layer->GetName(), _File);

	// Layer �� �����ϰ� �ִ� GameObject ���� ����
	const vector<HYGameObject*>& vecObject = _Layer->GetParentObjects();

	// ������Ʈ�� ���� ����
	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _File);
	}
}

void HYLevelSaveLoad::SaveGameObject(HYGameObject* _Obj, FILE* _File)
{
	// GameObject �� �̸��� ����
	SaveWString(_Obj->GetName(), _File);

	UINT i = 0;

	// �����ϰ� �ִ� ��� ������Ʈ ���� ����
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		HYComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// ������Ʈ Ÿ�� ���� ����
		fwrite(&i, sizeof(UINT), 1, _File);

		// �ش� ������Ʈ�� ������ ������ ����(���� �Լ�)
		pCom->SaveToFile(_File);
	}
	fwrite(&i, sizeof(UINT), 1, _File);

	// ��ũ��Ʈ ���� ����
	const vector<HYScript*>& vecScripts = _Obj->GetScripts();
	size_t ScriptCount = vecScripts.size();

	// ��ũ��Ʈ ���� ����
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		SaveWString(HYScriptMgr::GetScriptName(vecScripts[i]), _File);
		vecScripts[i]->SaveToFile(_File);
	}

	// �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����
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

	return pLevel;
}

// Layer�� �ϳ��� �����ؼ� �ִ� ������ �ƴ� Layer�� �޾Ƽ� �Է����� ���� ���̾ ���Ͽ� ����� �ɷ� ä��
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

	HYGameObject* pObject = new HYGameObject;

	// GameObject �� �̸��� �ε�
	wstring strName;
	LoadWString(strName, _File);
	pObject->SetName(strName);

	// ������Ʈ ������ �ҷ�����
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

			// ���ο� Component �� �߰��ߴµ� case �߰��� ������ ��� �� �� �ֵ��� assert
			assert(nullptr);
			break;
		}

		// �ش� ������Ʈ�� ������ �����͸� �ε�
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// ��ũ��Ʈ ���� �б�
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

	// �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����	
	size_t childcount = 0;
	fread(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		pObject->AddChild(LoadGameObject(_File));
	}

	return pObject;
}
