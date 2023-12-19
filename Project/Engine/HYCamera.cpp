#include "pch.h"
#include "HYCamera.h"

#include "HYDevice.h"
#include "HYTransform.h"

HYCamera::HYCamera()
	: HYComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::PERSPECTIVE)
	, m_FOV(XM_PI / 2.f) // 90도 
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
{
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// 종횡비 : 가로 대 세로에 대한 비율값(가로가 세로 대비 어느 정도 비율인가)
	m_AspectRatio = vResol.x / vResol.y;

}

HYCamera::~HYCamera()
{
}

// 뷰행렬, 투영행렬 생성
void HYCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();
	// 물체가 반대방향으로 이동
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라의 각 우, 상, 전방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
	Vec3 vRight = Transform()->GetDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetDir(DIR_TYPE::FRONT);

	// XMMatrixIdentity : 단위행렬로 초기화
	Matrix matRotate = XMMatrixIdentity();
	matRotate._11 = vRight.x; matRotate._12 = vUp.x; matRotate._13 = vFront.x;
	matRotate._21 = vRight.y; matRotate._22 = vUp.y; matRotate._23 = vFront.y;
	matRotate._31 = vRight.z; matRotate._32 = vUp.z; matRotate._33 = vFront.z;

	// 이동과 회전을 둘 다 해야 World Space -> View Space가 되었다고 봄
	// 이동 x 회전(카메라가 바라보는 방향이 z축이어야 하므로) = view 행렬
	// 곱해주는 순서는 카메라 변환도 적용되는 물체 입장에서는 이동한 후에
	// 회전을 하기 때문에 이렇게 됨
	m_matView = matTrans * matRotate;

	// 투영 방식에 따른 투영 행렬을 계산
	m_matProj = XMMatrixIdentity();

	// 직교투영
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{

		// 실제 화면 해상도
		Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();
		// LH : 왼손 좌표계
		// View Width : View Space에서 투영하고 싶은 가로 길이 -> 해상도와 일치시킴
		// 게임 내에서의 좌표 1이 1픽셀이 됨
		// View Height : 종횡비를 통해 구함
		// Far를 넘어서는 애들은 직교 투영을 곱했을 때 1을 넘어버림
		// 가로,세로 내에 들어오고 1~10000사이에 있는 애들만 최종적으로 래스터라이저에게
		// 넘겨져서 NDC좌표계 안으로 들어옴
		// m_Scale : 투영하고 있는 범위를 넓게하거나 좁게할 때 사용
		// 투영하는 범위가 클수록 화면에서 보이는 것은 더 작아짐
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	// 원근투영
	else
	{
		// 시야각을 활용하여 화면에서 보이는 크기를 조절할 수 있음
		// 화면이 확 확대되거나 멀어지는 것 같은 느낌을 줄 수 있음
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
