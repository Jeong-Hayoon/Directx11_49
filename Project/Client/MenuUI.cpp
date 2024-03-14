#include "pch.h"
#include "MenuUI.h"

#include <Engine/HYTaskMgr.h>
#include <Engine/HYPathMgr.h>

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>

#include <Engine/HYGameObject.h>
#include <Engine/components.h>

#include <Scripts/HYScriptMgr.h>
#include <Engine/HYScript.h>

#include "HYImGuiMgr.h"
#include "Inspector.h"

#include "HYLevelSaveLoad.h"

MenuUI::MenuUI()
    : UI("Menu", "##Menu")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        render_update();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    File();

    Level();

    GameObject();

    Asset();
}


void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save Level", ""))
        {
            // ���ϴ� ��θ� �����ϸ� ���� ��ΰ� �迭�� ��ܼ� ����
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";         // ��� Ȯ���� ǥ��
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // Ž��â �ʱ� ��ġ ����(COntent ������ Level ������ �ٷ� ���̰Բ�)
            wstring strInitPath = HYPathMgr::GetContentPath();  
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // ��ΰ� ���� �ش� �迭�� Level Save ����
            if (GetSaveFileName(&ofn))
            {
                HYLevelSaveLoad::SaveLevel(HYLevelMgr::GetInst()->GetCurrentLevel(), HYPathMgr::GetRelativePath(szSelect));
            }
        }

        if (ImGui::MenuItem("Load Level", ""))
        {
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // Ž��â �ʱ� ��ġ ����
            wstring strInitPath = HYPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                HYLevel* pLevel = HYLevelSaveLoad::LoadLevel(HYPathMgr::GetRelativePath(szSelect));
                HYLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);

                // Inspector �� Ÿ�������� nullptr �� �ǵ�����
                Inspector* pInspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");
                pInspector->SetTargetObject(nullptr);
            }
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();

        // ���� Level�� ���� ��������
        LEVEL_STATE State = pCurLevel->GetState();

        // ��ư�� Ȱ��ȭ ����
        bool PlayEnable = false;
        bool PauseEnable = false;
        bool StopEnable = false;

        // ���� or �Ͻ����� ���¿��ٸ� -> ��� ��ư Ȱ��ȭ
        if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
            PlayEnable = true;
        else
            PlayEnable = false;

        // ��� ���¿��ٸ� -> �Ͻ����� ��ư Ȱ��ȭ(�Ͻ������� ��� ���߿��� ����)
        if (LEVEL_STATE::PLAY == State)
            PauseEnable = true;
        else
            PauseEnable = false;

        // ��� or �Ͻ����� ���¿��ٸ� -> ���� ��ư Ȱ��ȭ
        if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
            StopEnable = true;
        else
            StopEnable = false;

        if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
        {
            // Stop ���¿��ٰ� Play ��ư�� ������ Play �Ǳ� ������ Level ����
            if (LEVEL_STATE::STOP == pCurLevel->GetState())
            {
                HYLevelSaveLoad::SaveLevel(pCurLevel, L"Level//temp.lv");
            }

            HYLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
        {
            HYLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
        {
            // ó�� ���·� ���ư� �� �ֵ���
            HYLevel* pLoadedLevel = HYLevelSaveLoad::LoadLevel(L"Level//temp.lv");
            HYLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector �� Ÿ�������� nullptr �� �ǵ�����
            Inspector* pInspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        // GameObject �����ϴ� ���
        if (ImGui::MenuItem("Create Empty Object", ""))
        {
            HYGameObject* pNewObj = new HYGameObject;
            pNewObj->SetName(L"New GameObject");
            pNewObj->AddComponent(new HYTransform);
            GamePlayStatic::SpawnGameObject(pNewObj, 0);
        }
        ImGui::Separator();

        if (ImGui::BeginMenu("Component", ""))
        {
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Script", ""))
        {
            vector<wstring> vecScriptName;
            HYScriptMgr::GetScriptInfo(vecScriptName);

            for (size_t i = 0; i < vecScriptName.size(); ++i)
            {
                if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
                {
                    Inspector* inspector = (Inspector*)HYImGuiMgr::GetInst()->FindUI("##Inspector");
                    if (nullptr != inspector->GetTargetObject())
                    {
                        inspector->GetTargetObject()->AddComponent(HYScriptMgr::GetScript(vecScriptName[i]));
                    }
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        // ���� �����ϴ� ���
        if (ImGui::MenuItem("Create Empty Material"))
        {
            wchar_t szPath[255] = {};
            wstring FilePath = HYPathMgr::GetContentPath();

            int num = 0;
            while (true)
            {
                // ���� �����ϴ� ������ �̸��� ��ġ�� �ʵ��� ���ڸ� �ο��Ͽ� �̸� ����
                swprintf_s(szPath, L"Material//New Material_%d.mtrl", num);
                
                // ���� �ִ��� Check
                if (!exists(FilePath + szPath))
                    break;
                ++num;
            }

            HYMaterial* pMtrl = new HYMaterial;
            pMtrl->SetName(szPath);
            pMtrl->Save(szPath);
            GamePlayStatic::AddAsset(pMtrl);
        }

        ImGui::EndMenu();
    }
}