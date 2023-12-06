#include "pch.h"
#include "Test.h"

#include "HYDevice.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYPathMgr.h"


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

// InputLayout �����ϳ��� ����(Position, Color, UV ��)�� �˸��� ��ü
ComPtr<ID3D11InputLayout> g_Layout = nullptr;

// hlsl�� �ִ� VS_Std2D��� �Լ��� �������ؼ� Binary �ڵ�� �����
// Binary �ڵ带 �����ϴ� ����
// Blob : Shader�Լ��� �������ؼ� Binary �ڵ�� ���� �� ��������
// �̸� ���ؼ� �� Shader�� ���� �� ����
ComPtr<ID3DBlob> g_VSBlob = nullptr;
ComPtr<ID3DBlob> g_PSBlob = nullptr;
ComPtr<ID3DBlob> g_ErrBlob = nullptr;

// VS(VertexShader) : IA �ܰ迡�� ���޽����� �������� �Լ��� ȣ��
// Rasterize : ������ �������� �ȼ��� ã��
ComPtr<ID3D11VertexShader>	g_VS = nullptr;
ComPtr<ID3D11PixelShader>	g_PS = nullptr;



int TestInit()
{
	// ���������� �ﰢ�� ��ġ ����
	//      0(Red)
	//    /    \
	//  2(G) -- 1(Blue)
	// GPU�� ���� ������ ��ġ�� ���� �� ����ȭ�� ��ǥ(NDC ��ǥ��)�� ���
	// NDC ��ǥ�� : -1 ~ 1 ������ ���簢�� ������ ��ǥ��
	// Color ���� �� �������� �ٸ��� ������ ��� ������ ��ġ�� �ȼ� ��ġ��
	// ��ġ�ϴ� �ۼ�Ʈ�� ���� �ȼ��� �����Ǵ� RGB���� �޶���
	// ��������***
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);		// �̹����� ��ġ ����

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
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

	// Element�� �������� ���(Position, Color, UV��) ������ŭ �־�� ��
	// ���� ��������(Layout) ����
	// �� ��Һ��� ������ �ʿ�
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	// IASetVertexBuffers �Ҷ� ���õǴ� ����
	arrElement[0].InputSlot = 0;
	// VERTEX_DATA��� ���� �˷���
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	// SemanticIndex : SemanticName�� �����ϰ� �ϰ� ���� ��� ���
	// �ε����� 1�� �ϰ� hlsl�� _IN�� ��ҿ��� Sementic �ڿ� 1�� ���̸�
	// ���� ���� �ִ� �����͸� ���� SemanticName���� �� �� ����
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	// Vertex�� ���� ��ġ AlignedByteOffset�κ���
	arrElement[0].AlignedByteOffset = 0;
	// �ش� ����� ũ��(DXGI_FORMAT_R32G32B32_FLOAT -> 4byte*3, Float 3��)
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


	// ���ؽ� ���̴�
	// HLSL ���ؽ� ���̴� �Լ� �������� �ؾ� ��
	// ��� ��� �ʿ�
	// ���� ������ ���� ���� ���̴� �޾Ƽ� ������ �� �� ����
	wstring strFilePath = HYPathMgr::GetContentPath();

	// D3DCompileFromFile  - Blob�� Binary �ڵ带 �־���
	// Entry Point : ������ -> VS_Std2D
	// Target : �������� �� ���� ���� -> vs_5_0
	// D3DCOMPILE_DEBUG : ������ ���� ������ ������ ��
	// g_ErrBlob : �������� ���� ��������� Binary �ڵ尡 �ƴ� ���ڿ��� ������ ����
	// Blob ��ü �ϳ��� �����Ǹ鼭 GetAddressOf�� ���� ����
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

	// Shaderbytecode : Binary �ڵ� �޶�� ��
	// Size :  Binary �ڵ��� ������
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize(), nullptr
		, g_VS.GetAddressOf());

	// Layout ����(VertexShader�� ������� ���Ŀ� ȣ��)
	DEVICE->CreateInputLayout(arrElement, 3
		, g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf());

	
	// �ȼ� ���̴� ����
	// �ȼ� ���̴� ������
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
	// ���� �ʱ�ȭ
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);


	// �ﰢ�� �׸���
	// stride : ���� ��ġ
	// iStride : Vertex �ϳ��� ũ��
	// iOffset : Buffer�� �ִ� ��� ������ Vertex Shader�� ȣ���Ű�� ���� ���� �ֿ� ����
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	// IASetVertexBuffers : ���� �ϳ��� ����� �˷��ְ� offset���� ���� ���� ��ġ�� �˷���
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	// TOPOLOGY : VertexBuffer�� ����մ� ������ �ؼ��ϴ� ����(������ �����ְ� ���� ���� ����)
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(g_Layout.Get());

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	// 3 - ������ ����
	CONTEXT->Draw(3, 0);

	HYDevice::GetInst()->Present();
}

void TestRelease()
{

}
