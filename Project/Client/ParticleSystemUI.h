#pragma once
#include "ComponentUI.h"

// 무슨 Mesh와 Shdaer를 참조하고 있는지
class ParticleSystemUI :
    public ComponentUI
{

public:
    virtual void render_update() override;

public:


public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

