#include "pch.h"
#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYFontMgr.h"

#include "HYEngine.h"

HYTimeMgr::HYTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_Time(0.f)
	, m_szText{}
{
}

HYTimeMgr::~HYTimeMgr()
{
}

void HYTimeMgr::init()
{
	// 초당 빈도
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

	// DT 보정
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1.f / 60.f);

	// 시간 누적 ==> 1초마다 if 구문 실행
	m_Time += m_EngineDeltaTime;

	m_szText[50] = {};

	if (1.f <= m_Time)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
		// SetWindowText(HYEngine::GetInst()->GetMainWind(), szText);

		m_iCall = 0;
		m_Time = 0.f;
	}

	++m_iCall;

	// 키 누르면 시간 정지
	//if (KEY_PRESSED(KEY::Y))
	//{
	//	g_global.g_dt = 0.f;
	//}
	//else
	//{
	//	g_global.g_dt = (float)m_DeltaTime;
	//}

	//g_global.g_dt = (float)m_DeltaTime;
	g_global.g_time += (float)m_DeltaTime;
}

void HYTimeMgr::render()
{
	// 폰트 출력(문자열, 윈도우 좌표, 사이즈, 색상값)
	HYFontMgr::GetInst()->DrawFont(m_szText, 10.f, 30.f, 16, FONT_RGBA(255, 30, 30, 255));
}