#include "pch.h"
#include "HYImGuiMgr.h"

#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYGameObject.h>
#include <Engine/HYKeyMgr.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Inspector.h"
#include "Content.h"
#include "Outliner.h"
#include "MenuUI.h"
#include "ListUI.h"

#include "ParamUI.h"

#include "AnimationEditor.h"
#include "TilemapEditor.h"

HYImGuiMgr::HYImGuiMgr()
    : m_bDemoUI(true)
    , m_bCameraOn(false)
{

}

HYImGuiMgr::~HYImGuiMgr()
{
    // ImGui Clear       
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // UI 
    Delete_Map(m_mapUI);
}

void HYImGuiMgr::init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device
    , ComPtr<ID3D11DeviceContext> _Context)
{
    // ImGui 초기화
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hMainWnd);
    ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    create_ui();

    HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
    HYGameObject* pObject = pCurLevel->FindObjectByName(L"Player");
    // Inspector UI한테 Target Object를 알려줌(이 Target에 대한 정보를 띄워라)
    // FindUI는 Inspector에만 있는 기능이기 때문에 다운 캐스팅
    // == 해당 UI 기능으로 타겟을 지정
    ((Inspector*)FindUI("##Inspector"))->SetTargetObject(pObject);

}

void HYImGuiMgr::progress()
{
    tick();

    render();
}

void HYImGuiMgr::tick()
{
    // 프레임 시작, 정보 초기화
    // Begin(인자로는 창 이름 키값)과 End는 둘이 짝
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (m_bDemoUI)
    {
        // Demo UI - 빌드한 후 UI를 보면서 카테고리 키워드를 ShowDemoWindow에 가서 검색하여 활용
        ImGui::ShowDemoWindow(&m_bDemoUI);
    }

    for (const auto& pair : m_mapUI)
    {
        // Inspector, Outliner, Content 돌고 그 다음 각각의 자식 UI들도 tick
        pair.second->tick();
    }

    ParamUI::ResetID();

    if (KEY_TAP(KEY::K) && !m_bCameraOn)
    {
        m_bCameraOn = true;
        HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
        HYGameObject* pObject = pCurLevel->FindObjectByName(L"MainCamera");
        ((Inspector*)FindUI("##Inspector"))->SetTargetObject(pObject);
    }
    if (KEY_TAP(KEY::L))
    {
        m_bCameraOn = false;
        HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
        HYGameObject* pObject = pCurLevel->FindObjectByName(L"Player");
        ((Inspector*)FindUI("##Inspector"))->SetTargetObject(pObject);
    }

}

void HYImGuiMgr::render()
{
    // 부모쪽 render 호출
    for (const auto& pair : m_mapUI)
    {
        pair.second->render();
    }

    // 메인 윈도우 내부에 있을 때 랜더링
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // 메인 윈도우 외부로 나갔을 때 랜더링
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

UI* HYImGuiMgr::FindUI(const string& _strUIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_strUIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}

void HYImGuiMgr::AddUI(const string& _strKey, UI* _UI)
{
    UI* pUI = FindUI(_strKey);
    assert(!pUI);
    m_mapUI.insert(make_pair(_strKey, _UI));
}


// UI를 등록시키는 함수
void HYImGuiMgr::create_ui()
{
    UI* pUI = nullptr;

    // Inspector
    pUI = new Inspector;
    AddUI(pUI->GetID(), pUI);

    // Content
    pUI = new Content;
    AddUI(pUI->GetID(), pUI);

    // Outliner
    pUI = new Outliner;
    AddUI(pUI->GetID(), pUI);

    // MenuUI
    pUI = new MenuUI;
    AddUI(pUI->GetID(), pUI);

    // List
    pUI = new ListUI;
    AddUI(pUI->GetID(), pUI);

    // Animation Editor
    pUI = new AnimationEditor;
    AddUI(pUI->GetID(), pUI);

    // Tilemao Editor
    pUI = new TilemapEditor;
    AddUI(pUI->GetID(), pUI);
}


