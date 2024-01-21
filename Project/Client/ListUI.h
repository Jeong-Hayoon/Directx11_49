#pragma once
#include "UI.h"

// 버튼을 눌렀을 때 내가 원하는 특정 항목을 출력해줄 수 있는 UI
class ListUI :
    public UI
{
private:
    // ListUI가 보여줄 목록 관리
    vector<string>      m_vecStr;
    // ListUI가 비활성화되더라도 메모리가 날아가지 않도록하는 중간 전달용 객체
    string              m_strDBClicked;
    // 더블 클릭 발생 시 호출해줄 함수 주소를 콜백 1번 타입에 담아놓음
    CALL_BACK_1         m_CallBackFunc;

    // Deligate 함수 포인터로 호출시킬 멤버 함수와 그 멤버 함수를 호출시킬 객체
    UI*                 m_pUI;
    Delegate_1          m_Func;

public:
    // 항목을 등록하는 함수
    // 한 개씩 등록
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    // 준비되어 있는 벡터가 있을 경우 벡터 단위로 등록
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    // 함수 포인터 등록시켜주는 함수(CallBack, Deligate)
    void SetDbClickCallBack(CALL_BACK_1 _CallBack) { m_CallBackFunc = _CallBack; }
    void SetDbClickDelegate(UI* _Inst, Delegate_1 _MemFunc)
    {
        m_pUI = _Inst;
        m_Func = _MemFunc;
    }

    // 더블 클릭된 항목이 무엇인지 알려주는 함수
    string GetDoubleClicked() { return m_strDBClicked; }


public:
    virtual void render_update() override;
    virtual void Deactivate() override;


public:
    ListUI();
    ~ListUI();
};


