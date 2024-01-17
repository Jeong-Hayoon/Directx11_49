#include "pch.h"
#include "TransformUI.h"

#include <Engine/HYTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", "##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Transform");
}

TransformUI::~TransformUI()
{

}


void TransformUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();
	Vec3 vScale = GetTargetObject()->Transform()->GetRelativeScale();
	Vec3 vRot = GetTargetObject()->Transform()->GetRelativeRotation();
	// 내부에서는 Rotation를 Radian으로 사용하기 때문에 
	vRot.ToDegree();

	// InputFloat3(문자열, float3 배열) -> operator conversion을 통해 Vec3로도 가능 / 문자열이 우측으로 고정이 되어 있음
	// arrfloat[3] = {vPos.x, vPos.y, vPos.z}; / InputFloat3(""", arrfloat); - 원래는 이렇게 해야 함
	// DragFloat3 : 값 수정 가능, Object 수치를 드래그 할 수 있음
	ImGui::Text("Position"); ImGui::SameLine();  ImGui::DragFloat3("##Relative Position", vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat3("##Relative Scale", vScale);
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::DragFloat3("##Relative Rotation", vRot);

	vRot.ToRadian();
	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeScale(vScale);
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// Absolute 값 on/off
	bool bAbsolute = GetTargetObject()->Transform()->IsAbsolute();
	// Checkbox : 우측에 붙음
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	GetTargetObject()->Transform()->SetAbsolute(bAbsolute);
}
