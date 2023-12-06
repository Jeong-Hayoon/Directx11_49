#pragma once

// 최상위 부모 클래스
class HYEntity
{
private:
	static UINT g_NextID;

private:
	const UINT	m_ID;
	wstring		m_strName;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_ID; }

public:
	HYEntity();
	~HYEntity();
};