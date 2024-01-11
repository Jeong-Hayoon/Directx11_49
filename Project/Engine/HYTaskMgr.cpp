#include "pch.h"
#include "HYTaskMgr.h"

#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYGameObject.h"
#include "HYComponent.h"

HYTaskMgr::HYTaskMgr()
{
}

HYTaskMgr::~HYTaskMgr()
{
}

void HYTaskMgr::tick()
{
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			int LayerIdx = (int)m_vecTask[i].Param_1;
			HYGameObject* Object = (HYGameObject*)m_vecTask[i].Param_2;

			// ���� ������ �������� Object�� AddObject
			HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(Object, LayerIdx, true);

			/*if (LEVEL_STATE::PLAY == pCurLevel->GetState())
			{
				Object->begin();
			}*/
		}
		break;
		// �θ� ������Ʈ�� ������� �ڽ� ������Ʈ�� ������� ��
		case TASK_TYPE::DELETE_OBJECT:
		{
			HYGameObject* pDeadObj = (HYGameObject*)m_vecTask[i].Param_1;

			list<HYGameObject*> queue;
			queue.push_back(pDeadObj);

			// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
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