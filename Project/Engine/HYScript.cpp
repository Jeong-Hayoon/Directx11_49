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

void HYScript::Instantiate(Ptr<HYPrefab> _Prefab, Vec3 _vWorldPos, int _layerIdx)
{
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

