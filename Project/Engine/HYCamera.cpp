#include "pch.h"
#include "HYCamera.h"

#include "HYTransform.h"

HYCamera::HYCamera()
	: HYComponent(COMPONENT_TYPE::CAMERA)
	, m_FOV(0.f)
	, m_Width(0.f)
	, m_Scale(0.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
{
}

HYCamera::~HYCamera()
{
}

// 뷰행렬, 투영행렬 생성
void HYCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();

	// XMMatrixIdentity : 단위행렬로 초기화
	m_matView = XMMatrixIdentity();

	// 카메라가 이동함에 따라 View Space는 반대 방향으로 
	// 이동해야 하므로 이동정보가 필요함
	m_matView._41 = -vCamPos.x;
	m_matView._42 = -vCamPos.y;
	m_matView._43 = -vCamPos.z;

	m_matProj = XMMatrixIdentity();

	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
