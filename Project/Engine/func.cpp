#include "pch.h"

#include "HYTaskMgr.h"

// ������ �ֿ� �Լ����� GamePlayStatic namespace ���

// TaskMgr���� Task ����� ���ִ� �Լ�
void GamePlayStatic::SpawnGameObject(HYGameObject* _Target, int _LayerIdx)
{
	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	HYTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(HYGameObject* _Target)
{

}
