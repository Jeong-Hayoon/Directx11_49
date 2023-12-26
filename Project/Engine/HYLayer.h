#pragma once
#include "HYEntity.h"

class HYGameObject;

// Layer�� GameObject ����
class HYLayer :
    public HYEntity
{
private:
    
    // �ֻ��� �θ� ������Ʈ ����
    vector<HYGameObject*>    m_vecParent;
    vector<HYGameObject*>    m_vecObjects;

    int                      m_iLayerIdx;


private:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    // Ư�� ������Ʈ�� ���̾�� ����
    void DetachGameObject(HYGameObject* _Object);
    // ���� ������Ʈ ���
    void RegisterGameObject(HYGameObject* _Object) { m_vecObjects.push_back(_Object); }



public:
    HYLayer();
    ~HYLayer();


    // Layer������ private, Level���� public(��� friend ����)
private:
    // _bMove�� true�� �ڽ� ������Ʈ ���� -> �θ�� ���� ���̾�� ����
    // _bMove�� false�� �ڽ� ������Ʈ ����, �Ҽ� X -> �θ�� ���� ���̾�� ����
    void AddObject(HYGameObject* _Object, bool _bMove);
    friend class HYLevel;
};

