#pragma once

class HYEngine
	: public HYSingleton<HYEngine>
{
	SINGLE(HYEngine);
private:
	HWND		m_hMainWnd;		// ���� ������ �ڵ�
	Vec2		m_vResolution;	// �ػ� ����


public:
	HWND GetMainWind() { return m_hMainWnd; }

private:
	void DebugFunctionCheck();


public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void progress();
};

