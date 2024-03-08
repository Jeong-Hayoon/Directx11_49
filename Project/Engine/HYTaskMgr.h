#pragma once

#include "singleton.h"

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Address
	CREATE_OBJECT,

	// Param1 : Object Address
	DELETE_OBJECT,

	// Param1 : Asset Address(생성시킨 에셋 포인터)
	ADD_ASSET,

	// Param1 : AssetType, Param2 : Asset Adress
	DELETE_ASSET,

	// Param1 : Level(상태가 바뀔), Param2 : LEVEL_STATE(바뀔 Level의 상태)
	CHANGE_LEVELSTATE,

	// Param1 : Level Address(바꿀 레벨의 주소), Param2 : Next Level Start State
	CHANGE_LEVEL,

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

	// Create Object 진행 시 true
	bool			m_bCreateObject;

	// Delete Object 진행 시 true
	bool			m_bDeleteObject;

	// Asset Change 진행 시 true
	bool			m_bAssetChange;

	int				m_DeleteFrameCount;

public:
	void tick();

	void AddTask(const tTask& _Task)
	{
		m_vecTask.push_back(_Task);
	}

	// Object가 생성/삭제되는 변화가 발생하면 true
	bool GetObjectEvent() { return m_bCreateObject || m_bDeleteObject; }

	// Asset에 변화가 발생하면 true
	bool GetAssetEvent() { return m_bAssetChange; }

private:
	void Clear();

};

