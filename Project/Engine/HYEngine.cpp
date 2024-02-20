#include "pch.h"
#include "HYEngine.h"

#include "HYDevice.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYPathMgr.h"
#include "HYAssetMgr.h"
#include "HYLevelMgr.h"
#include "HYTaskMgr.h"
#include "HYGC.h"
#include "HYRenderMgr.h"
#include "HYCollisionMgr.h"

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
	SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	// FAILED 매크로 : 인자가 음수면 실패, 양수면 성공
	// E_FAIL : 음수
	// S_OK : 0
	if (FAILED(HYDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
		return E_FAIL;
	}

	// Manager 초기화((PathMgr는 싱글톤 X)
	HYPathMgr::init();
	HYTimeMgr::GetInst()->init();
	HYKeyMgr::GetInst()->init();
	HYAssetMgr::GetInst()->init();
	HYRenderMgr::GetInst()->init();
	HYLevelMgr::GetInst()->init();

	return S_OK;
}

// 매 프레임마다 호출
void HYEngine::progress()
{
	// Manager Update
	HYTimeMgr::GetInst()->tick();
	HYKeyMgr::GetInst()->tick();
	
	// Level Update
	HYLevelMgr::GetInst()->tick();
	HYCollisionMgr::GetInst()->tick();

	// Render
	HYRenderMgr::GetInst()->tick();

	// GC
	HYGC::GetInst()->tick();

	// Task
	HYTaskMgr::GetInst()->tick();

	DebugFunctionCheck();


}

// 디버그 랜더링 옵션
void HYEngine::DebugFunctionCheck()
{
	// 삼항 연산자
	// HYRenderMgr::GetInst()->IsDebugPosition()가 true면
	// SetDebugPosition(false) 함수 호출
	// false면
	// HYRenderMgr::GetInst()->SetDebugPosition(true) 함수 호출 
	if (KEY_TAP(KEY::O))
	{
		HYRenderMgr::GetInst()->IsDebugPosition() ? HYRenderMgr::GetInst()->SetDebugPosition(false) : HYRenderMgr::GetInst()->SetDebugPosition(true);
	}
}