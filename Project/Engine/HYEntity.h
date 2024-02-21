#pragma once

// 최상위 부모 클래스
// 상속 시 virtual을 안 붙여 놓으면 
// 부모 타입 포인터로 관리하면서 delete를 하게 되면
// 실제 자식 클래스의 소멸자까지 안 들어옴
class HYEntity
{
private:
	static UINT g_NextID;

private:
	const UINT	m_ID;
	wstring		m_strName;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() const { return m_strName; }
	UINT GetID() const { return m_ID; }

	// 본인 스스로의 복제를 주는 함수
	virtual HYEntity* Clone() = 0;

public:
	HYEntity();
	HYEntity(const HYEntity& _Origin);
	virtual ~HYEntity();
};