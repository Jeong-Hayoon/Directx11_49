#pragma once
#include "HYComputeShader.h"

class HYParticleUpdate :
    public HYComputeShader
{
private:
    HYStructuredBuffer* m_ParticleBuffer;
    HYStructuredBuffer* m_ParticleModuleBuffer;
    HYStructuredBuffer* m_SpawnCountBuffer;

    Vec3                m_vParticleWorldPos;

public:
    void SetParticleBuffer(HYStructuredBuffer* _ParticleBuffer) { m_ParticleBuffer = _ParticleBuffer; }
    void SetParticleModuleBuffer(HYStructuredBuffer* _Buffer) { m_ParticleModuleBuffer = _Buffer; }
    void SetParticleSpawnCount(HYStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetParticleWorldPos(Vec3 _vWorldPos) { m_vParticleWorldPos = _vWorldPos; };

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    HYParticleUpdate();
    ~HYParticleUpdate();
};

