#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/HYMeshRender.h>

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", "##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(ImVec2(0.f, 100.f));
	SetComopnentTitle("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	HYGameObject* pTarget = GetTargetObject();
	HYMeshRender* pMeshRender = pTarget->MeshRender();

	Ptr<HYMesh> pMesh = pMeshRender->GetMesh();
	Ptr<HYMaterial> pMtrl = pMeshRender->GetMaterial();

	string meshname = ToString(pMesh->GetKey()).c_str();
	string mtrlname = ToString(pMtrl->GetKey()).c_str();

	// 무슨 Mesh를 참조하고 있는지 
	ImGui::Text("Mesh    ");
	// Seperator : 구분선
	// SameLine : 다음 줄로 안가고 우측으로 붙음
	ImGui::SameLine();
	// InputText : 입력받아서 수정할 수 있음, ImGuiInputTextFlags_ReadOnly값 넣으면 수정 불가능
	// 원래 InputText는 배열에 받아서 사용해야 하는데 애초에 내용을 수정할 용도로 사용하지 않을거라 상관 X
	// ImGuiInputTextFlags : 타입 재정의
	// ImGuiInputTextFlags_ : enum
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);

	// 무슨 Material를 참조하고 있는지 
	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);

}