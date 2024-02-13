#include "pch.h"
#include "CameraUI.h"

#include <Engine/HYCamera.h>
#include <Engine/HYTransform.h>
#include <Engine/HYTimeMgr.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

CameraUI::CameraUI()
	: ComponentUI("Camera", "##Camera", COMPONENT_TYPE::CAMERA)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Camera");
}

CameraUI::~CameraUI()
{
}

void CameraUI::render_update()
{
	ComponentUI::render_update();

	HYGameObject* pTarget = GetTargetObject();

	PROJ_TYPE ProjType = pTarget->Camera()->GetProjType();
	float vFov= pTarget->Camera()->GetFOV();		// Perspective
	float vScale = pTarget->Camera()->GetScale();	// OrthoGraphic
	float vFar = pTarget->Camera()->GetFar();		

	float arrFov[1] = { vFov };
	float arrScale[1] = { vScale };
	float arrFar[1] = { vFar };

	string cameratypename = (pTarget->Camera()->GetCameraTypeName(ProjType)).c_str();

	ImGui::Text("CameraType");
	ImGui::SameLine();
	ImGui::InputText("##CameraTypeName", (char*)cameratypename.c_str(), cameratypename.length(), ImGuiInputTextFlags_ReadOnly);
	
	// Drop 체크(InputText에 Drop이 되었는지 Check하는 함수)
	//if (ImGui::BeginDragDropTarget())
	//{
	//	// ContentTree로부터 날라온 Payload를 받음
	//	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

	//	if (payload)
	//	{
	//		// 데이터 꺼냄
	//		DWORD_PTR data = *((DWORD_PTR*)payload->Data);
	//		HYAsset* pAsset = (HYAsset*)data;
	//		if (ASSET_TYPE::MESH == pAsset->GetType())
	//		{
	//			pTarget->Camera()->GetCameraTypeName();
	//		}
	//	}
	//	ImGui::EndDragDropTarget();
	//}

	ImGui::SameLine();
	if (ImGui::Button("##CameraBtn", ImVec2(20, 20)))
	{
		ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecCameraTypeName;
		pTarget->Camera()->GetCameraTypeName(vecCameraTypeName);

		// 벡터를 보여줄 List에 추가
		pListUI->AddString(vecCameraTypeName);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&CameraUI::ProjTypeSelect);
		pListUI->Activate();
	}

	ImGui::Text("Far       "); ImGui::SameLine(); ImGui::DragFloat("##Camera Far", arrFar, 1.f, 0.f, 10000.f);
	pTarget->Camera()->SetFar(arrFar[0]);

	if (cameratypename == "OrthoGraphic")
	{
		// ?Q음수 안됨
		pTarget->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		ImGui::Text("Scale     "); ImGui::SameLine(); ImGui::DragFloat("##Camera Scale", arrScale, 0.2f, 0.f, 20.f);
		pTarget->Camera()->SetScale(arrScale[0]);
	}

	else if (cameratypename == "Perspective")
	{
		// Q 1~3 사이 값밖에 안됨
		ImGui::Text("FOV     "); ImGui::SameLine(); ImGui::DragFloat("##Camera Fov", arrFov, 0.5f, 1.f, 3.f);
		pTarget->Camera()->SetFOV(arrFov[0] );
	}

	/*ImGui::Text("Position	"); ImGui::SameLine();  ImGui::DragFloat2("##ColliderRelative Position", vPos);
	ImGui::Text("Scale		"); ImGui::SameLine(); ImGui::DragFloat2("##ColliderRelative Scale", vScale);
	pTarget->Camera()->SetOffsetPos(vPos);
	pTarget->Camera()->SetOffsetScale(vScale);*/

}
void CameraUI::ProjTypeSelect(DWORD_PTR _ptr)
{
	string strProTypename = (char*)_ptr;

	PROJ_TYPE ProjType = GetTargetObject()->Camera()->GetCameraType(strProTypename);

	GetTargetObject()->Camera()->SetProjType(ProjType);

}