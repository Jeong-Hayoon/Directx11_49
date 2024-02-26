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

	// ���� �����ӿ� ��ϵ� ������Ʈ�� clear
	// �� �����Ӹ��� ������ �ڱ� �ڽ��� �ǽð����� ����� �� ���̱� ������
	// clear�� ����� ��(���� �������� �� ������ �Ŵϱ�)
	m_CurLevel->clear();

	m_CurLevel->tick();
	m_CurLevel->finaltick();

	// �浹 ó��
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

// Level�� ���¸� ������Ѵ޶�� TaskMgr���� ��û
void HYLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_1 = (DWORD_PTR)m_CurLevel;
	task.Param_2 = (DWORD_PTR)_State;

	HYTaskMgr::GetInst()->AddTask(task);
}

// TaskMgr���Ը� ȣ��Ǵ� �Լ�
void HYLevelMgr::ChangeLevel_Task(HYLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	assert(!(m_CurLevel == _NextLevel));

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (nullptr != m_CurLevel)
		m_CurLevel->ChangeState(_NextLevelState);
}
