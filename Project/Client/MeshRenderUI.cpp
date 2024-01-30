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
	ImGui::SameLine();
	// Button을 누르면(보통 Button은 return값을 if문의 조건으로 활용)
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		// 리스트 UI를 가져옴
		// 부모 포인터로 나오기 때문에 더 안전하게 하려면 dynamic cast로 확인해주는 작업 필요
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		// vecMeshName에 Asset 타입이 Mesh인 Asset을 찾아 담아줌
		HYAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		// Mesh 벡터를 보여줄 List에 추가
		pListUI->AddString(vecMeshName);
		// 더블 클릭된다면 이 함수를 호출하도록 함수 포인터 지정
		//pListUI->SetDbClickCallBack(MeshSelect);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MeshSelect);
		pListUI->Activate();
	}

	// 무슨 Material를 참조하고 있는지 
	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
	{
		// 리스트 UI를 가져와서
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
	// 누가 클릭했는지 알 수 있어짐
	string strMesh = (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	// 세팅해줄 Mesh
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
