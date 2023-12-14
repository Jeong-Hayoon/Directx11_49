#include "pch.h"
#include "HYGameObject.h"

#include "HYComponent.h"
#include "HYRenderComponent.h"

HYGameObject::HYGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

HYGameObject::~HYGameObject()
{
	// ���� ���������δ� �̷������� �迭�� �迭�� ũ�⸦ �˷�����ϴµ�
	// <m_arrCom, 14>Delete_Array(m_arrCom*[14])
	// template�̶� �˾Ƽ� ��Ī�� ��
	Delete_Array(m_arrCom);
}

void HYGameObject::begin()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->begin();
		}
	}
}

void HYGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}
}

void HYGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}
}

// render �Լ��� Component �� Render Component�� 
// ��ӹ��� Ŭ������ ������ �Ǿ� ����
void HYGameObject::render()
{
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void HYGameObject::AddComponent(HYComponent* _Comonent)
{
	// GameObject �ȿ� ���� Component�� GameObject��
	// �˸��� �迭 �����ͷ� Component�� ����Ű�� �ǰ�
	// Component�� ������ GameObject��� ���� �����Ѽ�
	// ���θ� �˰� ��
	COMPONENT_TYPE type = _Comonent->GetType();

	m_arrCom[(UINT)type] = _Comonent;
	_Comonent->m_Owner = this;

	// Render Component�� �ڽ� Ŭ�������� �ٿ� ĳ���� ����
	// _Comonent�� HYComponent Ÿ������ �������� 
	// Render Component�� ��ӹ޾ұ� ������ Render Component�� ĳ������ ����
	m_RenderCom = dynamic_cast<HYRenderComponent*>(_Comonent);
}