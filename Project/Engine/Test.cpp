#include "pch.h"
#include "Test.h"

#include "HYDevice.h"

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

// VS(VertexShader) : IA 단계에서 전달시켜준 정점마다 함수를 호출
// Rasterize : 정점을 기준으로 픽셀을 찾음
ComPtr<ID3D11VertexShader>	g_VS = nullptr;
ComPtr<ID3D11PixelShader>	g_PS = nullptr;



int TestInit()
{
	// 전역변수에 삼각형 위치 설정
	//     0
	//   /   \
	//  2 --- 1
	// GPU는 최종 정점의 위치를 따질 떄 정규화된 좌표(NDC 좌표계)를 사용
	// NDC 좌표계 : -1 ~ 1 범위의 정사각형 형태의 좌표계
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);		// 이미지의 위치 정보

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
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
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	// 3 - 정점의 개수
	CONTEXT->Draw(3, 0);

	HYDevice::GetInst()->Present();
}

void TestRelease()
{

}
