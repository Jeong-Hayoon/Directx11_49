#include "pch.h"
#include "HYLevel.h"

#include "HYLayer.h"
#include "HYGameObject.h"


HYLevel::HYLevel()
	: m_arrLayer{}
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = new HYLayer;
		// Layer�� ���� �� LayerIdx ����
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

HYLevel::~HYLevel()
{
	Delete_Array(m_arrLayer);
}

void HYLevel::begin()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->begin();
	}
}

void HYLevel::tick()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void HYLevel::finaltick()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}

void HYLevel::AddObject(HYGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	// ������ Layer�� GameObject�� �־���
	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
	m_ObjectName.push_back(_Object->GetName());

}

void HYLevel::AddObject(HYGameObject* _Object, const wstring& _strLayerName, bool _bChildMove)
{
	HYLayer* pLayer = GetLayer(_strLayerName);
	if (nullptr == pLayer)
		return;

	pLayer->AddObject(_Object, _bChildMove);
	m_ObjectName.push_back(_Object->GetName());
}

HYLayer* HYLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		if (_strLayerName == m_arrLayer[i]->GetName())
		{
			return m_arrLayer[i];
		}
	}
	return nullptr;
}

// ���ʷ� ã������ ������Ʈ�� �ٷ� return
HYGameObject* HYLevel::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		// ��� �θ� ������Ʈ�� ������� Layer �� �� Check
		// m_vecObjects�� �ƴ϶� m_vecParent�� �ϴ� ������ m_vecObjects�� ��� ���߿� Clear�� �Ǿ�
		// Ÿ�̹��� �������� ������ �ȵ� �� ����
		const vector<HYGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		// Parent �ݺ����� ���鼭 ��� �ڽ� ������Ʈ���� Queue�� ����
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<HYGameObject*> queue;
			queue.push_back(vecParent[j]);

			// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
			while (!queue.empty())
			{
				HYGameObject* pObject = queue.front();
				queue.pop_front();

				const vector<HYGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				if (_strName == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	return nullptr;
}

// vector���� ���� �̸��� �� ã�Ƽ� ��� vector�� �����
void HYLevel::FindObjectsByName(const wstring& _strName, vector<HYGameObject*>& _vecObj)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		const vector<HYGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		// Parent �ݺ����� ���鼭 ��� �ڽ� ������Ʈ���� Queue�� ����
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<HYGameObject*> queue;
			queue.push_back(vecParent[j]);

			// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
			while (!queue.empty())
			{
				HYGameObject* pObject = queue.front();
				queue.pop_front();

				const vector<HYGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				// ã�� ������Ʈ�� ���� �� �� ��� vector�� �����
				if (_strName == pObject->GetName())
				{
					_vecObj.push_back(pObject);
				}
			}
		}
	}
}

void HYLevel::clear()
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->m_vecObjects.clear();
	}
}

