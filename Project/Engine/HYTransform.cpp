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

void HYTransform::finaltick()
{
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
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	// _ElementCount�� 1���̱� �ѵ� ���� �Է� ���ص� ������۴� 
	// Transform * 1���� ������� ������ Default ���� 0�� �Ѿ��
	// �˾Ƽ� 1�� �ν�(������� ũ�Ⱑ Transform ����ü 1�� ũ���̹Ƿ�)
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}