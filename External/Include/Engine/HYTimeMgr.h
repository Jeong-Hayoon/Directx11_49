#pragma once

#include "singleton.h"

class HYTimeMgr
	: public HYSingleton<HYTimeMgr>
{
	SINGLE(HYTimeMgr);
private:
	// Larget Integer �� �׳� 8����Ʈ long long Ÿ�� ������ ���
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	double			m_DeltaTime;
	double			m_EngineDeltaTime;

	UINT			m_iCall;
	double			m_Time;

	// DT�� Lock ���� ����
	bool			m_bLock;

public:
	float GetDeltaTime() { return m_DeltaTime; }
	double GetDeltaTime_d() { return m_DeltaTime; }

	float GetEngineDeltaTime() { return (float)m_EngineDeltaTime; }
	double GetEngineDeltaTime_d() { return m_EngineDeltaTime; }

	void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

public:
	void init();
	void tick();
};

