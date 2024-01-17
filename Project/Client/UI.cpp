#include "pch.h"
#include "UI.h"


UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
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

	// ������ �ֻ��� �θ��� ���� Begin, End ����
	if (nullptr == GetParentUI())
	{
		ImGui::Begin(string(m_strName + m_strID).c_str(), &m_bActive);

		render_update();

		// �ڽ� UI�� �ִٸ�
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::End();
	}

	else
	{
		// EndChile�� ���� ������ �ش� ����
		ImGui::BeginChild(string(m_strName + m_strID).c_str(), m_vSize);

		render_update();

		// �ڽ� UI�� �ִٸ�
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}