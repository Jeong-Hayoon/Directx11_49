#pragma once
#include "HYComponent.h"

class HYTransform :
    public HYComponent
{
private:
    
    Vec3    m_vRelativePos;             // 위치
    Vec3    m_vRelativeScale;           // 크기
    Vec3    m_vRealtiveRotation;        // 각 축별의 회전량(회전 상태)

    // m_arrLocalDir - m_vRealtiveRotation가 0,0,0인 기본값일 때(회전하지 않은 상태)
    // m_arrLocalDir는 기저벡터로 월드의 축과 일치시켜 놓음
    // m_arrLocalDir는 로컬 스페이스에서 물체만의 자체적인 상태의 방향
    // m_arrWorldDir는 부모의 영향을 받은 월드 스페이스에서의 최종 방향
    Vec3    m_arrLocalDir[3];           // 물체의 방향 정보(Right, Up, Front)
    Vec3    m_arrWorldDir[3];           // Right, Up, Front

    Matrix  m_matWorld;                 // 물체의 상태 행렬(변환 이동 행렬)

    bool    m_bAbsolute;                // 절대 단위 : 부모의 크기에 영향을 받지 않도록

public:
    // 본인의 위치값을 최종적으로 연산하여 상태값을 가지고 있음
    virtual void finaltick() override;

    // 본인의 데이터를 GPU로 업데이트시키는 함수
    // render하기 직전에 해줘야 여러 물체를 원하는 위치에 배치시킬 수 있음
    // 만약 finaltick에서 호출하게 될 경우 물체가 전부 같은 위치에 배치가 되어버림
    // (상수버퍼에서 마지막으로 updata 호출된 정보로 다 덮어지기 때문에)
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRealtiveRotation = _Rotation; }

    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRealtiveRotation; }

    // 월드 공간에서의 최종 위치, 크기, 회전 상태
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();
    //Vec3 GetWorldRot();

    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

    const Matrix& GetWorldMat() { return m_matWorld; }

    Vec3 GetLocalDir(DIR_TYPE _type) { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) { return m_arrWorldDir[(UINT)_type]; }

public:
    HYTransform();
    ~HYTransform();
};

