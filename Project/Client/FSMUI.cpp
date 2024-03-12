#include "pch.h"
#include "FSMUI.h"

FSMUI::FSMUI()
	: AssetUI("FSM", "##FSM", ASSET_TYPE::FSM)
{
}

FSMUI::~FSMUI()
{
}

void FSMUI::render_update()
{
	AssetUI::render_update();

}