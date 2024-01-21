#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
    : UI("", "##List")
    , m_CallBackFunc(nullptr)
{
    // 버튼을 누를 때까지는 활성화되면 안되니까
    Deactivate();
    // 모달 방식을 true로 세팅
    SetModal(true);
}

ListUI::~ListUI()
{
}

void ListUI::render_update()
{
    // 최근에 열려있는 윈도우 크기(최대 사이즈)
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;

    static int item_current_idx = 0; // Here we store our selection data as an index.

    // List에 항목 추가
    // BeginListBox
    if (ImGui::BeginListBox("##ListBox", vWinSize))
    {
        for (int i = 0; i < m_vecStr.size(); i++)
        {
            const bool is_selected = (item_current_idx == i);

            // Selectable : List 안의 항목을 출력하는 함수
            // 안의 항목을 선택하면 true 반환
            if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
                item_current_idx = i;

            // 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
            if (is_selected)
                // 하이라이트 넣어주는 함수
                ImGui::SetItemDefaultFocus();

            // 마우스가 해당 항목 위에 있으면서 더블클릭이 발생한다면            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                // 선택된 항목의 정보가 날아가지 않도록 담아놓기
                m_strDBClicked = m_vecStr[i];

                // 등록된 CallBack 이 있으면 호출
                if (nullptr != m_CallBackFunc)
                {
                    // m_strDBClicked가 관리하고 있는 문자열 시작 주소값을 콜백으로 줌(클릭된 버튼의 문자열)
                    m_CallBackFunc((DWORD_PTR)m_strDBClicked.c_str());
                }

                // 등록된 Delegate 가 있으면 호출
                if (m_pUI && m_Func)
                {
                    // 인자로 들어온 객체를 통해 멤버 함수 호출(멤버 함수가 주소기 때문에 호출을 하려면 m_Func 앞에 원본값에 접근하는 *필요)
                    // (DWORD_PTR)m_strDBClicked.c_str() : 문자열의 주소
                    (m_pUI->*m_Func)((DWORD_PTR)m_strDBClicked.c_str());
                }

                // 더블 클릭을 했으니까 창을 종료
                Deactivate();
            }

        }
        ImGui::EndListBox();
    }
}

// 창을 닫는 버튼을 누르면 Deactivate 호출됨
void ListUI::Deactivate()
{
    UI::Deactivate();
    // 비활성화되었을 때 정보를 Clear해줘야 새로 켜질 때에는 새로운 것들을 입력받아서 보여줄 수 있음
    m_vecStr.clear();
    // 어떠한 UI도 포커스를 못가져가게 포커스 날려주는 함수
    ImGui::SetWindowFocus(nullptr);
}

