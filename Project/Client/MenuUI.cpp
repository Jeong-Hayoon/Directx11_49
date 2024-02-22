#include "pch.h"
#include "MenuUI.h"

#include <Engine/HYTaskMgr.h>
                 
#include <Engine/HYGameObject.h>
#include <Engine/components.h>

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
            HYMaterial* pMtrl = new HYMaterial;
            pMtrl->SetName(L"Material//New Material.mtrl");
            pMtrl->Save(L"Material//New Material.mtrl");
            GamePlayStatic::AddAsset(pMtrl);
        }

        ImGui::EndMenu();
    }
}