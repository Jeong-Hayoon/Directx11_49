#include "pch.h"
#include "HYLayer.h"

#include "HYGameObject.h"
#include "HYGC.h"

HYLayer::HYLayer()
	: m_iLayerIdx(-1)	
{
}

HYLayer::~HYLayer()
{
	// Layer�� ������ ���� Layer�� �����ϰ� �ִ� 
	// �θ� ������Ʈ���� Delete
	Delete_Vec(m_vecParent);
}

void HYLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void HYLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->tick();
	}
}

void HYLayer::finaltick()
{
	vector<HYGameObject*>::iterator iter = m_vecParent.begin();

	for (; iter != m_vecParent.end(); )
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			HYGC::GetInst()->Add(*iter);
			iter = m_vecParent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

// Layer�� AddObject�� �ֻ��� �θ� ������Ʈ�� ����
// AddObject : �θ� ������Ʈ���� m_vecParent�� LayerIdx�� �Բ� �־��ְ�
// �ڽ� ������Ʈ���� m_vecChild�� �־��ְ�_bMove ���ο� ���� LayerIdx �����ִ� �Լ�
void HYLayer::AddObject(HYGameObject* _Object, bool _bMove)
{
	// _bMove : �ڽ� Object�� ���̾� �̵� ���� �Ǵ� ����
	// _bMove : true  - ���̾ �ԷµǴ� Object�� �ڽ��� �ִ� ���, �ڽı��� ��� �ش� ���̾�� �Ѿ�´�.
	// _bMove : false - ���̾ �ԷµǴ� Object�� �ڽ��� �ش� ���̾�� ���� �Ѿ���� �ʴ´�. 
	// �� �ڽĿ�����Ʈ�� ���̾� �Ҽ��� ���� ���(-1)���� ���� �����Ѵ�.    

	// �ֻ��� �θ� ������Ʈ����.
	if (nullptr == _Object->GetParent())
	{
		// �ٸ� ���̾� �Ҽ��̾���
		if (-1 != _Object->m_iLayerIdx)
		{
			// ���� ���̾��� Parent ���Ϳ��� ���ŵǾ�� �Ѵ�.
			_Object->DisconnectWithLayer();
		}

		// �ֻ��� �θ� ������Ʈ�� m_vecParent ���� ����Ű���� �Ѵ�.
		m_vecParent.push_back(_Object);
	}

	// ������Ʈ�� ���̾� �Ҽ��� ���� ���̾�� ����
	// �ڽ� ������Ʈ���� ���̾� �Ҽ��� ����	
	list<HYGameObject*> queue;
	// ���� ������Ʈ���� �ְ�
	queue.push_back(_Object);


	// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
	// ť�� empty�� �Ǿ��ٴ� ���� ���� ��� �ڽ��� Ȯ���ߴٴ� �ǹ�
	while (!queue.empty())
	{
		// �� �տ� �ִ� ���� pObject�� �ְ�
		HYGameObject* pObject = queue.front();
		// �տ� �ִ� �� ������
		queue.pop_front();

		// �ڽ� ������Ʈ ��ȸ
		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
		{
			queue.push_back(pObject->m_vecChild[i]);
		}

		// ���� �Է� ������Ʈ(�ֻ��� �θ� ������Ʈ)�� ������ �ش� ���̾� �Ҽ����� ����
		if (pObject == _Object)
			pObject->m_iLayerIdx = m_iLayerIdx;
		else
		{
			// �ڽ� ������Ʈ���� _bMove �� true �� ���, �θ� ���� �ش� ���̾�Ҽ����� ����
			if (_bMove)
				pObject->m_iLayerIdx = m_iLayerIdx;
			// _bMove �� false ��쿡��, �ڽ� ������Ʈ�� ���̾�Ҽ��� ���ٸ� �θ� ���� ����
			else if (-1 == pObject->m_iLayerIdx)
				pObject->m_iLayerIdx = m_iLayerIdx;
		}
	}
}

// Ư�� ���� ������Ʈ�� Layer�ȿ��� �������� �Լ�
void HYLayer::DetachGameObject(HYGameObject* _Object)
{
	// m_iLayerIdx�� -1�̾��ų� ���� �ʾҴٸ� ��ü �����Ǽ� null�Ǵϱ� assert�� �ɸ�
	assert(!(-1 == _Object->m_iLayerIdx || _Object->m_iLayerIdx != m_iLayerIdx));

	// �θ� ������Ʈ�� �ִ� ���
	if (nullptr != _Object->GetParent())
	{
		// �θ� �ִ� ��� ���ʿ� Layer�ȿ��� �� �ʿ䰡 �����ϱ� m_iLayerIdx�� �ʱ�ȭ�����ָ� ��
		_Object->m_iLayerIdx = -1;
	}

	// �ֻ��� �θ� ������Ʈ�� ���
	else
	{
		vector<HYGameObject*>::iterator iter = m_vecParent.begin();
		for (; iter != m_vecParent.end(); ++iter)
		{
			if (*iter == _Object)
			{
				m_vecParent.erase(iter);
				_Object->m_iLayerIdx = -1;
				return;
			}
		}
	}

	// Layer�ȿ� �ִٰ� �ؼ� ������ �ߴµ� ���ٴ� ���� ���� �ȵǹǷ� assert
	assert(nullptr);
}