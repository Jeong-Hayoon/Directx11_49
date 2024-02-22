#pragma once

#include "singleton.h"

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : Asset Adress, Param2 : Asset 
	ADD_ASSET,

	// Param1 : Level(상태가 바뀔), Param2 : LEVEL_STATE(바뀔 Level의 상태)
	CHANGE_LEVELSTATE,

	// Param1 : LEVEL_TYPE
	LEVEL_CHANGE,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,
};

struct tTask
{
	TASK_TYPE Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
};

class HYTaskMgr
	: public HYSingleton<HYTaskMgr>
{
	SINGLE(HYTaskMgr);

private:
	vector<tTask>	m_vecTask;

	bool			m_bCreateObject;
	bool			m_bDeleteObject;
	bool			m_bAssetChange;

	int				m_DeleteFrameCount;

public:
	void tick();

	void AddTask(const tTask& _Task)
	{
		m_vecTask.push_back(_Task);
	}

	bool GetObjectEvent() { return m_bCreateObject || m_bDeleteObject; }
	bool GetAssetEvent() { return m_bAssetChange; }

private:
	void Clear();

};

