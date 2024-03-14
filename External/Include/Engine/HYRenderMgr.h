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
    vector<HYCamera*>           m_vecCam;

    HYCamera*                   m_EditorCam;

    // ��ó���� �ؽ�ó -> ���ҽ��� �ε��� �ʿ䰡 ���� => �޸� �� �ؽ�ó ����
    Ptr<HYTexture>              m_PostProcessTex;

    // ����ȭ ����
    HYStructuredBuffer*         m_Light2DBuffer;

    // ���� 2D ������ ��Ƴ��� ����
    vector<HYLight2D*>          m_vecLight2D;

    // ���� �ð��� ��� ���� �𸣴ϱ� list
    list<tDebugShapeInfo>       m_DbgShapeInfo;

    // ����׿� ��ü
    HYGameObject*               m_pDebugObj;

    // ĳ������ ��ġ �� ǥ�� ����
    bool                        m_DebugPosition;

    // NoiseTexture
    vector<Ptr<HYTexture>>      m_vecNoiseTex;

    // render function pointer(render_play or render_editor)
    typedef void(HYRenderMgr::* RENDER_FUNC)(void);
    RENDER_FUNC             m_RenderFunc;


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
    
    // ���� ���
    void RegisterLight2D(HYLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }

    void CopyRenderTargetToPostProcessTarget();
    Ptr<HYTexture> GetPostProcessTex() { return m_PostProcessTex; }

    // ������ ī�޶� ���
    void RegisterEditorCamera(HYCamera* _Cam) { m_EditorCam = _Cam; }

    void ClearCamera() { m_vecCam.clear(); }

    // ������ ��� ����
    void ActiveEditorMode(bool _bActive)
    {
        if (_bActive)
            m_RenderFunc = &HYRenderMgr::render_editor;
        else
            m_RenderFunc = &HYRenderMgr::render_play;
    }

public:
    void init();
    // tick���� Render ȣ��
    void tick();

private:
    void render_play();
    void render_editor();

    void render_debug();

    // ���ҽ� ���ε�
    void UpdateData();

    // ���ҽ� Ŭ����
    void Clear();
};

