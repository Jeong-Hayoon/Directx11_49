#include "pch.h"
#include "HYLevel.h"

#include "HYLayer.h"
#include "HYGameObject.h"

#include "HYTimeMgr.h"
#include "HYRenderMgr.h"


HYLevel::HYLevel()
	: m_arrLayer{}
	, m_State(LEVEL_STATE::NONE)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = new HYLayer;
		// Layer�� ���� �� LayerIdx ����
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

HYLevel::HYLevel(const HYLevel& _OriginLevel)
	: HYEntity(_OriginLevel)
	, m_arrLayer{}
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = _OriginLevel.m_arrLayer[i]->Clone();
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

void HYLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	// ���� or �Ͻ����� or none -> �÷���
	if ((LEVEL_STATE::STOP == m_State || LEVEL_STATE::PAUSE == m_State || LEVEL_STATE::NONE == m_State)
		&& LEVEL_STATE::PLAY == _NextState)
	{
		HYTimeMgr::GetInst()->LockDeltaTime(false);

		// ���� ī�޶� ���
		HYRenderMgr::GetInst()->ActiveEditorMode(false);

		if (LEVEL_STATE::STOP == m_State)
		{
			begin();
		}
	}

	// �÷��� -> ���� or �Ͻ����� or none
	else if ((LEVEL_STATE::PLAY == m_State || LEVEL_STATE::NONE == m_State) &&
		(LEVEL_STATE::STOP == _NextState || LEVEL_STATE::PAUSE == _NextState || LEVEL_STATE::NONE == _NextState))
	{
		HYTimeMgr::GetInst()->LockDeltaTime(true);

		// ������ ī�޶� ���
		HYRenderMgr::GetInst()->ActiveEditorMode(true);
	}

	// ���� ������Ʈ ����
	m_State = _NextState;
}