#include "pch.h"
#include "GraphicsShaderUI.h"

void GraphicsShader::render_update()
{
	AssetUI::render_update();
}

GraphicsShader::GraphicsShader()
	: AssetUI("GraphicsShader", "##GraphicsShader", ASSET_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShader::~GraphicsShader()
{
}
