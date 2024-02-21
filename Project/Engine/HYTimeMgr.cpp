#include "pch.h"
#include "HYTimeMgr.h"
#include "HYKeyMgr.h"

#include "HYEngine.h"

HYTimeMgr::HYTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_Time(0.f)
{
}

HYTimeMgr::~HYTimeMgr()
{
}

void HYTimeMgr::init()
{
	// �ʴ� ��
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevCount);
}

void HYTimeMgr::tick()
{
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDeltaTime = m_DeltaTime = double(m_CurCount.QuadPart - m_PrevCount.QuadPart) / double(m_Frequency.QuadPart);

	if (m_bLock)
	{
		m_DeltaTime = 0.f;
	}

	m_PrevCount = m_CurCount;

	// DT ����
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1.f / 60.f);


	// �ð� ���� ==> 1�ʸ��� if ���� ����
	m_Time += m_DeltaTime;
	if (1.f <= m_Time)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
		SetWindowText(HYEngine::GetInst()->GetMainWind(), szText);

		m_iCall = 0;
		m_Time = 0.f;
	}

	++m_iCall;

	// Ű ������ �ð� ����
	if (KEY_PRESSED(KEY::Y))
	{
		g_global.g_dt = 0.f;
	}
	else
	{
		g_global.g_dt = (float)m_DeltaTime;
	}

	g_global.g_dt = (float)m_DeltaTime;
	g_global.g_time += (float)m_DeltaTime;
}