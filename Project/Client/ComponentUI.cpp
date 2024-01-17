#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type)
	: UI(_strName, _ID)
	, m_TargetObject(nullptr)
	, m_Type(_Type)
{
}

ComponentUI::~ComponentUI()
{
}

// 머릿말, 구분선
void ComponentUI::render_update()
{
	// 구분선
	ImGui::Separator(); 

	ImGui::PushID(0);
	// 버튼이 눌렸을 때, 활성화되었을 때, 기본 색상을 일치시켜서 버튼이라기보다는 도형같은 느낌
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

	// Component Name
	ImGui::Button(m_ComponentTitle.c_str());

	// 등록되었던 색상과 ID를 없앰
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void ComponentUI::SetTargetObject(HYGameObject* _Target)
{
	m_TargetObject = _Target;

	// 타겟 오브젝트가 없으면 ComponentUI 비활성화 한다.
	if (nullptr == m_TargetObject)
	{
		Deactivate();
	}

	else
	{
		// TargetObject가 ComponentUI 담당 Component를 보유하고 있지 않다면 비활성화
		if (!m_TargetObject->GetComponent(m_Type))
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
	}
}