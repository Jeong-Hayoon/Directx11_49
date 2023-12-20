#include "pch.h"
#include "HYAssetMgr.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"

HYAssetMgr::HYAssetMgr()
{

}

HYAssetMgr::~HYAssetMgr()
{
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		for (auto pair : m_mapAsset[i])
		{
			if (nullptr != pair.second)
			{
				delete pair.second;
			}
		}
		m_mapAsset[i].clear();
	}
}

// 기본적으로 필요한 필수 Asset 등록
void HYAssetMgr::init()
{
	HYMesh* pMesh = nullptr;

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
	arrVtx[1].vUV = Vec2(0.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(0.f, 0.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 0.f);

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

	for (int i = 0; i <= iSlice; ++i)
	{
		// XM_2PI = radian 360도
		// fTheta : 지정된 슬라이스로 자를 때 하나의 각도 크기

		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (int i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new HYMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh);


	// Shader 생성
	HYGraphicsShader* pShader = nullptr;

	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	AddAsset(L"Std2DShader", pShader);
}
