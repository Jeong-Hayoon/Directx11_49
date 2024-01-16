#include "pch.h"
#include "TransformUI.h"

#include <Engine/HYTransform.h>

TransformUI::TransformUI()
	: UI("Transform", "##Transform")
	, m_TargetObject(nullptr)
{
}

TransformUI::~TransformUI()
{

}


void TransformUI::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	Vec3 vPos = m_TargetObject->Transform()->GetRelativePos();
	Vec3 vScale = m_TargetObject->Transform()->GetRelativeScale();
	Vec3 vRot = m_TargetObject->Transform()->GetRelativeRotation();
	// 내부에서는 Rotation를 Radian으로 사용하기 때문에 
	vRot.ToDegree();

	// InputFloat3(문자열, float3 배열) -> operator conversion을 통해 Vec3로도 가능
	//  arrfloat[3] = {vPos.x, vPos.y, vPos.z}; / InputFloat3(""", arrfloat); - 원래는 이렇게 해야 함
	ImGui::InputFloat3("Relative Position", vPos);
	ImGui::InputFloat3("Relative Scale", vScale);
	ImGui::InputFloat3("Relative Rotation", vRot);

	vRot.ToRadian();
	m_TargetObject->Transform()->SetRelativePos(vPos);
	m_TargetObject->Transform()->SetRelativeScale(vScale);
	m_TargetObject->Transform()->SetRelativeRotation(vRot);
}
