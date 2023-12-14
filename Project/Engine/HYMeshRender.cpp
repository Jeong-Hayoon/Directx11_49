#include "pch.h"
#include "HYMeshRender.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
		  
#include "HYGameObject.h"
#include "HYTransform.h"

HYMeshRender::HYMeshRender()
	: HYRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

HYMeshRender::~HYMeshRender()
{
}


void HYMeshRender::UpdateData()
{
	if (nullptr != GetShader())
	{
		GetShader()->UpdateData();
	}

	// render 직전에 위치 정보 업데이트
	GetOwner()->Transform()->UpdateData();
}

void HYMeshRender::render()
{
	// 아직 Mesh나 Shader 세팅이 안되어 있는 경우 return
	if (nullptr == GetMesh() || nullptr == GetShader())
		return;

	UpdateData();

	GetMesh()->render();
}