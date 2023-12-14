#include "pch.h"
#include "Test.h"

#include "HYDevice.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
#include "HYPathMgr.h"

#include "HYGameObject.h"

#include "HYMesh.h"
#include "HYGraphicsShader.h"

#include "HYTransform.h"
#include "HYMeshRender.h"


// 이동량, 사이즈 담아줄 구조체
//tTransform g_Transform = { Vec4(0.f, 0.f, 0.f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f) };

vector<HYGameObject*>	g_vecObj;

HYMesh* g_RectMesh = nullptr;
HYMesh* g_CircleMesh = nullptr;

HYGraphicsShader* g_Shader = nullptr;

// 상수 데이터를 전달하는 버퍼(16byte 단위로 정렬을 시켜줘야 함, 16byte 안되면 패딩 필요)
//ComPtr<ID3D11Buffer>	g_CB = nullptr;


int TestInit()
{
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

	g_RectMesh = new HYMesh;
	g_RectMesh->Create(arrVtx, 4, arrIdx, 6);

	// =================
	// CircleMesh 만들기 -> 삼각형이 무수히 많으면 곡선처럼 보이기 때문에 삼각형을 통해 만듦
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

	g_CircleMesh = new HYMesh;
	g_CircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());

	// Shader 생성
	g_Shader = new HYGraphicsShader;
	g_Shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	g_Shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");


	// GameObject 생성
	HYGameObject* pObj = nullptr;

	pObj = new HYGameObject;

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-0.5f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

	pObj->MeshRender()->SetMesh(g_RectMesh);
	pObj->MeshRender()->SetShader(g_Shader);

	g_vecObj.push_back(pObj);


	pObj = new HYGameObject;

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.5f, 0.25f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));

	pObj->MeshRender()->SetMesh(g_RectMesh);
	pObj->MeshRender()->SetShader(g_Shader);

	g_vecObj.push_back(pObj);

	// 이렇게 할 필요가 없어짐
	//CMeshRender* pMeshRender = (CMeshRender*)g_Object->GetComponent(COMPONENT_TYPE::MESHRENDER);
	//pMeshRender->SetMesh(g_RectMesh);
	//pMeshRender->SetShader(g_Shader);

	// 상수 버퍼(Constant Buffer) 생성
	D3D11_BUFFER_DESC BufferDesc = {};

	BufferDesc.ByteWidth = sizeof(tTransform);
	BufferDesc.StructureByteStride = sizeof(tTransform);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 버퍼에 데이터 쓰기 가능(계속해서 데이터를 수정할 수 있어야 하므로)
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;	

	return S_OK;
}


void Tick()
{
	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->tick();
		g_vecObj[i]->finaltick();
	}
}


void Render()
{
	
	// 0~255 <-> 0~1 Normalize 
	// 배경색 초기화
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	HYDevice::GetInst()->ClearRenderTarget(ClearColor);

	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->render();
	}

	HYDevice::GetInst()->Present();
}

void TestProgress()
{
	Tick();

	Render();
	
}

void TestRelease()
{
	if (nullptr != g_RectMesh)
	{
		delete g_RectMesh;
	}

	if (nullptr != g_CircleMesh)
	{
		delete g_CircleMesh;
	}

	delete g_Shader;

	Delete_Vec(g_vecObj);
}
