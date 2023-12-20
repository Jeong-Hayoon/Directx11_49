#include "pch.h"
#include "HYDevice.h"

#include "HYConstBuffer.h"


HYDevice::HYDevice()
	: m_hRenderWnd(nullptr)
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
{

}

HYDevice::~HYDevice()
{
	Delete_Array(m_arrCB);
}

int HYDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// ��� ������
	m_hRenderWnd = _hWnd;

	// ���� �ػ� 
	m_vRenderResolution = _vResolution;


	// ��ġ �ʱ�ȭ
	// FEATURE_LEVEL : ��� ������ ���� ����
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE : GPU�� ���� ��ġ �ʱ�ȭ
	// D3D11_CREATE_DEVICE_DEBUG : ����׷� ��ġ�� ������ ���� ������ ��
	// ���â�� � ���� �������� �˷��ִ� �ɼ�
	// D3D11_SDK_VERSION : ������ 7�̶�� ���� ����
	// ���������ͷ� �޶�� �ϰ� �ִ� �κ� - ������ ������ Ÿ���ε� �� ������ �ּҸ�
	// ���ڷ� �޶�� �ϴϱ� ���� ������
	if (FAILED(D3D11CreateDevice(nullptr
		, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, D3D11_CREATE_DEVICE_DEBUG
		, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// ����ü�� ���� - GPU �޸� ���
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// ����Ÿ��(GPU �޸� ���), ����Ÿ�� ��, �������ٽ� Ÿ��, ���� ���ٽ� �� ����
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"Ÿ�� �� View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"Blend State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// �ؽ�ó�� ���� �����ϴ� ���� �ƴ϶� ���� ������ ���� ���� -> Depth Texture
	// Depth Texture�� ���� ����? ������ ������ �Ű澲�� �ʾƵ� ��

	// ViewPort(������ ���� ��� ����) ����
	D3D11_VIEWPORT ViewportDesc = {};

	// Depth - 0�� ī�޶� �ٷ� ��, 1�� ���� �� �������� ����
	ViewportDesc.MinDepth = 0;
	ViewportDesc.MaxDepth = 1.f;

	// TopLeftX, TopLeftY�� ������ Width * Height�� ������ �簢�� �ȿ� ����Ʈ�� ������
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = m_vRenderResolution.x;
	ViewportDesc.Height = m_vRenderResolution.y;

	CONTEXT->RSSetViewports(1, &ViewportDesc);

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"������� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
	
}

// �츮�� ���� ����Ÿ���� Default�� 0�̹Ƿ� ������ ���������� ���� -> Clear ������� ��
void HYDevice::ClearRenderTarget(float(&Color)[4])
{
	// ����Ÿ���� ����Ű�� �ִ� �並 �޶�� ��
	m_Context->ClearRenderTargetView(m_RTView.Get(), Color);
	// DepthStenci�� �� �����Ӹ��� �ʱ�ȭ�� ������� ��(������ ���� �� �����Ƿ�)
	// DepthStenci Texture�� �̹����� �ƴ� 0~1�� ������ ���̰��� �����ϰ� ����
	// Texture�� �� �̹����� �����ϰ� �ִ� ���� �ƴ�
	// Ư�� ������ �����ϰ� ���� ���� �ִٴ� �Ϳ� �ͼ������� ��
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void HYDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

// ����ü���� ������� ����ü���� �����ϴ� ����Ÿ�ٿ� �ؽ�ó�� ������� -> ����
int HYDevice::CreateSwapChain()
{
	// DESC : descripsion (����)
	// SwapChain ���� ����ü
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// SwapChain �� �����ϴ� Buffer(RenderTarget) �� ���� ����
	
	// 1������ ������ ���������� �޸𸮸� �����ؼ�
	// 1�� ���纻�� ������ֱ� ������ ���߹��� ����(������۸�)
	tDesc.BufferCount = 1;

	// ������ �뵵�� ����Ÿ�� �뵵��� ���� �˷������
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// ����, ���� �ػ� ����
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;

	// Buffer�� �ȼ� Format / Normalize : ����ȭ���Ѽ� 0~1���հ����� �Ѿ
	// (float a = 0.0f~1.0f �� 0~255 �� ����ȭ �Ǿ��ִٰ� �����ϸ�ȴ�)
	// UNORM : Unsigned Normalize (����ȭ)
	// R8G8B8A8 - RGBA 4byte Format
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �и� 1�� �����ϰ� ���ڸ� 60���� ����, ȭ�� ���� ����(60������)
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;

	// ����Ʈ �ɼ�
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.Flags = 0;

	// SampleDesc : �̹����߿� �ϳ��� �����ϴ°�
	// MSAA(Multi Sampling Anti Aliasing) ����� ������
	/*�ȼ��� ������ ������� �߻� -> Aliasing
	��������� ���ַ��� �� -> Anti Aliasing
	SSAA(Super Sampling Anti Aliasing) - 4�辿 �ø��� ������ �� �� �ٽ� ���� -> ����
	���� : ���� ���

	MSAA(Multi Sampling Anti Aliasing) - �����忡�� ���� ���� ���
	Anti Aliasing�� �Ͼ�°� �ᱹ �׵θ� -> �׵θ��� ��� ����
	���� : ���� ���, ����ڰ� �� ����� ų �� �ְ� �ɼ�*/

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	// â���
	tDesc.Windowed = true; 
	// SwapChain �� ��� ������ ����
	tDesc.OutputWindow = m_hRenderWnd; 

	// ����ü�� ä�� ���� ����ü���� �����Լ��� ȣ����Ѿ� �ϴµ�
	// ����ü���� ������Ű�� ��ü�� ���;� ��
	// ����ü�� ��������� ������ �ִ� Factory �� �����Ѵ�.
	// IDXGIDevice : GPU�� �°� Resource �������ִ� Com ��ü(���� ����), GPU ����
	// IDXGIAdapter : �׷��� ī��
	// ComPtr - �Ҹ꿡 ���� �Ű��� �Ƚ��൵ �Ǵ� ���ø� Ŭ����
	// Ŭ�����̱� ������ �ڵ����� �Ҹ��ڰ� ȣ���� ��
	// Com ��ü�� ����Ű�� ������(Windows � ü������ ����ϴ� ��ǻ�� ������ �������� API)
	// .GetAddressOf() - ���� ���(������ ����)�� �ּҸ� �ִ� �Լ�(����������)
	// .Get() - ������ ��� �ִ� �ּҸ� �ִ� �Լ�
	ComPtr<IDXGIDevice>		pIdxgiDevice = nullptr;
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;
	ComPtr<IDXGIFactory>	pFactory = nullptr;

	// ?(20)
	//uuidof(uuid�� �������� ��)
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain ����
	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int HYDevice::CreateTargetView()
{
	// ID3D11Resource : GPU ���� �����͵��� ���� - �θ� Ŭ����
	// ID3D11Buffer : ��Ư��(�뵵�� �� �� ����) �޸� ���, �ǻ�� Ŭ����
	// ID3D11Texture2D : �̹��� ����, �ǻ�� Ŭ����

	// ?(30)
	// GetBuffer - Reference Count ���� -> �������� Release ����� �� => ComPtr Ȱ��
	// ����Ÿ�� �ؽ��ĸ� ����ü�����κ��� ���´�.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// RenderTargetView : ����Ÿ�ٿ� �ؽ�ó�� ����Ű�鼭 �������
	//(�뵵�� ����Ÿ�ٿ��� �ƴϸ� ���ʿ� �䰡 �ȸ������)
	m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

	// ���� ���ҽ� ��ü�� ���� �ְ� ���ҽ� ��ü�� ���� �߰��� �䰡 �������
	// ���ҽ��� �ʿ��� �������� �並 �޶�� ��
	// �並 ���� ������ �ϰ� ��
	// �� ���ҽ����� ������ �� �뵵�� ������
	// �뵵�� ����Ÿ�� �뵵�� �ƴϾ��ٸ� Ÿ�ٺ䰡 ������ �ȵ�
	// �䰡 �Ѿ�Դٴ� �� ��ü�� ����Ÿ�� �뵵�� ������ �Ǿ��ٴ� ��
	// �ؽ�ó�� ���� �� �뵵�� ���� ���� ���ÿ� ������ �� �ֵ��� �ϸ� 
	// ���� ���� Ÿ�ٺ� ������ ����
	// View? -> � �뵵���� �������ִ� ID
	// RenderTargetView
	// DepthStencilView
	// ShaderResourceView
	// UnorderedAccessView
	

	// DepthStencil : ���̰��� �����ϴ� �ؽ�ó
	// ���ҽ��� �ƴ϶� �ؽ�ó�� ���� �����ؾ� ��
	// ���� ���� �ؽ�ó�� �並 ����
	// DepthStencilTexture ����
	D3D11_TEXTURE2D_DESC Desc = {};

	// �ȼ� ������ Depth 3byte, Stencil 1byte(�ȼ� �ϳ��� 4byte)
	// ���� �ؽ�ó�� �ȼ� �ϳ��� 4byte�ε� 3byte - �ε� �Ҽ��� �Ǽ��� ����(0~1 �Ǽ�)
	// 1byte - Stencil �뵵(0~255)
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// DepthStencilState �ؽ��� �ػ󵵴� �ݵ�� RenderTargetTexture �� �����ؾ��Ѵ�.
	Desc.Width = (UINT)m_vRenderResolution.x;
	Desc.Height = (UINT)m_vRenderResolution.y;

	// BindFlags : �ؽ�ó�� �뵵
	// DepthStencil �뵵�� �ؽ���
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// CPU ���� �Ұ�
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	// ���ø�
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	// MipLevels : ������Ƽ ������ �纻 ��������
	// �����Ҽ��� ���ػ��� ������ �ػ󵵷� ���纻�� ����
	// �ָ� �ִ� ������Ʈ�� ���ػ��� �ʿ䰡 �����Ƿ� ���
	Desc.MipLevels = 1;		// ������ ����
	Desc.MiscFlags = 0;

	Desc.ArraySize = 1;

	if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencilView
	m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());

	// OM(Output Merge State) �� RenderTargetTexture �� DepthStencilTexture �� �����Ѵ�.
	m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

	return S_OK;
}


int HYDevice::CreateRasterizerState()
{
	// CULL_BACK�� Default �ɼ��̱� ������ nullptr�� �־��ָ� �ڵ����� ������
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC tDesc = {};

	// Rasterizer State ��� ����
	tDesc.CullMode = D3D11_CULL_FRONT;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	return S_OK;
}

int HYDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	// LESS�� Default �ɼ�
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less Equal
	tDesc.DepthEnable = true;							// ���� ���� ��� ����
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		// ���� �� �ɼ�
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// ���̸� �� ��� ��� ����
	tDesc.StencilEnable = false;	

	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// Greater
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// Greater Equal
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER_EQUAL].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// No Test
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// No Write
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// NoTest NoWrite
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

int HYDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc = {};

	// AlphaBlend
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = true;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

	return S_OK;
}

// ������۸� ������ �迭�� �̸� �������� �Լ�
int HYDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new HYConstBuffer;
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	return S_OK;
}
