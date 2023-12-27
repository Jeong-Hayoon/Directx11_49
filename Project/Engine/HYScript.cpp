#include "pch.h"
#include "HYScript.h"

HYScript::HYScript()
	: HYComponent(COMPONENT_TYPE::SCRIPT)
{
}

HYScript::~HYScript()
{
}

void HYScript::Destroy()
{
	GamePlayStatic::DestroyGameObject(GetOwner());
}
