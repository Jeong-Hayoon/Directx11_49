#pragma once
#include "singleton.h"

#include "HYTexture.h"

class HYCamera;
class HYGameObject;
class HYLight2D;

class HYStructuredBuffer;

// �����δ� LevelMgr�� �ƴ϶� RenderMgr�� ������ ��� ��� -> ī�޶� �����ͼ� ������
// Debug Object Rendering Manager
class HYRenderMgr :
    public HYSingleton<HYRenderMgr>
{
    SINGLE(HYRenderMgr);
private:
    // �켱������ ���� ���Ϳ� ���������� ����
    vector<HYCamera*>       m_vecCam;

    // ��ó���� �ؽ�ó -> ���ҽ��� �ε��� �ʿ䰡 ���� => �޸� �� �ؽ�ó ����
    Ptr<HYTexture>           m_PostProcessTex;

    // ����ȭ ����
    HYStructuredBuffer*     m_Light2DBuffer;
    // ���� 2D ������ ��Ƴ��� ����
    vector<HYLight2D*>       m_vecLight2D;


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
    
    // ������ ��Ͻ�Ű�� �Լ� 
    void RegisterLight2D(HYLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }


public:
    void init();
    // tick���� Render ȣ��
    void tick();

private:
    void render();
    void render_debug();

    // ���ҽ� ���ε�
    void UpdateData();

    // ���ҽ� Ŭ����
    void Clear();
};

