#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
private:
    bool			m_bActive;


    virtual void render_update() override;
public:
    Animator2DUI();
    ~Animator2DUI();
};

