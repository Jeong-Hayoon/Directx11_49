#include "pch.h"
#include "HYParticleUpdate.h"


HYParticleUpdate::HYParticleUpdate()
	: HYComputeShader(1024, 1, 1)
{
	Create(L"shader\\particle_update.fx", "CS_ParticleUpdate");
}

HYParticleUpdate::~HYParticleUpdate()
{
}

int HYParticleUpdate::UpdateData()
{
	if (nullptr == m_ParticleBuffer)
		return E_FAIL;

	// ��ƼŬ�� �ִ� ���� ������Ʈ
	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_ParticleBuffer->UpdateData_CS_UAV(0);
	m_SpawnCountBuffer->UpdateData_CS_UAV(1);

	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	return S_OK;
}

void HYParticleUpdate::UpdateGroupCount()
{
	// �������� ������ +1, ������ �׳�
	UINT GroupX = (m_ParticleBuffer->GetElementCount() / m_ThreadX);
	if (0 != m_ParticleBuffer->GetElementCount() % m_ThreadX)
		GroupX += 1;

	SetGroup(GroupX, 1, 1);
}

void HYParticleUpdate::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_ParticleModuleBuffer->Clear_CS_SRV();
	m_ParticleModuleBuffer = nullptr;

	m_SpawnCountBuffer->Clear_CS_UAV();
	m_SpawnCountBuffer = nullptr;
}