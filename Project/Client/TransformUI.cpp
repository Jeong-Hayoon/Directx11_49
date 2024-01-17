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
	// ���ο����� Rotation�� Radian���� ����ϱ� ������ 
	vRot.ToDegree();

	// InputFloat3(���ڿ�, float3 �迭) -> operator conversion�� ���� Vec3�ε� ���� / ���ڿ��� �������� ������ �Ǿ� ����
	// arrfloat[3] = {vPos.x, vPos.y, vPos.z}; / InputFloat3(""", arrfloat); - ������ �̷��� �ؾ� ��
	// DragFloat3 : �� ���� ����, Object ��ġ�� �巡�� �� �� ����
	ImGui::Text("Position"); ImGui::SameLine();  ImGui::DragFloat3("##Relative Position", vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat3("##Relative Scale", vScale);
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::DragFloat3("##Relative Rotation", vRot);

	vRot.ToRadian();
	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeScale(vScale);
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// Absolute �� on/off
	bool bAbsolute = GetTargetObject()->Transform()->IsAbsolute();
	// Checkbox : ������ ����
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	GetTargetObject()->Transform()->SetAbsolute(bAbsolute);
}
