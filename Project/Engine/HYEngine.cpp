#include "pch.h"
#include "HYEngine.h"

#include "HYDevice.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYPathMgr.h"

#include "HYAssetMgr.h"
#include "HYLevelMgr.h"

// #include "Test.h"

HYEngine::HYEngine()
	: m_hMainWnd(nullptr)
	, m_vResolution()
{

}

HYEngine::~HYEngine()
{
	TestRelease();
}

int HYEngine::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hMainWnd = _hWnd;
	m_vResolution = _vResolution;

	// �ػ� != ������ ũ��
	RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 10.f, 10.f, rt.right - rt.left, rt.bottom - rt.top, 0);

	// FAILED ��ũ�� : ���ڰ� ������ ����, ����� ����
	// E_FAIL : ����
	// S_OK : 0
	if (FAILED(HYDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// Manager �ʱ�ȭ((PathMgr�� �̱��� X)
	HYPathMgr::init();
	HYTimeMgr::GetInst()->init();
	HYKeyMgr::GetInst()->init();
	HYAssetMgr::GetInst()->init();
	HYLevelMgr::GetInst()->init();

	return S_OK;
}

// �� �����Ӹ��� ȣ��
void HYEngine::progress()
{
	// Manager Update
	HYTimeMgr::GetInst()->tick();
	HYKeyMgr::GetInst()->tick();
	
	// Level Update
	HYLevelMgr::GetInst()->tick();
	HYLevelMgr::GetInst()->render();

}