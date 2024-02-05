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
	// ���� �޽��� ���� ���� ���
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"PointMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"ParticleMtrl"));

	// ������ �ػ�
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// ��ƼŬ�� �����ϴ� ����ȭ ���� 
	// sysmemmove�� ���� �������� �ʾƵ� �Ǵµ� �ϴ� �� �۵��ϴ��� üũ�Ϸ��� true ���߿� ������
	m_ParticleBuffer = new HYStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

	// ��ƼŬ ��������� �����ϴ� ����ȭ����
	m_ParticleModuleBuffer = new HYStructuredBuffer;

	// 16byte ���� �����ֱ�
	UINT ModuleAddSize = 0;
	if (sizeof(tParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tParticleModule) % 16);
	}
	m_ParticleModuleBuffer = new HYStructuredBuffer;

	// ��ƼŬ �ý��� ��ü����, �������� ������ ������ 1���� Create �ϸ��, �ؽ�ó �������Ϳ��� ������ �Ŵϱ� SB_TYPE::READ_ONLY 
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule) + ModuleAddSize, 1, SB_TYPE::READ_ONLY, true);
	                                                                                                                                    
	// ��ƼŬ ������Ʈ�� ��ǻƮ ���̴� ����
	m_CSParticleUpdate = (HYParticleUpdate*)HYAssetMgr::GetInst()->FindAsset<HYComputeShader>(L"ParticleUpdateShader").Get();

	// SpawnCount ���޿� ����ȭ����
	m_SpawnCountBuffer = new HYStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);

	// �ʱ� ��� ����		
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
	// ũ���� ��ȭ���� ������ ������
	m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	// Noise Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	m_Module.NoiseForceScale = 50.f;
	m_Module.NoiseForceTerm = 0.3f;

	// Calculate Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

	// Render 
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;

	// �ӵ��� ���� ���� ����
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

// ��ƼŬ ���ۿ� ����ִ� �Ÿ� Unordered Access �������Ϳ� ���ε� �ɰ�
// ��ǻ�� ���̴� Execute ���� -> �����尡 �پ ��ƼŬ �ϳ��ϳ� ������Ʈ��
void HYParticleSystem::finaltick()
{
	// Module ������ SpawnRate�� ���� �ð��� �帧�� ���� ��ƼŬ Ȱ��ȭ
	m_Time += DT;

	if ((1.f / m_Module.SpawnRate) < m_Time)
	{
		// ���� �ð��� ���� �������� ���� ��(��)
		float fSpawnCount = m_Time / (1.f / m_Module.SpawnRate);

		// ���� ������ ������ �ܷ��� ���� �����ð����� ����(DT�� Ƣ� �� �����ӿ� ��ƼŬ�� ���� �� �����ؾ� �ϴ� ��쵵 ���� �� ����)
		m_Time -= (1.f / m_Module.SpawnRate) * floorf(fSpawnCount);

		tSpawnCount count = tSpawnCount{ (int)fSpawnCount, 0, 0, 0 };

		m_SpawnCountBuffer->SetData(&count);
	}
	else
	{
		tSpawnCount count = tSpawnCount{ 0, 0, 0, 0 };
		m_SpawnCountBuffer->SetData(&count);
	}


	// ��ƼŬ ������� ������Ʈ
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	// ��Ƽ�� ������Ʈ ��ǻƮ ���̴�(������Ʈ�� ���� ���۰� �������� ���)
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);
	// ��ƼŬ�� ���� ��ġ ����
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());

	m_CSParticleUpdate->Execute();

}

// ������Ʈ�� ��ƼŬ�� Ư�� �ؽ�Ʈ ������Ʈ�� ������ 
// ��ƼŬ ���� ������ ���̴����� �̾Ƽ� �� ��ƼŬ ��ġ���ٰ� ��ƼŬ ������
void HYParticleSystem::render()
{
	// View, Proj ��� ����(������ ���� ��ġ�� ������ �ʿ䰡 ����, ��ƼŬ�� �׷����� ��ġ�� �ƴϱ⿡)
	Transform()->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);
	m_ParticleModuleBuffer->UpdateData(21);

	// ��� ��ƼŬ ������
	// ��ƼŬ ���� ������ -> �ν��Ͻ�

	// ��ƼŬ �߿��� ���� �������Ǵ��� ������ �־���
	GetMaterial()->SetScalarParam(INT_0, 0);
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->UpdateData();
	// �ν��Ͻ��� ���� �ѹ��� ������
	GetMesh()->render_asparticle(m_MaxParticleCount);

	// �������� ����� ���ҽ� ���ε� Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void HYParticleSystem::UpdateData()
{

}