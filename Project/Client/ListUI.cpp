#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
    : UI("", "##List")
    , m_CallBackFunc(nullptr)
{
    // ��ư�� ���� �������� Ȱ��ȭ�Ǹ� �ȵǴϱ�
    Deactivate();
    // ��� ����� true�� ����
    SetModal(true);
}

ListUI::~ListUI()
{
}

void ListUI::render_update()
{
    // �ֱٿ� �����ִ� ������ ũ��(�ִ� ������)
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;

    static int item_current_idx = 0; // Here we store our selection data as an index.

    // List�� �׸� �߰�
    // BeginListBox
    if (ImGui::BeginListBox("##ListBox", vWinSize))
    {
        for (int i = 0; i < m_vecStr.size(); i++)
        {
            const bool is_selected = (item_current_idx == i);

            // Selectable : List ���� �׸��� ����ϴ� �Լ�
            // ���� �׸��� �����ϸ� true ��ȯ
            if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
                item_current_idx = i;

            // ����Ʈ �� �ش� �׸��� Ŭ���Ǹ� ���̶���Ʈ �ɾ���
            if (is_selected)
                // ���̶���Ʈ �־��ִ� �Լ�
                ImGui::SetItemDefaultFocus();

            // ���콺�� �ش� �׸� ���� �����鼭 ����Ŭ���� �߻��Ѵٸ�            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                // ���õ� �׸��� ������ ���ư��� �ʵ��� ��Ƴ���
                m_strDBClicked = m_vecStr[i];

                // ��ϵ� CallBack �� ������ ȣ��
                if (nullptr != m_CallBackFunc)
                {
                    // m_strDBClicked�� �����ϰ� �ִ� ���ڿ� ���� �ּҰ��� �ݹ����� ��(Ŭ���� ��ư�� ���ڿ�)
                    m_CallBackFunc((DWORD_PTR)m_strDBClicked.c_str());
                }

                // ��ϵ� Delegate �� ������ ȣ��
                if (m_pUI && m_Func)
                {
                    // ���ڷ� ���� ��ü�� ���� ��� �Լ� ȣ��(��� �Լ��� �ּұ� ������ ȣ���� �Ϸ��� m_Func �տ� �������� �����ϴ� *�ʿ�)
                    // (DWORD_PTR)m_strDBClicked.c_str() : ���ڿ��� �ּ�
                    (m_pUI->*m_Func)((DWORD_PTR)m_strDBClicked.c_str());
                }

                // ���� Ŭ���� �����ϱ� â�� ����
                Deactivate();
            }

        }
        ImGui::EndListBox();
    }
}

// â�� �ݴ� ��ư�� ������ Deactivate ȣ���
void ListUI::Deactivate()
{
    UI::Deactivate();
    // ��Ȱ��ȭ�Ǿ��� �� ������ Clear����� ���� ���� ������ ���ο� �͵��� �Է¹޾Ƽ� ������ �� ����
    m_vecStr.clear();
    // ��� UI�� ��Ŀ���� ���������� ��Ŀ�� �����ִ� �Լ�
    ImGui::SetWindowFocus(nullptr);
}

