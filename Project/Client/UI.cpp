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
	// 부모는 ImGuiMgr가 delete
	Delete_Vec(m_vecChildUI);
}

// UI가 수행해야 하는 기능
// 비활성화 상태면 return할지 백그라운드 동작이 필요한지에 따라
// UI마다 override해서 사용
void UI::tick()
{

}

// Begin과 End 사이에 그려져야 할 내용을 채워넣는 부분
void UI::render()
{
	// 비활성화 상태라면 return
	if (!m_bActive)
		return;

	// 본인이 최상위 부모일 때만 Begin, End 가능
	if (nullptr == GetParentUI())
	{
		ImGui::Begin(string(m_strName + m_strID).c_str(), &m_bActive);

		render_update();

		// 자식 UI가 있다면
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::End();
	}

	else
	{
		// EndChile를 만날 때까지 해당 영역
		ImGui::BeginChild(string(m_strName + m_strID).c_str(), m_vSize);

		render_update();

		// 자식 UI가 있다면
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}