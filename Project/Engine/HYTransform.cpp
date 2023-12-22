#include "pch.h"
#include "HYTransform.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"

HYTransform::HYTransform()
	: HYComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
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

	// ��ü�� ���Ⱚ�� �ٽ� ����Ѵ�.
	m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	// Vec3 �� Vec4 Ÿ������ Ȯ���ؼ� ����� ������Ѿ� ��
	// XMVector3TransformCoord	- w �� 1�� Ȯ��(�̵� ����)
	// XMVector3TransformNormal - w �� 0���� Ȯ��(�̵� ������)
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// ���� ���� ���� ����� �̵��� �������� ���� ����
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld ��Ŀ� ũ�������� ���� �� �ֱ� ������ �ٽ� ���̸� 1�� ����ȭ ��Ų��.
		m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
		m_arrDir[i].Normalize();
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
	pCB->UpdateData(0);
}