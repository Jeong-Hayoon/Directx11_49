#include "pch.h"
#include "HYSpotLightScript.h"

#include "HYTransform.h"


HYSpotLightScript::HYSpotLightScript()
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