#include "pch.h"
#include "GraphicsShaderUI.h"

GraphicsShader::GraphicsShader()
	: AssetUI("GraphicsShader", "##GraphicsShader", ASSET_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShader::~GraphicsShader()
{
}

void GraphicsShader::render_update()
{
	AssetUI::render_update();
}
