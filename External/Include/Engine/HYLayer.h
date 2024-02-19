#pragma once
#include "HYEntity.h"

class HYGameObject;

// Layer가 GameObject 관리
class HYLayer :
    public HYEntity
{
private:
    // 최상위 부모 오브젝트 관리
    vector<HYGameObject*>    m_vecParent;
    vector<HYGameObject*>    m_vecObjects;
    int                      m_iLayerIdx;

private:
    void begin();
    void tick();
    void finaltick();

public:
    // 특정 오브젝트를 레이어에서 제거
    void DetachGameObject(HYGameObject* _Object);
    // 게임 오브젝트 등록
    void RegisterGameObject(HYGameObject* _Object) { m_vecObjects.push_back(_Object); }
    int GetLayerIdx() { return m_iLayerIdx; }

    // 부모만 모아놓은 벡터 반환해주는 함수
    const vector<HYGameObject*>& GetParentObjects() { return  m_vecParent; }
    // 레이어에 담겨 있는 모든 오브젝트 벡터를 반환해주는 함수
    const vector<HYGameObject*>& GetLayerObjects() { return m_vecObjects; }

private:
    // Layer에서는 private, Level에서 public(대신 friend 지정)
    // _bMove가 true면 자식 오브젝트 존재 -> 부모와 같은 레이어로 세팅
    // _bMove가 false면 자식 오브젝트 존재, 소속 X -> 부모와 같은 레이어로 세팅
    void AddObject(HYGameObject* _Object, bool _bMove);

public:
    CLONE(HYLayer);

public:
    HYLayer();
    HYLayer(const HYLayer& _OriginLayer);
    ~HYLayer();

private:

    friend class HYLevel;
};

