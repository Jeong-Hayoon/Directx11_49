#pragma once
#include "HYScript.h"
class HYCameraMoveScript :
    public HYScript
{
private:
    // 카메라의 이동 속도
    float       m_CamSpeed;

public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    HYCameraMoveScript();
    ~HYCameraMoveScript();
};

