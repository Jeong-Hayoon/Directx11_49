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

			// 현재 레벨에 넣으려는 Object를 AddObject
			HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(Object, LayerIdx, true);

			/*if (LEVEL_STATE::PLAY == pCurLevel->GetState())
			{
				Object->begin();
			}*/
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			//CObj* pDeadObj = (CObj*)m_vecTask[i].Param_1;
			//pDeadObj->SetDead();
		}
		break;
		case TASK_TYPE::LEVEL_CHANGE:
		{


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