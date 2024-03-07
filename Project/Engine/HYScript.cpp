#include "pch.h"
#include "HYScript.h"

HYScript::HYScript(UINT _ScriptType)
	: HYComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_ScriptType)
{
}

HYScript::~HYScript()
{
}

// Prefab 한 개를 Spawn 시키는 함수(Spawn 시킬 Prefab, Spawn 위치, Layer 인덱스)
void HYScript::Instantiate(Ptr<HYPrefab> _Prefab, Vec3 _vWorldPos, int _layerIdx)
{
	if (nullptr == _Prefab)
		return;

	HYGameObject* pNewObj = _Prefab->Instantiate();

	pNewObj->Transform()->SetRelativePos(_vWorldPos);

	GamePlayStatic::SpawnGameObject(pNewObj, _layerIdx);
}

void HYScript::SaveToFile(FILE* _File)
{
}

void HYScript::LoadFromFile(FILE* _File)
{
}

