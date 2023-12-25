#include "pch.h"
#include "HYGameObject.h"

#include "HYComponent.h"
#include "HYRenderComponent.h"

#include "HYScript.h"

HYGameObject::HYGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
{
}

HYGameObject::~HYGameObject()
{
	// ���� ���������δ� �̷������� �迭�� �迭�� ũ�⸦ �˷�����ϴµ�
	// <m_arrCom, 14>Delete_Array(m_arrCom*[14])
	// template�̶� �˾Ƽ� ��Ī�� ��
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);

	// Layer�� ������ ���� Layer�� �����ϰ� �ִ� 
	// �θ� ������Ʈ���� Delete�ϰ� GameObject����
	// �ڽ� ������Ʈ���� Delete
	Delete_Vec(m_vecChild);
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

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

// �ֿ� ��� ����
void HYGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

// tick ���Ŀ� Object�� �߻��� ��� �� ������
// �⺻ Component���� ���� �۾� ������ ���� �۾�
// ex) �浹ó��, ������ ���
void HYGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}

	// Script ȣ�� ���ϴ� ����
	// Script�� �⺻ Component�� �ƴ� �߰� ����̱� ������
	// finaltick�� ȣ������ ����(finaltick�� �⺻ Component�� ������ �۾��̹Ƿ�)
	
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
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

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->render();
	}
}

void HYGameObject::AddComponent(HYComponent* _Comonent)
{
	// GameObject �ȿ� ���� Component�� GameObject��
	// �˸��� �迭 �����ͷ� Component�� ����Ű�� �ǰ�
	// Component�� ������ GameObject��� ���� �����Ѽ�
	// ���θ� �˰� ��
	COMPONENT_TYPE type = _Comonent->GetType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// Script Ÿ�� Component �� ������ Script Ŭ������ �ƴ� ���(���� ó��)
		// Script�� �ƴ϶�� �ٿ� ĳ������ ������� nullptr�̹Ƿ� assert�� �ɸ�
		assert(dynamic_cast<HYScript*>(_Comonent));

		m_vecScript.push_back((HYScript*)_Comonent);
		_Comonent->m_Owner = this;
	}
	else
	{
		// �̹� �ش�(����) Ÿ���� ������Ʈ�� �����ϰ� �ִ� ���
		// ���� ����� �ϴ� ������Ʈ�� �� ���� ������ �ǹǷ�
		// �̹� �ִ� ������Ʈ�� ���� �������� �ϸ� assert�� �ɸ�
		assert(!m_arrCom[(UINT)type]);

		m_arrCom[(UINT)type] = _Comonent;
		_Comonent->m_Owner = this;

		// Render Component�� �ڽ� Ŭ�������� �ٿ� ĳ���� ����
		// _Comonent�� HYComponent Ÿ������ �������� 
		// Render Component�� ��ӹ޾ұ� ������ Render Component�� ĳ������ ����
		HYRenderComponent* pRenderCom = dynamic_cast<HYRenderComponent*>(_Comonent);
		if (nullptr != pRenderCom)
		{
			// �̹� �� ���� �̻��� RenderComponent �� �����ϰ� �ִ� ���
			// �� Object�� Render Component�� �� ���� �����ؾ� �ϹǷ�
			// assert �� ����ó��
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}
	}
}


void HYGameObject::DisconnectWithParent()
{
	vector<HYGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	// �θ� ���� ������Ʈ�� DisconnectWithParent �Լ��� ȣ�� �߰ų�
	// �θ�� �ڽ��� ����Ű���� �ʰ� �ִµ�, �ڽ��� �θ� ����Ű�� �ִ� ���
	assert(nullptr);
}

void HYGameObject::AddChild(HYGameObject* _Child)
{
	// �̹� �θ� �־��ٸ�
	if (_Child->m_Parent)
	{
		// ���� �θ� ������Ʈ�� ���� ����
		_Child->DisconnectWithParent();
	}

	// �θ� �ڽ� ����
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}