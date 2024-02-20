#include "pch.h"
#include "HYGameObjectEx.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>

#include <Engine/components.h>

void HYGameObjectEx::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != GetComponent((COMPONENT_TYPE)i))
		{
			GetComponent((COMPONENT_TYPE)i)->finaltick();
		}
	}

	vector<HYGameObject*>::const_iterator iter = GetChild().begin();
	for (; iter != GetChild().end(); ++iter)
	{
		(*iter)->finaltick();
	}
}