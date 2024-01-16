#pragma once
#include <Engine/singleton.h>

#include "UI.h"

// UI °ü¸®
class HYImGuiMgr :
    public HYSingleton<HYImGuiMgr>
{
    SINGLE(HYImGuiMgr);

private:
    map<string, UI*> m_mapUI;

public:
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr <ID3D11DeviceContext> _Context);
    void progress();

private:
    void tick();
    void render();

    void create_ui();

public:
    UI* FindUI(const string& _strUIName);
    void AddUI(const string& _strKey, UI* _UI);

};



