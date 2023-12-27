#include "pch.h"

#include "HYTaskMgr.h"

// 게임의 주요 함수들은 GamePlayStatic namespace 사용

// TaskMgr에게 Task 등록을 해주는 함수
void GamePlayStatic::SpawnGameObject(HYGameObject* _Target, int _LayerIdx)
{
	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	HYTaskMgr::GetInst()->AddTask(task);
}

// TaskMgr에게 GameObject를 지워달라고 요쳥하는 함수
void GamePlayStatic::DestroyGameObject(HYGameObject* _Target)
{
	FTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;
	HYTaskMgr::GetInst()->AddTask(task);

}
