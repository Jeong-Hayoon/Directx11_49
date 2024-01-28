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
	// ���� �޽��� ���� ���� ���
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"ParticleMtrl"));

	// ������ �ػ�
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// �ӽ÷� 5���� ��ƼŬ�� �ʱ� �����͸� �Է��ϸ鼭 ����ȭ ���� ����
	tParticle arrParticle[5] = { };
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		arrParticle[i].vWorldPos = Vec3((vResol.x / -2.f) + (i + 1) * vResol.x / (m_MaxParticleCount + 1), 0.f, 200.f);
		arrParticle[i].vWorldScale = Vec3(50.f, 50.f, 1.f);
		arrParticle[i].Active = 1;
	}

	// ��ƼŬ�� �����ϴ� ����ȭ ����
	m_ParticleBuffer = new HYStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true, arrParticle);

	// ��ƼŬ ��������� �����ϴ� ����ȭ����
	m_ParticleModuleBuffer = new HYStructuredBuffer;
	// sysmemmove�� ���� �������� �ʾƵ� �Ǵµ� �ϴ� �� �۵��ϴ��� üũ�Ϸ��� true ���߿� ������
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
	                                                                                                                                    
	// ��ƼŬ ������Ʈ�� ��ǻƮ ���̴� ����
	m_CSParticleUpdate = (HYParticleUpdate*)HYAssetMgr::GetInst()->FindAsset<HYComputeShader>(L"ParticleUpdateShader").Get();

	// SpawnCount ���޿� ����ȭ����
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

// ��ƼŬ ���ۿ� ����ִ� �Ÿ� Unordered Access �������Ϳ� ���ε� �ɰ�
// ��ǻ�� ���̴� Execute ���� -> �����尡 �پ ��ƼŬ �ϳ��ϳ� ������Ʈ��
void HYParticleSystem::finaltick()
{
	// ��ƼŬ ��Ⱚ ����
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


	// ��ƼŬ ������� ������Ʈ
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	// ��Ƽ�� ������Ʈ ��ǻƮ ���̴�(������Ʈ�� ���� ���۰� �������� ���)
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);

	m_CSParticleUpdate->Execute();

	tParticle arrParticle[100] = { };
	m_ParticleBuffer->GetData(arrParticle);

}

// ������Ʈ�� ��ƼŬ�� Ư�� �ؽ�Ʈ ������Ʈ�� ������ 
// ��ƼŬ ���� ������ ���̴����� �̾Ƽ� �� ��ƼŬ ��ġ���ٰ� ��ƼŬ ������
void HYParticleSystem::render()
{
	// View, Proj ��� ����(������ ���� ��ġ�� ������ �ʿ䰡 ����, ��ƼŬ�� �׷����� ��ġ�� �ƴϱ⿡)
	Transform()->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);

	// ��� ��ƼŬ ������
	// ��ƼŬ ���� ������ -> �ν��Ͻ�

	// ��ƼŬ �߿��� ���� �������Ǵ��� ������ �־���
	GetMaterial()->SetScalarParam(INT_0, 0);
	GetMaterial()->UpdateData();
	// �ν��Ͻ��� ���� �ѹ��� ������
	GetMesh()->render_asparticle(m_MaxParticleCount);

	// �������� ����� ���ҽ� ���ε� Clear
	m_ParticleBuffer->Clear(20);
}

void HYParticleSystem::UpdateData()
{
}