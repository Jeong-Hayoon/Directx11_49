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
	float			m_DeltaTime;


	UINT			m_iCall;
	float			m_fTime;

public:
	float GetDeltaTime() { return m_DeltaTime; }

public:
	void init();
	void tick();
};

