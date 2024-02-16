#include "pch.h"
#include "HYAssetMgr.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYMaterial.h"

// 기본적으로 필요한 필수 Asset 등록
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

	// Geometry Shader 전용 Mesh
	Vtx vPoint;
	UINT Idx = 0;
	pMesh = new HYMesh;
	// 점이기 때문에 Position, Color, UV 아무것도 설정해주지 않아도 됨
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(L"PointMesh", pMesh);

	// 전역변수에 삼각형 위치 설정
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

	// 인덱스
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

	// Topology LineStrip 용도	
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	// VS를 0-1-2-3-0으로 5번 호출
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; 	arrIdx[4] = 0;

	pMesh = new HYMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh);

	// =================
	// CircleMesh 만들기
	// =================

	// 벡터로 한 이유는 원이기 때문에 배열의 개수를 특정짓기가 힘들기 때문
	// 타원 형태의 원이 나오는 이유 - 타원모양이 나오는 이유는 NDC 좌표계를 사용하기 때문에
	// 화면의 해상도 비율(종횡비)에 맞춰서 원이 찌그러지기 대문에

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// 중심 점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	// iSlice의 개수만큼 삼각형을 만듦(만들 삼각형의 개수)
	UINT iSlice = 40;
	float fTheta = 0.f;
	// 반지름
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		// XM_2PI = radian 360도
		// fTheta : 지정된 슬라이스로 자를 때 하나의 각도 크기

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

	// 기존 인덱스 초기화
	vecIdx.clear();
	// i가 1인 이유는 중심점을 제외하기 위해
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new HYMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh);

	// 또 다른 Mesh를 만들 수도 있으니까 초기화시키기
	vecVtx.clear();
	vecIdx.clear();


	// =================
	// CrossMesh 만들기
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
	// 테스트용
	// pShader->AddScalarParam(INT_0, "Test Parameter");
	// 0번 텍스처 자리에 이 Shader를 사용한다면 재질은 0번 텍스처 자리에다가 전달시킬 텍스처를 전달을 시켜줘야 함
	// 세팅된 텍스처는 출력 용도
	// pShader->AddTexParam(TEX_0, "Output Texture");		
	// Parameter	
	// pShader->AddScalarParam(VEC2_2, "Test Parameter");
	// pShader->AddScalarParam(VEC4_1, "Test Parameter");

	pShader->AddScalarParam(INT_0, "Test Param");

	pShader->AddTexParam(TEX_0, "Output Texture 1");
	//pShader->AddTexParam(TEX_1, "Output Texture 2");

	// =========================================================
	// EffectShader : 함수는 같은데 블랜드 스테이트만 다름
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
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
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
	// Mesh			: 어떤 Msch든 OK
	// RS_TYPE		: CULL_NONE(뒷면으로 돌렸을 때 안보이면 안되니까)
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
	// DebugShape Shader - 카메라 입장에서는 무시, 이 Shader를 사용하는 오브젝트는 Level 내에 존재하면 X
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