#pragma once
#include "singleton.h"

class HYCamera;
class HYGameObject;

// 앞으로는 LevelMgr가 아니라 RenderMgr가 랜더링 기능 담당 -> 카메라를 가져와서 랜더링
// Debug Object Rendering Manager
class HYRenderMgr :
    public HYSingleton<HYRenderMgr>
{
    SINGLE(HYRenderMgr);
private:
    // 우선순위에 따라 벡터에 순차적으로 나열
    vector<HYCamera*>        m_vecCam;

    // 유지 시간이 어떻게 될지 모르니까 list
    list<tDebugShapeInfo>   m_DbgShapeInfo;

    // 디버그용 물체
    HYGameObject* m_pDebugObj;


public:
    // 카메라를 등록시키는 함수, 0번 Camera가 Main(Sub Camera도 사용은 함)
    void RegisterCamera(HYCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_DbgShapeInfo.push_back(_info);
    }
public:
    void init();
    // tick에서 Render 호출
    void tick();

private:
    void render();
    void render_debug();
};

