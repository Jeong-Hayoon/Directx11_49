#include "pch.h"
#include "HYAssetMgr.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYMaterial.h"

// �⺻������ �ʿ��� �ʼ� Asset ���
void HYAssetMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultGraphicsShader();

	CreateDefaultMaterial();

}

void HYAssetMgr::CreateDefaultMesh()
{
	HYMesh* pMesh = nullptr;

	// ���������� �ﰢ�� ��ġ ����
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// �ε���
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	pMesh = new HYMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", pMesh);

	// Topology LineStrip �뵵	
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	// VS�� 0-1-2-3-0���� 5�� ȣ��
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; 	arrIdx[4] = 0;

	pMesh = new HYMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh);

	// =================
	// CircleMesh �����
	// =================

	// ���ͷ� �� ������ ���̱� ������ �迭�� ������ Ư�����Ⱑ ����� ����
	// Ÿ�� ������ ���� ������ ���� - Ÿ������� ������ ������ NDC ��ǥ�踦 ����ϱ� ������
	// ȭ���� �ػ� ����(��Ⱦ��)�� ���缭 ���� ��׷����� �빮��

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// �߽� ��
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	// iSlice�� ������ŭ �ﰢ���� ����(���� �ﰢ���� ����)
	UINT iSlice = 40;
	float fTheta = 0.f;
	// ������
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		// XM_2PI = radian 360��
		// fTheta : ������ �����̽��� �ڸ� �� �ϳ��� ���� ũ��

		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new HYMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh);

	// CircleMesh_Debug

	// ���� �ε��� �ʱ�ȭ
	vecIdx.clear();
	// i�� 1�� ������ �߽����� �����ϱ� ����
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new HYMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh);

	// �� �ٸ� Mesh�� ���� ���� �����ϱ� �ʱ�ȭ��Ű��
	vecVtx.clear();
	vecIdx.clear();


	// =================
	// CrossMesh �����
	// =================
	v.vPos = Vec3(0.f, 0.5f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.f, -0.5f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh = new HYMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CrossMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}


void HYAssetMgr::CreateDefaultGraphicsShader()
{
	// =========================================================
	// Std2DShader
	// =========================================================
	HYGraphicsShader* pShader = nullptr;

	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"Std2DShader", pShader);

	// =========================================================
	// EffectShader : �Լ��� ������ ������ ������Ʈ�� �ٸ�
	// =========================================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"EffectShader", pShader);

	// =================================
	// GrayFilter Shader
	// Mesh			: RectMesh
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"GrayFilterShader", pShader);

	// ==================================================================
	// Distortion Shader	
	// Mesh			: � Msch�� OK
	// RS_TYPE		: CULL_NONE(�޸����� ������ �� �Ⱥ��̸� �ȵǴϱ�)
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// ==================================================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"DistortionShader", pShader);

	// ===================================================================================================
	// DebugShape Shader - ī�޶� ���忡���� ����, �� Shader�� ����ϴ� ������Ʈ�� Level ���� �����ϸ� X
	// ===================================================================================================
	
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);


	AddAsset(L"DebugShapeShader", pShader);
}

void HYAssetMgr::CreateDefaultMaterial()
{
	HYMaterial* pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"Std2DShader"));
	AddAsset<HYMaterial>(L"Std2DMtrl", pMtrl);

	// BackgroundMtrl
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"Std2DShader"));
	AddAsset<HYMaterial>(L"BackgroundMtrl", pMtrl);

	// GrayFilterMtrl
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"GrayFilterShader"));
	AddAsset<HYMaterial>(L"GrayFilterMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"DistortionShader"));
	AddAsset<HYMaterial>(L"DistortionMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"DebugShapeShader"));
	AddAsset<HYMaterial>(L"DebugShapeMtrl", pMtrl);
}