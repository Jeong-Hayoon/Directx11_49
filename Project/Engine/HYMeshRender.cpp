#include "pch.h"
#include "HYMeshRender.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
		  
#include "HYGameObject.h"
#include "HYAnimator2D.h"
#include "HYTransform.h"
#include "HYMaterial.h"

#include "HYRenderMgr.h"


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

	// render 직전에 위치 정보 업데이트
	GetOwner()->Transform()->UpdateData();
}

void HYMeshRender::finaltick()
{
	if (HYRenderMgr::GetInst()->IsDebugPosition())
	{
		GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 20.f, Vec3(0.f, 1.f, 0.f), true);
	}
}


void HYMeshRender::render()
{
	// 아직 Mesh나 Shader 세팅이 안되어 있는 경우 return
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// 애니메이션 정보가 있다면 재생중인 애니메이션 정보를 가져와서 랜더링해줘야 함
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		// Clear를 안해놓으면 b2 레지스터에 애니메이션을 사용한다라는 값이 true일테니까
		Animator2D()->Clear();
	}

	UpdateData();

	GetMesh()->render();
}