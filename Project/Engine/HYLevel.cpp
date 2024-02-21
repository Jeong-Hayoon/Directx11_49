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
		// Layer를 만들 때 LayerIdx 설정
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
	// 지정된 Layer에 GameObject를 넣어줌
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

// 최초로 찾아지는 오브젝트를 바로 return
HYGameObject* HYLevel::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		// 모든 부모 오브젝트를 기반으로 Layer 싹 다 Check
		// m_vecObjects가 아니라 m_vecParent로 하는 이유는 m_vecObjects의 경우 도중에 Clear가 되어
		// 타이밍이 안좋으면 검출이 안될 수 있음
		const vector<HYGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		// Parent 반복문을 돌면서 모든 자식 오브젝트까지 Queue에 넣음
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<HYGameObject*> queue;
			queue.push_back(vecParent[j]);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
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

// vector에서 같은 이름을 다 찾아서 결과 vector에 담아줌
void HYLevel::FindObjectsByName(const wstring& _strName, vector<HYGameObject*>& _vecObj)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		const vector<HYGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		// Parent 반복문을 돌면서 모든 자식 오브젝트까지 Queue에 넣음
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<HYGameObject*> queue;
			queue.push_back(vecParent[j]);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				HYGameObject* pObject = queue.front();
				queue.pop_front();

				const vector<HYGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				// 찾은 오브젝트가 여러 개 일 경우 vector에 담아줌
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

	// 정지 or 일시정지 or none -> 플레이
	if ((LEVEL_STATE::STOP == m_State || LEVEL_STATE::PAUSE == m_State || LEVEL_STATE::NONE == m_State)
		&& LEVEL_STATE::PLAY == _NextState)
	{
		HYTimeMgr::GetInst()->LockDeltaTime(false);

		// 레벨 카메라 모드
		HYRenderMgr::GetInst()->ActiveEditorMode(false);

		if (LEVEL_STATE::STOP == m_State)
		{
			begin();
		}
	}

	// 플레이 -> 정지 or 일시정지 or none
	else if ((LEVEL_STATE::PLAY == m_State || LEVEL_STATE::NONE == m_State) &&
		(LEVEL_STATE::STOP == _NextState || LEVEL_STATE::PAUSE == _NextState || LEVEL_STATE::NONE == _NextState))
	{
		HYTimeMgr::GetInst()->LockDeltaTime(true);

		// 에디터 카메라 모드
		HYRenderMgr::GetInst()->ActiveEditorMode(true);
	}

	// 레벨 스테이트 변경
	m_State = _NextState;
}