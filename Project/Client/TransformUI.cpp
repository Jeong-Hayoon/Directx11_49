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
	// ���ο����� Rotation�� Radian���� ����ϱ� ������ 
	vRot.ToDegree();

	// InputFloat3(���ڿ�, float3 �迭) -> operator conversion�� ���� Vec3�ε� ����
	//  arrfloat[3] = {vPos.x, vPos.y, vPos.z}; / InputFloat3(""", arrfloat); - ������ �̷��� �ؾ� ��
	ImGui::InputFloat3("Relative Position", vPos);
	ImGui::InputFloat3("Relative Scale", vScale);
	ImGui::InputFloat3("Relative Rotation", vRot);

	vRot.ToRadian();
	m_TargetObject->Transform()->SetRelativePos(vPos);
	m_TargetObject->Transform()->SetRelativeScale(vScale);
	m_TargetObject->Transform()->SetRelativeRotation(vRot);
}
