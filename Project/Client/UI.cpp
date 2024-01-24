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

	bool Active = m_bActive;

	// 본인이 최상위 부모일 때만 Begin, End 가능 - Inspector / Outliner / Content
	if (nullptr == GetParentUI())
	{
		// Modalless
		if (!m_bModal)
		{
			// Animation2D 일때만 플래그
			if (m_strID == "##AnimationEditor")
				ImGui::Begin(string(m_strName + m_strID).c_str(), &Active, ImGuiWindowFlags_MenuBar);
			else

			ImGui::Begin(string(m_strName + m_strID).c_str(), &Active);

			// 활성화, 비활성화 전환이 발생한 경우에는 Activate or Deactivate 를 호출시킨다.
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

				// EndPopup은 반드시 BeginPopupModal if문 안에서 호출되어야 함
				ImGui::EndPopup();
			}

			// 모달 방식의 창을 닫기 버튼 눌렀을 때
			else
			{
				// 활성화, 비활성화 전환이 발생한 경우에는 Activate or Deactivate 를 호출시킨다.
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

	// Child UI 인 경우(자식 UI인 경우 Modal 개념 자체가 없음, 별도의 창인 경우에만 의미가 있기 때문)
	else
	{
		// EndChile를 만날 때까지 해당 영역
		ImGui::BeginChild(string(m_strName + m_strID).c_str(), m_vSize);

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}