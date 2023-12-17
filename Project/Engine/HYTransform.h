#pragma once
#include "HYComponent.h"

class HYTransform :
    public HYComponent
{
private:
    
    Vec3    m_vRelativePos;             // 위치
    Vec3    m_vRelativeScale;           // 크기
    Vec3    m_vRealtiveRotation;        // 회전량(회전 상태)
    Vec3    m_arrDir[3];                // 물체의 방향 정보(Right, Up, Front)

    Matrix  m_matWorld;                 // 물체의 상태 행렬(변환 이동 행렬)

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

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRealtiveRotation; }

    const Matrix& GetWorldMat() { return m_matWorld; }

    Vec3 GetDir(DIR_TYPE _type) { return m_arrDir[(UINT)_type]; }

public:
    HYTransform();
    ~HYTransform();
};

