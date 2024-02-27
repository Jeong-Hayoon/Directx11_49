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

