#include "pch.h"
#include "HYTransform.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"

HYTransform::HYTransform()
	: HYComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(true)
{
}

HYTransform::~HYTransform()
{
}

// tick을 거쳐 최종 확정된 정보를 한 행렬안에 상태를 넣음
void HYTransform::finaltick()
{
	// 단위행렬로 초기화
	m_matWorld = XMMatrixIdentity();

	// XMMatrixScaling : 크기 행렬을 만들어주는 함수
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	// XMMatrixRotationX : x축 회전행렬
	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	// XMMatrixRotationY : y축 회전행렬
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	// XMMatrixRotationZ : z축 회전행렬
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	// XMMatrixTranslation : 이동 행렬을 만들어주는 함수
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	// 월드행렬은 이 세가지의 행렬을 모두 합한 것
	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// static이기 때문에 딱 한 번만 초기화되고 Transform::finaltick에서만 접근할 수 있는 전역 변수
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장(이동 적용)
	// XMVector3TransformNormal - w 를 0으로 확장(이동 미적용)
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태 행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
		m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
	}

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bAbsolute)
		{
			// 절대 단위 적용을 위해 부모 오브젝트의 Scale을 가져옴
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();

			// Scale을 통해 크기 행렬을 만들되 단위 행렬이 되도록 역수로 만듦
			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			// 자식의 최종 월드행렬
			m_matWorld *= matParentWorld;
		}

		for (int i = 0; i < 3; ++i)
		{
			// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
			m_arrWorldDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
			m_arrWorldDir[i].Normalize();
		}
	}
}

// Transform에 저장되어 있던 위치정보를 
// 상수버퍼에 담아서 레지스터로 보냄
// (여기에서 세팅된 위치정보를 기반으로 쉐이더가 동작)
// Device가 관리하고 있는 여러 상수버퍼 중
// 위치값을 전달받을 수 있는 transform 전용 상수버퍼가 있고
// Device를 통해 얻어내서 위치데이터를 보내놓음
// 랜더링하기 전에 해당 상수버퍼를 b0 레지스터에 바인딩을 할 것임
void HYTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	// _ElementCount는 1개이긴 한데 굳이 입력 안해도 상수버퍼는 
	// Transform * 1개로 만들었기 때문에 Default 인자 0이 넘어가도
	// 알아서 1로 인식(상수버퍼 크기가 Transform 구조체 1개 크기이므로)
	pCB->SetData(&g_Transform);
	pCB->UpdateData();
}


// 자식 Scale * 부모 Scale
Vec3 HYTransform::GetWorldScale()
{
	HYGameObject* pParent = GetOwner()->GetParent();
	// 자식 오브젝트 Scale
	Vec3 vWorldScale = m_vRelativeScale;

	// 위쪽에 부모가 없을 때까지 Scale 누적 곱셈
	// 부모가 존재한다면
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();
		// 부모에게 또 부모가 있다면
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}
