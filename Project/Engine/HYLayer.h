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

public:
    // Ư�� ������Ʈ�� ���̾�� ����
    void DetachGameObject(HYGameObject* _Object);
    // ���� ������Ʈ ���
    void RegisterGameObject(HYGameObject* _Object) { m_vecObjects.push_back(_Object); }
    int GetLayerIdx() { return m_iLayerIdx; }

    // �θ� ��Ƴ��� ���� ��ȯ���ִ� �Լ�
    const vector<HYGameObject*>& GetParentObjects() { return  m_vecParent; }
    // ���̾ ��� �ִ� ��� ������Ʈ ���͸� ��ȯ���ִ� �Լ�
    const vector<HYGameObject*>& GetLayerObjects() { return m_vecObjects; }

private:
    // Layer������ private, Level���� public(��� friend ����)
    // _bMove�� true�� �ڽ� ������Ʈ ���� -> �θ�� ���� ���̾�� ����
    // _bMove�� false�� �ڽ� ������Ʈ ����, �Ҽ� X -> �θ�� ���� ���̾�� ����
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

