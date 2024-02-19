#pragma once
#include "HYEntity.h"

class HYLayer;
class HYGameObject;

// ������ ����
enum class LEVEL_STATE
{
	PLAY,		// tick, DT ����, Main Camera Object�� Player�� ���� �ٴ�
	PAUSE,
	STOP,		// �������� �� ó�� ����, tick ���� X, DT == 0 -> �ʱ� ���·� ���ư� ��
};


// �ڽ� ������Ʈ�� �����ϰ� �θ� ������Ʈ�鸸 ����
// �θ� ������Ʈ�� ȣ���Ű�� �θ��� �ڽ� ������Ʈ�� ȣ���Ű�鼭
// �θ𺸴� ���� ȣ��Ǵ� �ڽ� ������Ʈ�� ������ ���� ���� ����
class HYLevel :
	public HYEntity
{
private:
	// Layer�� �迭�� ����(Level�� ���� Layer ����)
	HYLayer* m_arrLayer[LAYER_MAX];

	// UI ����
	vector<wstring>         m_ObjectName;

public:
	void begin();
	void tick();
	void finaltick();

	// Level�ȿ� Layer�� 32���� �����ϱ� Level�� Object�� ��������
	// �� �� ���̾ �־�� ���� �˾ƾ� ��
	void AddObject(HYGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
	// �̸����ε� ���� �� �ֵ��� ����
	void AddObject(HYGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

	// ����� ������ Idx�ε� ã�� �� �ְ� �̸����ε� ã�� �� �ֵ��� ����
	HYLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	// �̸��� �־��ָ� ã�Ƴ�����
	HYLayer* GetLayer(const wstring& _strLayerName);

	// Level �ȿ��� Ư�� �̸��� ���� ��ü�� �ҷ����� �Լ�(�������ϰ� ���� �� �ֱ� ������ �������� ���)
	HYGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(const wstring& _strName, vector<HYGameObject*>& _vecObj);

	// UI ����
	const vector<wstring> GetObjectName() { return m_ObjectName; }

private:
	void clear();


	CLONE(HYLevel);

public:
	HYLevel();
	HYLevel(const HYLevel& _OriginLevel);
	~HYLevel();

	friend class HYLevelMgr;
};