#include "pch.h"
#include "Test.h"

#include "HYDevice.h"

// �ﰢ�� �׸���
// ���� - ���� ����, ��ǥ, ����, ǥ�鿡 ���� ���� �� �پ��� ������ ������ �� ����
// �츮�� ������ ��ǥ�� �����ؼ� GPU ���� �˷��ְ� �̸� ����
// ����Ÿ�ٿ� �׸� ���� Present�� ��½�Ű���� �츮�� ������ ���� ������
// GPU �޸𸮷� �̵����Ѿ� ��
// GPU�� �޸𸮸� �Ҵ��ϰ� �����ϴ� Ŭ������ ID3D11Resource(Buffer, Texture 2D 2 ����)
// Buffer : �Ϲ����� ������ ������ ������ ���� ����
// Texture 2D : �̹��� ������ ������
// ���� ������ �����͸� �����ؾ� �ϹǷ� Buffer�� �����ؾ� ��

Vtx g_vtx[3] = {};


// ������ �����ϴ� ��������
ComPtr<ID3D11Buffer>		g_VB = nullptr;

// VS(VertexShader) : IA �ܰ迡�� ���޽����� �������� �Լ��� ȣ��
// Rasterize : ������ �������� �ȼ��� ã��
ComPtr<ID3D11VertexShader>	g_VS = nullptr;
ComPtr<ID3D11PixelShader>	g_PS = nullptr;



int TestInit()
{
	// ���������� �ﰢ�� ��ġ ����
	//     0
	//   /   \
	//  2 --- 1
	// GPU�� ���� ������ ��ġ�� ���� �� ����ȭ�� ��ǥ(NDC ��ǥ��)�� ���
	// NDC ��ǥ�� : -1 ~ 1 ������ ���簢�� ������ ��ǥ��
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);		// �̹����� ��ġ ����

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[2].vUV = Vec2(0.f, 0.f);


	// ���ؽ� ���� ����
	D3D11_BUFFER_DESC BufferDesc = {};

	// �ʿ��� Buffer�� �뷮(���� �뷮) -> �ܼ��� �ʿ��� byte�� �ѷ��� ���
	BufferDesc.ByteWidth = sizeof(Vtx) * 3;
	// ���� �ϳ��� �� byte����
	BufferDesc.StructureByteStride = sizeof(Vtx);
	// �뵵 : ���� ����
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// === �̷��� �� ������ �������� ===
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// ���� ���۸� ����� ������ �ϰ� �������� �� �� ����
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;


	// D3D11_SUBRESOURCE_DATA - g_Vtx �迭�� �����͸� �ʱ� �����ͷ� ����(�ʱⰪ ����)
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = g_vtx;

	// ���ؽ� ���� ����
	if (FAILED(DEVICE->CreateBuffer(&BufferDesc, &tSubData, g_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"���ؽ� ���� ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}



	return S_OK;
}

void TestProgress()
{
	// 0~255 <-> 0~1 Normalize 
	// ���� �ʱ�ȭ
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	// �ﰢ�� �׸���
	// stride : ���� ��ġ
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	// 3 - ������ ����
	CONTEXT->Draw(3, 0);

	HYDevice::GetInst()->Present();
}

void TestRelease()
{

}
