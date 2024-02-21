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

// tick�� ���� ���� Ȯ���� ������ �� ��ľȿ� ���¸� ����
void HYTransform::finaltick()
{
	// ������ķ� �ʱ�ȭ
	m_matWorld = XMMatrixIdentity();

	// XMMatrixScaling : ũ�� ����� ������ִ� �Լ�
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	// XMMatrixRotationX : x�� ȸ�����
	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	// XMMatrixRotationY : y�� ȸ�����
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	// XMMatrixRotationZ : z�� ȸ�����
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	// XMMatrixTranslation : �̵� ����� ������ִ� �Լ�
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	// ��������� �� �������� ����� ��� ���� ��
	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// static�̱� ������ �� �� ���� �ʱ�ȭ�ǰ� Transform::finaltick������ ������ �� �ִ� ���� ����
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// Vec3 �� Vec4 Ÿ������ Ȯ���ؼ� ����� ������Ѿ� ��
	// XMVector3TransformCoord	- w �� 1�� Ȯ��(�̵� ����)
	// XMVector3TransformNormal - w �� 0���� Ȯ��(�̵� ������)
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// ���� ���� ���� ����� �̵��� �������� ���� ����
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld ��Ŀ� ũ�������� ���� �� �ֱ� ������ �ٽ� ���̸� 1�� ����ȭ ��Ų��.
		m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
		m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
	}

	// �θ� ������Ʈ�� �ִٸ�
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bAbsolute)
		{
			// ���� ���� ������ ���� �θ� ������Ʈ�� Scale�� ������
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();

			// Scale�� ���� ũ�� ����� ����� ���� ����� �ǵ��� ������ ����
			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			// �ڽ��� ���� �������
			m_matWorld *= matParentWorld;
		}

		for (int i = 0; i < 3; ++i)
		{
			// m_matWorld ��Ŀ� ũ�������� ���� �� �ֱ� ������ �ٽ� ���̸� 1�� ����ȭ ��Ų��.
			m_arrWorldDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
			m_arrWorldDir[i].Normalize();
		}
	}
}

// Transform�� ����Ǿ� �ִ� ��ġ������ 
// ������ۿ� ��Ƽ� �������ͷ� ����
// (���⿡�� ���õ� ��ġ������ ������� ���̴��� ����)
// Device�� �����ϰ� �ִ� ���� ������� ��
// ��ġ���� ���޹��� �� �ִ� transform ���� ������۰� �ְ�
// Device�� ���� ���� ��ġ�����͸� ��������
// �������ϱ� ���� �ش� ������۸� b0 �������Ϳ� ���ε��� �� ����
void HYTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	// _ElementCount�� 1���̱� �ѵ� ���� �Է� ���ص� ������۴� 
	// Transform * 1���� ������� ������ Default ���� 0�� �Ѿ��
	// �˾Ƽ� 1�� �ν�(������� ũ�Ⱑ Transform ����ü 1�� ũ���̹Ƿ�)
	pCB->SetData(&g_Transform);
	pCB->UpdateData();
}


// �ڽ� Scale * �θ� Scale
Vec3 HYTransform::GetWorldScale()
{
	HYGameObject* pParent = GetOwner()->GetParent();
	// �ڽ� ������Ʈ Scale
	Vec3 vWorldScale = m_vRelativeScale;

	// ���ʿ� �θ� ���� ������ Scale ���� ����
	// �θ� �����Ѵٸ�
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();
		// �θ𿡰� �� �θ� �ִٸ�
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}
