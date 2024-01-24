
#include "pch.h"

#include "AnimationEditor.h"

#include <Engine/HYDevice.h>
#include <Engine/HYAnimator2D.h>
#include <Engine/HYPathMgr.h>
#include <Engine/HYAssetMgr.h>
#include <Engine/HYTexture.h>
#include <Engine/HYTransform.h>
#include <Engine/HYMeshRender.h>



#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"




AnimationEditor::AnimationEditor()
    : UI("AnimationEditor", "##AnimationEditor")
    ,m_bActive(false)

{
    Deactivate();
    SetSize(ImVec2(0.f, 0.f));
   // SetComopnentTitle("AnimationEditor");
}

AnimationEditor::~AnimationEditor()
{
}


// ���� �����
//void AnimationEditor::Save(const wstring& _strRelativePath)
//{
//	// open a file name
//	OPENFILENAME ofn = {};
//
//	//szFilePath - ���
//	wstring szFilePath = HYPathMgr::GetContentPath();
//	szFilePath += _strRelativePath;
//
//	// Ŀ�� ������Ʈ
//	FILE* pFile = nullptr;
//
//	// ������ ���� �Լ� - �����ϸ� pFile�� ä����
//	// w : ���� ���
//	// wb : ������ �������� ������ ������
//	_wfopen_s(&pFile, szFilePath.c_str(), L"wb");
//	assert(pFile);
//
//	// �Ȱ��� ������ � �����͸� ������ ���ΰ�
//    // fwrite(&������ ����, size of(������ ������ �ڷ���). ���� ����(�迭�ϼ��� �����ϱ�), pFile) �Լ� ���
//
//	// �޸� �Ҵ�� ���� �������ִ� �Լ�
//	fclose(pFile);
//}
//
//void AnimationEditor::Load(const wstring& _strRelativePath)
//{
//	OPENFILENAME ofn = {};
//	wchar_t szFilePath[256] = {};
//
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = NULL;
//	// ����� ���
//	ofn.lpstrFile = szFilePath;
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFilePath);
//	// Ư�� Ȯ���ڸ� ���̰� �ϴ� ����
//	ofn.lpstrFilter = L"Tile\0*.tile\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	if (false == GetSaveFileName(&ofn))
//		return;
//
//	//szFilePath - ���
//	//wstring szFilePath = HYPathMgr::GetContentPath();
//	//szFilePath += _strRelativePath;
//
//	// rb : �������� ������ ����
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, szFilePath.c_str(), L"rb");
//
//	assert(pFile);
//
//    // fread(&���� ���� ����, size of(���� ���� ������ �ڷ���). ���� ����(�迭�ϼ��� �����ϱ�), pFile) �Լ� ���
//	
//
//	// �޸� �Ҵ�� ���� �������ִ� �Լ�
//	fclose(pFile);
//}


void AnimationEditor::render_update()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Open"))
            {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

    if (ImGui::BeginTabBar("Animation", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Image Select"))
        {
            //ImGui::Image()
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Cut"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Auto Cut"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Smart Cut"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Add Animation"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


    
    //ImGuiIO& io = ImGui::GetIO();


  //   ImTextureID my_tex_id = (ImTextureID)"texture\\Background.png";

   // mGui::Image(my_tex_id, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));


}

