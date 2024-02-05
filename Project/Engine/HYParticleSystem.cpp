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
	, m_MaxParticleCount(1000)
{
	// 전용 메쉬와 전용 재질 사용
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"PointMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"ParticleMtrl"));

	// 렌더링 해상도
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// 파티클을 저장하는 구조화 버퍼 
	// sysmemmove는 원래 가져오지 않아도 되는데 일단 잘 작동하는지 체크하려고 true 나중에 꺼야함
	m_ParticleBuffer = new HYStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

	// 파티클 모듈정보를 저장하는 구조화버퍼
	m_ParticleModuleBuffer = new HYStructuredBuffer;

	// 16byte 정렬 맞춰주기
	UINT ModuleAddSize = 0;
	if (sizeof(tParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tParticleModule) % 16);
	}
	m_ParticleModuleBuffer = new HYStructuredBuffer;

	// 파티클 시스템 전체적인, 전역적인 정보기 때문에 1개만 Create 하면됨, 텍스처 레지스터에만 전달할 거니까 SB_TYPE::READ_ONLY 
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule) + ModuleAddSize, 1, SB_TYPE::READ_ONLY, true);
	                                                                                                                                    
	// 파티클 업데이트용 컴퓨트 쉐이더 참조
	m_CSParticleUpdate = (HYParticleUpdate*)HYAssetMgr::GetInst()->FindAsset<HYComputeShader>(L"ParticleUpdateShader").Get();

	// SpawnCount 전달용 구조화버퍼
	m_SpawnCountBuffer = new HYStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);

	// 초기 모듈 세팅		
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	m_Module.SpaceType = 1;
	m_Module.vSpawnColor = Vec4(0.2f, 0.4f, 0.9f, 1.f);
	m_Module.vSpawnMinScale = Vec4(100.f, 30.f, 1.f, 1.f);
	m_Module.vSpawnMaxScale = Vec4(100.f, 30.f, 1.f, 1.f);
	m_Module.MinLife = 3.f;
	m_Module.MaxLife = 5.f;
	m_Module.MinMass = 1.f;
	m_Module.MaxMass = 1.f;
	m_Module.SpawnShape = 1; // 0 : Sphere, 1 : Box
	m_Module.Radius = 100.f;
	m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);
	m_Module.SpawnRate = 50;

	// Add Velocity Module
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
	m_Module.AddVelocityType = 0;
	m_Module.MinSpeed = 100;
	m_Module.MaxSpeed = 150;
	m_Module.FixedDirection;
	m_Module.FixedAngle;

	// Scale
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
	// 크기의 변화량을 배율로 적어줌
	m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	// Noise Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	m_Module.NoiseForceScale = 50.f;
	m_Module.NoiseForceTerm = 0.3f;

	// Calculate Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

	// Render 
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;

	// 속도에 따른 방향 정렬
	m_Module.VelocityAlignment = 1; 
	m_Module.AlphaBasedLife = 1;
	m_Module.AlphaMaxAge = 2.f;

	m_ParticleTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\particle\\Sparks.png", L"texture\\particle\\Sparks.png");

	// m_ParticleTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\particle\\HardRain.png", L"texture\\particle\\HardRain.png");

	// m_ParticleTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\particle\\CartoonSmoke.png", L"texture\\particle\\CartoonSmoke.png");
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
	// Module 정보의 SpawnRate를 통해 시간이 흐름에 따라 파티클 활성화
	m_Time += DT;

	if ((1.f / m_Module.SpawnRate) < m_Time)
	{
		// 누적 시간을 스폰 간격으로 나눈 값(몫)
		float fSpawnCount = m_Time / (1.f / m_Module.SpawnRate);

		// 스폰 간격을 제외한 잔량을 남은 누적시간으로 설정(DT가 튀어서 한 프레임에 파티클을 여러 개 생성해야 하는 경우도 생길 수 있음)
		m_Time -= (1.f / m_Module.SpawnRate) * floorf(fSpawnCount);

		tSpawnCount count = tSpawnCount{ (int)fSpawnCount, 0, 0, 0 };

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
	// 파티클의 월드 위치 세팅
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());

	m_CSParticleUpdate->Execute();

}

// 업데이트한 파티클을 특정 텍스트 레지스트에 보내서 
// 파티클 전용 랜더링 쉐이더에서 뽑아서 각 파티클 위치에다가 파티클 랜더링
void HYParticleSystem::render()
{
	// View, Proj 행렬 전달(본인의 월드 위치는 전달할 필요가 없음, 파티클이 그려지는 위치가 아니기에)
	Transform()->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);
	m_ParticleModuleBuffer->UpdateData(21);

	// 모든 파티클 렌더링
	// 파티클 개별 랜더링 -> 인스턴싱

	// 파티클 중에서 누가 랜더링되는지 순번을 넣어줌
	GetMaterial()->SetScalarParam(INT_0, 0);
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->UpdateData();
	// 인스턴싱을 통해 한번에 랜더링
	GetMesh()->render_asparticle(m_MaxParticleCount);

	// 렌더링때 사용한 리소스 바인딩 Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void HYParticleSystem::UpdateData()
{

}