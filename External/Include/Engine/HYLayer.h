#pragma once
#include "HYEntity.h"

class HYGameObject;

class HYLayer :
    public HYEntity
{
private:
    // Layer�� GameObject ����
    vector<HYGameObject*>    m_vecParent;


private:
    void begin();
    void tick();
    void finaltick();
    void render();


public:
    HYLayer();
    ~HYLayer();


    // Layer������ private, Level���� public(��� friend ����)
private:
    void AddObject(HYGameObject* _Object) { m_vecParent.push_back(_Object); }
    friend class HYLevel;
};

