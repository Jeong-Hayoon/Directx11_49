#include "pch.h"
#include "UI.h"


UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
	, m_bModal(false)
{
}

UI::~UI()
{
	// �θ�� ImGuiMgr�� delete
	Delete_Vec(m_vecChildUI);
}

// UI�� �����ؾ� �ϴ� ���
// ��Ȱ��ȭ ���¸� return���� ��׶��� ������ �ʿ������� ����
// UI���� override�ؼ� ���
void UI::tick()
{

}

// Begin�� End ���̿� �׷����� �� ������ ä���ִ� �κ�
void UI::render()
{	
	// ��Ȱ��ȭ ���¶�� return
	if (!m_bActive)
		return;

	bool Active = m_bActive;

	// ������ �ֻ��� �θ��� ���� Begin, End ���� - Inspector / Outliner / Content
	if (nullptr == GetParentUI())
	{
		// Modalless
		if (!m_bModal)
		{
			// Animation2D �϶��� �÷���
			if (m_strID == "##AnimationEditor")
				ImGui::Begin(string(m_strName + m_strID).c_str(), &Active, ImGuiWindowFlags_MenuBar);
			else

			ImGui::Begin(string(m_strName + m_strID).c_str(), &Active);

			// Ȱ��ȭ, ��Ȱ��ȭ ��ȯ�� �߻��� ��쿡�� Activate or Deactivate �� ȣ���Ų��.
			if (Active != m_bActive)
			{
				m_bActive = Active;

				if (m_bActive)
					Activate();
				else
					Deactivate();
			}

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(string(m_strName + m_strID).c_str());
			if (ImGui::BeginPopupModal(string(m_strName + m_strID).c_str(), &Active))
			{
				render_update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->render();
				}

				// EndPopup�� �ݵ�� BeginPopupModal if�� �ȿ��� ȣ��Ǿ�� ��
				ImGui::EndPopup();
			}

			// ��� ����� â�� �ݱ� ��ư ������ ��
			else
			{
				// Ȱ��ȭ, ��Ȱ��ȭ ��ȯ�� �߻��� ��쿡�� Activate or Deactivate �� ȣ���Ų��.
				if (Active != m_bActive)
				{
					m_bActive = Active;

					if (m_bActive)
						Activate();
					else
						Deactivate();
				}
			}
		}		
	}

	// Child UI �� ���(�ڽ� UI�� ��� Modal ���� ��ü�� ����, ������ â�� ��쿡�� �ǹ̰� �ֱ� ����)
	else
	{
		// EndChile�� ���� ������ �ش� ����
		ImGui::BeginChild(string(m_strName + m_strID).c_str(), m_vSize);

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}