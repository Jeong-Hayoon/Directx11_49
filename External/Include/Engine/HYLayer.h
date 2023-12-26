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
    void render();

public:
    // 특정 오브젝트를 레이어에서 제거
    void DetachGameObject(HYGameObject* _Object);
    // 게임 오브젝트 등록
    void RegisterGameObject(HYGameObject* _Object) { m_vecObjects.push_back(_Object); }



public:
    HYLayer();
    ~HYLayer();


    // Layer에서는 private, Level에서 public(대신 friend 지정)
private:
    // _bMove가 true면 자식 오브젝트 존재 -> 부모와 같은 레이어로 세팅
    // _bMove가 false면 자식 오브젝트 존재, 소속 X -> 부모와 같은 레이어로 세팅
    void AddObject(HYGameObject* _Object, bool _bMove);
    friend class HYLevel;
};

