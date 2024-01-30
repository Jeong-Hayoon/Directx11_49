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

    Ptr<HYTexture>           m_ParticleTex;

    float                   m_Time;

    
public:
    // UI 관련 모듈 세팅 함수들
    Vec4 GetSpawnColor() { return m_Module.vSpawnColor; }
    Vec4 GetSpawnMinScale() { return m_Module.vSpawnMinScale; }
    Vec4 GetSpawnMaxScale() { return m_Module.vSpawnMaxScale; }
    float GetSpawnMinLife() { return m_Module.MinLife; }
    float GetSpawnMaxLife() { return m_Module.MaxLife; }
    int GetSpawnShape() { return m_Module.SpawnShape; }
    float GetSpawnRadius() { return m_Module.Radius; }
    Vec4 GetSpawnBoxScale() { return m_Module.vSpawnBoxScale; }
    int GetSpawnRate() { return m_Module.SpawnRate; }
    // int GetarrModuleCheck() { return m_Module.arrModuleCheck; } -> 반복문 돌면서 배열의 값이 1인거 찾아서 리턴시키는 함수 만들기
    int GetAddVelocityType() { return m_Module.AddVelocityType; }
    float GetMinSpeed() { return m_Module.MinSpeed; }
    float GetMaxSpeed() { return m_Module.MaxSpeed; }
    Vec4 GetFixedDirection() { return m_Module.FixedDirection; }
    float GetFixedAngle() { return m_Module.FixedAngle; }


    void SetSpawnColor(Vec4 _color) {  m_Module.vSpawnColor = _color; }
    void SetSpawnMinScale(Vec4 _scale) {  m_Module.vSpawnMinScale = _scale; }
    void SetSpawnMaxScale(Vec4 _scale) {  m_Module.vSpawnMaxScale = _scale; }
    void SetSpawnMinLife(float _life) {  m_Module.MinLife = _life; }
    void SetSpawnMaxLife(float _life) {  m_Module.MaxLife = _life; }
    void SetSpawnShape(int _shape) {  m_Module.SpawnShape = _shape; }
    void SetSpawnRadius(float _radius) {  m_Module.Radius = _radius; }
    void SetSpawnBoxScale(Vec4 _scale) {  m_Module.vSpawnBoxScale = _scale; }
    void SetSpawnRate(int _rate) {  m_Module.SpawnRate = _rate; }
    // void SetarrModuleCheck(int _modulecheck) {  m_Module.arrModuleCheck = _modulecheck; } -> 반복문 돌면서 배열의 값이 1인거 찾아서 리턴시키는 함수 만들기
    void SetAddVelocityType(int _velocitytype) {  m_Module.AddVelocityType = _velocitytype; }
    void SetMinSpeed(float _speed) {  m_Module.MinSpeed = _speed; }
    void SetMaxSpeed(float _speed) {  m_Module.MaxSpeed = _speed; }
    void SetFixedDirection(Vec4 _direction) {  m_Module.FixedDirection = _direction; }
    void SetFixedAngle(float _angle) {  m_Module.FixedAngle = _angle; }



public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    HYParticleSystem();
    ~HYParticleSystem();
};

