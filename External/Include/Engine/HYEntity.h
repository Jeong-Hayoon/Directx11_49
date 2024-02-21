#pragma once

// �ֻ��� �θ� Ŭ����
// ��� �� virtual�� �� �ٿ� ������ 
// �θ� Ÿ�� �����ͷ� �����ϸ鼭 delete�� �ϰ� �Ǹ�
// ���� �ڽ� Ŭ������ �Ҹ��ڱ��� �� ����
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

	// ���� �������� ������ �ִ� �Լ�
	virtual HYEntity* Clone() = 0;

public:
	HYEntity();
	HYEntity(const HYEntity& _Origin);
	virtual ~HYEntity();
};