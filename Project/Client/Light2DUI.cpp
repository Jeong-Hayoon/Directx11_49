#include "pch.h"
#include "Light2DUI.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::render_update()
{
	ComponentUI::render_update();


}
