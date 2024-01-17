#pragma once

#include "imgui.h"

class UI
{
private:
	// 겉으로 보여지는 이름
	string			m_strName;
	// 고유 ID
	string			m_strID;
	// UI 사이즈(기본적으로 값을 안넣어주게 되면 부모 UI의 최대 사이즈가 Default)
	// BeginChild의 인자로 들어감
	ImVec2			m_vSize;
	// true : 활성화(UI 그려짐) / false : 비활성화(이번 프레임에 해당 UI 존재 X)
	// X버튼을 누르게 되면 false가 됨
	bool			m_bActive;

	// 부모를 가리키는 변수
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

	// m_bActive 제어하는 함수들
	void Activate() { m_bActive = true; }
	void Deactivate() { m_bActive = false; }
	bool IsActivate() { return m_bActive; }

public:
	virtual void tick();
	
	// render는 부모 클래인 UI쪽만 호출됨
	virtual void render();
	// UI를 상속받은 클래스들은 반드시 render_update 함수를 재정의 해야 함
	// 부모쪽에서 Begin/End가 들어오기 때문에 
	virtual void render_update() = 0;

public:
	// 기본 생성자를 지웠기 때문에 반드시 아이와 이름을 넣어줘야 함
	UI(const string& _strName, const string& _strID);
	// 상속을 했다면 소멸자에 반드시 virtual 붙이기
	virtual ~UI();
};


