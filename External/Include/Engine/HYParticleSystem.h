#pragma once
#include "HYRenderComponent.h"

class HYStructuredBuffer;

class HYParticleSystem :
    public HYRenderComponent
{

private:
    HYStructuredBuffer* m_ParticleBuffer;
    UINT                m_MaxParticleCount;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    HYParticleSystem();
    ~HYParticleSystem();
};

