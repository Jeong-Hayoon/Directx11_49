#pragma once
#include "singleton.h"

class HYCamera;
class HYGameObject;
class HYStructuredBuffer;


// �����δ� LevelMgr�� �ƴ϶� RenderMgr�� ������ ��� ��� -> ī�޶� �����ͼ� ������
// Debug Object Rendering Manager
class HYRenderMgr :
    public HYSingleton<HYRenderMgr>
{
    SINGLE(HYRenderMgr);
private:
    // �켱������ ���� ���Ϳ� ���������� ����
    vector<HYCamera*>        m_vecCam;

    HYStructuredBuffer* m_Light2DBuffer;

    // ���� �ð��� ��� ���� �𸣴ϱ� list
    list<tDebugShapeInfo>   m_DbgShapeInfo;

    // ����׿� ��ü
    HYGameObject* m_pDebugObj;

    // ĳ������ ��ġ �� ǥ�� ����
    bool                    m_DebugPosition;


public:
    // ī�޶� ��Ͻ�Ű�� �Լ�, 0�� Camera�� Main(Sub Camera�� ����� ��)
    void RegisterCamera(HYCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_DbgShapeInfo.push_back(_info);
    }

    // Position�� �������Ұ��� 
    void SetDebugPosition(bool _OnOff) { m_DebugPosition = _OnOff; }
    bool IsDebugPosition() { return m_DebugPosition; }

public:
    void init();
    // tick���� Render ȣ��
    void tick();

private:
    void render();
    void render_debug();
};

