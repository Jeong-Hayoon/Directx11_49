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

	// ������ ������ �밢�� ��Ŀ� �־���
	m_matWorld._11 = m_vRelativeScale.x;
	m_matWorld._22 = m_vRelativeScale.y;
	m_matWorld._33 = m_vRelativeScale.z;

	// 4�࿡ �̵� ������ �־���
	m_matWorld._41 = m_vRelativePos.x;
	m_matWorld._42 = m_vRelativePos.y;
	m_matWorld._43 = m_vRelativePos.z;
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
	tTransform transform = {};
	transform.matWorld = m_matWorld;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	// _ElementCount�� 1���̱� �ѵ� ���� �Է� ���ص� ������۴� 
	// Transform * 1���� ������� ������ Default ���� 0�� �Ѿ��
	// �˾Ƽ� 1�� �ν�(������� ũ�Ⱑ Transform ����ü 1�� ũ���̹Ƿ�)
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}