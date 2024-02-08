#include "pch.h"
#include "ComputeShaderUI.h"

void ComputeShaderUI::render_update()
{
	AssetUI::render_update();
}

ComputeShaderUI::ComputeShaderUI()
	: AssetUI("ComputeShader", "##ComputeShader", ASSET_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}
