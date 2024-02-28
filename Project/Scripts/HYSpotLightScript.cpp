#include "pch.h"
#include "HYSpotLightScript.h"

#include <Engine/HYTransform.h>


HYSpotLightScript::HYSpotLightScript()
	: HYScript(SCRIPT_TYPE::SPOTLIGHTSCRIPT)
{
}

HYSpotLightScript::~HYSpotLightScript()
{
}

void HYSpotLightScript::tick()
{
	Vec3 vRot = Transform()->GetRelativeRotation();

	Transform()->SetRelativeRotation(vRot);


	if (KEY_PRESSED(KEY::B))
	{
		vRot.z += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::N))
	{
		vRot.z -= DT * XM_PI;
	}

	Transform()->SetRelativeRotation(vRot);

}

void HYSpotLightScript::SaveToFile(FILE* _File)
{
}

void HYSpotLightScript::LoadFromFile(FILE* _File)
{
}