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
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

	// Component Name
	ImGui::Button(m_ComponentTitle.c_str());

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