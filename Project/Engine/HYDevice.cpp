#include "pch.h"
#include "HYDevice.h"

#include "HYConstBuffer.h"
#include "HYAssetMgr.h"

HYDevice::HYDevice()
	: m_hRenderWnd(nullptr)
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
	, m_arrSampler{}
{
}

HYDevice::~HYDevice()
{
	Delete_Array(m_arrCB);
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

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"Blend State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSamplerState()))
	{
		MessageBox(nullptr, L"Sampler State 생성 실패", L"Device 초기화 실패", MB_OK);
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

	CONTEXT->RSSetViewports(1, &ViewportDesc);

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
	
}

// 우리가 만든 랜더타겟은 Default가 0이므로 색상이 검정색으로 보임 -> Clear 시켜줘야 함
void HYDevice::ClearRenderTarget(float(&Color)[4])
{
	// 랜더타겟을 가리키고 있는 뷰를 달라고 함
	m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), Color);
	// DepthStenci도 매 프레임마다 초기화를 시켜줘야 함(변동이 있을 수 있으므로)
	// DepthStenci Texture는 이미지가 아닌 0~1의 값으로 깊이값을 저장하고 있음
	// Texture가 꼭 이미지를 저장하고 있는 것이 아님
	// 특정 정보를 저장하고 있을 수도 있다는 것에 익숙해져야 함
	m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
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

	// 전역 데이터에 렌더링 해상도 기록(랜덤 타겟의 해상도 정보가 쉐이더 상에서 필요할 수도 있기 때문에 랜더링 해상도를 쉐이더에 전달해야 함)
	g_global.g_RenderResolution = m_vRenderResolution;

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
	ComPtr<ID3D11Texture2D> tex2D;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

	// RenderTargetTexture 를 AssetMgr 에 등록
	m_RTTex = HYAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

	// DepthStencil : 깊이감을 저장하는 텍스처
	// 리소스가 아니라서 텍스처를 직접 생성해야 함
	// 직접 만든 텍스처로 뷰를 만듦
	// DepthStencilTexture 생성
	m_DSTex = HYAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
												,(UINT)m_vRenderResolution.x
												, (UINT)m_vRenderResolution.y
												, DXGI_FORMAT_D24_UNORM_S8_UINT
												, D3D11_BIND_DEPTH_STENCIL);

	return S_OK;
}


int HYDevice::CreateRasterizerState()
{
	// CULL_BACK은 Default 옵션이기 때문에 nullptr을 넣어주면 자동으로 지정됨
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC tDesc = {};

	// Rasterizer State 모드 생성
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

	// LESS가 Default 옵션
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less Equal
	tDesc.DepthEnable = true;							// 깊이 판정 사용 여부
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		// 깊이 비교 옵션
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 깊이를 쓸 경우 기록 여부
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

	// ======== AlphaBlend ========
	// AlphaToCoverage : 물체들이 복잡하게 얽혀있을 때 알파값에 따른 깊이값을 깊이 보정 계산 여부
	// ex) 수풀, 나뭇잎
	tDesc.AlphaToCoverageEnable = false;
	// IndependentBlend : RenderTarget 인덱스 하나에 적용된 옵션이 다른 인덱스에도 적용되게 할지
	// 출력할 RenderTarget은 최대 8개까지 조합이 가능한데 RenderTarget간에 옵션을 일치시킬지
	// true면 각각 RenderTarget별로 블랜드 수식 옵션을 주는 것 / false면 일괄 적용
	tDesc.IndependentBlendEnable = false;

	// BlendEnable : BlendState 사용 여부
	tDesc.RenderTarget[0].BlendEnable = true;
	// BlendOp : 두 색상을 더할 때 덧셈으로 옵션 세팅
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// SrcBlend, DestBlend -> 색상을 더할 때 계수값 설정
	// SrcBlend : Pixel Shader에서 리턴시킨 값(출력되는 색상)에 SRC_ALPHA가 곱해짐
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	// DestBlend : 원래 RenderTarget에 그려진 색상에 INV(1-SRC_ALPHA)를 곱함
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	// ======== 별로 의미없는 부분 ========
	// 알파끼리는 어떻게 합칠지
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// 소스 알파에 들어갈 계수
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	// 목적지 알파에 들어갈 계수
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	// =====================================

	// 출력을 하겠다는 옵션
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());


	// ======== ONE_ONE Blend : 검은색 뒷배경을 안보이게 해주는 블랜드 ========
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = false;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

	return S_OK;
}

// 샘플링 : 빈틈을 채우는 방식
int HYDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};

	// 이방성 필터링 방식의 샘플링(3D) : 주변에 가까운 색상으로 평균치를 내서 그라데이션처럼 빈틈을 채우는 방식
	// D3D11_TEXTURE_ADDRESS_WRAP를 이용하여 물 같이 흐르는 오브젝트 표현 가능
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = 1;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[0].GetAddressOf());

	// MIN_MAG_MIP_POINT 방식의 샘플링 : 지정된 위치의 색상 값을 변동없이 가져오는 방식(2D)
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = 1;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[1].GetAddressOf());

	// 모든 Shader 단계에서 사용할 수 있도록 모든 스테이지에 바인딩
	CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());

	return S_OK;
}

// 상수버퍼를 포인터 배열로 미리 만들어놓는 함수
int HYDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new HYConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST] = new HYConstBuffer(CB_TYPE::MATERIAL_CONST);
	m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST]->Create(sizeof(tMtrlConst), 1);

	m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA] = new HYConstBuffer(CB_TYPE::ANIM2D_DATA);
	m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA]->Create(sizeof(tAnimData2D), 1);

	m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA] = new HYConstBuffer(CB_TYPE::GLOBAL_DATA);
	m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA]->Create(sizeof(tGlobalData), 1);

	return S_OK;
}
