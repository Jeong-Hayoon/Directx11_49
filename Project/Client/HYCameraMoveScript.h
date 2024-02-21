#pragma once

#include <Engine/HYScript.h>

class HYCameraMoveScript :
    public HYScript
{
private:
    // ī�޶��� �̵� �ӵ�
    float       m_CamSpeed;

public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

    CLONE(HYCameraMoveScript);

public:
    HYCameraMoveScript();
    ~HYCameraMoveScript();
};

