#include "pch.h"
#include "HYRenderComponent.h"


HYRenderComponent::HYRenderComponent(COMPONENT_TYPE _Type)
	: HYComponent(_Type)
{
}

HYRenderComponent::HYRenderComponent(const HYRenderComponent& _OriginRenderCom)
	: HYComponent(_OriginRenderCom)
	, m_Mesh(_OriginRenderCom.m_Mesh)
	, m_SharedMtrl(_OriginRenderCom.m_SharedMtrl)
{
	// ���� ������Ʈ�� ���������� �����ϰ� �ְ�, ���� ��������� ���������� �ƴѰ��
	if (_OriginRenderCom.m_SharedMtrl != _OriginRenderCom.m_CurMtrl)
	{
		assert(_OriginRenderCom.m_DynamicMtrl.Get());

		// ���� ���� ������Ʈ�� ������ ���������� �����Ѵ�.
		GetDynamicMaterial();
	}
	else
	{
		m_CurMtrl = m_SharedMtrl;
	}
}

HYRenderComponent::~HYRenderComponent()
{
	// ������� AssetMgr�� ��ϵ��� �ʴ� ���� ������ ������� ������ Ptr�̱� ������ �Ű澲�� �ʾƵ� ��
}

// �ƿ� ������ ��ü�ϴ� �Լ� -> ���� ������ �ʱ�ȭ ���Ѿ� ��
void HYRenderComponent::SetMaterial(Ptr<HYMaterial> _Mtrl)
{
	// ������ ����Ǹ� ������ ���纻 �޾Ƶ� DynamicMaterial �� �����Ѵ�.
	m_CurMtrl = m_SharedMtrl = _Mtrl;
	m_DynamicMtrl = nullptr;
}

// ���� ������ ��ȯ���ִ� �Լ�(���� ������ Ư�� ������Ʈ���� ����)
Ptr<HYMaterial> HYRenderComponent::GetDynamicMaterial()
{
	// �̹� ���� ������ �����ϰ� ������ �װ� �ش�.
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	// ���� ������ ���� ���� ������ �ִٸ�
	if (nullptr != m_SharedMtrl)
	{
		// ���� ������ �����ؼ� ���������� ����� �װ� ���� ��� ������ �����Ѵ�.
		m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

		return m_DynamicMtrl;
	}

	return nullptr;
}

// ���� ������ ���ư����� ���ִ� �Լ�
void HYRenderComponent::RestoreMaterial()
{
	m_CurMtrl = m_SharedMtrl;
}