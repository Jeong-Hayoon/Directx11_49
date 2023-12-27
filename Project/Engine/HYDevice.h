#pragma once

// 스마트 포인터 동작 방식
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

// DirectX11 기준으로 GPU 제어
class HYDevice : public HYSingleton<HYDevice>
{
	SINGLE(HYDevice);

private:
	//ID3D11Device* m_Device;			// GPU 메모리 관리 및 객체(버퍼, 리소스) 생성
	//ID3D11DeviceContext* m_Context;	// GPU 렌더링 명령

	ComPtr<ID3D11Device>			m_Device;	// GPU 메모리 관리, 객체 생성
	ComPtr<ID3D11DeviceContext>		m_Context;	// GPU 렌더링 명령 

	ComPtr<IDXGISwapChain>			m_SwapChain; // 스왚체인(출력 버퍼 지정)

	// OM(OutputMergeState)
	ComPtr<ID3D11Texture2D>			m_RTTex;     // 렌더타겟 텍스쳐
	ComPtr<ID3D11RenderTargetView>	m_RTView;    // 렌더타겟 뷰

	ComPtr<ID3D11Texture2D>			m_DSTex;	 // 뎊스 스텐실 텍스쳐	
	ComPtr<ID3D11DepthStencilView>	m_DSView;	// 뎊스 스텐실 뷰

	HWND							m_hRenderWnd;
	Vec2							m_vRenderResolution;

	HYConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	// 랜더타겟의 색상을 Clear 시켜주는 함수
	// float(&Color)[4] - 원하는 색상이 함수의 인자로 필요
	void ClearRenderTarget(float(&Color)[4]);
	// 화면에 출력시켜주는 함수
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
	HYConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }
	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }

private:
	int CreateSwapChain();
	int CreateTargetView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

	int CreateConstBuffer();


};