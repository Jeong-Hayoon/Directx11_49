#include "pch.h"
#include "HYBackgroundScript.h"

#include <Engine/HYKeyMgr.h>

HYBackgroundScript::HYBackgroundScript()
{
}

HYBackgroundScript::~HYBackgroundScript()
{
}


void HYBackgroundScript::tick()
{
	static int i = 0;
	if (KEY_TAP(KEY::T))
	{
		i ? MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1) : MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
		i = !i;
	}
}
