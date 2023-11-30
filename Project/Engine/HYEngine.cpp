#include "pch.h"
#include "HYEngine.h"

#include "HYDevice.h"

HYEngine::HYEngine()
	: m_hMainWnd(nullptr)
	, m_vResolution()
{

}

HYEngine::~HYEngine()
{

}

int HYEngine::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hMainWnd = _hWnd;
	m_vResolution = _vResolution;

	// 해상도 != 윈도우 크기
	RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 10.f, 10.f, rt.right - rt.left, rt.bottom - rt.top, 0);

	// FAILED 매크로 : 인자가 음수면 실패, 양수면 성공
	// E_FAIL : 음수
	// S_OK : 0
	if (FAILED(HYDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

// 매 프레임마다 호출
void HYEngine::progress()
{

}