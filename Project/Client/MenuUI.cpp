#include "pch.h"
#include "MenuUI.h"

#include <Engine/HYTaskMgr.h>
#include <Engine/HYPathMgr.h>

#include <Engine/HYGameObject.h>
#include <Engine/components.h>

#include <Scripts/HYScriptMgr.h>
#include <Engine/HYScript.h>

#include "HYImGuiMgr.h"
#include "Inspector.h"

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

        }

        if (ImGui::MenuItem("Load Level", ""))
        {

        }

        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Play"))
        {

        }

        if (ImGui::MenuItem("Pause"))
        {

        }

        if (ImGui::MenuItem("Stop"))
        {

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