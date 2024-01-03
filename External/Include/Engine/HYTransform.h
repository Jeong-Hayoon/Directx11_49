#pragma once
#include "HYComponent.h"

class HYTransform :
    public HYComponent
{
private:
    
    Vec3    m_vRelativePos;             // ��ġ
    Vec3    m_vRelativeScale;           // ũ��
    Vec3    m_vRealtiveRotation;        // �� �ະ�� ȸ����(ȸ�� ����)

    // m_arrLocalDir - m_vRealtiveRotation�� 0,0,0�� �⺻���� ��(ȸ������ ���� ����)
    // m_arrLocalDir�� �������ͷ� ������ ��� ��ġ���� ����
    // m_arrLocalDir�� ���� �����̽����� ��ü���� ��ü���� ������ ����
    // m_arrWorldDir�� �θ��� ������ ���� ���� �����̽������� ���� ����
    Vec3    m_arrLocalDir[3];           // ��ü�� ���� ����(Right, Up, Front)
    Vec3    m_arrWorldDir[3];           // Right, Up, Front

    Matrix  m_matWorld;                 // ��ü�� ���� ���(��ȯ �̵� ���)

    bool    m_bAbsolute;                // ���� ���� : �θ��� ũ�⿡ ������ ���� �ʵ���

public:
    // ������ ��ġ���� ���������� �����Ͽ� ���°��� ������ ����
    virtual void finaltick() override;

    // ������ �����͸� GPU�� ������Ʈ��Ű�� �Լ�
    // render�ϱ� ������ ����� ���� ��ü�� ���ϴ� ��ġ�� ��ġ��ų �� ����
    // ���� finaltick���� ȣ���ϰ� �� ��� ��ü�� ���� ���� ��ġ�� ��ġ�� �Ǿ����
    // (������ۿ��� ���������� updata ȣ��� ������ �� �������� ������)
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRealtiveRotation = _Rotation; }

    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRealtiveRotation; }

    // ���� ���������� ���� ��ġ, ũ��, ȸ�� ����
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

