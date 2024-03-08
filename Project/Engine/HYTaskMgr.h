#pragma once

#include "singleton.h"

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Address
	CREATE_OBJECT,

	// Param1 : Object Address
	DELETE_OBJECT,

	// Param1 : Asset Address(������Ų ���� ������)
	ADD_ASSET,

	// Param1 : AssetType, Param2 : Asset Adress
	DELETE_ASSET,

	// Param1 : Level(���°� �ٲ�), Param2 : LEVEL_STATE(�ٲ� Level�� ����)
	CHANGE_LEVELSTATE,

	// Param1 : Level Address(�ٲ� ������ �ּ�), Param2 : Next Level Start State
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

	// Create Object ���� �� true
	bool			m_bCreateObject;

	// Delete Object ���� �� true
	bool			m_bDeleteObject;

	// Asset Change ���� �� true
	bool			m_bAssetChange;

	int				m_DeleteFrameCount;

public:
	void tick();

	void AddTask(const tTask& _Task)
	{
		m_vecTask.push_back(_Task);
	}

	// Object�� ����/�����Ǵ� ��ȭ�� �߻��ϸ� true
	bool GetObjectEvent() { return m_bCreateObject || m_bDeleteObject; }

	// Asset�� ��ȭ�� �߻��ϸ� true
	bool GetAssetEvent() { return m_bAssetChange; }

private:
	void Clear();

};

