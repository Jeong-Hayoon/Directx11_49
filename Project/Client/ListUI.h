#pragma once
#include "UI.h"

// ��ư�� ������ �� ���� ���ϴ� Ư�� �׸��� ������� �� �ִ� UI
class ListUI :
    public UI
{
private:
    // ListUI�� ������ ��� ����
    vector<string>      m_vecStr;
    // ListUI�� ��Ȱ��ȭ�Ǵ��� �޸𸮰� ���ư��� �ʵ����ϴ� �߰� ���޿� ��ü
    string              m_strDBClicked;
    // ���� Ŭ�� �߻� �� ȣ������ �Լ� �ּҸ� �ݹ� 1�� Ÿ�Կ� ��Ƴ���
    CALL_BACK_1         m_CallBackFunc;

    // Deligate �Լ� �����ͷ� ȣ���ų ��� �Լ��� �� ��� �Լ��� ȣ���ų ��ü
    UI*                 m_pUI;
    Delegate_1          m_Func;

public:
    // �׸��� ����ϴ� �Լ�
    // �� ���� ���
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    // �غ�Ǿ� �ִ� ���Ͱ� ���� ��� ���� ������ ���
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    // �Լ� ������ ��Ͻ����ִ� �Լ�(CallBack, Deligate)
    void SetDbClickCallBack(CALL_BACK_1 _CallBack) { m_CallBackFunc = _CallBack; }
    void SetDbClickDelegate(UI* _Inst, Delegate_1 _MemFunc)
    {
        m_pUI = _Inst;
        m_Func = _MemFunc;
    }

    // ���� Ŭ���� �׸��� �������� �˷��ִ� �Լ�
    string GetDoubleClicked() { return m_strDBClicked; }


public:
    virtual void render_update() override;
    virtual void Deactivate() override;


public:
    ListUI();
    ~ListUI();
};


