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
	// 파티컬 업데이트 컴퓨트 쉐이더

}

void HYParticleSystem::render()
{
	// View, Proj 행렬 전달
	Transform()->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);

	// 모든 파티클 렌더링
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		GetMaterial()->SetScalarParam(INT_0, i);
		GetMaterial()->UpdateData();
		GetMesh()->render();
	}

	// 파티클 개별 랜더링 -> 인스턴싱
}

void HYParticleSystem::UpdateData()
{
}