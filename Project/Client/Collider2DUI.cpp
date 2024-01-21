#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/HYCollider2D.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"


Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", "##Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Collider2D");
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();
	
	// �浹ü Ÿ��
	HYGameObject* pTarget = GetTargetObject();
	
	COLLIDER2D_TYPE ColliderType= pTarget->Collider2D()->GetType();
	Vec3 vPos = (Vec3)(pTarget->Collider2D()->GetOffsetPos());
	Vec3 vScale = (Vec3)(pTarget->Collider2D()->GetOffsetScale());

	float arrRad[1] = {pTarget->Collider2D()->GetRadius()};
	bool vAbsolute = pTarget->Collider2D()->IsAbsolute();

	string collidertypename = (pTarget->Collider2D()->GetColliderTypeName(ColliderType)).c_str();

	ImGui::Text("ColliderType");
	ImGui::SameLine();
	ImGui::InputText("##ColliderTypeName", (char*)collidertypename.c_str(), collidertypename.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	// Button�� ������(���� Button�� return���� if���� �������� Ȱ��)
	if (ImGui::Button("##ColliderBtn", ImVec2(20, 20)))
	{
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecColliderTypeName;
		pTarget->Collider2D()->GetColliderTypeName(vecColliderTypeName);

		// Mesh ���͸� ������ List�� �߰�
		pListUI->AddString(vecColliderTypeName);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&Collider2DUI::ColliderTypeSelect);
		pListUI->Activate();
	}	

	if(collidertypename == "Rect")
	{
		ImGui::Text("Position	"); ImGui::SameLine();  ImGui::DragFloat2("##ColliderRelative Position", vPos);
		ImGui::Text("Scale       "); ImGui::SameLine(); ImGui::DragFloat2("##ColliderRelative Scale", vScale);
		pTarget->Collider2D()->SetOffsetPos(vPos);
		pTarget->Collider2D()->SetOffsetScale(vScale);
	}
	else if (collidertypename == "Circle")
	{
		ImGui::Text("Position	"); ImGui::SameLine();  ImGui::DragFloat2("##ColliderRelative Position", vPos);
		pTarget->Collider2D()->SetOffsetPos(vPos);
		ImGui::Text("Radius		"); ImGui::SameLine(); ImGui::DragFloat("##ColliderRadius Size", arrRad);
		pTarget->Collider2D()->SetRadius(arrRad[0]);
	}

	// ?Q Absolute �� on/off
	bool bAbsolute = pTarget->Collider2D()->IsAbsolute();
	ImGui::Text("Collider On/off"); ImGui::SameLine(); ImGui::Checkbox("##ColliderAbsolute", &bAbsolute);

	pTarget->Collider2D()->SetAbsolute(bAbsolute);
}

void Collider2DUI::ColliderTypeSelect(DWORD_PTR _ptr)
{
	string strColliderTypename= (char*)_ptr;

	COLLIDER2D_TYPE ColliderType = GetTargetObject()->Collider2D()->GetColliderType(strColliderTypename);

	GetTargetObject()->Collider2D()->SetColliderType(ColliderType);

}
