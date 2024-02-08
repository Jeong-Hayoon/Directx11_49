#include "pch.h"
#include "MeshDataUI.h"

void MeshDataUI::render_update()
{
	AssetUI::render_update();
}

MeshDataUI::MeshDataUI()
	: AssetUI("MeshData", "##MeshData", ASSET_TYPE::MESHDATA)
{
}

MeshDataUI::~MeshDataUI()
{
}
