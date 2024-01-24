#include "pch.h"
#include "HYParticleSystem.h"

#include "HYDevice.h"
#include "HYStructuredBuffer.h"

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
	}

	m_ParticleBuffer = new HYStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), 5, SB_TYPE::READ_WRITE, true, arrParticle);
}

HYParticleSystem::~HYParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}


void HYParticleSystem::finaltick()
{
	// ��Ƽ�� ������Ʈ ��ǻƮ ���̴�

}

void HYParticleSystem::render()
{
	// View, Proj ��� ����
	Transform()->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);

	// ��� ��ƼŬ ������
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		GetMaterial()->SetScalarParam(INT_0, i);
		GetMaterial()->UpdateData();
		GetMesh()->render();
	}

	// ��ƼŬ ���� ������ -> �ν��Ͻ�
}

void HYParticleSystem::UpdateData()
{
}