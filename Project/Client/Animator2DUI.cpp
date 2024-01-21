#include "pch.h"
#include "Animator2DUI.h"

#include <Engine/HYAnimator2D.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}


void Animator2DUI::render_update()
{
	ComponentUI::render_update();
}