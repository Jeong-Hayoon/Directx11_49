#include "pch.h"
#include "HYScriptMgr.h"

#include "HYBackgroundScript.h"
#include "HYMissileScript.h"
#include "HYMonsterScript.h"
#include "HYPlayerScript.h"
#include "HYSpotLightScript.h"

void HYScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"HYBackgroundScript");
	_vec.push_back(L"HYMissileScript");
	_vec.push_back(L"HYMonsterScript");
	_vec.push_back(L"HYPlayerScript");
	_vec.push_back(L"HYSpotLightScript");
}

HYScript * HYScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"HYBackgroundScript" == _strScriptName)
		return new HYBackgroundScript;
	if (L"HYMissileScript" == _strScriptName)
		return new HYMissileScript;
	if (L"HYMonsterScript" == _strScriptName)
		return new HYMonsterScript;
	if (L"HYPlayerScript" == _strScriptName)
		return new HYPlayerScript;
	if (L"HYSpotLightScript" == _strScriptName)
		return new HYSpotLightScript;
	return nullptr;
}

HYScript * HYScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new HYBackgroundScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new HYMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new HYMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new HYPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SPOTLIGHTSCRIPT:
		return new HYSpotLightScript;
		break;
	}
	return nullptr;
}

const wchar_t * HYScriptMgr::GetScriptName(HYScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"HYBackgroundScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"HYMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"HYMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"HYPlayerScript";
		break;

	case SCRIPT_TYPE::SPOTLIGHTSCRIPT:
		return L"HYSpotLightScript";
		break;

	}
	return nullptr;
}