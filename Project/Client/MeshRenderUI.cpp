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

	// ���� Mesh�� �����ϰ� �ִ��� 
	ImGui::Text("Mesh    ");
	// Seperator : ���м�
	// SameLine : ���� �ٷ� �Ȱ��� �������� ����
	ImGui::SameLine();
	// InputText : �Է¹޾Ƽ� ������ �� ����, ImGuiInputTextFlags_ReadOnly�� ������ ���� �Ұ���
	// ���� InputText�� �迭�� �޾Ƽ� ����ؾ� �ϴµ� ���ʿ� ������ ������ �뵵�� ������� �����Ŷ� ��� X
	// ImGuiInputTextFlags : Ÿ�� ������
	// ImGuiInputTextFlags_ : enum
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);

	// ���� Material�� �����ϰ� �ִ��� 
	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);

}