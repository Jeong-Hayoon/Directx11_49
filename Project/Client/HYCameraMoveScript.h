#pragma once

#include <Engine/HYScript.h>

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

    virtual void SaveToFile(FILE* _File) override {};
    virtual void LoadFromFile(FILE* _File) override {};

    CLONE(HYCameraMoveScript);

public:
    HYCameraMoveScript();
    ~HYCameraMoveScript();
};


