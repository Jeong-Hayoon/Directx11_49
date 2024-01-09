#pragma once

#include "singleton.h"


class HYTimeMgr
	: public HYSingleton<HYTimeMgr>
{
	SINGLE(HYTimeMgr);
private:
	// Larget Integer 는 그냥 8바이트 long long 타입 정수로 취급
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

