#pragma once
#include "HYEntity.h"

class HYLayer;
class HYGameObject;


class HYLevel :
	public HYEntity
{
private:
	// Layer를 배열로 관리(Level을 통해 Layer 진입)
	HYLayer* m_arrLayer[LAYER_MAX];


public:
	void begin();
	void tick();
	void finaltick();
	void render();

	// Level안에 Layer가 32개가 있으니까 Level에 Object를 넣으려면
	// 몇 번 레이어에 넣어야 할지 알아야 함
	void AddObject(HYGameObject* _Object, int _LayerIdx);

public:
	HYLevel();
	~HYLevel();
};