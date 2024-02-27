#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BACKGROUNDSCRIPT,
	MISSILESCRIPT,
	PLAYERSCRIPT,
	SPOTLIGHTSCRIPT,
	END,
};

using namespace std;

class HYScript;

class HYScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static HYScript * GetScript(const wstring& _strScriptName);
	static HYScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(HYScript * _pScript);
};
