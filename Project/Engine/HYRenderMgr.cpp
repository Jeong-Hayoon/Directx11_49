#include "pch.h"
#include "HYRenderMgr.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"

#include "HYStructuredBuffer.h"

#include "HYDevice.h"
#include "HYAssetMgr.h"
#include "HYTimeMgr.h"
#include "components.h"


HYRenderMgr::HYRenderMgr()
	: m_pDebugObj(nullptr)
	, m_DebugPosition(true)
	, m_Light2DBuffer(nullptr)

{
}

HYRenderMgr::~HYRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void HYRenderMgr::tick()
{
	Vec4 vClearColor = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	HYDevice::GetInst()->ClearRenderTarget(vClearColor);

	// 구조화 버퍼로 옮기고 특정 레지스터에서 리소스 바인딩하여 보냄
	UpdateData();

	render();
	render_debug();

	// 매 프레임마다 등록될거니까 clear시켜줘야 함
	Clear();
}

void HYRenderMgr::render()
{
	// OM(Output Merge State) 에 RenderTargetTexture 와 DepthStencilTexture 를 전달한다.
	// 랜더타겟 텍스처와 깊이 텍스처의 해상도는 동일해야 함
	// 렌더타겟 및 깊이 타겟 설정
	Ptr<HYTexture> pRTTex = HYAssetMgr::GetInst()->FindAsset<HYTexture>(L"RenderTargetTex");
	Ptr<HYTexture> pDSTex = HYAssetMgr::GetInst()->FindAsset<HYTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
}

void HYRenderMgr::render_debug()
{
	// 이 작업을 해줘야 Main Camera의 View 행렬, Projection 행렬이 적용될 수 있음
	g_Transform.matView = m_vecCam[0]->GetViewMat();
	g_Transform.matProj = m_vecCam[0]->GetProjMat();

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end();)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CrossMesh"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"SphereMesh"));
			break;
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"DebugShapeMtrl"));
		// vColor인데 VEC4_0에 받을 수 있는 이유는 Vec3를 인자로 받는 생성자가 존재하기 때문
		m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// Cross Shape를 사용했을 경우에는 우선 Topology를 저장해놨다가 사용 이후에 더사 되돌려주기
		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			// Cross일 때만 Topology를 LINELIST로 함 
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();

		// 랜더링 후에 원래 Topology로 되돌리기
		m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);


		// 실행 시간이 유지 시간을 넘어서면 Erase
		(*iter).fLifeTime += DT;
		if ((*iter).fDuration <= (*iter).fLifeTime)
		{
			// erase를 하게 되면 해당 iterator가 삭제되고 다음번 iterator를 반환해주니까
			// iter를 받아놓고 따로 ++은 안해줘야 함
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void HYRenderMgr::UpdateData()
{
	g_global.g_Light2DCount = (int)m_vecLight2D.size();
	//g_global.g_Light3DCount = (int)m_vecLight3D.size();

	// 전역 데이터 업데이트
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
	pCB->SetData(&g_global);
	pCB->UpdateData();

	// 2D 광원 정보 업데이트
	// 임시로 사용하는 벡터
	static vector<tLightInfo> vecLight2DInfo;

	// 반복문을 돌면서 광원의 info 정보를 받음
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
		vecLight2DInfo.push_back(info);
	}

	// 취합한 광원 정보들을 
	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	}
	m_Light2DBuffer->UpdateData(11);

	vecLight2DInfo.clear();

	// 3D 광원정보 업데이트
}

void HYRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void HYRenderMgr::RegisterCamera(HYCamera* _Cam, int _Idx)
{
	// 사이즈가 Idx보다 작거나 같다면
	if (m_vecCam.size() <= _Idx + 1)
	{
		// resize
		m_vecCam.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}