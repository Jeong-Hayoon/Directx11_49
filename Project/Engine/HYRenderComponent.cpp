#include "pch.h"
#include "HYRenderComponent.h"

#include "HYLevelMgr.h"
#include "HYLevel.h"

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

		// ���� ����������Ʈ�� �������� ���� ���� ������ ���������� �����Ѵ�.(�Ķ���� ����)
		*m_DynamicMtrl.Get() = *_OriginRenderCom.m_DynamicMtrl.Get();
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
	// ������ �÷��� ������ ��쿡��
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
		return nullptr;

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

void HYRenderComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void HYRenderComponent::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMtrl, _File);

	SetMaterial(m_SharedMtrl);
}

