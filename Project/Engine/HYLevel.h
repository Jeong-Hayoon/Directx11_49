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

	// UI 관련
	vector<wstring>         m_ObjectName;

public:
	void begin();
	void tick();
	void finaltick();

	// Level안에 Layer가 32개가 있으니까 Level에 Object를 넣으려면
	// 몇 번 레이어에 넣어야 할지 알아야 함
	void AddObject(HYGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
	// 이름으로도 넣을 수 있도록 구현
	void AddObject(HYGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

	// 기능은 같지만 Idx로도 찾을 수 있고 이름으로도 찾을 수 있도록 구현
	HYLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	// 이름을 넣어주면 찾아내도록
	HYLayer* GetLayer(const wstring& _strLayerName);

	// Level 안에서 특정 이름을 가진 물체를 불러오는 함수(성능저하가 생길 수 있기 때문에 툴에서만 사용)
	HYGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(const wstring& _strName, vector<HYGameObject*>& _vecObj);

	// UI 관련
	const vector<wstring> GetObjectName() { return m_ObjectName; }

private:
	void clear();

public:
	HYLevel();
	~HYLevel();

	friend class HYLevelMgr;
};