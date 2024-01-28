#pragma once
#include "HYRenderComponent.h"

#include "HYParticleUpdate.h"

class HYStructuredBuffer;

class HYParticleSystem :
    public HYRenderComponent
{

private:
    HYStructuredBuffer*     m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tParticleModule         m_Module;
    HYStructuredBuffer*     m_ParticleModuleBuffer;

    HYStructuredBuffer*     m_SpawnCountBuffer;

    Ptr<HYParticleUpdate>   m_CSParticleUpdate;

    float                   m_Time;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    HYParticleSystem();
    ~HYParticleSystem();
};

