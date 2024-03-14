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
            // 원하는 경로를 지정하면 최종 경로가 배열에 담겨서 나옴
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";         // 모든 확장자 표시
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정(COntent 폴더의 Level 폴더가 바로 보이게끔)
            wstring strInitPath = HYPathMgr::GetContentPath();  
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // 경로가 나온 해당 배열에 Level Save 진행
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

            // 탐색창 초기 위치 지정
            wstring strInitPath = HYPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                HYLevel* pLevel = HYLevelSaveLoad::LoadLevel(HYPathMgr::GetRelativePath(szSelect));
                HYLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);

                // Inspector 의 타겟정보를 nullptr 로 되돌리기
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

        // 현재 Level의 상태 가져오기
        LEVEL_STATE State = pCurLevel->GetState();

        // 버튼의 활성화 여부
        bool PlayEnable = false;
        bool PauseEnable = false;
        bool StopEnable = false;

        // 정지 or 일시정지 상태였다면 -> 재생 버튼 활성화
        if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
            PlayEnable = true;
        else
            PlayEnable = false;

        // 재생 상태였다면 -> 일시정지 버튼 활성화(일시정지는 재생 도중에만 가능)
        if (LEVEL_STATE::PLAY == State)
            PauseEnable = true;
        else
            PauseEnable = false;

        // 재생 or 일시정지 상태였다면 -> 정지 버튼 활성화
        if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
            StopEnable = true;
        else
            StopEnable = false;

        if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
        {
            // Stop 상태였다가 Play 버튼을 누르면 Play 되기 직전에 Level 저장
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
            // 처음 상태로 돌아갈 수 있도록
            HYLevel* pLoadedLevel = HYLevelSaveLoad::LoadLevel(L"Level//temp.lv");
            HYLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector 의 타겟정보를 nullptr 로 되돌리기
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
        // GameObject 생성하는 기능
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
        // 재질 생성하는 기능
        if (ImGui::MenuItem("Create Empty Material"))
        {
            wchar_t szPath[255] = {};
            wstring FilePath = HYPathMgr::GetContentPath();

            int num = 0;
            while (true)
            {
                // 새로 생성하는 재질의 이름이 겹치지 않도록 숫자를 부여하여 이름 설정
                swprintf_s(szPath, L"Material//New Material_%d.mtrl", num);
                
                // 파일 있는지 Check
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