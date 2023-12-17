#pragma once
#include "HYEntity.h"

class HYGameObject;

class HYLayer :
    public HYEntity
{
private:
    // Layer가 GameObject 관리
    vector<HYGameObject*>    m_vecParent;


private:
    void begin();
    void tick();
    void finaltick();
    void render();


public:
    HYLayer();
    ~HYLayer();


    // Layer에서는 private, Level에서 public(대신 friend 지정)
private:
    void AddObject(HYGameObject* _Object) { m_vecParent.push_back(_Object); }
    friend class HYLevel;
};

