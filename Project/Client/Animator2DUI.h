#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
private:
    bool			m_bActive;

    virtual void render_update() override;

    void AnimationSelect(DWORD_PTR _ptr);

public:
    Animator2DUI();
    ~Animator2DUI();
};

