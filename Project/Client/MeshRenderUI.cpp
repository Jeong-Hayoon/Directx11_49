#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/HYAssetMgr.h>
#include <Engine/HYMeshRender.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"


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
	ImGui::SameLine();
	// Button�� ������(���� Button�� return���� if���� �������� Ȱ��)
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		// ����Ʈ UI�� ������
		// �θ� �����ͷ� ������ ������ �� �����ϰ� �Ϸ��� dynamic cast�� Ȯ�����ִ� �۾� �ʿ�
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		// vecMeshName�� Asset Ÿ���� Mesh�� Asset�� ã�� �����
		HYAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		// Mesh ���͸� ������ List�� �߰�
		pListUI->AddString(vecMeshName);
		// ���� Ŭ���ȴٸ� �� �Լ��� ȣ���ϵ��� �Լ� ������ ����
		//pListUI->SetDbClickCallBack(MeshSelect);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MeshSelect);
		pListUI->Activate();
	}

	// ���� Material�� �����ϰ� �ִ��� 
	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
	{
		// ����Ʈ UI�� �����ͼ�
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMtrlName;
		HYAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

		pListUI->AddString(vecMtrlName);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MaterialSelect);
		pListUI->Activate();
	}
}

void MeshRenderUI::MeshSelect(DWORD_PTR _ptr)
{
	// ���� Ŭ���ߴ��� �� �� �־���
	string strMesh = (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	// �������� Mesh
	Ptr<HYMesh> pMesh = HYAssetMgr::GetInst()->FindAsset<HYMesh>(strMeshName);

	GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::MaterialSelect(DWORD_PTR _ptr)
{
	string strMtrl = (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<HYMaterial> pMtrl = HYAssetMgr::GetInst()->FindAsset<HYMaterial>(strMtrlName);

	GetTargetObject()->MeshRender()->SetMaterial(pMtrl);
}
