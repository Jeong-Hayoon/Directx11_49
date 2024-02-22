#include "pch.h"

#include "HYTaskMgr.h"
#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYGameObject.h"
#include "HYComponent.h"

#include "HYAssetMgr.h"

HYTaskMgr::HYTaskMgr()
	: m_bCreateObject(false)
	, m_bDeleteObject(false)
{
}

HYTaskMgr::~HYTaskMgr()
{
}

void HYTaskMgr::tick()
{
	Clear();

	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			int LayerIdx = (int)m_vecTask[i].Param_1;
			HYGameObject* Object = (HYGameObject*)m_vecTask[i].Param_2;

			// 현재 레벨에 넣으려는 Object를 AddObject
			HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(Object, LayerIdx, true);

			m_bCreateObject = true;

			/*if (LEVEL_STATE::PLAY == pCurLevel->GetState())
			{
				Object->begin();
			}*/
		}
		break;
		// 부모 오브젝트가 사라지면 자식 오브젝트도 사라지게 됨
		case TASK_TYPE::DELETE_OBJECT:
		{
			HYGameObject* pDeadObj = (HYGameObject*)m_vecTask[i].Param_1;

			list<HYGameObject*> queue;
			queue.push_back(pDeadObj);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				HYGameObject* pObject = queue.front();
				queue.pop_front();

				pObject->m_bDead = true;

				for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
				{
					queue.push_back(pObject->m_vecChild[i]);
				}
			}

			if (m_DeleteFrameCount == 0)
				++m_DeleteFrameCount;
			else if (m_DeleteFrameCount == 2)
				m_DeleteFrameCount = 1;
		}
		break;

		case TASK_TYPE::ADD_ASSET:
		{
			// Param1 : Asset Address
			HYAsset* pAsset = (HYAsset*)m_vecTask[i].Param_1;
			HYAssetMgr::GetInst()->AddAsset(pAsset->GetName(), pAsset);
			m_bAssetChange = true;
		}
		break;

		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			HYLevel* pLevel = (HYLevel*)m_vecTask[i].Param_1;
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_2;

			pLevel->ChangeState(NextState);
		}
		break;
		case TASK_TYPE::LEVEL_CHANGE:
		{
			break;
		}
		case TASK_TYPE::ADD_CHILD:

			break;

		case TASK_TYPE::DISCONNECT_PARENT:

			break;

			break;
		}
	}

	m_vecTask.clear();
}

void HYTaskMgr::Clear()
{
	m_bCreateObject = false;

	// 억지
	if (1 == m_DeleteFrameCount)
	{
		++m_DeleteFrameCount;
		m_bDeleteObject = true;
	}
	else if (2 <= m_DeleteFrameCount)
	{
		m_DeleteFrameCount = 0;
		m_bDeleteObject = false;
	}

	m_bAssetChange = false;
}