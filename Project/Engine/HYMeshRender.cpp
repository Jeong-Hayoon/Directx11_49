#include "pch.h"
#include "HYMeshRender.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
		  
#include "HYGameObject.h"
#include "HYTransform.h"
#include "HYMaterial.h"

HYMeshRender::HYMeshRender()
	: HYRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

HYMeshRender::~HYMeshRender()
{
}


void HYMeshRender::UpdateData()
{
	if (nullptr != GetMaterial())
	{
		GetMaterial()->UpdateData();
	}

	// render ������ ��ġ ���� ������Ʈ
	GetOwner()->Transform()->UpdateData();
}

void HYMeshRender::render()
{
	// ���� Mesh�� Shader ������ �ȵǾ� �ִ� ��� return
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	UpdateData();

	GetMesh()->render();
}