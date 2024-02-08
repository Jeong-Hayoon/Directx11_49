#include "pch.h"
#include "SoundUI.h"

void SoundUI::render_update()
{
	AssetUI::render_update();
}

SoundUI::SoundUI()
	: AssetUI("Sound", "##Sound", ASSET_TYPE::SOUND)
{
}

SoundUI::~SoundUI()
{
}
