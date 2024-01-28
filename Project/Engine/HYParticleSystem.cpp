#include "pch.h"
#include "HYParticleSystem.h"

#include "HYDevice.h"
#include "HYStructuredBuffer.h"

#include "HYTimeMgr.h"


#include "HYAssetMgr.h"
#include "HYMesh.h"
#include "HYMaterial.h"

HYParticleSystem::HYParticleSystem()
	: HYRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(5)
{
	// 전용 메쉬와 전용 재질 사용
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"ParticleMtrl"));

	// 렌더링 해상도
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// 임시로 5개의 파티클이 초기 데이터를 입력하면서 구조화 버퍼 생성
	tParticle arrParticle[5] = { };
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		arrParticle[i].vWorldPos = Vec3((vResol.x / -2.f) + (i + 1) * vResol.x / (m_MaxParticleCount + 1), 0.f, 200.f);
		arrParticle[i].vWorldScale = Vec3(50.f, 50.f, 1.f);
		arrParticle[i].Active = 1;
	}

	// 파티클을 저장하는 구조화 버퍼
	m_ParticleBuffer = new HYStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true, arrParticle);

	// 파티클 모듈정보를 저장하는 구조화버퍼
	m_ParticleModuleBuffer = new HYStructuredBuffer;
	// sysmemmove는 원래 가져오지 않아도 되는데 일단 잘 작동하는지 체크하려고 true 나중에 꺼야함
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
	                                                                                                                                    
	// 파티클 업데이트용 컴퓨트 쉐이더 참조
	m_CSParticleUpdate = (HYParticleUpdate*)HYAssetMgr::GetInst()->FindAsset<HYComputeShader>(L"ParticleUpdateShader").Get();

	// SpawnCount 전달용 구조화버퍼
	m_SpawnCountBuffer = new HYStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);
}

HYParticleSystem::~HYParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	if (nullptr != m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}

// 파티클 버퍼에 들어있는 거를 Unordered Access 레지스터에 바인딩 걸고
// 컴퓨터 쉐이더 Execute 실행 -> 스레드가 붙어서 파티클 하나하나 업데이트됨
void HYParticleSystem::finaltick()
{
	// 파티클 모듈값 세팅
	m_Module.SpaceType = 1;
	m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_Module.vSpawnMinScale = Vec4(20.f, 20.f, 1.f, 1.f);
	m_Module.vSpawnMaxScale = Vec4(20.f, 20.f, 1.f, 1.f);
	m_Module.MinLife = 5.f;
	m_Module.MaxLife = 5.f;
	m_Module.SpawnRate = 10;

	m_Time += DT;
	if ((1.f / m_Module.SpawnRate) < m_Time)
	{
		m_Time = 0.f;
		tSpawnCount count = tSpawnCount{ 1, 1, 1, 1 };
		m_SpawnCountBuffer->SetData(&count);
	}
	else
	{
		tSpawnCount count = tSpawnCount{ 0, 0, 0, 0 };
		m_SpawnCountBuffer->SetData(&count);
	}


	// 파티클 모듈정보 업데이트
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	// 파티컬 업데이트 컴퓨트 쉐이더(업데이트를 받을 버퍼가 누구인지 명시)
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);

	m_CSParticleUpdate->Execute();

	tParticle arrParticle[100] = { };
	m_ParticleBuffer->GetData(arrParticle);

}

// 업데이트한 파티클을 특정 텍스트 레지스트에 보내서 
// 파티클 전용 랜더링 쉐이더에서 뽑아서 각 파티클 위치에다가 파티클 랜더링
void HYParticleSystem::render()
{
	// View, Proj 행렬 전달(본인의 월드 위치는 전달할 필요가 없음, 파티클이 그려지는 위치가 아니기에)
	Transform()->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);

	// 모든 파티클 렌더링
	// 파티클 개별 랜더링 -> 인스턴싱

	// 파티클 중에서 누가 랜더링되는지 순번을 넣어줌
	GetMaterial()->SetScalarParam(INT_0, 0);
	GetMaterial()->UpdateData();
	// 인스턴싱을 통해 한번에 랜더링
	GetMesh()->render_asparticle(m_MaxParticleCount);

	// 렌더링때 사용한 리소스 바인딩 Clear
	m_ParticleBuffer->Clear(20);
}

void HYParticleSystem::UpdateData()
{
}