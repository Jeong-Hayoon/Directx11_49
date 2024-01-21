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

// �Ӹ���, ���м�
void ComponentUI::render_update()
{
	// ���м�
	ImGui::Separator(); 

	ImGui::PushID(0);
	// ��ư�� ������ ��, Ȱ��ȭ�Ǿ��� ��, �⺻ ������ ��ġ���Ѽ� ��ư�̶�⺸�ٴ� �������� ����
	ImGui::PushStyleColor(ImGuiCol_Separator, (ImVec4)ImColor::HSV(0.8f, 1.f, 0.8f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, (ImVec4)ImColor::HSV(0.8f, 1.f, 0.8f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, (ImVec4)ImColor::HSV(0.8f, 1.f, 0.8f, 0.2f));

	// Component Name
	ImGui::SeparatorText(m_ComponentTitle.c_str());

	// ��ϵǾ��� ����� ID�� ����
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void ComponentUI::SetTargetObject(HYGameObject* _Target)
{
	m_TargetObject = _Target;

	// Ÿ�� ������Ʈ�� ������ ComponentUI ��Ȱ��ȭ �Ѵ�.
	if (nullptr == m_TargetObject)
	{
		Deactivate();
	}

	else
	{
		// TargetObject�� ComponentUI ��� Component�� �����ϰ� ���� �ʴٸ� ��Ȱ��ȭ
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