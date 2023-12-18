#pragma once

// ����Ʈ ������ ���� ���
//class SmartPointer
//{
//	int* pInt;
//
//public:
//	int* Get()
//	{
//		return pInt;
//	}
//
//	int** GetAdressOf()
//	{
//		return &pInt;
//	}
//};

class HYConstBuffer;

// DirectX11 �������� GPU ����
class HYDevice : public HYSingleton<HYDevice>
{
	SINGLE(HYDevice);

private:
	//ID3D11Device* m_Device;			// GPU �޸� ���� �� ��ü(����, ���ҽ�) ����
	//ID3D11DeviceContext* m_Context;	// GPU ������ ���

	ComPtr<ID3D11Device>			m_Device;	// GPU �޸� ����, ��ü ����
	ComPtr<ID3D11DeviceContext>		m_Context;	// GPU ������ ��� 

	ComPtr<IDXGISwapChain>			m_SwapChain; // ����ü��(��� ���� ����)

	// OM(OutputMergeState)
	ComPtr<ID3D11Texture2D>			m_RTTex;     // ����Ÿ�� �ؽ���
	ComPtr<ID3D11RenderTargetView>	m_RTView;    // ����Ÿ�� ��

	ComPtr<ID3D11Texture2D>			m_DSTex;	 // �X�� ���ٽ� �ؽ���	
	ComPtr<ID3D11DepthStencilView>	m_DSView;	// �X�� ���ٽ� ��

	HWND							m_hRenderWnd;
	Vec2							m_vRenderResolution;

	HYConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	// ����Ÿ���� ������ Clear �����ִ� �Լ�
	// float(&Color)[4] - ���ϴ� ������ �Լ��� ���ڷ� �ʿ�
	void ClearRenderTarget(float(&Color)[4]);
	// ȭ�鿡 ��½����ִ� �Լ�
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	HYConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }


private:
	int CreateSwapChain();
	int CreateTargetView();
	int CreateConstBuffer();


};