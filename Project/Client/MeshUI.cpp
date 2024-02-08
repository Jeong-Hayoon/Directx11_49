#include "pch.h"
#include "MeshUI.h"

MeshUI::MeshUI()
	: AssetUI("Mesh", "##Mesh", ASSET_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}


void MeshUI::render_update()
{
	AssetUI::render_update();
}