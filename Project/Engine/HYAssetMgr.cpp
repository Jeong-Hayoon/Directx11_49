#include "pch.h"
#include "HYAssetMgr.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYMaterial.h"

HYAssetMgr::HYAssetMgr()
{

}

HYAssetMgr::~HYAssetMgr()
{
}

// �⺻������ �ʿ��� �ʼ� Asset ���
void HYAssetMgr::init()
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

	AddAsset(L"Std2DShader", pShader);

	// =========================================================
	// EffectShader : �Լ��� ������ ������ ������Ʈ�� �ٸ�
	// =========================================================
	pShader = new HYGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ONE_ONE);

	AddAsset(L"EffectShader", pShader);

	// Std2DMtrl
	HYMaterial* pMtrl = nullptr;
	pMtrl = new HYMaterial;
	pMtrl->SetShader(FindAsset<HYGraphicsShader>(L"Std2DShader"));
	AddAsset<HYMaterial>(L"Std2DMtrl", pMtrl);
}