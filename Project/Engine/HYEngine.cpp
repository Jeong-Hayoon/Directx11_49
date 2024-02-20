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

	// �ػ� != ������ ũ��
	RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

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
	HYRenderMgr::GetInst()->init();
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
	HYCollisionMgr::GetInst()->tick();

	// Render
	HYRenderMgr::GetInst()->tick();

	// GC
	HYGC::GetInst()->tick();

	// Task
	HYTaskMgr::GetInst()->tick();

	DebugFunctionCheck();


}

// ����� ������ �ɼ�
void HYEngine::DebugFunctionCheck()
{
	// ���� ������
	// HYRenderMgr::GetInst()->IsDebugPosition()�� true��
	// SetDebugPosition(false) �Լ� ȣ��
	// false��
	// HYRenderMgr::GetInst()->SetDebugPosition(true) �Լ� ȣ�� 
	if (KEY_TAP(KEY::O))
	{
		HYRenderMgr::GetInst()->IsDebugPosition() ? HYRenderMgr::GetInst()->SetDebugPosition(false) : HYRenderMgr::GetInst()->SetDebugPosition(true);
	}
}