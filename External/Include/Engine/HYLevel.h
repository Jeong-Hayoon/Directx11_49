#pragma once
#include "HYEntity.h"

class HYLayer;
class HYGameObject;


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
	void render();

	// Level�ȿ� Layer�� 32���� �����ϱ� Level�� Object�� ��������
	// �� �� ���̾ �־�� ���� �˾ƾ� ��
	void AddObject(HYGameObject* _Object, int _LayerIdx);

public:
	HYLevel();
	~HYLevel();
};