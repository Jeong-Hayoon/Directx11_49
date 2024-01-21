#pragma once
#include <Engine/singleton.h>

class UI;

// Delegate ��� : ��� �Լ� ������(��� �Լ��� ȣ���Ű�� ������ ��ü�� ���ڿ� ���ԵǾ�� ��)
// ��ȯ Ÿ�� : void, ���� : void -> UIŬ������ ��� �Լ� Ÿ�� Delegate_0
typedef void(UI::* Delegate_0)();
// ��ȯ Ÿ�� : void, ���� : DWORD_PTR -> UIŬ������ ��� �Լ� Ÿ�� Delegate_1
typedef void(UI::* Delegate_1)(DWORD_PTR);
// ��ȯ Ÿ�� : void, ���� : DWORD_PTR, DWORD_PTR -> UIŬ������ ��� �Լ� Ÿ�� Delegate_2
typedef void(UI::* Delegate_2)(DWORD_PTR, DWORD_PTR);

// CallBack : ���� �Լ� ������
// DWORD_PTR ����� ���� : �÷����� ���� 8����Ʈ�ų� 4����Ʈ�⵵ �ϴϱ� �����ͱ��� Ŀ���� �� �ִ� �ڷ����̱� ������
// ��ȯ Ÿ�� : void, ���� : void -> CALL_BACK_0
typedef void (*CALL_BACK_0)(void);
// ��ȯ Ÿ�� : void, ���� : DWORD_PTR -> CALL_BACK_1
typedef void (*CALL_BACK_1)(DWORD_PTR);
// ��ȯ Ÿ�� : void, ���� : DWORD_PTR, DWORD_PTR -> CALL_BACK_2
typedef void (*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

// UI ����
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



