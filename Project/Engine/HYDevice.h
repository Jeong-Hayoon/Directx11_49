#pragma once

// DirectX11 �������� GPU ����
class HYDevice : public HYSingleton<HYDevice>
{
	SINGLE(HYDevice);

public:
	int init(HWND _hWnd, Vec2 _vResolution);

private:
	ID3D11Device* m_Device;			// GPU �޸� ���� �� ��ü(����, ���ҽ�) ����
	ID3D11DeviceContext* m_Context; // GPU ������ ���

};