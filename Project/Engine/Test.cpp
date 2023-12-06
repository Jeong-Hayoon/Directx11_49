#include "pch.h"
#include "Test.h"

#include "HYDevice.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYPathMgr.h"


// 삼각형 그리기
// 정점 - 점의 정보, 좌표, 색상, 표면에 대한 정보 등 다양한 정보를 저장할 수 있음
// 우리가 정점의 좌표를 설정해서 GPU 한테 알려주고 이를 통해
// 랜더타겟에 그린 다음 Present로 출력시키려면 우리가 설정한 정점 정보를
// GPU 메모리로 이동시켜야 함
// GPU에 메모리를 할당하고 관리하는 클래스가 ID3D11Resource(Buffer, Texture 2D 2 종류)
// Buffer : 일반적인 목적의 데이터 형태의 정보 저장
// Texture 2D : 이미지 형태의 데이터
// 지금 정점의 데이터를 저장해야 하므로 Buffer를 생성해야 함

Vtx g_vtx[3] = {};


// 정점을 저장하는 정점버퍼
ComPtr<ID3D11Buffer>		g_VB = nullptr;

// InputLayout 정점하나의 구조(Position, Color, UV 등)를 알리는 객체
ComPtr<ID3D11InputLayout> g_Layout = nullptr;

// hlsl에 있는 VS_Std2D라는 함수를 컴파일해서 Binary 코드로 만들고
// Binary 코드를 저장하는 공간
// Blob : Shader함수를 컴파일해서 Binary 코드로 만든 후 저장해줌
// 이를 통해서 각 Shader를 만들 수 있음
ComPtr<ID3DBlob> g_VSBlob = nullptr;
ComPtr<ID3DBlob> g_PSBlob = nullptr;
ComPtr<ID3DBlob> g_ErrBlob = nullptr;

// VS(VertexShader) : IA 단계에서 전달시켜준 정점마다 함수를 호출
// Rasterize : 정점을 기준으로 픽셀을 찾음
ComPtr<ID3D11VertexShader>	g_VS = nullptr;
ComPtr<ID3D11PixelShader>	g_PS = nullptr;



int TestInit()
{
	// 전역변수에 삼각형 위치 설정
	//      0(Red)
	//    /    \
	//  2(G) -- 1(Blue)
	// GPU는 최종 정점의 위치를 따질 떄 정규화된 좌표(NDC 좌표계)를 사용
	// NDC 좌표계 : -1 ~ 1 범위의 정사각형 형태의 좌표계
	// Color 값을 각 정점마다 다르게 지정할 경우 정점의 위치와 픽셀 위치가
	// 일치하는 퍼센트에 따라 픽셀에 지정되는 RGB값이 달라짐
	// 선형보간***
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);		// 이미지의 위치 정보

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	g_vtx[2].vUV = Vec2(0.f, 0.f);


	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC BufferDesc = {};

	// 필요한 Buffer의 용량(정점 용량) -> 단순히 필요한 byte의 총량을 명시
	BufferDesc.ByteWidth = sizeof(Vtx) * 3;
	// 정점 하나가 몇 byte인지
	BufferDesc.StructureByteStride = sizeof(Vtx);
	// 용도 : 정점 저장
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// === 이렇게 두 조건이 합쳐지면 ===
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 정점 버퍼를 만들고 수정을 하고 재저장을 할 수 있음
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;


	// D3D11_SUBRESOURCE_DATA - g_Vtx 배열의 데이터를 초기 데이터로 설정(초기값 설정)
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = g_vtx;

	// 버텍스 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&BufferDesc, &tSubData, g_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	// Element는 정점언의 요소(Position, Color, UV등) 개수만큼 있어야 함
	// 정점 구조정보(Layout) 생성
	// 각 요소별로 설명이 필요
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	// IASetVertexBuffers 할때 세팅되는 슬롯
	arrElement[0].InputSlot = 0;
	// VERTEX_DATA라는 것을 알려줌
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	// SemanticIndex : SemanticName을 동일하게 하고 싶은 경우 사용
	// 인덱스를 1로 하고 hlsl의 _IN의 요소에서 Sementic 뒤에 1을 붙이면
	// 정점 내에 있는 데이터를 같은 SemanticName으로 줄 수 있음
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	// Vertex의 시작 위치 AlignedByteOffset로부터
	arrElement[0].AlignedByteOffset = 0;
	// 해당 요소의 크기(DXGI_FORMAT_R32G32B32_FLOAT -> 4byte*3, Float 3개)
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;


	// 버텍스 쉐이더
	// HLSL 버텍스 쉐이더 함수 컴파일을 해야 함
	// 상대 경로 필요
	// 실행 폴더만 있을 때도 쉐이더 받아서 컴파일 할 수 있음
	wstring strFilePath = HYPathMgr::GetContentPath();

	// D3DCompileFromFile  - Blob에 Binary 코드를 넣어줌
	// Entry Point : 진입접 -> VS_Std2D
	// Target : 컴파일할 때 문법 버전 -> vs_5_0
	// D3DCOMPILE_DEBUG : 컴파일 실패 사유가 나오게 됨
	// g_ErrBlob : 실패했을 때만 만들어지며 Binary 코드가 아닌 문자열로 에러가 생성
	// Blob 객체 하나가 생성되면서 GetAddressOf룰 통해 받음
	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Std2D", "vs_5_0", D3DCOMPILE_DEBUG, 0
		, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	// Shaderbytecode : Binary 코드 달라고 함
	// Size :  Binary 코드의 사이즈
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize(), nullptr
		, g_VS.GetAddressOf());

	// Layout 생성(VertexShader가 만들어진 이후에 호출)
	DEVICE->CreateInputLayout(arrElement, 3
		, g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf());

	
	// 픽셀 쉐이더 생성
	// 픽셀 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Std2D", "ps_5_0", D3DCOMPILE_DEBUG, 0
		, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer()
		, g_PSBlob->GetBufferSize(), nullptr
		, g_PS.GetAddressOf());



	return S_OK;
}

void TestProgress()
{
	// 0~255 <-> 0~1 Normalize 
	// 배경색 초기화
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	// 삼각형 그리기
	// stride : 시작 위치
	// iStride : Vertex 하나의 크기
	// iOffset : Buffer에 있는 모든 정점을 Vertex Shader를 호출시키지 않을 수도 있울 때뮨
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	// IASetVertexBuffers : 정점 하나의 사이즈를 알려주고 offset값을 통해 시작 위치를 알려줌
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	// TOPOLOGY : VertexBuffer에 담겨잇는 정점을 해석하는 구조(정점이 말해주고 싶은 도형 정보)
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(g_Layout.Get());

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	// 3 - 정점의 개수
	CONTEXT->Draw(3, 0);

	HYDevice::GetInst()->Present();
}

void TestRelease()
{

}
