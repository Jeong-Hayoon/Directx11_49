#pragma once
#include "singleton.h"

class HYCamera;
class HYGameObject;

// �����δ� LevelMgr�� �ƴ϶� RenderMgr�� ������ ��� ��� -> ī�޶� �����ͼ� ������
// Debug Object Rendering Manager
class HYRenderMgr :
    public HYSingleton<HYRenderMgr>
{
    SINGLE(HYRenderMgr);
private:
    // �켱������ ���� ���Ϳ� ���������� ����
    vector<HYCamera*>        m_vecCam;

    // ���� �ð��� ��� ���� �𸣴ϱ� list
    list<tDebugShapeInfo>   m_DbgShapeInfo;

    // ����׿� ��ü
    HYGameObject* m_pDebugObj;


public:
    // ī�޶� ��Ͻ�Ű�� �Լ�, 0�� Camera�� Main(Sub Camera�� ����� ��)
    void RegisterCamera(HYCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_DbgShapeInfo.push_back(_info);
    }
public:
    void init();
    // tick���� Render ȣ��
    void tick();

private:
    void render();
    void render_debug();
};

