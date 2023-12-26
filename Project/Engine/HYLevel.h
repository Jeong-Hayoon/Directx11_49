#pragma once
#include "HYEntity.h"

class HYLayer;
class HYGameObject;

// 자식 오브젝트를 제외하고 부모 오브젝트들만 관리
// 부모 오브젝트를 호출시키면 부모의 자식 오브젝트를 호출시키면서
// 부모보다 먼저 호출되는 자식 오브젝트가 없도록 계층 구조 설계
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
	void AddObject(HYGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
	HYLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }


private:
	void clear();

public:
	HYLevel();
	~HYLevel();

	friend class HYLevelMgr;
};