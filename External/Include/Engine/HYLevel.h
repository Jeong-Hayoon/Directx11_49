#pragma once
#include "HYEntity.h"

class HYLayer;
class HYGameObject;

// �ڽ� ������Ʈ�� �����ϰ� �θ� ������Ʈ�鸸 ����
// �θ� ������Ʈ�� ȣ���Ű�� �θ��� �ڽ� ������Ʈ�� ȣ���Ű�鼭
// �θ𺸴� ���� ȣ��Ǵ� �ڽ� ������Ʈ�� ������ ���� ���� ����
class HYLevel :
	public HYEntity
{
private:
	// Layer�� �迭�� ����(Level�� ���� Layer ����)
	HYLayer* m_arrLayer[LAYER_MAX];

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

private:
	void clear();

public:
	HYLevel();
	~HYLevel();

	friend class HYLevelMgr;
};