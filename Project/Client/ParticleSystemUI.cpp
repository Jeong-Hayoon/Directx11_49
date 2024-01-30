#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/HYParticleSystem.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetSize(ImVec2(0.f, 200.f));
	SetComopnentTitle("Particle System");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();

	HYGameObject* pTarget = GetTargetObject();

	HYParticleSystem* pParticle = pTarget->ParticleSystem();

	Vec4 vColor = pParticle->GetSpawnColor();

	ImGui::Text("Color");
	ImGui::SameLine();
	//ImGui::InputText("##Color", (char*)lighttypename.c_str(), lighttypename.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	ImGui::ColorPicker4("Spawncolor", vColor);
	pParticle->SetSpawnColor(vColor);

}
