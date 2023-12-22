#include "pch.h"
#include "HYTimeMgr.h"

#include "HYEngine.h"

HYTimeMgr::HYTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_fTime(0.f)
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

	m_DeltaTime = float(m_CurCount.QuadPart - m_PrevCount.QuadPart) / float(m_Frequency.QuadPart);

	m_PrevCount = m_CurCount;

	// DT ����
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1.f / 60.f);


	// �ð� ���� ==> 1�ʸ��� if ���� ����
	m_fTime += m_DeltaTime;
	if (1.f <= m_fTime)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
		SetWindowText(HYEngine::GetInst()->GetMainWind(), szText);

		m_iCall = 0;
		m_fTime = 0.f;
	}

	++m_iCall;
}