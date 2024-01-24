#include "pch.h"
#include "Light2DUI.h"

#include <Engine/HYLight2D.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 350.f));
	SetComopnentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::render_update()
{
	ComponentUI::render_update();

	HYGameObject* pTarget = GetTargetObject();

	LIGHT_TYPE LightType = pTarget->Light2D()->GetLightType();
	Vec4 vLightColor = pTarget->Light2D()->GetLightColor();	
	Vec4 vAmbient = pTarget->Light2D()->GetAmbient();
	float vRadius= pTarget->Light2D()->GetRadius();
	float vAngle= pTarget->Light2D()->GetAngle();

	float arrRad[1] = { vRadius };
	float arrAng[1] = { vAngle };

	string lighttypename = (pTarget->Light2D()->GetLightTypeName(LightType)).c_str();

	ImGui::Text("LightType");
	ImGui::SameLine();
	ImGui::InputText("##LightTypeName", (char*)lighttypename.c_str(), lighttypename.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##LightBtn", ImVec2(20, 20)))
	{
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecLightTypeName;
		pTarget->Light2D()->GetLightTypeName(vecLightTypeName);

		pListUI->AddString(vecLightTypeName);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&Light2DUI::LightTypeSelect);
		pListUI->Activate();
	}

	if (lighttypename == "Directional")
	{
		ImGui::ColorPicker4("Ambient", vAmbient);
		//ImGui::Text("Ambient   "); ImGui::SameLine(); ImGui::DragFloat4("##Light Ambient", vAmbient, 0.2f, 0.f, 20.f);
		pTarget->Light2D()->SetAmbient(vAmbient);
	}

	else if (lighttypename == "Point")
	{
		ImGui::ColorPicker4("Lightcolor", vLightColor);
		//ImGui::Text("Lightcolor   "); ImGui::SameLine(); ImGui::DragFloat4("##Light Colot", vLightColor);
		pTarget->Light2D()->SetLightColor(vLightColor);
		ImGui::Text("Radius     "); ImGui::SameLine(); ImGui::DragFloat("##Light Radius", arrRad);
		pTarget->Light2D()->SetRadius(arrRad[0]);
	}

	else if (lighttypename == "Spot")
	{
		ImGui::ColorPicker4("Lightcolor", vLightColor);
		//ImGui::Text("Lightcolor   "); ImGui::SameLine(); ImGui::DragFloat4("##Light Colot", vLightColor);
		pTarget->Light2D()->SetLightColor(vLightColor);
		ImGui::Text("Radius     "); ImGui::SameLine(); ImGui::DragFloat("##Light Radius", arrRad);
		pTarget->Light2D()->SetRadius(arrRad[0]);
		ImGui::Text("Angle     "); ImGui::SameLine(); ImGui::DragFloat("##Light Angle", arrAng);
		pTarget->Light2D()->SetAngle(arrAng[0]);
	}
}

void Light2DUI::LightTypeSelect(DWORD_PTR _ptr)
{
	string strLightTypename = (char*)_ptr;

	LIGHT_TYPE LightType = GetTargetObject()->Light2D()->GetLightType(strLightTypename);

	GetTargetObject()->Light2D()->SetLightType(LightType);
}