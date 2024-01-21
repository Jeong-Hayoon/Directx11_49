#pragma once
#include <Engine/singleton.h>

class UI;

// Delegate 방식 : 멤버 함수 포인터(멤버 함수를 호출시키기 때문에 객체도 인자에 포함되어야 함)
// 반환 타입 : void, 인자 : void -> UI클래스의 멤버 함수 타입 Delegate_0
typedef void(UI::* Delegate_0)();
// 반환 타입 : void, 인자 : DWORD_PTR -> UI클래스의 멤버 함수 타입 Delegate_1
typedef void(UI::* Delegate_1)(DWORD_PTR);
// 반환 타입 : void, 인자 : DWORD_PTR, DWORD_PTR -> UI클래스의 멤버 함수 타입 Delegate_2
typedef void(UI::* Delegate_2)(DWORD_PTR, DWORD_PTR);

// CallBack : 전역 함수 포인터
// DWORD_PTR 사용한 이유 : 플랫폼에 따라서 8바이트거나 4바이트기도 하니까 포인터까지 커버할 수 있는 자료형이기 때문에
// 반환 타입 : void, 인자 : void -> CALL_BACK_0
typedef void (*CALL_BACK_0)(void);
// 반환 타입 : void, 인자 : DWORD_PTR -> CALL_BACK_1
typedef void (*CALL_BACK_1)(DWORD_PTR);
// 반환 타입 : void, 인자 : DWORD_PTR, DWORD_PTR -> CALL_BACK_2
typedef void (*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

// UI 관리
class HYImGuiMgr :
    public HYSingleton<HYImGuiMgr>
{
    SINGLE(HYImGuiMgr);

private:
    map<string, UI*>        m_mapUI;

    // DemoUI on/off
    bool                    m_bDemoUI;


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



