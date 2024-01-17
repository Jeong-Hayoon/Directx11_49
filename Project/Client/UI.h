#pragma once

#include "imgui.h"

class UI
{
private:
	// ������ �������� �̸�
	string			m_strName;
	// ���� ID
	string			m_strID;
	// UI ������(�⺻������ ���� �ȳ־��ְ� �Ǹ� �θ� UI�� �ִ� ����� Default)
	// BeginChild�� ���ڷ� ��
	ImVec2			m_vSize;
	// true : Ȱ��ȭ(UI �׷���) / false : ��Ȱ��ȭ(�̹� �����ӿ� �ش� UI ���� X)
	// X��ư�� ������ �Ǹ� false�� ��
	bool			m_bActive;

	// �θ� ����Ű�� ����
	UI*				m_Parent;
	vector<UI*>		m_vecChildUI;

public:
	void SetName(const string& _name) { m_strName = _name; }
	const string& GetName() { return m_strName; }
	const string& GetID() { return m_strID; }

	void SetSize(ImVec2 _Size) { m_vSize = _Size; }
	ImVec2 GetSize() { return m_vSize; }

	void AddChildUI(UI* _ChildUI)
	{
		m_vecChildUI.push_back(_ChildUI);
		_ChildUI->m_Parent = this;
	}

	UI* GetParentUI() { return m_Parent; }

	// m_bActive �����ϴ� �Լ���
	void Activate() { m_bActive = true; }
	void Deactivate() { m_bActive = false; }
	bool IsActivate() { return m_bActive; }

public:
	virtual void tick();
	
	// render�� �θ� Ŭ���� UI�ʸ� ȣ���
	virtual void render();
	// UI�� ��ӹ��� Ŭ�������� �ݵ�� render_update �Լ��� ������ �ؾ� ��
	// �θ��ʿ��� Begin/End�� ������ ������ 
	virtual void render_update() = 0;

public:
	// �⺻ �����ڸ� ������ ������ �ݵ�� ���̿� �̸��� �־���� ��
	UI(const string& _strName, const string& _strID);
	// ����� �ߴٸ� �Ҹ��ڿ� �ݵ�� virtual ���̱�
	virtual ~UI();
};


