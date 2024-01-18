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

public:
    // �׸��� ����ϴ� �Լ�
    // �� ���� ���
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    // �غ�Ǿ� �ִ� ���Ͱ� ���� ��� ���� ������ ���
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }


    void SetDbClickCallBack(CALL_BACK_1 _CallBack)
    {
        m_CallBackFunc = _CallBack;
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


