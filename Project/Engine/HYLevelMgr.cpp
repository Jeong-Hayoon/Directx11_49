#include "pch.h"
#include "HYLevelMgr.h"
		  
#include "HYDevice.h"

#include "HYAssetMgr.h"
#include "HYTaskMgr.h"
#include "HYRenderMgr.h"		  
#include "HYCollisionMgr.h"

#include "HYLevel.h"
#include "HYLayer.h"
		 

HYLevelMgr::HYLevelMgr()
	: m_CurLevel(nullptr)
{
}

HYLevelMgr::~HYLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void HYLevelMgr::init()
{

}

void HYLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// 이전 프레임에 등록된 오브젝트들 clear
	// 매 프레임마다 어차피 자기 자신을 실시간으로 등록을 할 것이기 때문에
	// clear을 해줘야 함(이전 프레임이 다 모여있을 거니까)
	m_CurLevel->clear();

	m_CurLevel->tick();
	m_CurLevel->finaltick();

	// 충돌 처리
	HYCollisionMgr::GetInst()->tick();

	// Render
	HYRenderMgr::GetInst()->tick();
}

void HYLevelMgr::ChangeLevel(HYLevel* _NextLevel, LEVEL_STATE _NextLevelStartState)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param_1 = (DWORD_PTR)_NextLevel;
	task.Param_2 = (DWORD_PTR)_NextLevelStartState;

	HYTaskMgr::GetInst()->AddTask(task);
}

// Level의 상태를 변경시켜달라고 TaskMgr에게 요청
void HYLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_1 = (DWORD_PTR)m_CurLevel;
	task.Param_2 = (DWORD_PTR)_State;

	HYTaskMgr::GetInst()->AddTask(task);
}

// TaskMgr에게만 호출되는 함수
void HYLevelMgr::ChangeLevel_Task(HYLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	assert(!(m_CurLevel == _NextLevel));

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (nullptr != m_CurLevel)
		m_CurLevel->ChangeState(_NextLevelState);
}
