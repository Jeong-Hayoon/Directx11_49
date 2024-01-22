#include "pch.h"
#include "TilemapUI.h"

#include <Engine/HYTilemap.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

TilemapUI::TilemapUI()
	: ComponentUI("Tilemap", "##Tilemap", COMPONENT_TYPE::TILEMAP)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Tilemap");
}

TilemapUI::~TilemapUI()
{
}


void TilemapUI::render_update()
{
	ComponentUI::render_update();
}