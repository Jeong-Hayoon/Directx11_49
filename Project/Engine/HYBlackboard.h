#pragma once
#include "HYEntity.h"

// �����忡�� ����� �ֿ� ������ Ÿ��
enum class BB_DATA
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OBJECT,
};

struct tBlackboardData
{
    // ������ Ÿ��
    BB_DATA         Type;
    // ���� �������� �ּҰ�
    void*           pData;
};

class HYBlackboard :
    public HYEntity
{
private:
    map<wstring, tBlackboardData>   m_mapBBData;

public:
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data);

    CLONE(HYBlackboard);

public:
    HYBlackboard();
    HYBlackboard(const HYBlackboard& _board);
    ~HYBlackboard();
};

