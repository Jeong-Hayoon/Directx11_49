#include "pch.h"
#include "HYDevice.h"

HYDevice::HYDevice()
	: m_hRenderWnd(nullptr)
{

}

HYDevice::~HYDevice()
{

}

int HYDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// 출력 윈도우
	m_hRenderWnd = _hWnd;

	// 버퍼 해상도 
	m_vRenderResolution = _vResolution;


	// 장치 초기화
	// FEATURE_LEVEL : 기능 레벨의 버전 세팅
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE : GPU를 통해 장치 초기화
	// D3D11_CREATE_DEVICE_DEBUG : 디버그로 장치를 만들어야 문제 생겼을 때
	// 출력창에 어떤 것이 문제인지 알려주는 옵션
	// D3D11_SDK_VERSION : 들어가보면 7이라고 나와 있음
	// 이중포인터로 달라고 하고 있는 부분 - 변수가 포인터 타입인데 그 변수의 주소를
	// 인자로 달라고 하니까 이중 포인터
	if (FAILED(D3D11CreateDevice(nullptr
		, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, D3D11_CREATE_DEVICE_DEBUG
		, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 스왑체인 생성 - GPU 메모리 사용
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 렌더타겟(GPU 메모리 사용), 렌더타겟 뷰, 뎁스스텐실 타겟, 뎁스 스텐실 뷰 생성
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"타겟 및 View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 텍스처는 색만 저장하는 것이 아니라 깊이 정보도 저장 가능 -> Depth Texture
	// Depth Texture를 쓰는 이유? 랜더링 순서를 신경쓰지 않아도 됨

	// ViewPort(윈도우 상의 출력 지점) 설정
	D3D11_VIEWPORT ViewportDesc = {};

	// Depth - 0은 카메라 바로 앞, 1은 제일 먼 지점으로 세팅
	ViewportDesc.MinDepth = 0;
	ViewportDesc.MaxDepth = 1.f;

	// TopLeftX, TopLeftY의 지점에 Width * Height의 사이즈 사각형 안에 뷰포트가 설정됨
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = m_vRenderResolution.x;
	ViewportDesc.Height = m_vRenderResolution.y;

	return S_OK;
	
}

// 우리가 만든 랜더타겟은 Default가 0이므로 색상이 검정색으로 보임 -> Clear 시켜줘야 함
void HYDevice::ClearRenderTarget(float(&Color)[4])
{
	// 랜더타겟을 가리키고 있는 뷰를 달라고 함
	m_Context->ClearRenderTargetView(m_RTView.Get(), Color);
	// DepthStenci도 매 프레임마다 초기화를 시켜줘야 함(변동이 있을 수 있으므로)
	// DepthStenci Texture는 이미지가 아닌 0~1의 값으로 깊이값을 저장하고 있음
	// Texture가 꼭 이미지를 저장하고 있는 것이 아님
	// 특정 정보를 저장하고 있을 수도 있다는 것에 익숙해져야 함
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void HYDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

// 스왑체인을 만들고나면 스왑체인이 관리하는 랜더타겟용 텍스처도 만들어짐 -> 버퍼
int HYDevice::CreateSwapChain()
{
	// DESC : descripsion (설명서)
	// SwapChain 생성 구조체
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// SwapChain 이 관리하는 Buffer(RenderTarget) 의 구성 정보
	
	// 1개라고는 하지만 내부적으로 메모리를 복사해서
	// 1개 복사본을 만들어주기 때문에 이중버퍼 역할(더블버퍼링)
	tDesc.BufferCount = 1;

	// 버퍼의 용도가 랜더타겟 용도라는 것을 알려줘야함
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 가로, 세로 해상도 정보
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;

	// Buffer의 픽셀 Format / Normalize : 정규화시켜서 0~1사잇값으로 넘어감
	// (float a = 0.0f~1.0f 로 0~255 가 정규화 되어있다고 생각하면된다)
	// UNORM : Unsigned Normalize (정규화)
	// R8G8B8A8 - RGBA 4byte Format
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 분모를 1로 설정하고 분자를 60으로 설정, 화면 갱신 비율(60프레임)
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;

	// 디폴트 옵션
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.Flags = 0;

	// SampleDesc : 이미지중에 하나를 추출하는것
	// MSAA(Multi Sampling Anti Aliasing) 사용할 것인지
	/*픽셀이 적으면 계단현상 발생 -> Aliasing
	계단현상을 없애려는 것 -> Anti Aliasing
	SSAA(Super Sampling Anti Aliasing) - 4배씩 늘리고 보정을 한 후 다시 줄임 -> 선명
	단점 : 높은 비용

	MSAA(Multi Sampling Anti Aliasing) - 스레드에서 많이 쓰는 기법
	Anti Aliasing이 일어나는건 결국 테두리 -> 테두리를 잡고 보정
	단점 : 높은 비용, 사용자가 이 기능을 킬 수 있게 옵션*/

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	// 창모드
	tDesc.Windowed = true; 
	// SwapChain 의 출력 윈도우 지정
	tDesc.OutputWindow = m_hRenderWnd; 

	// 구조체를 채운 다음 스왑체인을 생성함수를 호출시켜야 하는데
	// 스왑체인을 생성시키는 객체를 얻어와야 함
	// 스왚체인 생성기능을 가지고 있는 Factory 에 접근한다.
	// IDXGIDevice : GPU에 온갖 Resource 생성해주는 Com 객체(유닛 생성), GPU 묘사
	// IDXGIAdapter : 그래픽 카드
	// ComPtr - 소멸에 대해 신경을 안써줘도 되는 템플릿 클래스
	// 클래스이기 때문에 자동으로 소멸자가 호출이 됨
	// Com 객체를 가리키는 포인터(Windows 운영 체제에서 사용하는 컴퓨터 정보를 가져오는 API)
	// .GetAddressOf() - 본인 멤버(포인터 변수)의 주소를 주는 함수(이중포인터)
	// .Get() - 본인이 들고 있는 주소를 주는 함수
	ComPtr<IDXGIDevice>		pIdxgiDevice = nullptr;
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;
	ComPtr<IDXGIFactory>	pFactory = nullptr;

	// ?(20)
	//uuidof(uuid를 꺼내오는 것)
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain 생성
	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int HYDevice::CreateTargetView()
{
	// ID3D11Resource : GPU 버퍼 데이터들을 관리 - 부모 클래스
	// ID3D11Buffer : 불특정(용도를 알 수 없는) 메모리 블록, 실사용 클래스
	// ID3D11Texture2D : 이미지 형태, 실사용 클래스

	// ?(30)
	// GetBuffer - Reference Count 증가 -> 연쇄적인 Release 해줘야 함 => ComPtr 활용
	// 렌더타겟 텍스쳐를 스왑체인으로부터 얻어온다.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// RenderTargetView : 랜더타겟용 텍스처를 가리키면서 만들어짐
	//(용도가 랜더타겟용이 아니면 애초에 뷰가 안만들어짐)
	m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

	// 실제 리소스 객체는 따로 있고 리소스 객체를 통해 중간에 뷰가 만들어짐
	// 리소스가 필요한 곳에서는 뷰를 달라고 함
	// 뷰를 통해 접근을 하게 함
	// 각 리소스들은 생성할 때 용도가 정해짐
	// 용도가 렌더타겟 용도가 아니었다면 타겟뷰가 생성이 안됨
	// 뷰가 넘어왔다는 것 자체가 랜더타겟 용도로 생성이 되었다는 것
	// 텍스처를 만들 때 용도를 여러 개를 동시에 수행할 수 있도록 하면 
	// 여러 개의 타겟뷰 생성도 가능
	// View? -> 어떤 용도인지 증명해주는 ID
	// RenderTargetView
	// DepthStencilView
	// ShaderResourceView
	// UnorderedAccessView
	

	// DepthStencil : 깊이감을 저장하는 텍스처
	// 리소스가 아니라서 텍스처를 직접 생성해야 함
	// 직접 만든 텍스처로 뷰를 만듦
	// DepthStencilTexture 생성
	D3D11_TEXTURE2D_DESC Desc = {};

	// 픽셀 포맷은 Depth 3byte, Stencil 1byte(픽셀 하나당 4byte)
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// DepthStencilState 텍스쳐 해상도는 반드시 RenderTargetTexture 와 동일해야한다.
	Desc.Width = (UINT)m_vRenderResolution.x;
	Desc.Height = (UINT)m_vRenderResolution.y;

	// BindFlags : 텍스처의 용도
	// DepthStencil 용도의 텍스쳐
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// CPU 접근 불가
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	// 샘플링
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	// MipLevels : 저퀄리티 버전의 사본 생성여부
	// 증가할수록 저해상도의 절반의 해상도로 복사본을 만듦
	// 멀리 있는 오브젝트는 고해상도일 필요가 없으므로 사용
	Desc.MipLevels = 1;		// 만들지 않음
	Desc.MiscFlags = 0;

	Desc.ArraySize = 1;

	if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencilView
	m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());

	// OM(Output Merge State) 에 RenderTargetTexture 와 DepthStencilTexture 를 전달한다.
	m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

	return S_OK;
}