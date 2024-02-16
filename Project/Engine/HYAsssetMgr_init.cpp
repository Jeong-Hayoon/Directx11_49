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

	CreateDefaultComputeShader();

	CreateDefaultMaterial();

}

void HYAssetMgr::CreateDefaultMesh()
{
	HYMesh* pMesh = nullptr;

	// Geometry Shader ���� Mesh
	Vtx vPoint;
	UINT Idx = 0;
	pMesh = new HYMesh;
	// ���̱� ������ Position, Color, UV �ƹ��͵� ���������� �ʾƵ� ��
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(L"PointMesh", pMesh);

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
	Ptr<HYGraphicsShader> pShader = nullptr;

	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"Std2DShader", pShader.Get());

	// Parameter
	// �׽�Ʈ��
	// pShader->AddScalarParam(INT_0, "Test Parameter");
	// 0�� �ؽ�ó �ڸ��� �� Shader�� ����Ѵٸ� ������ 0�� �ؽ�ó �ڸ����ٰ� ���޽�ų �ؽ�ó�� ������ ������� ��
	// ���õ� �ؽ�ó�� ��� �뵵
	// pShader->AddTexParam(TEX_0, "Output Texture");		
	// Parameter	
	// pShader->AddScalarParam(VEC2_2, "Test Parameter");
	// pShader->AddScalarParam(VEC4_1, "Test Parameter");

	pShader->AddScalarParam(INT_0, "Test Param");

	pShader->AddTexParam(TEX_0, "Output Texture 1");
	//pShader->AddTexParam(TEX_1, "Output Texture 2");

	// =========================================================
	// EffectShader : �Լ��� ������ ���� ������Ʈ�� �ٸ�
	// =========================================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"EffectShader", pShader.Get());

	// =============
	// TileMapShader
	// =============
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader.Get());

	// ====================
	// ParticleRenderShader
	// ====================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// ���� �׽�Ʈ�� ����, ���̴� ��� X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"ParticleRenderShader", pShader.Get());

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

	AddAsset(L"GrayFilterShader", pShader.Get());

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

	AddAsset(L"DistortionShader", pShader.Get());

	// =================================
	// Wave Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Wave");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Wave");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"WaveFilterShader", pShader.Get());

	// =================================
	// OutLine Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\outline.fx", "VS_Outline");
	pShader->CreatePixelShader(L"shader\\outline.fx", "PS_Outline");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"OutLineShader", pShader.Get());

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


	AddAsset(L"DebugShapeShader", pShader.Get());
}

void HYAssetMgr::CreateDefaultMaterial()
{
	HYMaterial* pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"Std2DShader"));
	AddAsset<HYMaterial>(L"Std2DMtrl", pMtrl);

	// TestMtrl
	// pMtrl = new CMaterial(true);
	// pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	// pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"BackgroundTex", L"texture\\Background.jpg"));
	// pMtrl->SetScalarParam(INT_0, 10);
	// pMtrl->Save(L"material\\testmtrl.mtrl");
	// AddAsset<CMaterial>(L"TestMtrl", pMtrl);

	// BackgroundMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"Std2DShader"));
	AddAsset<HYMaterial>(L"BackgroundMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"TileMapShader"));
	AddAsset<HYMaterial>(L"TileMapMtrl", pMtrl);

	// ParticleMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"ParticleRenderShader"));
	AddAsset<HYMaterial>(L"ParticleMtrl", pMtrl);

	// GrayFilterMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"GrayFilterShader"));
	AddAsset<HYMaterial>(L"GrayFilterMtrl", pMtrl);

	// WaveMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"WaveFilterShader"));
	AddAsset<HYMaterial>(L"WaveFilterMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"DistortionShader"));
	AddAsset<HYMaterial>(L"DistortionMtrl", pMtrl);

	// OutLineMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"OutLineShader"));
	AddAsset<HYMaterial>(L"OutLineMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new HYMaterial(true);
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"DebugShapeShader"));
	AddAsset<HYMaterial>(L"DebugShapeMtrl", pMtrl);
}


#include "HYSetColorShader.h"
#include "HYParticleUpdate.h"
void HYAssetMgr::CreateDefaultComputeShader()
{
	Ptr<HYComputeShader> pShader = nullptr;

	// SetColorShader
	pShader = new HYSetColorShader;
	AddAsset(L"SetColorShader", pShader.Get());

	// ParticleUpdateShader
	pShader = new HYParticleUpdate;
	AddAsset(L"ParticleUpdateShader", pShader.Get());
}